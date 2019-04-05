/**
 * \file cowichan_tbb/invperc.cpp
 * \brief TBB (NOT parallel) invperc implementation.
 *
 * The same as CowichanSerial::invperc
 */

#include "cowichan_tbb.hpp"
#include <queue>

namespace cowichan_tbb
{

/**
 * \brief Percolation point.
 *
 * These are put into a heap to extract the one with lowest value.
 */
class PercPoint {
public:

  /**
   * Point.
   */
  Point point;

  /**
   * Matrix.
   */
  IntMatrix matrix;

  /**
   * Number of columns.
   */
  index_t nc;

public:

  /**
   * Construct a percolation point.
   * \param point point.
   */
  PercPoint(Point point): point(point) { }

  /**
   * Less than comparison - we want to extract lowest values.
   * \param other right hand side percolation point.
   */
  bool operator<(const PercPoint &other) const {
    return value() > other.value();
  }

  /**
   * Get the value at this point.
   * \return value from the matrix.
   */
  INT_TYPE value() const {
    return MATRIX_RECT(matrix, (index_t)point.y, (index_t)point.x);
  }

};

}

/*****************************************************************************/

void CowichanTBB::invperc(IntMatrix matrix, BoolMatrix mask) {
  
  if (nr * nc < invpercNFill) {
    not_enough_points();
  }

  PercPoint pp(Point(0, 0));
  pp.matrix = matrix;
  pp.nc = nc;
  
  // "seed" with the middle value; start a priority queue.
  std::vector<PercPoint> points;

  PercPoint initialPoint(Point((real) (nc / 2), (real) (nr / 2)));
  initialPoint.matrix = matrix;
  initialPoint.nc = nc;
  points.push_back(initialPoint);
  std::make_heap(points.begin(), points.end());

  // perform invasion percolation nfill times.
  index_t r, c;
  for (index_t it = 0; it < invpercNFill; ++it) {

    // get the highest-priority point that hasn't already
    // been filled.
    do {
      std::pop_heap(points.begin(), points.end());
      pp = points.back();
      points.pop_back();
      r = (index_t)pp.point.y;
      c = (index_t)pp.point.x;
    } while (MATRIX_RECT(mask, r, c)); // find a free one

    // fill it.
    MATRIX_RECT(mask, r, c) = true;

    // add all of its neighbours to the party...
    
    // top neighbour
    if (r > 0) {
      PercPoint topPoint(Point((real)c, (real)r - 1));
      topPoint.matrix = matrix;
      topPoint.nc = nc;
      points.push_back(topPoint);
      push_heap(points.begin(), points.end());
    }
    
    // bottom neighbour
    if (r < (nr - 1)) {
      PercPoint bottomPoint(Point((real)c, (real)r + 1));
      bottomPoint.matrix = matrix;
      bottomPoint.nc = nc;
      points.push_back(bottomPoint);
      push_heap(points.begin(), points.end());
    }
    
    // left neighbour
    if (c > 0) {
      PercPoint leftPoint(Point((real)c - 1, (real)r));
      leftPoint.matrix = matrix;
      leftPoint.nc = nc;
      points.push_back(leftPoint);
      push_heap(points.begin(), points.end());
    }
    
    // right neighbour
    if (c < (nc - 1)) {
      PercPoint rightPoint(Point((real)c + 1, (real)r));
      rightPoint.matrix = matrix;
      rightPoint.nc = nc;
      points.push_back(rightPoint);
      push_heap(points.begin(), points.end());
    }
    
  }
  
}

