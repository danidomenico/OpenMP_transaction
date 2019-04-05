/**
 * Convex hull
 *
 * \file hull.cpp
 * \author Andrew Borzenko
 * \date 03-06-09
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

  pt1D* points;
  pt1D* hullPoints;
  int    n;    // number of points
  int    hn;    // number of hull points
  int limit;
  int i;

  srand (333);

  n = MAXEXT;
  limit = 10;
  
  points = new pt1D[MAXEXT];
  for (i = 0; i < n; i++)
  {
    points[i].x = rand () % limit;
    points[i].y = rand () % limit;
    points[i].w = 0;
  }

  hullPoints = new pt1D[MAXEXT];

#ifdef TEST_OUTPUT
  printf ("Points:\n");
  print_points (points, n, limit);
#endif

#ifdef TEST_TIME
  INT64 start, end;
  start = get_ticks ();
#endif

#ifdef IS_PARALLEL
  hull_mpi (world, points, n, hullPoints, &hn);
#else
  hull (points, n, hullPoints, &hn);
#endif

#ifdef TEST_TIME
  end = get_ticks ();
  print_elapsed_time (start, end);
#endif

#ifdef TEST_OUTPUT
  printf ("Convex Hull Points:\n");
  print_points (hullPoints, hn, limit);
#endif

  delete [] points;
  delete [] hullPoints;

  return 0;
}
