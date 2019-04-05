/**
 * \file cowichan_tbb/norm.cpp
 * \brief TBB norm implementation.
 * \see CowichanTBB::norm
 */

#include "cowichan_tbb.hpp"

namespace cowichan_tbb
{

/**
 * \brief Performs the minimum and maximum reductions.
 */
class MinMaxReducer {
private:

  /**
   * Input points.
   */
  PointVector pointsIn;

  /**
   * Min point.
   */
  Point minPoint;
  
  /**
   * Max point.
   */
  Point maxPoint;

public:

  /**
   * Construct a min/max reducer.
   * \param pointsIn input points.
   */
  MinMaxReducer(PointVector pointsIn) : pointsIn(pointsIn) { }

  /**
   * Get point of minimums.
   * \return Min point.
   */
  Point getMinimum() const {
    return minPoint;
  }

  /**
   * Get point of maximums.
   * \return Max point.
   */
  Point getMaximum() const {
    return maxPoint;
  }

  /**
   * Calculates the minimum and maximum co-ordinates over the given array
   * range.
   * \param range point range.
   */
  void operator()(const Range& range) {

    // get pointers locally.
    Point min = pointsIn[0];
    Point max = pointsIn[0];

    // calculate the minimum and maximum co-ordinates over the range.
    for (index_t i = range.begin(); i != range.end(); ++i) {
      if (pointsIn[i].x < min.x) {
        min.x = pointsIn[i].x;
      }
      if (pointsIn[i].y < min.y) {
        min.y = pointsIn[i].y;
      }
      if (pointsIn[i].x > max.x) {
        max.x = pointsIn[i].x;
      }
      if (pointsIn[i].y > max.y) {
        max.y = pointsIn[i].y;
      }
    }
    
    // refresh member variables.
    minPoint = min;
    maxPoint = max;
    
  }

  /**
   * Splitting (TBB) constructor.
   * \param other object to split.
   */
  MinMaxReducer(MinMaxReducer& other, split)
      : pointsIn(other.pointsIn) { }

  /**
   * Joiner (TBB).
   * \param other object to join.
   */
  void join(const MinMaxReducer& other) {
    if (other.minPoint.x < minPoint.x) {
      minPoint.x = other.minPoint.x;
    }
    if (other.minPoint.y < minPoint.y) {
      minPoint.y = other.minPoint.y;
    }
    if (other.maxPoint.x > maxPoint.x) {
      maxPoint.x = other.maxPoint.x;
    }
    if (other.maxPoint.y > maxPoint.y) {
      maxPoint.y = other.maxPoint.y;
    }
  }
  
};


/**
 * \brief Normalizes points.
 *
 * This class performs point normalization -- points are put onto the unit
 * square. 
 */
class Normalizer {
public:
  
  /**
   * Input points.
   */
  PointVector pointsIn;

  /**
   * Output points.
   */
  PointVector pointsOut;

  /**
   * Min x coordinate.
   */
  real minX;
  
  /**
   * Min y coordinate.
   */
  real minY;

  /**
   * x scaling factor.
   */
  real xfactor;
  
  /**
   * y scaling factor.
   */
  real yfactor;

public:

  /**
   * Construct a normalizer object.
   * \param pointsIn input points.
   * \param pointsOut output points.
   * \param minX min x coordinate.
   * \param minY min y coordinate.
   * \param xfactor scaling x factor.
   * \param yfactor scaling y factor.
   */
  Normalizer(PointVector pointsIn, PointVector pointsOut, real minX, real minY,
      real xfactor, real yfactor) : pointsIn(pointsIn), pointsOut(pointsOut),
      minX(minX), minY(minY), xfactor(xfactor), yfactor(yfactor) { }

  /**
   * Performs normalization over given range
   * \param range range of points.
   */
  void operator()(const Range& range) const {

    // normalize the points that lie in the given range.
    for (index_t i = range.begin(); i != range.end(); ++i) {

      pointsOut[i].x = xfactor * (pointsIn[i].x - minX);
      pointsOut[i].y = yfactor * (pointsIn[i].y - minY);

    }
    
  }
  
};

}

/*****************************************************************************/

void CowichanTBB::norm(PointVector pointsIn, PointVector pointsOut) {

  MinMaxReducer minmax(pointsIn);

  // find min/max coordinates
  parallel_reduce(Range(0, n), minmax, auto_partitioner());

  Point minPoint = minmax.getMinimum();
  Point maxPoint = minmax.getMaximum();

  // compute scaling factors
  real xfactor = (real)((maxPoint.x == minPoint.x) ?
      0.0 : 1.0 / (maxPoint.x - minPoint.x));
  real yfactor = (real)((maxPoint.y == minPoint.y) ?
      0.0 : 1.0 / (maxPoint.y - minPoint.y));

  Normalizer normalizer(pointsIn, pointsOut, minPoint.x, minPoint.y, xfactor,
      yfactor);

  // normalize the vector
  parallel_for(Range(0, n), normalizer, auto_partitioner());

}

