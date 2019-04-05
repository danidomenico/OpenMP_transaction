/**
 * Conway's game of life
 *
 * \file life.cpp
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

  bool2D* matrix; /* world to evolve */
  int    nr;    /* row size */
  int    nc;    /* column size */
  int    iters; /* number of iterations */

  int i, j;

  srand (222);

  nr = MAXEXT;
  nc = MAXEXT;
  iters = 10;

  matrix = new bool2D[MAXEXT];
  for (i = 0; i < nr; i++)
  {
    for (j = 0; j < nc; j++)
    {
      matrix[i][j] = rand () % 2;
    }
  }

#ifdef TEST_OUTPUT
  print_matrix (matrix, nr, nc);
#endif

#ifdef TEST_TIME
  INT64 start, end;
  start = get_ticks ();
#endif

#ifdef IS_PARALLEL
  life_mpi (world, matrix, nr, nc, iters);
#else
  life (matrix, nr, nc, iters);
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
