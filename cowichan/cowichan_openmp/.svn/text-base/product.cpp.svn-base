/**
 * \file cowichan_openmp/product.cpp
 * \brief OpenMP product implementation.
 * \see CowichanOpenMP::product
 */

#include "cowichan_openmp.hpp"

void CowichanOpenMP::product (Matrix matrix, Vector candidate, Vector solution)
{
  index_t r;
  index_t c;

#pragma omp parallel for schedule(static) private(c)
  for (r = 0; r < n; r++) {
    solution[r] = MATRIX_SQUARE(matrix, r, 0) * candidate[0];
    for (c = 1; c < n; c++) {
      solution[r] += MATRIX_SQUARE(matrix, r, c) * candidate[c];
    }
  }
}

