/**
 * \file cowichan_tbb/mandel.cpp
 * \brief TBB mandel implementation.
 * \see CowichanTBB::mandel
 */

#include "cowichan_tbb.hpp"

namespace cowichan_tbb
{

/**
 * \brief Perform mandelbrot generation.
 */
class Mandelbrot {
private:

  /**
   * Matrix.
   */
  IntMatrix _matrix;    // to store the result.

  /**
   * Number of rows in the matrix.
   */
  index_t nr;
  
  /**
   * Number of columns in the matrix.
   */
  index_t nc;

  /**
   * x-coordinate of the lower left corner.
   */
  real baseX;
  
  /**
   * y-coordinate of the lower left corner.
   */
  real baseY;

  /**
   * Extent of the region along the x axis.
   */
  real dX;
  
  /**
   * Extent of the region along the y axis.
   */
  real dY;

private:

  /**
   * Performs the mandelbrot value calculation.
   * \param x x-coordinate.
   * \param y y-coordinate.
   */
  INT_TYPE mandelCalc(real x, real y) const {

    real r = 0.0, i = 0.0;
    real rs = 0.0, is = 0.0;
    INT_TYPE numIterations = 0;    
    do {
    
      // calculate the complex value according to the mandelbrot set specs.
      i = (((real)2.0) * r * i) + x;
      r = (rs - is) + y;
      
      // calculate squared complex value
      rs = r * r;
      is = i * i;
      
      // "step" the simulation for this co-ordinate.
      ++numIterations;      
      
    } while ((numIterations < MANDEL_MAX_ITER) && ((rs + is) < MANDEL_INFINITY));
    
    // we are interested if the series converges or diverges. Return the
    // number of iterations before such an event (divergence).
    return numIterations;
    
  }
  
public:

  /**
   * Construct a mandelbrot generation object.
   * \param matrix matrix to fill.
   * \param nr number of rows.
   * \param nc number of columns.
   * \param x base x.
   * \param y base y.
   * \param width width.
   * \param height height.
   */
  Mandelbrot(IntMatrix matrix, index_t nr, index_t nc, real x, real y,
      real width, real height) : _matrix(matrix), nr(nr), nc(nc), baseX(x),
      baseY(y) {
    
    dX = width / (nc - 1);
    dY = height / (nr - 1);
      
  }

  /**
   * Calculates a given portion of the current mandelbrot set "window".
   * \param range two-dimensional range.
   */
  void operator()(const Range2D& range) const {

    IntMatrix matrix = _matrix;

    const Range& rows = range.rows();
    const Range& cols = range.cols();
    
    for (index_t y = rows.begin(); y != rows.end(); ++y) {
      for (index_t x = cols.begin(); x != cols.end(); ++x) {
        MATRIX_RECT(matrix, y, x) = mandelCalc(baseX + (x * dX), baseY + (y * dY));
      }
    }
    
  }
  
};

}

/*****************************************************************************/

void CowichanTBB::mandel(IntMatrix matrix)
{
  Mandelbrot mandel(matrix, nr, nc, mandelX0, mandelY0, mandelDx, mandelDy);

  parallel_for(Range2D(0, nr, 0, nc), mandel,
    auto_partitioner());
}

