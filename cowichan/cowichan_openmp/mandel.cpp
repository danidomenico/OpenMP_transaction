/**
 * \file cowichan_openmp/mandel.cpp
 * \brief OpenMP mandel implementation.
 * \see CowichanOpenMP::mandel
 */

#include "cowichan_openmp.hpp"

namespace cowichan_openmp
{

/**
 * Calculate mandelbrot value.
 * \param x x-coordinate.
 * \param y y-coordinate.
 * \return Mandelbrot value.
 */
INT_TYPE mandel_calc (real x, real y);

}

/*****************************************************************************/

void CowichanOpenMP::mandel (IntMatrix matrix)
{
  index_t r, c;
  real dx, dy;

  dx = mandelDx / (nc - 1);
  dy = mandelDy / (nr - 1);
  
#pragma omp parallel for schedule(dynamic)
  for (r = 0; r < nr; r++) {
#pragma omp parallel for schedule(static)
    for (c = 0; c < nc; c++) {
      MATRIX_RECT(matrix, r, c) = mandel_calc (mandelX0 + (c * dx),
          mandelY0 + (r * dy));
    }
  }
}

/*****************************************************************************/

namespace cowichan_openmp
{

INT_TYPE mandel_calc (real x, real y)
{
  real r = 0.0, i = 0.0; // real and imaginary parts
  real rs = 0.0, is = 0.0; // " ", squared
  INT_TYPE iter = 0; // number of iterations

  do {
    i = (((real)2.0) * r * i) + x;
    r = (rs - is) + y;
    iter++;
    rs = r * r;
    is = i * i;
  } while ((iter < MANDEL_MAX_ITER) && ((rs + is) < MANDEL_INFINITY));

  return iter;
}

}

