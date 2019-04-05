#include "cowichan.hpp"

class Mandelbrot {

	IntMatrix _matrix;		// to store the result.

	real dX, dY;			// co-ordinate -> complex plane mapping coeff.
	real baseX, baseY;		// where to start the mandelbrot set

private:

	/**
 	 * Performs the mandelbrot set calculation.
 	 */
	int mandelCalc(real x, real y) const {

		real r = 0.0, i = 0.0;
		real rs = 0.0, is = 0.0;
		int numIterations = 0;		
		do {
		
			// calculate the complex value according to the mandelbrot set specs.
			i = (2.0 * r * i) + x;
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
 	 * Calculates the given mandelbrot set "window", and stores the result in matrix.
 	 */
	static void exec(IntMatrix matrix, real x, real y, real width, real height) {
		
		Mandelbrot mandel(matrix, x, y, width, height);
		parallel_for(Range2D(0, Cowichan::NROWS, 0, Cowichan::NCOLS), mandel,
			auto_partitioner());
		
	}
	
	
public:

	Mandelbrot(IntMatrix matrix, real x, real y, real width, real height):
		_matrix(matrix), baseX(x), baseY(y) {
		
		dX = width / (Cowichan::NCOLS - 1);
		dY = height / (Cowichan::NROWS - 1);
			
	}

	/**
 	 * Calculates a given portion of the current mandelbrot set "window".
 	 */
	void operator()(const Range2D& range) const {

		IntMatrix matrix = _matrix;

		const Range& rows = range.rows();
		const Range& cols = range.cols();
		
    >>> Task stealing should avoid the load imbalance theoretically.
    >>> How effective is it in this case when we set MANDEL_MAX_ITER to some
    >>> large number?

		for (size_t y = rows.begin(); y != rows.end(); ++y) {
			for (size_t x = cols.begin(); x != cols.end(); ++x) {
				MATRIX_RECT(matrix, y,x) = mandelCalc(baseX + (x * dX), baseY + (y * dY));
			}
		}
		
	}
	
};

/*****************************************************************************/

void Cowichan::mandel(IntMatrix* matrix) {

	*matrix = NEW_MATRIX_RECT(uint);
	Mandelbrot::exec(*matrix, x0, y0, dx, dy);

}

