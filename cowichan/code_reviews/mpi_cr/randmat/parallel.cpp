/**
 * Parallel implementation of random matrix generation
 *
 * \file parallel.cpp
 * \author Andrew Borzenko
 * \date 02-02-09
 */

#include "../include/main.h"
#include "parallel.h"

// shared data structures

static unsigned int* state;		/* random state vector */
static unsigned int aPrime, cPrime;		/* modified constants */ 

// public

void
randmat_mpi (
  mpi::communicator world,			/* mpi communicator */
  int2D*		matrix,			/* to fill */
  int		nr,			/* row size */
  int		nc,			/* column size */
  unsigned int		limit,			/* value limit */
  unsigned int		seed			/* RNG seed */
){
  int		i, j;			/* loop index */
  int		lo, hi, str;		/* work controls */

  int rank = world.rank ();
  int size = world.size ();

  state = new unsigned int[size];

  // set up
  if (rank == 0) {
    randStateInit(seed, size, state, &aPrime, &cPrime);
  }
  // broadcast set up
  broadcast (world, state, size, 0);
  broadcast (world, aPrime, 0);
  broadcast (world, cPrime, 0);

  // special scheduling
  if (get_cyclic_rows_mpi (world, 0, nr * nc, &lo, &hi, &str)) {
    for (i = lo; i < hi; i += str) {
      matrix[i / nr][i % nr] = state[rank] % limit;
      state[rank] = (aPrime * state[rank] + cPrime) % RAND_M;
    }
  }
  // broadcast result
  for (i = 0; i < nr; i++) {
    for (j = 0; j < nc; j++) {
      rank = get_cyclic_rank_mpi (world, 0, nr * nc, i * nc + j);
      broadcast (world, matrix[i][j], rank);
    }
  }

  delete [] state;

  /* return */
}
