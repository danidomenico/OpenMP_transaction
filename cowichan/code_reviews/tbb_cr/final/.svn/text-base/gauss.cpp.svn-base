#include "cowichan.hpp"

/**
 * This class performs Gauss-Jordan elimination on a matrix. It computes the
 * modified answer vector and the rank of the matrix.
 */
class GaussJordan {
public:
	
	Matrix _matrix;
	Vector _vector;

public:
	
	/**
	 * Performs the forward-eliminiation phase (=> row-echelon).
	 * Expects a pivoted matrix as input (but makes no other assumptions).
	 */

  >>> Uh oh, I am not good at math. What does expects a pivoted matrix as
  >>> input mean?

	class ForwardElimination {
	public:
	
		GaussJordan* parent;
		ForwardElimination(GaussJordan* parent): parent(parent) { };
		
		void operator()(const Range& rows) const {
			
			// Get pointers locally.
			Matrix matrix = parent->_matrix;
			Vector vector = parent->_vector;
			
			for (size_t row = rows.begin(); row != rows.end(); ++row) {
				
				// for each preceding row...
				for (int previous = 0; previous < row; ++previous) {
					
					// subtract out that row scaled by a factor to introduce a zero
					real factor = MATRIX(matrix, row, previous) / MATRIX(matrix, previous, previous);
					for (int col = 0; col < Cowichan::NELTS; ++col) {
						MATRIX(matrix, row, col) -= factor * MATRIX(matrix, previous, col);
					}
					VECTOR(vector, row) -= factor * VECTOR(vector, previous);
					
					// should be zero anyway, but because of rounding issues...					
					MATRIX(matrix, row, previous) = 0.0;
					
				}
							
				// re-normalize the remaining values
				for (int col = row + 1; col < Cowichan::NELTS; ++col) {
					MATRIX(matrix, row, col) /= DIAG(matrix, row);
				}
				VECTOR(vector, row) /= DIAG(matrix, row);
				
				// rounding issues again.
				DIAG(matrix, row) = 1.0;
				
			}
		}
		
	};

	/** 
	 * Performs the back-substitution phase (=> reduced row-echelon)
	 */
	class BackSubstitution {
	public:
		
		GaussJordan* parent;
		BackSubstitution(GaussJordan* parent): parent(parent) { };

		void operator()(const blocked_range<size_t>& rows) const {
			
			// Get pointers locally.
			Matrix matrix = parent->_matrix;
			Vector vector = parent->_vector;
			
			for (size_t row = rows.begin(); row != rows.end(); ++row) {
	
				// for each following row...
				for (int next = row + 1; next < Cowichan::NELTS; ++next) {

					// subtract out that row scaled by a factor to introduce a zero
					real factor = MATRIX(matrix, row, next);
					for (int col = row + 1; col < Cowichan::NELTS; ++col) {
						MATRIX(matrix, row, col) -= factor * MATRIX(matrix, next, col);
					}
					VECTOR(vector, row) -= factor * VECTOR(vector, next);
					
					// rounding issues again (see ForwardElimination::operator()).
					MATRIX(matrix, row, next) = 0.0;
			
				}
	
			}			
		}
		
	};
	
	ForwardElimination forward;
	BackSubstitution backward;

public:

	GaussJordan(Matrix matrix, Vector vector):
		forward(this), backward(this),
		_matrix(matrix), _vector(vector)
		{ }
		
	/**
	 * Performs Gauss-Jordan elimination to solve Ax = b, given A and b.
	 * @param A the matrix. This matrix is modified to become the identity matrix,
	 *          and may be rank-deficient (of course, based on A!)
	 * @param b the vector. This vector is modified to become the answer.
	 */
	static void perform(Matrix A, Vector b) {
		
		GaussJordan gauss(A, b);
		
		// pivoting must be done in series.
		gauss.pivot();
		
		// the forward/backward phases can be split up among the workers.
		parallel_for(Range(0, Cowichan::NELTS), gauss.forward, auto_partitioner());
		parallel_for(Range(0, Cowichan::NELTS), gauss.backward, auto_partitioner());
		
		// b is transformed to become the answer, so we needn't return anything.
		return;
		
	}

private:

	/**
	 * Performs sequential partial pivoting of the current matrix
	 * and the associated "b" vector.
	 */
	void pivot() {
		
		// Get pointers locally.
		Matrix matrix = _matrix;
		Vector vector = _vector;
			
		for (int col = 0; col < Cowichan::NELTS; ++col) {
	
			// compute the pivot for this column
			int pivotRow = 0, maxValue = 0;
			for (int row = col; row < Cowichan::NELTS; ++row) {
				if (abs(MATRIX(matrix, row, col)) > maxValue) {
					pivotRow = row;
				}
			}
		
			// swap rows (col) and (pivotRow)
			std::swap_ranges(
				matrix + (col * Cowichan::NELTS),
				matrix + ((col+1) * Cowichan::NELTS),
				matrix + (pivotRow * Cowichan::NELTS));

			// swap vector rows, as well
			real temp = VECTOR(vector, col);
			VECTOR(vector, col) = VECTOR(vector, pivotRow);
			VECTOR(vector, pivotRow) = temp;
			
		}
	}
	
};

/*****************************************************************************/

void Cowichan::gauss(Matrix matrix, Vector target, Vector* solution) {

	// copy target into solution, as GaussJordan performs in-place.
	*solution = NEW_VECTOR(real);
	std::copy(target, target + NELTS, *solution);

	// multiply the matrix by the vector (NB. answer == vector).
	GaussJordan::perform(matrix, *solution);
	
}

