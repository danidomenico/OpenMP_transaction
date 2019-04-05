/**
 * Vector norm
 *
 * \file norm.cpp
 * \author Andrew Borzenko
 * \date 02-10-09
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

  pt1D* vec;
  int n;
  int limit;
  int i;

  n = MAXEXT;
  limit = MAXEXT;

  srand (333);

  vec = new pt1D[MAXEXT];
  for (i = 0; i < n; i++) {
    vec[i].x = rand () % limit;
    vec[i].y = rand () % limit;
    vec[i].w = rand () % limit;
  }

#ifdef TEST_OUTPUT
  printf ("Vector:\n");
  print_vector (vec, n);
#endif

#ifdef TEST_TIME
  INT64 start, end;
  start = get_ticks ();
#endif

#ifdef IS_PARALLEL
  norm_mpi (world, vec, n);
#else
  norm (vec, n);
#endif

#ifdef TEST_TIME
  end = get_ticks ();
  print_elapsed_time (start, end);
#endif

#ifdef TEST_OUTPUT
  printf ("Norm:\n");
  print_vector (vec, n);
#endif

  delete [] vec;

  return 0;
}
