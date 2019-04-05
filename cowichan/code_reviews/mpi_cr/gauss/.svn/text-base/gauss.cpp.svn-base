/**
 * Generic gauss implementation
 *
 * \file gauss.cpp
 * \author Andrew Borzenko
 * \date 02-03-09
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

  real2D*	matrix;			/* matrix x */
  real1D*	answer;			/* answer */
  real1D*	vector;			/* = vector */
  int		n;			/* matrix size */
  int limit;
  int i, j;

  srand (333);

  n = MAXEXT;
  limit = 10;

  matrix = new real2D[MAXEXT];
  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
    {
      matrix[i][j] = rand () % limit;
    }
  }

  vector = new real1D[MAXEXT];
  for (i = 0; i < n; i++) {
    vector[i] = rand () % limit;
  }

  answer = new real1D[MAXEXT];

#ifdef TEST_OUTPUT
  printf ("\n");

  printf ("Matrix\n");
  print_matrix (matrix, n, n);

  printf ("Target\n");
  print_vector (vector, n);
#endif

#ifdef TEST_TIME
  INT64 start, end;
  start = get_ticks ();
#endif

#ifdef IS_PARALLEL
  gauss_mpi (world, matrix, vector, answer, n);
#else
  gauss (matrix, vector, answer, n);
#endif

#ifdef TEST_TIME
  end = get_ticks ();
  print_elapsed_time (start, end);
#endif

#ifdef TEST_OUTPUT
  printf ("Answer\n");
  print_vector (answer, n);
#endif

  delete [] matrix;
  delete [] vector;
  delete [] answer;

  return 0;
}
