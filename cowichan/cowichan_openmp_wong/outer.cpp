/**
 * \file cowichan_openmp_wong/outer.cpp
 * \brief OpenMP outer implementation (transactional memory).
 * \see CowichanOpenMP::outer
 */

#include "../cowichan_openmp/cowichan_openmp.hpp"

void CowichanOpenMP::outer (PointVector points, Matrix matrix, Vector vector) {
    Point zeroPoint(0.0, 0.0);
    real d; // distance between points
    real dMax = -1.0; // maximum distance
    index_t r, c; // loop indices

    // all elements except matrix diagonal
    #pragma omp parallel for schedule(static) private(d)
    for (r = 0; r < n; r++) {
        vector[r] = Point::distance (points[r], zeroPoint);
        #pragma omp parallel for schedule(static)
        for (c = 0; c < r; c++) {
            d = Point::distance (points[r], points[c]);
            
            #pragma omp transaction 
			{
				if (d > dMax) {
					dMax = d;
				}
			}
            
            MATRIX_SQUARE(matrix, r, c) = MATRIX_SQUARE(matrix, c, r) = d;
        }
    }
  
    // matrix diagonal
    dMax *= n;
    #pragma omp parallel for schedule(static)
    for (r = 0; r < n; r++) {
        MATRIX_SQUARE(matrix, r, r) = dMax;
    }
}
