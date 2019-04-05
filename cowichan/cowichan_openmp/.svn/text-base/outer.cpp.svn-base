/**
 * \file cowichan_openmp/outer.cpp
 * \brief OpenMP outer implementation.
 * \see CowichanOpenMP::outer
 */

#include "cowichan_openmp.hpp"

void CowichanOpenMP::outer (PointVector points, Matrix matrix, Vector vector)
{
  Point zeroPoint(0.0, 0.0);
  real d; // distance between points
  real dMax; // maximum distance
  index_t r, c; // loop indices

  index_t num_threads = omp_get_max_threads();
  Vector dMaxes = NULL;

  try {
    dMaxes = NEW_VECTOR_SZ(real, num_threads);
  }
  catch (...) {out_of_memory();}

  // all elements except matrix diagonal
#pragma omp parallel private(dMax, d)
  {
    index_t thread_num = omp_get_thread_num();
    d = 0.0;
    dMax = -1.0;
#pragma omp for schedule(guided)
    for (r = 0; r < n; r++) {
      vector[r] = Point::distance (points[r], zeroPoint);
#pragma omp parallel for schedule(static)
      for (c = 0; c < r; c++) {
        d = Point::distance (points[r], points[c]);
        if (d > dMax) {
          dMax = d;
        }
        MATRIX_SQUARE(matrix, r, c) = MATRIX_SQUARE(matrix, c, r) = d;
      }
    }
    dMaxes[thread_num] = dMax;
  }

  dMax = dMaxes[0];
  for (index_t i = 1; i < num_threads; i++) {
    if (dMax < dMaxes[i]) {
      dMax = dMaxes[i];
    }
  }

  delete [] dMaxes;

  // matrix diagonal
  dMax *= n;
#pragma omp parallel for schedule(static)
  for (r = 0; r < n; r++) {
    MATRIX_SQUARE(matrix, r, r) = dMax;
  }
}

