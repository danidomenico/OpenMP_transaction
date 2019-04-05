/**
 * Random matrix generation
 *
 * \file randmat.cpp
 * \author Andrew Borzenko
 * \date 01-26-09
 */

#include "../include/main.h"
#ifdef IS_PARALLEL
  #include "parallel.h"
#else
  #include "serial.h"
#endif

int main(int argc, char* argv[])
{
#ifdef IS_PARALLEL
  mpi::environment env(argc, argv);
  mpi::communicator world;

#ifdef TEST_OUTPUT
  printf ("I am process %d\n", world.rank ());
#endif
#endif

  int2D* matrix; /* to fill */
  int   nr;     /* row size */
  int   nc;     /* column size */
  unsigned int   limit;  /* value limit */
  unsigned int   seed;   /* RNG seed */

  nr = MAXEXT;
  nc = MAXEXT;
  limit = 10;
  seed = 222;

  matrix = new int2D[MAXEXT];

#ifdef TEST_TIME
  INT64 start, end;
  start = get_ticks ();
#endif

#ifdef IS_PARALLEL
  randmat_mpi (world, matrix, nr, nc, limit, seed);
#else
  randmat (matrix, nr, nc, limit, seed);
#endif

#ifdef TEST_TIME
  end = get_ticks ();
  print_elapsed_time (start, end);
#endif

#ifdef TEST_OUTPUT
  print_matrix (matrix, nr, nc);
#endif

  delete [] matrix;

  return 0;
}
