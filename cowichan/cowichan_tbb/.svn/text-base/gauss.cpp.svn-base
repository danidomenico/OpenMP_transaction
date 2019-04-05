/**
 * \file cowichan_tbb/gauss.cpp
 * \brief TBB gauss implementation.
 * \see CowichanTBB::gauss
 */

#include "cowichan_tbb.hpp"

namespace cowichan_tbb
{

/** 
 * \brief Performs row elimination, i.e. eliminate i-th column in j-th row.
 */
class RowElimination {
private:

  /**
   * Matrix.
   */
  Matrix _matrix;

  /**
   * Target vector.
   */
  Vector _target;

  /**
   * Matrix size.
   */
  index_t n;

  /**
   * Current row index.
   */
  index_t i;

  /**
   * Current i-th row i-th column value.
   */
  real column_i;

public:

  /**
   * Construct a row elimination object.
   * \param matrix matrix.
   * \param target target vector.
   * \param n matrix size.
   */
  RowElimination(Matrix matrix, Vector target, index_t n) : _matrix(matrix),
      _target(target), n(n) { };

  /**
   * Set current row index.
   */
  void setI(index_t i) {
    this->i = i;
    column_i = MATRIX(_matrix, i, i);
  }

  /**
   * Perform row elimination on a range of rows.
   * \param rows range of rows.
   */
  void operator()(const Range& rows) const {
    
    // Get pointers locally.
    Matrix matrix = _matrix;
    Vector target = _target;
    
    for (index_t j = rows.begin(); j != rows.end(); ++j) {
      real factor = -(MATRIX(matrix, j, i) / column_i);
      for (index_t k = n - 1; k >= i; k--) {
        MATRIX(matrix, j, k) += MATRIX(matrix, i, k) * factor;
      }
      target[j] += target[i] * factor;
    }
  }
  
};

}

void CowichanTBB::gauss (Matrix matrix, Vector target, Vector solution)
{
  index_t i, j, k;

  RowElimination rowElimination(matrix, target, n);

  // forward elimination
  for (i = 0; i < n; i++) {
    // get row with maximum column i
    index_t max = i;
    for (j = i + 1; j < n; j++) {
      if (fabs(MATRIX(matrix, j, i)) > fabs(MATRIX(matrix, max, i))) {
        max = j;
      }
    }

    real tmp;
    // swap max row with row i
    for (j = i; j < n; j++) {
      tmp = MATRIX(matrix, i, j);
      MATRIX(matrix, i, j) = MATRIX(matrix, max, j);
      MATRIX(matrix, max, j) = tmp;
    }
    tmp = target[i];
    target[i] = target[max];
    target[max] = tmp;

    // eliminate i-th column in rows (i + 1, n)
    rowElimination.setI(i);
    parallel_for(Range(i + 1, n), rowElimination, auto_partitioner());
  }

  // back substitution
  for (k = (n - 1); k >= 0; k--) {
    solution[k] = target[k] / MATRIX_SQUARE(matrix, k, k);
    for (i = k - 1; i >= 0; i--) {
      target[i] = target[i] - (MATRIX_SQUARE(matrix, i, k) * solution[k]);
    }
  }
}

