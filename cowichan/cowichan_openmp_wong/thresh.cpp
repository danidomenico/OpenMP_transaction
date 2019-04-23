/**
 * \file cowichan_openmp_wong/thresh.cpp
 * \brief OpenMP thresh implementation (transactional memory).
 * \see CowichanOpenMP::thresh
 */

#include "../cowichan_openmp/cowichan_openmp.hpp"

/**
 * Works only on positive input.
 */
void CowichanOpenMP::thresh(IntMatrix matrix, BoolMatrix mask) {

    index_t* hist = NULL; // histogram
    index_t i;
    index_t r, c;
    INT_TYPE vMax; // max value in matrix
    index_t retain; // selection

    // find max value in matrix
    vMax = 0;
    #pragma omp parallel for schedule(static) transaction
    for (r = 0; r < nr; r++) {
        #pragma omp parallel for schedule(static)
        for (c = 0; c < nc; c++) {
            if (vMax < MATRIX_RECT(matrix, r, c)) {
                vMax = MATRIX_RECT(matrix, r, c);
            }
        }
    }

    // initialize histogram
    try {
        hist = NEW_VECTOR_SZ(index_t, vMax + 1);
    }
    catch (...) {out_of_memory();}

    #pragma omp parallel for schedule(static)
    for (i = 0; i <= (index_t)vMax; i++) {
        hist[i] = 0;
    }
    
    // count
    #pragma omp parallel for schedule(static) transaction
    for (r = 0; r < nr; r++) {
        #pragma omp parallel for schedule(static)
        for (c = 0; c < nc; c++) {
            hist[MATRIX_RECT(matrix, r, c)]++;
        }
    }

    // include
    retain = (index_t)(threshPercent * nc * nr);
    for (i = vMax; ((i >= 0) && (retain > 0)); i--) {
        retain -= hist[i];
    }
    retain = i;

    delete [] hist;

    // threshold
    #pragma omp parallel for schedule(static)
    for (r = 0; r < nr; r++) {
        #pragma omp parallel for schedule(static)
        for (c = 0; c < nc; c++) {
            MATRIX_RECT(mask, r, c) = ((index_t)MATRIX_RECT(matrix, r, c)) > retain;
        }
    }

}
