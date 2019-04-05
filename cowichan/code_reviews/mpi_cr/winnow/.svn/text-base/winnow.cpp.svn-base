/**
 * Weighted point selection
 *
 * \file winnow.cpp
 * \author Andrew Borzenko
 * \date 02-23-09
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
  pt1D* pt;
  int    nr;    /* row size */
  int    nc;    /* column size */
  int limit;
  int npt;
  int i,j;

  srand (222);

  nr = MAXEXT;
  nc = MAXEXT;
  limit = 10;
  npt = MAXEXT;
  
  matrix = new int2D[MAXEXT];
  for (i = 0; i < nr; i++)
  {
    for (j = 0; j < nc; j++)
    {
      matrix[i][j] = rand () % limit;
    }
  }

  mask = new bool2D[MAXEXT];
  for (i = 0; i < nr; i++)
  {
    for (j = 0; j < nc; j++)
    {
      mask[i][j] = rand () % 2;
    }
  }

  pt = new pt1D[npt];

#ifdef TEST_OUTPUT
  printf ("Matrix:\n");
  print_matrix (matrix, nr, nc);

  printf ("Mask:\n");
  print_matrix (mask, nr, nc);
#endif

#ifdef TEST_TIME
  INT64 start, end;
  start = get_ticks ();
#endif

#ifdef IS_PARALLEL
  winnow_mpi (world, matrix, mask, nr, nc, pt, npt); 
#else
  winnow (matrix, mask, nr, nc, pt, npt); 
#endif

#ifdef TEST_TIME
  end = get_ticks ();
  print_elapsed_time (start, end);
#endif

#ifdef TEST_OUTPUT
  printf ("Points:\n");
  print_vector (pt, nr);
#endif

  delete [] matrix;
  delete [] mask;
  delete [] pt;

  return 0;
}
