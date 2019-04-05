/**
 * \file cowichan_tbb/winnow.cpp
 * \brief TBB winnow implementation.
 * \see CowichanTBB::winnow
 */

#include "cowichan_tbb.hpp"

namespace cowichan_tbb
{

/**
 * \brief Calculates the number of points to sort for winnow.
 */
class PointCount {
private:

  /**
   * Candidate points matrix.
   */
  IntMatrix _candidates;

  /**
   * Mask.
   */
  BoolMatrix _mask;

  /**
   * Number of columns in the matrix.
   */
  index_t nc;

  /**
   * Number of points to sort.
   */
  index_t count;

public:

  /**
   * Return point count.
   */
  index_t getCount()
  {
    return count;
  }

  /**
   * Construct a point count object.
   * \param candidates candidate points matrix.
   * \param mask mask.
   * \param nc number of columns in the matrix.
   */
  PointCount(IntMatrix candidates, BoolMatrix mask, index_t nc):
    _candidates(candidates), _mask(mask), nc(nc), count(0) { }

  /**
   * Calculate number of points to sort (TBB).
   * \param range row/column range to work on.
   */
  void operator()(const Range2D& range) {
    
    // bring pointers into cache
    const BoolMatrix mask = _mask;
    const IntMatrix candidates = _candidates;
    const Range& rows = range.rows();
    const Range& cols = range.cols();

    // count values marked as good by the mask.
    for (index_t y = rows.begin(); y != rows.end(); ++y) {
      for (index_t x = cols.begin(); x != cols.end(); ++x) {
        
        if (MATRIX_RECT(mask, y, x)) {
          count++;
        }
        
      }
    }
    
  }
  
  /**
   * Splitting (TBB) constructor.
   * \param other object to split.
   */
  PointCount(PointCount& other, split) : _candidates(other._candidates),
      _mask(other._mask), nc(other.nc), count(0) { }

  /**
   * Joiner (TBB).
   * \param other object to join.
   */
  void join(const PointCount& other) {
    count += other.count;
  }
  
};

}

/*****************************************************************************/

void CowichanTBB::winnow(IntMatrix matrix, BoolMatrix mask,
    PointVector points) {

  // count points to sort
  PointCount pc(matrix, mask, nc);
  parallel_reduce(Range2D(0, nr, 0, nc), pc, auto_partitioner());

  index_t len = pc.getCount();

  WeightedPointVector weightedPoints = NULL;
  try {
    weightedPoints = NEW_VECTOR_SZ(WeightedPoint, len);
  }
  catch (...) {out_of_memory();}

  index_t i = 0;

  // fill in weighted points
  for (index_t y = 0; y != nr; ++y) {
    for (index_t x = 0; x != nc; ++x) {
      if (MATRIX_RECT(mask, y, x)) {
        weightedPoints[i++] = WeightedPoint((real)x, (real)y,
            MATRIX_RECT(matrix, y, x));
      }
    }
  }

#ifdef SORT_TIME
  INT64 start, end;
  start = get_ticks ();
#endif

  // sort the extracted points
  parallel_sort(weightedPoints, &weightedPoints[len]);

#ifdef SORT_TIME
  end = get_ticks ();
#endif

  index_t stride, j;

  // copy over points
  stride = len / n;

  for (i = n - 1, j = len - 1; i >= 0; i--, j -= stride) {
#ifdef WINNOW_OUTPUT
    std::cout << weightedPoints[j].weight << "\n";
#endif
    points[i] = weightedPoints[j].point;
  }
  
#ifdef SORT_TIME
  std::cout << "winnow sort: ";
  print_elapsed_time(start, end);
  std::cout << std::endl;
#endif

}

