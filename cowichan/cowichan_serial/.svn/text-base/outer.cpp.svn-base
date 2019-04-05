/**
 * \file cowichan_serial/outer.cpp
 * \brief Serial outer implementation.
 * \see CowichanSerial::outer
 */

#include "cowichan_serial.hpp"

void CowichanSerial::outer (PointVector points, Matrix matrix, Vector vector)
{
  Point zeroPoint(0.0, 0.0);
  real d; // distance between points
  real dMax = -1.0; // maximum distance
  index_t r, c; // loop indices

  // all elements except matrix diagonal
  for (r = 0; r < n; r++) {
    vector[r] = Point::distance (points[r], zeroPoint);
    for (c = 0; c < r; c++) {
      d = Point::distance (points[r], points[c]);
      if (d > dMax) {
        dMax = d;
      }
      MATRIX_SQUARE(matrix, r, c) = MATRIX_SQUARE(matrix, c, r) = d;
    }
  }

  // matrix diagonal
  dMax *= n;
  for (r = 0; r < n; r++) {
    MATRIX_SQUARE(matrix, r, r) = dMax;
  }
}

