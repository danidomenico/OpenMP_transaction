#include "cowichan.hpp"

/**
 * This class does a halving shuffle.
 */
class Shuffle {

public:
	
	IntMatrix _first, _second;

  >>> Two matrices are used. This doubles the amount of memory required
  >>> compared to sequential algorithm.

	Shuffle(IntMatrix input, IntMatrix output):
		_first(input), _second(output) { }

	/**
	 * Performs the halving shuffle over the given range.
	 */
	void operator()(const Range2D& range) const {
		
		IntMatrix first = _first;
		IntMatrix second = _second;
		const Range& rows = range.rows();
		const Range& cols = range.cols();
		
		for (size_t y = rows.begin(); y != rows.end(); ++y) {
			for (size_t x = cols.begin(); x != cols.end(); ++x) {
				
				// assign from swapped co-ordinates.
				MATRIX_RECT(second, y, x) = MATRIX_RECT(first, y ^ 1, x ^ 1);
				
			}
		}
		
	}
};

/*****************************************************************************/

void Cowichan::half(IntMatrix matrixIn, IntMatrix* matrixOut) {

	// allocate the output matrix.
	*matrixOut = NEW_MATRIX_RECT(uint);

	// perform the halving shuffle.
	Shuffle shuffle(matrixIn, *matrixOut);
	parallel_for(Range2D(0, NROWS, 0, NCOLS), shuffle,
		auto_partitioner());
		
}

