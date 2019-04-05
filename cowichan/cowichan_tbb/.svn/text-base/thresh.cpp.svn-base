/**
 * \file cowichan_tbb/thresh.cpp
 * \brief TBB thresh implementation.
 * \see CowichanTBB::thresh
 */

#include "cowichan_tbb.hpp"

namespace cowichan_tbb
{

/**
 * \brief Finds the maximum value in the image.
 */
class MaxReducer {
private:

  /**
   * Image matrix.
   */
  IntMatrix _image;

  /**
   * Maximum value.
   */
  INT_TYPE _max;

  /**
   * Number of columns in the matrix.
   */
  const index_t nc;

public:

  /**
   * Constructs a max reducer. Initialise max with the lowest possible value.
   * \param image image matrix.
   * \param nc number of matrix columns.
   */
  MaxReducer(IntMatrix image, index_t nc):
    _image(image), _max(MINIMUM_INT), nc(nc) { }

  /**
   * Get maximum value.
   * \return Maximum value.
   */
  INT_TYPE getMaximum() const {
    return _max;
  }

  /**
   * Calculates the maximum value over the given range.
   * \param range row/column range.
   */
  void operator()(const Range2D& range) {

    IntMatrix image = _image;

    const Range& rows = range.rows();
    const Range& cols = range.cols();
    
    for (index_t y = rows.begin(); y != rows.end(); ++y) {
      for (index_t x = cols.begin(); x != cols.end(); ++x) {
        if (_max < MATRIX_RECT(image, y, x)) {
          _max = MATRIX_RECT(image, y, x);
        }
      }
    }
    
  }
  
  /**
   * Splitting (TBB) constructor.
   * \param other object to split.
   */
  MaxReducer(MaxReducer& other, split) : _image(other._image),
      _max(MINIMUM_INT), nc(other.nc) { }

  /**
   * Joiner (TBB).
   * \param other object to join.
   */
  void join(const MaxReducer& other) {
    if (_max < other._max) {
      _max = other._max;
    }
  }
  
};

/**
 * \brief This class calculates the histogram of a matrix.
 */
class Histogram {
  
  /**
   * Image matrix.
   */
  IntMatrix _image;
  
  /**
   * Histogram values.
   */
  index_t* histogram;

  /** 
   * Number of bins in the histogram.
   */
  const index_t bins;
  
  /**
   * Number of rows in the matrix.
   */
  const index_t nr;
  
  /**
   * Number of columns in the matrix.
   */
  const index_t nc;

public:

  /**
   * Construct a histogram object.
   * \param image image matrix.
   * \param maxValue number of bins to use.
   * \param nr number of rows in the matrix.
   * \param nc number of columns in the matrix.
   */
  Histogram(IntMatrix image, INT_TYPE maxValue, index_t nr, index_t nc)
      : _image(image), bins(maxValue), nr(nr), nc(nc)
  {
    histogram = new index_t[maxValue + 1];

    for (index_t i = 0; i <= (index_t)maxValue; ++i) {
      histogram[i] = 0;
    }
  }
  
  /**
   * Get retention value.
   * \param cutoff percentage of values to retain.
   * \return Retention value.
   */
  index_t getValue(real cutoff) const {
    index_t i;
    index_t retain = (index_t)(cutoff * nc * nr);

    for (i = bins; (i >= 0) && (retain > 0); --i) {
      retain -= histogram[i];
    }

    return i;
  }
  
  /**
   * Histogram calculation.
   * \param range row/column range to work on.
   */
  void operator()(const Range2D& range) const {
    IntMatrix image = _image;
    index_t* hist = histogram;
    
    const Range& rows = range.rows();
    const Range& cols = range.cols();
    
    for (index_t y = rows.begin(); y != rows.end(); ++y) {
      for (index_t x = cols.begin(); x != cols.end(); ++x) {
        hist[MATRIX_RECT(image, y, x)]++;
      }
    }
  }
  
  /**
   * Splitting (TBB) constructor.
   * \param other object to split.
   */
  Histogram(Histogram& other, split): _image(other._image), bins(other.bins),
      nr(other.nr), nc(other.nc)
  {
    histogram = new index_t[bins + 1];

    for (index_t i = 0; i <= bins; ++i) {
      histogram[i] = 0;
    }    
  }
  
  /**
   * Joiner (TBB).
   * \param other object to join.
   */
  void join(const Histogram& other) {
    // SERIAL... can we speed up by parallel_for here, too?
    for (index_t i = 0; i < bins; ++i) {
      histogram[i] += other.histogram[i];
    }
  }
  
};

/**
 * \brief Creates a boolean array based on a cut-off value.
 */
class Threshold {

  /**
   * Image matrix.
   */
  IntMatrix _image;
  
  /**
   * Resulting boolean matrix.
   */
  BoolMatrix _result;

  /**
   * Retention value.
   */
  const index_t retain;

  /**
   * Number of columns in the matrix.
   */
  const index_t nc;

public:

  /**
   * Perform thresholding.
   * \param range row/column range to operate on.
   */
  void operator()(const Range2D& range) const {
    IntMatrix image = _image;
    BoolMatrix result = _result;
    
    const Range& rows = range.rows();
    const Range& cols = range.cols();
    
    for (index_t y = rows.begin(); y != rows.end(); ++y) {
      for (index_t x = cols.begin(); x != cols.end(); ++x) {
        MATRIX_RECT(result, y, x) =
            ((index_t)MATRIX_RECT(image, y, x)) > retain;
      }
    }
  }
  
  /**
   * Construct a threshold object.
   * \param image image matrix.
   * \param retain retention value.
   * \param result result matrix to fill.
   * \param nc number of columns in the matrix.
   */
  Threshold(IntMatrix image, index_t retain, BoolMatrix result, index_t nc):
    _image(image), _result(result), retain(retain), nc(nc) { }

};

}

/*****************************************************************************/

/**
 * Works only on positive input.
 */
void CowichanTBB::thresh(IntMatrix matrix, BoolMatrix mask) {
  
  // get the maximum value in the matrix (need 0-that number of bins)
  MaxReducer reducer(matrix, nc);
  parallel_reduce(Range2D(0, nr, 0, nc), reducer, auto_partitioner());
  INT_TYPE max = reducer.getMaximum();
  
  // compute the histogram to get a thresholding value
  Histogram hist(matrix, max, nr, nc);
  parallel_reduce(Range2D(0, nr, 0, nc), hist, auto_partitioner());
  
  // perform the thresholding opearation
  Threshold thresh(matrix, hist.getValue(THRESH_PERCENT), mask, nc);
  parallel_for(Range2D(0, nr, 0, nc), thresh, auto_partitioner());

}

