/**
 * \file cowichan_tbb/vecdiff.cpp
 * \brief TBB vecdiff implementation.
 * \see CowichanTBB::vecdiff
 */

#include "cowichan_tbb.hpp"

namespace cowichan_tbb
{

/**
 * \brief This class takes the 1-norm of the difference between two vectors.
 */
class NormDiff {

  /**
   * First (actual) vector.
   */
  Vector _actual;

  /**
   * Second (computed) vector.
   */
  Vector _computed;

  /**
   * 1-norm difference.
   */
  real normDiff;

public:
  
  /**
   * Get 1-norm difference.
   */
  real getNormDiff() const {
    return normDiff;
  }
  
  /**
   * Construct a norm diff object.
   * \param actual first (actual) vector.
   * \param computed second (computed) vector.
   */
  NormDiff(Vector actual, Vector computed) : _actual(actual),
      _computed(computed), normDiff(0.0) { }

  /**
   * Perform the computation.
   * \param range row range.
   */
  void operator()(const Range& range) {
    Vector actual = _actual;
    Vector computed = _computed;

    for (index_t i = range.begin(); i != range.end(); ++i) {
    
      // get the element-wise difference; store the maximum
      real diff = (real)fabs((double)(actual[i] - computed[i]));

      if (diff > normDiff) {
        normDiff = diff;
      }
      
    }
  }
  
  /**
   * Splitting (TBB) constructor.
   * \param other object to split.
   */
  NormDiff(NormDiff& other, split):
    _actual(other._actual), _computed(other._computed), normDiff(0.0) { }
  
  /**
   * Joiner (TBB).
   * \param other object to join.
   */
  void join(const NormDiff& other) {
    if (normDiff < other.normDiff) {
      normDiff = other.normDiff;
    }
  }

};

}

/*****************************************************************************/

real CowichanTBB::vecdiff (Vector actual, Vector computed)
{
  NormDiff norm(actual, computed);
  parallel_reduce(Range(0, n), norm, auto_partitioner());

  return norm.getNormDiff();
}

