/**
 * \file cowichan_tbb/half.cpp
 * \brief TBB half implementation.
 * \see CowichanTBB::half
 */

#include "cowichan_tbb.hpp"

namespace cowichan_tbb
{

/**
 * \brief This class does a halving shuffle.
 */
class Shuffle {
private:

  /**
   * Middle x.
   */
  index_t xBreak;
  
  /**
   * Middle y;
   */
  index_t yBreak;

  /**
   * Number of rows in the matrix.
   */
  index_t nr;
  
  /**
   * Number of columns in the matrix.
   */
  index_t nc;

  /**
   * Input matrix.
   */
  IntMatrix _input;
  
  /**
   * Output matrix.
   */
  IntMatrix _output;

public:
  
  /**
   * Construct a halving shuffle object.
   */
  Shuffle(IntMatrix input, IntMatrix output, index_t nr, index_t nc):
      _input(input), _output(output),
      nr(nr), nc(nc),
      xBreak((nc + 1) / 2),
      yBreak((nr + 1) / 2) {}

  /**
   * Performs the halving shuffle over the given range.
   * \param range two-dimensional range.
   */
  void operator()(const Range2D& range) const {
    
    IntMatrix input = _input;
    IntMatrix output = _output;
    const Range& rows = range.rows();
    const Range& cols = range.cols();
    
    index_t xSrc, ySrc;
    for (index_t y = rows.begin(); y != rows.end(); ++y) {
      for (index_t x = cols.begin(); x != cols.end(); ++x) {
        
        // calculate unswapped x co-ordinate.
        if (x < xBreak) {
          // odd columns
          xSrc = x * 2;
        } else {
          // even columns
          xSrc = (x - xBreak) * 2 + 1;
        }
        
        // calculate unswapped y co-ordinate.
        if (y < yBreak) {
          // odd rows
          ySrc = y * 2;
        } else {
          // even columns
          ySrc = (y - yBreak) * 2 + 1;
        }
              
        // assign new values in the output matrix.
        MATRIX_RECT(output, y, x) = MATRIX_RECT(input, ySrc, xSrc);
        
      }
    }
    
  }
};

}

/*****************************************************************************/

void CowichanTBB::half(IntMatrix matrixIn, IntMatrix matrixOut) {

  // perform the halving shuffle.
  Shuffle shuffle(matrixIn, matrixOut, nr, nc);
  parallel_for(Range2D(0, nr, 0, nc), shuffle,
      auto_partitioner());
    
}

