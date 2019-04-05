/**
 * \file cowichan_openmp/hull.cpp
 * \brief OpenMP hull implementation.
 * \see CowichanOpenMP::hull
 */

#include "cowichan_openmp.hpp"

namespace cowichan_openmp
{

/**
 * Runs quickhull algorithm.
 * \param pointsIn input points.
 * \param n number of inputs points to use.
 * \param pointsOut output points.
 * \param hn number of output points generated so far.
 */
void quickhull(PointVector pointsIn, index_t n, PointVector pointsOut,
    index_t* hn);

/**
 * Recursive step of the quickhull algorithm - compute hull on one side of the
 * splitting line.
 * \param pointsIn input points.
 * \param n number of inputs points to use.
 * \param pointsOut output points.
 * \param hn number of output points generated so far.
 * \param p1 first point of the splitting line (p1,p2).
 * \param p2 second point of the splitting line (p1,p2).
 */
void split(PointVector pointsIn, index_t n, PointVector pointsOut, index_t* hn,
    Point* p1, Point* p2);

}

/*****************************************************************************/

/**
 * For description see \ref hull_sec
 *
 * Runs quickhull algorithm until all points have been used up from the
 * original vector. At each step the hull points are marked as used and a new
 * convex hull is computed on the rest of points.
 * The points that have been used up are in the range (n - hn, n), i.e. at
 * the end of pointsIn vector.
 * NOTE: pointsIn vector gets modified by the algorithm.
 */
void CowichanOpenMP::hull (PointVector pointsIn, PointVector pointsOut)
{
  index_t hn = 0;
  index_t previous_hn = 0;

  // while not all points are used up then run quickhull on the rest of points
  while (n != hn) {
    // exclude added points from pointsIn by swapping them with points from the
    // end of pointsIn vector in range (0, n - nused)
    index_t added_i;
#pragma omp parallel for schedule(static)
    for (added_i = previous_hn; added_i < hn; added_i++) {
      // search for the added point
      for (index_t i = 0; i < n - previous_hn; i++) {
        if ((pointsIn[i].x == pointsOut[added_i].x)
            && (pointsIn[i].y == pointsOut[added_i].y)) {
          Point tmp = pointsIn[i];
          pointsIn[i] = pointsIn[n - added_i - 1];
          pointsIn[n - added_i - 1] = tmp;
          break;
        }
      }
    }
    
    previous_hn = hn;
    quickhull (pointsIn, n - hn, pointsOut, &hn);
  }
}

/*****************************************************************************/

namespace cowichan_openmp
{

void quickhull(PointVector pointsIn, index_t n, PointVector pointsOut,
    index_t* hn)
{
  // base case
  if (n == 1) {
    pointsOut[(*hn)++] = pointsIn[0];
    return;
  }

  Point* minPoint;
  Point* maxPoint;

  // checking cutoff value here prevents allocating unnecessary memory
  // for the reduction
  if(n > CowichanOpenMP::HULL_CUTOFF) {
    index_t num_threads = omp_get_max_threads();

    Point** minPoints = NULL;
    Point** maxPoints = NULL;

    try {
      minPoints = NEW_VECTOR_SZ(Point*, num_threads);
      maxPoints = NEW_VECTOR_SZ(Point*, num_threads);
    }
    catch (...) {out_of_memory();}

    // figure out the points with minimum and maximum x values
  #pragma omp parallel
    {
      index_t thread_num = omp_get_thread_num();
      minPoints[thread_num] = &pointsIn[0];
      maxPoints[thread_num] = &pointsIn[0];
      index_t i;
  #pragma omp for schedule(static)
      for (i = 1; i < n; i++) {
        if (minPoints[thread_num]->x > pointsIn[i].x) {
          minPoints[thread_num] = &pointsIn[i];
        }
        if (maxPoints[thread_num]->x < pointsIn[i].x) {
          maxPoints[thread_num] = &pointsIn[i];
        }
      }
    }

    minPoint = minPoints[0];
    maxPoint = maxPoints[0];

    for (index_t i = 1; i < num_threads; i++) {
      if (minPoint->x > minPoints[i]->x) {
        minPoint = minPoints[i];
      }
      if (maxPoint->x < maxPoints[i]->x) {
        maxPoint = maxPoints[i];
      }
    }

    delete [] minPoints;
    delete [] maxPoints;
  }
  else {
    minPoint = &pointsIn[0];
    maxPoint = &pointsIn[0];

    // figure out the points with minimum and maximum x values
    index_t i;
    for (i = 1; i < n; i++) {
      if (minPoint->x > pointsIn[i].x) {
        minPoint = &pointsIn[i];
      }
      if (maxPoint->x < pointsIn[i].x) {
        maxPoint = &pointsIn[i];
      }
    }
  }

  // use these as initial pivots
  split (pointsIn, n, pointsOut, hn, minPoint, maxPoint);
  split (pointsIn, n, pointsOut, hn, maxPoint, minPoint);
}

void split (PointVector pointsIn, index_t n, PointVector pointsOut, index_t* hn,
    Point* p1, Point* p2) {

  Point* maxPoint;
  real maxCross;

  // checking cutoff value here prevents allocating unnecessary memory
  // for the reduction
  if (n > CowichanOpenMP::HULL_CUTOFF) {
    index_t num_threads = omp_get_max_threads();

    Point** maxPoints = NULL;
    Vector maxCrosses = NULL;

    try {
      maxPoints = NEW_VECTOR_SZ(Point*, num_threads);
      maxCrosses = NEW_VECTOR_SZ(real, num_threads);
    }
    catch (...) {out_of_memory();}

    // compute the signed distances from the line for each point
#pragma omp parallel
    {
      index_t thread_num = omp_get_thread_num();
      maxPoints[thread_num] = &pointsIn[0];
      maxCrosses[thread_num] = Point::cross (*p1, *p2, pointsIn[0]);
#pragma omp for schedule(static)
      for (index_t i = 1; i < n; i++) {
        real currentCross = Point::cross (*p1, *p2, pointsIn[i]);
        if (currentCross > maxCrosses[thread_num]) {
          maxPoints[thread_num] = &pointsIn[i];
          maxCrosses[thread_num] = currentCross;
        }
      }
    }

    maxPoint = maxPoints[0];
    maxCross = maxCrosses[0];

    for (index_t i = 0; i < num_threads; i++) {
      if (maxCross < maxCrosses[i]) {
        maxPoint = maxPoints[i];
        maxCross = maxCrosses[i];
      }
    }

    delete [] maxPoints;
    delete [] maxCrosses;
  }
  else
  {
    maxPoint = &pointsIn[0];
    maxCross = Point::cross (*p1, *p2, pointsIn[0]);

    // compute the signed distances from the line for each point
    for (index_t i = 1; i < n; i++) {
      real currentCross = Point::cross (*p1, *p2, pointsIn[i]);
      if (currentCross > maxCross) {
        maxPoint = &pointsIn[i];
        maxCross = currentCross;
      }
    }
  }

  // is there a point in the positive half-space?
  // if so, it has maximal distance, and we must recurse based on that point.
  if (maxCross > 0.0) {
    // recurse on the new set with the given far point
    split (pointsIn, n, pointsOut, hn, p1, maxPoint);
    split (pointsIn, n, pointsOut, hn, maxPoint, p2);
    return;
  }

  // otherwise, it's not on the right side; we don't need to split anymore.
  // this is because all points are inside the hull when we use this half-space.
  // add the first point and return.
  pointsOut[(*hn)++] = *p1;

}

}

