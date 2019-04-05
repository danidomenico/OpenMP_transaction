/**
 * \file cowichan_serial/product.cpp
 * \brief Serial product implementation.
 * \see CowichanSerial::product
 */

#include "cowichan_serial.hpp"

void CowichanSerial::product (Matrix matrix, Vector candidate, Vector solution)
{
  index_t r, c;

  for (r = 0; r < n; r++) {
    solution[r] = MATRIX_SQUARE(matrix, r, 0) * candidate[0];
    for (c = 1; c < n; c++) {
      solution[r] += MATRIX_SQUARE(matrix, r, c) * candidate[c];
    }
  }
}

