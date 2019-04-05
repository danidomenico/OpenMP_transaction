/**
 * \file cowichan_openmp/thresh.cpp
 * \brief OpenMP thresh implementation.
 * \see CowichanOpenMP::thresh
 */

#include "cowichan_openmp.hpp"

/**
 * Works only on positive input.
 */
void CowichanOpenMP::thresh(IntMatrix matrix, BoolMatrix mask) {

  index_t* hist = NULL; // histogram
  index_t i, j;
  index_t r, c;
  INT_TYPE vMax; // max value in matrix
  index_t retain; // selection

  index_t num_threads = omp_get_max_threads();

  INT_TYPE* vMaxLocal = NULL;
  try {
    vMaxLocal = NEW_VECTOR_SZ(INT_TYPE, num_threads);
  }
  catch (...) {out_of_memory();}

  // find max value in matrix
#pragma omp parallel
  {
  index_t cur_thread = omp_get_thread_num();
  vMaxLocal[cur_thread] = 0;
#pragma omp for schedule(static)
  for (r = 0; r < nr; r++) {
#pragma omp parallel for schedule(static)
    for (c = 0; c < nc; c++) {
      if (vMaxLocal[cur_thread] < MATRIX_RECT(matrix, r, c)) {
        vMaxLocal[cur_thread] = MATRIX_RECT(matrix, r, c);
      }
    }
  }
  }

  vMax = 0;
  for (i = 0; i < num_threads; i++) {
    if (vMax < vMaxLocal[i]) {
      vMax = vMaxLocal[i];
    }
  }

  delete [] vMaxLocal;

  // initialize histogram
  try {
    hist = NEW_VECTOR_SZ(index_t, vMax + 1);
  }
  catch (...) {out_of_memory();}

  index_t** histLocal = NULL;
  try {
    histLocal = NEW_VECTOR_SZ(index_t*, num_threads);
    for (i = 0; i < num_threads; i++) {
      histLocal[i] = NEW_VECTOR_SZ(index_t, vMax + 1);
    }
  }
  catch (...) {out_of_memory();}

#pragma omp parallel for schedule(static) private(j)
  for (i = 0; i <= (index_t)vMax; i++) {
    hist[i] = 0;
    for (j = 0; j < num_threads; j++) {
      histLocal[j][i] = 0;
    }
  }

  // count
#pragma omp parallel
  {
    index_t cur_thread = omp_get_thread_num();
#pragma omp for schedule(static)
    for (r = 0; r < nr; r++) {
#pragma omp parallel for schedule(static)
      for (c = 0; c < nc; c++) {
        histLocal[cur_thread][MATRIX_RECT(matrix, r, c)]++;
      }
    }
  }

  for (i = 0; i < num_threads; i++) {
    for (j = 0; j <= (index_t)vMax; j++) {
      hist[j] += histLocal[i][j];
    }
    delete [] histLocal[i];
  }

  delete [] histLocal;

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

