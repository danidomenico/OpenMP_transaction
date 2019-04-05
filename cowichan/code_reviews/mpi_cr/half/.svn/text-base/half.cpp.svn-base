/**
 * Halving shuffle
 *
 * \file half.cpp
 * \author Andrew Borzenko
 * \date 02-27-09
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

  int2D* matrix;
  int    nr;    /* row size */
  int    nc;    /* column size */
  int limit;
  int i, j;

  srand (222);

  nr = MAXEXT;
  nc = MAXEXT;
  limit = 10;

  matrix = new int2D[MAXEXT];
  for (i = 0; i < nr; i++)
  {
    for (j = 0; j < nc; j++)
    {
      matrix[i][j] = rand () % limit;
    }
  }

#ifdef TEST_OUTPUT
  printf ("matrix before shuffle:\n");
  print_matrix (matrix, nr, nc);
#endif

#ifdef TEST_TIME
  INT64 start, end;
  start = get_ticks ();
#endif

#ifdef IS_PARALLEL
  half_mpi (world, matrix, nr, nc);
#else
  half (matrix, nr, nc);
#endif

#ifdef TEST_TIME
  end = get_ticks ();
  print_elapsed_time (start, end);
#endif

#ifdef TEST_OUTPUT
  printf ("matrix after shuffle:\n");
  print_matrix (matrix, nr, nc);
#endif

  delete [] matrix;

  return 0;
}
