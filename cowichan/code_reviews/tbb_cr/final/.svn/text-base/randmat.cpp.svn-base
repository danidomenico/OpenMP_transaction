#include "cowichan.hpp"

/**
 * This class generates random integers in a matrix, using a simple linear
 * congruential number generator with a given seed value, i.e. using the
 * recurrence:
 * 
 * 		X_i+1 = (a*X_i + c) mod m
 *
 * The class also provides a parallel implementation with parallel_for.
 */
class RandomGenerator {

	const static uint a = 1103515245; // these two constants are values for the
	const static uint c = 12345;	  // linear congruential RNG algorithm
	const static uint m = 1024;		  // generate values 0-1023

  >>> the range of values that are generated should be a parameter to
  >>> RandomGenerator. 1024 seems low to me.

	uint s;							  // the initial, seed value.

private:

	IntMatrix _matrix;
	IntVector state;
	uint aPrime, cPrime;
	
	/**
	 * Generates the next random number.
	 * Convenience method for next(current, a, c);
	 */
	inline uint next(uint& current) const {
		return (a * current + c) % m;
	}
	
	/**
	 * Generates the k-th next random number, using an identity,
	 * 		where A = a^k mod m
	 *        and C = c * sum[j=0..k-1](a^j mod m).
	 */
	inline uint nextK(uint& current) const {
		return (aPrime * current + cPrime) % m;
	}
	
	/**
	 * Initialises the random number generator to operate on
	 * NROWS different rows completely independently.
	 */
	void initialise() {
	
		state = NEW_VECTOR(uint);
	
		// generate first column values
		VECTOR(state, 0) = s % m;
		for (int row = 1; row < Cowichan::NROWS; ++row) {
			VECTOR(state, row) = next(VECTOR(state, row - 1));
		}
		
		// generate the A and C values for the next(k) method.
		aPrime = a;
		cPrime = 1;
		for (int i = 1; i < Cowichan::NROWS; ++i) {
			cPrime = (cPrime + aPrime) % m;
			aPrime = (aPrime * a) % m;
		}
		cPrime = (cPrime * c) % m;
	
	}

public:

	/**
	 * Provides NROWS-seperated linear congruential RNG on the specified range
	 * of rows.
	 */
	void operator()(const Range& rows) const {
		
		IntMatrix matrix = _matrix;
		IntVector init = state;
		
		for (size_t row = rows.begin(); row != rows.end(); ++row) {
			
			// copy over the seed value for this row
			MATRIX_RECT(matrix, row, 0) = VECTOR(init, row);
			
			// for every other column, provide NROWS-spaced random numbers
			// using the specialty method RandomGenerator.nextK(current).

      >>> The comment above says for every other column... The loop is over
      >>> all columns. Is it taking about every other row in case of two
      >>> threads?

			for (int col = 1; col < Cowichan::NCOLS; ++col) {
				MATRIX_RECT(matrix, row, col) = nextK(MATRIX_RECT(matrix, row, col - 1));
			}
			
		}
		
	}

public:

	RandomGenerator(uint seed): s(seed) {
		initialise();
	}

	void executeParallel(IntMatrix matrix) {
		_matrix = matrix;
		parallel_for(Range(0, Cowichan::NROWS), *this, auto_partitioner());
	}
	
	void executeSerial(IntMatrix matrix) {
		_matrix = matrix;
		(*this)(Range(0, Cowichan::NROWS));
	}

};

/*****************************************************************************/

void Cowichan::randmat(IntMatrix* matrix) {
	
	*matrix = NEW_MATRIX_RECT(uint);
	
	// run the random number generator.
	RandomGenerator generator(Cowichan::SEED);
	generator.executeParallel(*matrix);

}

