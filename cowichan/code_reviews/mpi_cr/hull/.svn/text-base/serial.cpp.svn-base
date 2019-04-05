/**
 * Serial implementation of convex hull
 *
 * \file serial.cpp
 * \author Andrew Borzenko
 * \date 03-06-09
 */

#include "../include/main.h"
#include "serial.h"

// public

void hull (pt1D* points,     // list of points
           int n,            // number of points
           pt1D* hullPoints, // list of points in convex hull
           int* hn)          // number of points in convex hull
{
  int i;
  pt min_p;
  pt max_p;

  min_p = points[0];
  max_p = points[0];
  *hn = 0;

  // figure out the points with minimum and maximum x values
  for (i = 1; i < n; i++) {
    if (min_p.x > points[i].x) {
      min_p = points[i];
    }
    if (max_p.x < points[i].x) {
      max_p = points[i];
    }
  }

  // use these as initial pivots
  split (points, n, hullPoints, hn, min_p, max_p);
  split (points, n, hullPoints, hn, max_p, min_p);
}

// private

void split (pt1D* points,     // list of points
            int n,            // number of points
            pt1D* hullPoints, // list of points in convex hull
            int* hn,          // number of points in convex hull
            pt p1,            // boundary point #1
            pt p2)            // boundary point #2
{
  pt* maxPoint = NULL;
  real maxCross = -std::numeric_limits<real>::infinity ();

  // compute the signed distances from the line for each point
  for (int i = 0; i < n; i++) {
    real currentCross = cross (p1, p2, points[i]);
    if (currentCross > maxCross) {
      maxPoint = &(points[i]);
      maxCross = currentCross;
    }
  }

  // is there a point in the positive half-space?
  // if so, it has maximal distance, and we must recurse based on that point.
  if (maxCross > 0.0) {
    // recurse on the new set with the given far point
    split (points, n, hullPoints, hn, p1, *maxPoint);
    split (points, n, hullPoints, hn, *maxPoint, p2);
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
inline real cross (pt l1, pt l2, pt p)
{
  return (l1.x - p.x) * (l2.y - p.y) - (l1.y - p.y) * (l2.x - p.x);
}
