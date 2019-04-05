/**
 * \file cowichan_serial/randmat.cpp
 * \brief Serial randmat implementation.
 * \see CowichanSerial::randmat
 */

#include "cowichan_serial.hpp"

void CowichanSerial::randmat (IntMatrix matrix)
{
  index_t r, c;
  INT_TYPE v = seed % RAND_M;

  for (r = 0; r < nr; r++) {
    for (c = 0; c < nc; c++) {
      MATRIX_RECT(matrix, r, c) = v;
      v = (RANDMAT_A * v + RANDMAT_C) % RAND_M;
    }
  }
}

