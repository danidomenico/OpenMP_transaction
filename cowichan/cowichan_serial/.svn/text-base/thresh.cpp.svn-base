/**
 * \file cowichan_serial/thresh.cpp
 * \brief Serial thresh implementation.
 * \see CowichanSerial::thresh
 */

#include "cowichan_serial.hpp"

/**
 * Works only on positive input.
 */
void CowichanSerial::thresh(IntMatrix matrix, BoolMatrix mask) {

  index_t* hist = NULL; // histogram
  index_t i;
  index_t r, c;
  INT_TYPE vMax; // max value in matrix
  index_t retain; // selection

  // find max value in matrix
  vMax = 0;
  for (r = 0; r < nr; r++) {
    for (c = 0; c < nc; c++) {
      if (vMax < MATRIX_RECT(matrix, r, c)) {
        vMax = MATRIX_RECT(matrix, r, c);
      }
    }
  }

  // initialize histogram
  try {
    hist = NEW_VECTOR_SZ(index_t, vMax + 1);
  }
  catch (...) {out_of_memory();}

  for (i = 0; i <= (index_t)vMax; i++) {
    hist[i] = 0;
  }

  // count
  for (r = 0; r < nr; r++) {
    for (c = 0; c < nc; c++) {
      hist[MATRIX_RECT(matrix, r, c)]++;
    }
  }

  // include
  retain = (index_t)(threshPercent * nc * nr);
  for (i = vMax; ((i >= 0) && (retain > 0)); i--) {
    retain -= hist[i];
  }
  retain = i;

  delete [] hist;

  // threshold
  for (r = 0; r < nr; r++) {
    for (c = 0; c < nc; c++) {
      MATRIX_RECT(mask, r, c) = ((index_t)MATRIX_RECT(matrix, r, c)) > retain;
    }
  }

}

