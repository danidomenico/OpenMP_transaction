/**
 * \file cowichan_tbb/product.cpp
 * \brief TBB product implementation.
 * \see CowichanTBB::product
 */

#include "cowichan_tbb.hpp"

namespace cowichan_tbb
{

/**
 * \brief Performs matrix vector product.
 *
 * This class multiplies a matrix by a vector, producing a vector, a la:
 *  [x1 x2 x3]   [y1]   [(x1*y1 + x2*y2 + x3*y3)]
 *  [x4 x5 x6] * [y2] = [(x4*y1 + x5*y2 + x6*y3)]
 *  [x7 x8 x9]   [y3]   [(x7*y1 + x8*y2 + x9*y3)]
 */
class Product {
  
  /**
   * Given matrix.
   */
  Matrix _matrix;

  /**
   * Given vector.
   */
  Vector _vector;
  
  /**
   * Solution vector.
   */
  Vector _result;

  /**
   * Matrix size.
   */
  index_t n;

public:

  /**
   * Construct a product object.
   * \param matrix given matrix.
   * \param vector given vector.
   * \param result solution vector.
   * \param n matrix size.
   */
  Product(Matrix matrix, Vector vector, Vector result, index_t n):
    _matrix(matrix), _vector(vector), _result(result), n(n) { }

  /**
   * Performs matrix-vector multiplication on the given row range.
   * \param rows range of rows to use.
   */
  void operator()(const Range& rows) const {
    
    Matrix matrix = _matrix;
    Vector vector = _vector;
    Vector result = _result;
    
    for (index_t row = rows.begin(); row != rows.end(); ++row) {
      
      VECTOR(result, row) = 0.0;
      for (index_t col = 0; col < n; ++col) {
        VECTOR(result, row) += MATRIX(matrix, row,col) * VECTOR(vector, col);
      }
      
    }
    
  }
};

}

/*****************************************************************************/

void CowichanTBB::product(Matrix matrix, Vector candidate, Vector solution)
{
  Product product(matrix, candidate, solution, n);
  parallel_for(Range(0, n), product, auto_partitioner());
}

