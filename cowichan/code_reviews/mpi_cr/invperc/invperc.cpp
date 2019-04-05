/**
 * Invasion percolation
 *
 * \file vecdiff.cpp
 * \author Andrew Borzenko
 * \date 02-28-09
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
  bool2D* mask;
  int nr, nc;
  int limit;
  real fraction;
  int i, j;

  nr = MAXEXT;
  nc = MAXEXT;
  limit = 10;
  fraction = 0.75;

  srand (222);

  matrix = new int2D[MAXEXT];
  for (i = 0; i < nr; i++) {
    for (j = 0; j < nc; j++) {
      matrix[i][j] = rand () % limit;
    }
  }

  mask = new bool2D[MAXEXT];
  memset (mask, 0, sizeof(boolean) * nr * nc);

#ifdef TEST_OUTPUT
  printf ("Matrix:\n");
  print_matrix (matrix, nr, nc);

  printf ("Mask before:\n");
  print_matrix (mask, nr, nc);
#endif

#ifdef TEST_TIME
  INT64 start, end;
  start = get_ticks ();
#endif

#ifdef IS_PARALLEL
  invperc_mpi (world, matrix, mask, nr, nc, fraction);
#else
  invperc (matrix, mask, nr, nc, fraction);
#endif

#ifdef TEST_TIME
  end = get_ticks ();
  print_elapsed_time (start, end);
#endif

#ifdef TEST_OUTPUT
  printf ("Mask after:\n");
  print_matrix (mask, nr, nc);
#endif

  delete [] matrix;
  delete [] mask;

  return 0;
}
