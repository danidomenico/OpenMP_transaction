/**
 * \file cowichan_mpi/randmat.cpp
 * \brief MPI random matrix implementation.
 * \see CowichanMPI::randmat
 */

#include "cowichan_mpi.hpp"
void CowichanMPI::randmat (IntMatrix matrix)
{
  index_t rlo, rhi;
  index_t r, c;

  index_t size = nr;

  IntVector state = NEW_VECTOR_SZ(INT_TYPE, size);
  INT_TYPE aPrime, cPrime;    /* modified constants */ 

  // set up
  if (world.rank () == 0) {
    randStateInit(seed, size, state, &aPrime, &cPrime);
  }
  // broadcast set up
  broadcast (world, state, (int)size, 0);
  broadcast (world, aPrime, 0);
  broadcast (world, cPrime, 0);

  // assign rows to processes
  if (get_block (world, 0, nr, &rlo, &rhi)) {
    for (r = rlo; r < rhi; r++) {
      MATRIX_RECT(matrix, r, 0) = state[r];
      for (c = 1; c < nc; c++) {
        MATRIX_RECT(matrix, r, c) =
            (aPrime * MATRIX_RECT(matrix, r, c - 1) + cPrime) % RAND_M;
      }
    }
  }
  
  // broadcast matrix rows
  for (r = 0; r < world.size (); r++) {
    if (get_block (world, 0, nr, &rlo, &rhi, r)) {
      broadcast (world, &MATRIX_RECT(matrix, rlo, 0), (int)((rhi - rlo) * nc),
          (int)r);
    }
  }

  delete [] state;
}

namespace cowichan_mpi {

void randStateInit(INT_TYPE seed, index_t width, IntVector state,
    INT_TYPE* aPrime, INT_TYPE* cPrime)
{
  index_t i;

  state[0] = seed % RAND_M;
  *aPrime = RANDMAT_A;
  *cPrime = 1;

  for (i = 1; i < width; i++) {
    state[i] = (RANDMAT_A * state[i-1] + RANDMAT_C) % RAND_M;
    *cPrime = (*cPrime + *aPrime) % RAND_M;
    *aPrime = (*aPrime * RANDMAT_A) % RAND_M;
  }

  *cPrime = (*cPrime * RANDMAT_C) % RAND_M;
}

}
