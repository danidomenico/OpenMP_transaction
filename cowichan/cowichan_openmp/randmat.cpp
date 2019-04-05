/**
 * \file cowichan_openmp/randmat.cpp
 * \brief OpenMP randmat implementation.
 * \see CowichanOpenMP::randmat
 */

#include "cowichan_openmp.hpp"

void CowichanOpenMP::randmat (IntMatrix matrix)
{
  IntVector state = NULL;

  // initialize first column values
  try {
    state = NEW_VECTOR_SZ(INT_TYPE, nr);
  }
  catch (...) {out_of_memory();}

  index_t r;
  INT_TYPE aPrime, cPrime;

  state[0] = seed % RAND_M;
  aPrime = RANDMAT_A;
  cPrime = 1;
  for (r = 1; r < nr; r++) {
    state[r] = (RANDMAT_A * state[r - 1] + RANDMAT_C) % RAND_M;
    cPrime = (cPrime + aPrime) % RAND_M;
    aPrime = (aPrime * RANDMAT_A) % RAND_M;
  }
  cPrime = (cPrime * RANDMAT_C) % RAND_M;

  // fill in random matrix
  index_t c;
  INT_TYPE v;

#pragma omp parallel for schedule(static) private(c, v)
  for (r = 0; r < nr; r++) {
    v = state[r];
    for (c = 0; c < nc; c++) {
      MATRIX_RECT(matrix, r, c) = v;
      v = (aPrime * v + cPrime) % RAND_M;
    }
  }
}

