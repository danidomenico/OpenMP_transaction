/**
 * \file cowichan_tbb/hull.cpp
 * \brief TBB hull implementation.
 * \see CowichanTBB::hull
 */

#include "cowichan_tbb.hpp"

namespace cowichan_tbb
{

/**
 * \brief Compute maximum distance for hull.
 *
 * Computes the maximum signed distance from a point to a line, given
 * the line and list of points.
 */
class MaximumDistance {
private:

  /**
   * Points.
   */
  const PointVector& points;

  /**
   * First point forming the line.
   */
  const Point& p1;

  /**
   * Second point forming the line.
   */
  const Point& p2;

  /**
   * Point of maximum distance.
   */
  Point* maxPoint;

  /**
   * Maximum cross product.
   */
  real maxCross;

public:

  /**
   * Construct a maximum distance object.
   * \param points points.
   * \param p1 first point forming the line.
   * \param p2 second point forming the line.
   */
  MaximumDistance(const PointVector& points, const Point& p1, const Point& p2)
      : points(points), p1(p1), p2(p2), maxPoint(&points[0]),
        maxCross(Point::cross (p1, p2, points[0])) { }

  /**
   * Gets the point with maximum signed distance (if it has already been
   * calculated).
   * \return Max distance point.
   */  
  Point* getPoint() const {
    return maxPoint;
  }
  
  /**
   * Gets the signed cross product to that point.
   * \return Max point cross product.
   */
  real getDistance() const {
    return maxCross;
  }
  
  /**
   * Calculates the Point with maximum signed distance.
   * \param range range of points to work on.
   */
  void operator()(const Range& range) {

    // compute the signed distances from the line for each point in the range.
    for (index_t i = range.begin(); i < range.end(); ++i) {
      real currentCross = Point::cross (p1, p2, points[i]);
      if (currentCross > maxCross) {
        maxPoint = &points[i];
        maxCross = currentCross;
      }
    }
    
  }

  /**
   * Splitting (TBB) constructor.
   * \param other object to split.
   */
  MaximumDistance(MaximumDistance& other, split) : points(other.points),
      p1(other.p1), p2(other.p2), maxPoint(&points[0]),
      maxCross(Point::cross (p1, p2, points[0])) { }

  /**
   * Joiner (TBB).
   * \param other object to join.
   */
  void join(const MaximumDistance& other) {
    if (other.maxCross > maxCross) {
      maxPoint = other.maxPoint;
      maxCross = other.maxCross;
    }
  }

};

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
void hull_split(PointVector pointsIn, index_t n, PointVector pointsOut,
    index_t* hn, Point* p1, Point* p2);

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
void CowichanTBB::hull(PointVector pointsIn, PointVector pointsOut) {

  index_t hn = 0;
  index_t previous_hn = 0;

  // while not all points are used up then run quickhull on the rest of points
  while (n != hn) {
    // exclude added points from pointsIn by swapping them with points from the
    // end of pointsIn vector in range (0, n - nused)
    index_t added_i;
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

  // compute the convex hull of the points.
  
}

/*****************************************************************************/

namespace cowichan_tbb
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

  // use these as initial pivots
  hull_split (pointsIn, n, pointsOut, hn, minPoint, maxPoint);
  hull_split (pointsIn, n, pointsOut, hn, maxPoint, minPoint);
}

void hull_split (PointVector pointsIn, index_t n, PointVector pointsOut,
    index_t* hn, Point* p1, Point* p2) {

  // find the point with maximal signed distance from the line (p1,p2)
  MaximumDistance maximumDistance(pointsIn, *p1, *p2);
  parallel_reduce(Range(0, n), maximumDistance, auto_partitioner());

  Point* maxPoint = maximumDistance.getPoint();
  real maxCross = maximumDistance.getDistance();

  // is there a point in the positive half-space?
  // if so, it has maximal distance, and we must recurse based on that point.
  if (maxCross > 0.0) {
    // recurse on the new set with the given far point
    hull_split (pointsIn, n, pointsOut, hn, p1, maxPoint);
    hull_split (pointsIn, n, pointsOut, hn, maxPoint, p2);
    return;
  }

  // otherwise, it's not on the right side; we don't need to split anymore.
  // this is because all points are inside the hull when we use this
  // half-space. add the first point and return.
  pointsOut[(*hn)++] = *p1;

}

}

