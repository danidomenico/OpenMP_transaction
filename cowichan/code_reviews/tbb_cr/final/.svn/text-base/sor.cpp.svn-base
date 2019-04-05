#include "cowichan.hpp"

/**
 * Computes, for each row, the summed products of matrix and answer elements,
 * and stores the result in a pre-defined "sums" vector.
 */
class RowSummer {

	Matrix _matrix;
	Vector _sums;
	Vector _answer;

public:

	static void perform(const Matrix matrix, const Vector answer, const Vector sums) {
		RowSummer sum(matrix, answer, sums);
		parallel_for(
			Range(0, Cowichan::NELTS),
			sum,
			auto_partitioner());
	}
	
public:

	RowSummer(Matrix matrix, Vector answer, Vector sums):
		_matrix(matrix), _answer(answer), _sums(sums) { }
		
	void operator()(const Range& rows) const {

		Matrix matrix = _matrix;
		Vector sums   = _sums;
		Vector answer = _answer;

		for (size_t y = rows.begin(); y != rows.end(); ++y) {
			VECTOR(sums, y) = 0.0;
			for (size_t x = 0; x < Cowichan::NELTS; ++x) {
				if (x != y) {
					VECTOR(sums, y) += MATRIX_SQUARE(matrix, y, x) * VECTOR(answer, x);
				}
			}
		}
	
	}

};

/**
 * Performs the successive over-relaxation process.
 */
class Relaxer {

	Matrix _matrix;
	Vector _vector;
	Vector _sums;
	Vector _answer;
	real norm;

public:

	/**
	 * Performs the SOR process. Requires that answer already contains a guess.
	 */
	static void performSOR(Matrix matrix, Vector vector, Vector answer, real tolerance) {
				
		int numIterations = 0;		

		Relaxer* relaxer = NULL;
		do {
		
			// gather row sums for the SOR process
			Vector sums = NEW_VECTOR(real);
			RowSummer::perform(matrix, answer, sums);
			
			// "relax" the answer vector, while at the same time calculating tolerance
			if (relaxer) delete relaxer;
			relaxer = new Relaxer(matrix, vector, answer, sums);
			parallel_reduce(
				Range(0, Cowichan::NELTS),
				*relaxer,
				auto_partitioner());
			
			// increase the iteration counter
			++numIterations;

		} while (relaxer->getNorm() >= tolerance);
		
		// DEBUG: print the number of iterations.

    >>> Debug statements should be surrounded by an ifdef DEBUG or something.

		std::cout << "SOR in " << numIterations << " iterations." << std::endl;
		
		// make sure we get rid of the final Relaxer instance.
		delete relaxer;
		
	}

public:

	real getNorm() const {
		return norm;
	}

	void operator()(const Range& rows) {

		Matrix matrix = _matrix;
		Vector vector = _vector;
		Vector answer = _answer;
		Vector sums   = _sums;

		for (size_t y = rows.begin(); y != rows.end(); ++y) {

			// save the old answer
			real oldAnswer = VECTOR(answer,y);

			// blend between the old answer and the new answer
			VECTOR(answer,y) =
				(1.0 - SOR_OMEGA) * oldAnswer +
				SOR_OMEGA 		  * (VECTOR(vector,y) - VECTOR(sums,y)) / MATRIX(matrix, y,y);

			// store the magnitude of the quickest change (1-norm)
			norm = std::max(norm, (real) fabs(oldAnswer - VECTOR(answer,y)));

		}
		
	}

	Relaxer(Matrix matrix, Vector vector, Vector answer, Vector sums):
		_matrix(matrix), _vector(vector), _sums(sums), _answer(answer), norm(0.0)
		{ }

	Relaxer(Relaxer& p, split):
		_matrix(p._matrix), _vector(p._vector), _sums(p._sums), _answer(p._answer), norm(0.0)
		{ }

	void join(const Relaxer& other) {
		norm = std::max(norm, other.norm);
	}

};

/*****************************************************************************/

void Cowichan::sor(Matrix matrix, Vector target, Vector* solution) {

	// create a vector to hold the solution; start a guess
	// for the answer (a vector with all ones).
	*solution = NEW_VECTOR(real);
	for (int row = 0; row < Cowichan::NELTS; ++row) {
		VECTOR(*solution, row) = 1.0;
	}
	
	// N.B. in the chain, outer generates matrix,
	// and ensures that it is diagonally dominant. therefore we need
	// not do anything here to ensure this condition.

	// perform Successive Over Relaxation.
	Relaxer::performSOR(matrix, target, *solution, SOR_TOLERANCE);
	
}


>>> There's commented main below - it's not needed here.

/**
 * Entry point of the program.
 */ /*
int main(int argc, char** argv) {

	Matrix matrix		= NEW_MATRIX_SQUARE(real);
	Matrix savedmatrix	= NEW_MATRIX_SQUARE(real);
	Vector vector		= NEW_VECTOR(real);
	Vector answer		= NEW_VECTOR(real);
	real maxValue = MINIMUM_REAL;
	
	// initialise for a cowichan program in parallel
	COWICHAN_PARALLEL;
	
	// SERIAL: generate random values for the vector and matrix;
	// start a guess for the answer (a vector with all ones).
	for (int row = 0; row < Cowichan::NELTS; ++row) {
		VECTOR(vector,row) = uniform(0.0f, 1000.0f);
		VECTOR(answer,row) = 1.0;
		for (int col = 0; col < Cowichan::NELTS; ++col) {
			real randomValue = uniform(0.0, 20.0f);
			MATRIX(matrix,row,col) = randomValue;
			MATRIX(savedmatrix,row,col) = randomValue;
			maxValue = std::max(maxValue, randomValue);
		}
	}
	
	// SERIAL: ensure that the matrix is diagonally dominant.
	maxValue *= Cowichan::NELTS;
	for (int x = 0; x < Cowichan::NELTS; ++x) {
		DIAG(matrix,x) = maxValue;
		DIAG(savedmatrix,x) = maxValue;
	}
	
	// figure out the answer.
	Relaxer::performSOR(matrix, vector, answer, SOR_TOLERANCE);
	
	// we've got the answer, now, in answer. Print it out.
	printAxb(savedmatrix, answer, vector);

} */

