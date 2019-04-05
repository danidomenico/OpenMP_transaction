/**
 * \file cowichan_serial/mandel.cpp
 * \brief Serial mandel implementation.
 * \see CowichanSerial::mandel
 */

#include "cowichan_serial.hpp"

namespace cowichan_serial
{

/**
 * Calculate mandelbrot value.
 * \param x x-coordinate.
 * \param y y-coordinate.
 * \return Mandelbrot value.
 */
INT_TYPE mandel_calc (real x, real y);

}

void CowichanSerial::mandel (IntMatrix matrix)
{
  index_t r, c;
  real dx, dy;

  dx = mandelDx / (nc - 1);
  dy = mandelDy / (nr - 1);

  for (r = 0; r < nr; r++) {
    for (c = 0; c < nc; c++) {
      MATRIX_RECT(matrix, r, c) = mandel_calc (mandelX0 + (c * dx),
          mandelY0 + (r * dy));
    }
  }
}

namespace cowichan_serial
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

