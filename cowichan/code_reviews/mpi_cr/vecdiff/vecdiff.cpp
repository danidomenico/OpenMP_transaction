/**
 * Vector difference
 *
 * \file vecdiff.cpp
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

  real1D*	left;
  real1D*	right;
  real	norm1diff;
  int n;
  int limit;
  int i;

  n = MAXEXT;
  limit = 10;

  srand (222);

  left = new real1D[MAXEXT];
  for (i = 0; i < n; i++)
  {
    left[i] = (real) (rand () % limit);
  }

  right = new real1D[MAXEXT];
  for (i = 0; i < n; i++)
  {
    right[i] = (real) (rand () % limit);
  }

#ifdef TEST_OUTPUT
  printf ("left is:\n");
  print_vector (left, n);

  printf ("right is:\n");
  print_vector (right, n);
#endif

#ifdef TEST_TIME
  INT64 start, end;
  start = get_ticks ();
#endif

#ifdef IS_PARALLEL
  vecdiff_mpi (world, left, right, n, &norm1diff);
#else
  vecdiff (left, right, n, &norm1diff);
#endif

#ifdef TEST_TIME
  end = get_ticks ();
  print_elapsed_time (start, end);
#endif

#ifdef TEST_OUTPUT
  printf ("norm1 diff is:\n");
  printf ("%lg\n", norm1diff);
#endif

  delete [] left;
  delete [] right;

  return 0;
}
