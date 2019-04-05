/**
 * \file cowichan_openmp/half.cpp
 * \brief OpenMP half implementation.
 * \see CowichanOpenMP::half
 */

#include "cowichan_openmp.hpp"

void CowichanOpenMP::half (IntMatrix matrixIn, IntMatrix matrixOut)
{
  index_t r, c;

  index_t middle_r = (nr + 1) / 2;
  index_t middle_c = (nc + 1) / 2;

#pragma omp parallel for schedule(static)
  for (r = 0; r < nr; r++) {

#pragma omp parallel for schedule(static)
    for (c = 0; c < nc; c++) {

      index_t previous_r, previous_c;

      // calculate unswapped x co-ordinate.
      if (c < middle_c) {
        previous_c = c * 2;
      } else {
        previous_c = (c - middle_c) * 2 + 1;
      }
      
      // calculate unswapped y co-ordinate.
      if (r < middle_r) {
        previous_r = r * 2;
      } else {
        previous_r = (r - middle_r) * 2 + 1;
      }

      MATRIX_RECT(matrixOut, r, c) = MATRIX_RECT(matrixIn, previous_r,
          previous_c);
    }
  }
}

