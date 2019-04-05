/**
 * \file cowichan_tbb/outer.cpp
 * \brief TBB outer implementation.
 * \see CowichanTBB::outer
 */

#include "cowichan_tbb.hpp"

namespace cowichan_tbb
{

/**
 * \brief Fills in matrix and vector with distances.
 */
class PointDistances {
  
  /**
   * Given points.
   */
  PointVector _points;
  
  /**
   * Matrix to fill.
   */
  Matrix _matrix;

  /**
   * Vector to fill.
   */
  Vector _vector;

  /**
   * Matrix size.
   */
  index_t n;

  /**
   * Maximum distance.
   */
  real _max;
  
public:

  /**
   * Construct point distances object.
   * \param points given points.
   * \param matrix matrix to fill.
   * \param vector vector to fill.
   * \param n matrix size.
   */
  PointDistances(PointVector points, Matrix matrix, Vector vector, index_t n)
      : _points(points), _matrix(matrix), _vector(vector), n(n), _max(-1) { }

  /**
   * Get maximum of the distances.
   * \return Max distance.
   */
  real getMaximum() const {
    return _max;
  }

  /**
   * Calculates inter-point distances on the given range.
   * \param rows range of rows to work on.
   */
  void operator()(const Range& rows) {
    
    PointVector points = _points;
    Matrix matrix = _matrix;
    Vector vector = _vector;
    
    for (index_t y = rows.begin(); y != rows.end(); ++y) {
      
      // compute distances from points to origin
      VECTOR(vector, y) = Point::distance(points[y], Point(0.0, 0.0));
    
      // compute distances between points
      for (index_t x = 0; x < y; ++x) {
        real d = Point::distance(points[x], points[y]);
        if (d > _max) {
          _max = d;
        }
        MATRIX(matrix, y, x) = MATRIX(matrix, x, y) = d;
      }
    }
  }
  
  /**
   * Splitting (TBB) constructor.
   * \param other object to split.
   */
  PointDistances(PointDistances& other, split) : _points(other._points),
      _matrix(other._matrix), _vector(other._vector), n(other.n), _max(-1) { }

  /**
   * Joiner (TBB).
   * \param other object to join.
   */
  void join(const PointDistances& other) {
    if (_max < other._max) {
      _max = other._max;
    }
  }
  
};


/**
 * \brief Makes a given matrix diagonally dominant.
 *
 * Modifies its diagonal elements.
 */
class MakeDominant {

  /**
   * Matrix to modify.
   */
  Matrix _matrix;

  /**
   * Matrix size.
   */
  index_t n;

  /**
   * Value to use for diagonal.
   */
  const real value;
  
public:

  /**
   * Construct a make dominant object.
   * \param matrix matrix to modify.
   * \param n matrix size.
   * \param value value for diagonal.
   */
  MakeDominant(Matrix matrix, index_t n, real value):
    _matrix(matrix), n(n), value(value) { }
  
  /**
   * Sets diagonal elements to a given constant.
   * \param rows range of rows to work on.
   */  
  void operator()(const Range& rows) const {
    Matrix matrix = _matrix;
    
    for (index_t i = rows.begin(); i != rows.end(); ++i) {
      DIAG(matrix, i) = value;
    }
  }
  
};

}

/*****************************************************************************/

void CowichanTBB::outer(PointVector points, Matrix matrix, Vector vector) {
  
  // figure out the matrix and vector
  PointDistances dist(points, matrix, vector, n);
  parallel_reduce(Range(0, n), dist, auto_partitioner());

  // fix up the diagonal
  MakeDominant dom(matrix, n, dist.getMaximum() * n);
  parallel_for(Range(0, n), dom, auto_partitioner());
  
}

