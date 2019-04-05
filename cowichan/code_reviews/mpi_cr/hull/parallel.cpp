/**
 * Parallel implementation of convex hull
 *
 * \file parallel.cpp
 * \author Andrew Borzenko
 * \date 03-06-09
 */

#include "../include/main.h"
#include "parallel.h"

// public

void hull_mpi (mpi::communicator world,
               pt1D* points,     // list of points
               int n,            // number of points
               pt1D* hullPoints, // list of points in convex hull
               int* hn)          // number of points in convex hull
{
  int i;
  pt min_p;
  pt max_p;
  pt min_p_local;
  pt max_p_local;
  int lo, hi;

  min_p_local = points[0];
  max_p_local = points[0];
  *hn = 0;

  // figure out the points with minimum and maximum x values
  get_block_rows_mpi (world, 1, n, &lo, &hi);
  for (i = lo; i < hi; i++) {
    if (min_p_local.x > points[i].x) {
      min_p_local = points[i];
    }
    if (max_p_local.x < points[i].x) {
      max_p_local = points[i];
    }
  }
  // reduce to min_p and max_p
  all_reduce (world, min_p_local, min_p, minimum_x_pt ());
  all_reduce (world, max_p_local, max_p, maximum_x_pt ());

  // use these as initial pivots
  split_mpi (world, points, n, hullPoints, hn, min_p, max_p);
  split_mpi (world, points, n, hullPoints, hn, max_p, min_p);
}

// private

void split_mpi (mpi::communicator world,
                pt1D* points,     // list of points
                int n,            // number of points
                pt1D* hullPoints, // list of points in convex hull
                int* hn,          // number of points in convex hull
                pt p1,            // boundary point #1
                pt p2)            // boundary point #2
{
  pt_cross max_cp;
  pt_cross max_cp_local;
  max_cp_local.cross = -std::numeric_limits<real>::infinity ();
  int lo, hi;
  int i;

  // compute the signed distances from the line for each point
  get_block_rows_mpi (world, 0, n, &lo, &hi);
  for (i = lo; i < hi; i++) {
    real currentCross = cross_mpi (p1, p2, points[i]);
    if (currentCross > max_cp_local.cross) {
      max_cp_local.p = points[i];
      max_cp_local.cross = currentCross;
    }
  }

  // reduce max_cp
  all_reduce (world, max_cp_local, max_cp, maximum_cp ());

  // is there a point in the positive half-space?
  // if so, it has maximal distance, and we must recurse based on that point.
  if (max_cp.cross > 0.0) {
    // recurse on the new set with the given far point
    split_mpi (world, points, n, hullPoints, hn, p1, max_cp.p);
    split_mpi (world, points, n, hullPoints, hn, max_cp.p, p2);
    return;
  } 

  // otherwise, it's not on the right side; we don't need to split anymore.
  // this is because all points are inside the hull when we use this half-space.
  // add the first point and return.
  hullPoints[(*hn)++] = p1;
}

/**
 * Computes the cross product of the vectors (l1,l2) and (l1,p).
 */
inline real cross_mpi (pt l1, pt l2, pt p)
{
  return (l1.x - p.x) * (l2.y - p.y) - (l1.y - p.y) * (l2.x - p.x);
}
