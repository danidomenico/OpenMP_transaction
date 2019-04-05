#include "cowichan.hpp"

/**
 * This class multiplies a matrix by a vector, producing a vector, a la:
 *	[x1 x2 x3]   [y1]   [(x1*y1 + x2*y2 + x3*y3)]
 *	[x4 x5 x6] * [y2] = [(x4*y1 + x5*y2 + x6*y3)]
 *	[x7 x8 x9]   [y3]   [(x7*y1 + x8*y2 + x9*y3)]
 */
class Product {
	
	Matrix _matrix;
	Vector _vector, _result;

public:

	static void exec(Matrix matrix, Vector vector, Vector result) {
		Product product(matrix, vector, result);
		parallel_for(blocked_range<size_t>(0, Cowichan::NELTS), product, auto_partitioner());
	}

public:

	Product(Matrix matrix, Vector vector, Vector result):
		_matrix(matrix), _vector(vector), _result(result) { }

	/**
	 * Performs matrix-vector multiplication on the given row range.
	 */
	void operator()(const Range& rows) const {
		
		Matrix matrix = _matrix;
		Vector vector = _vector;
		Vector result = _result;
		
		for (size_t row = rows.begin(); row != rows.end(); ++row) {
			
			VECTOR(result, row) = 0.0;
			for (int col = 0; col < Cowichan::NELTS; ++col) {
				VECTOR(result, row) += MATRIX(matrix, row,col) * VECTOR(vector, col);
			}
			
		}
		
	}
};

/**
 * This class takes the 1-norm of the difference between two vectors.
 */
class NormDiff {

  >>> This class should be in a separate file since it's another cowichan
  >>> problem.

	Vector _vec1;
	Vector _vec2;
	real _norm;

public: 

	static float exec(Vector vec1, Vector vec2, size_t n) {
		NormDiff norm(vec1, vec2);
		parallel_reduce(blocked_range<size_t>(0, n), norm, auto_partitioner());
		return norm.getNorm();
	}
	
public:
	
	float getNorm() const {
		return _norm;
	}
	
	void operator()(const Range& range) {
		Vector vec1 = _vec1;
		Vector vec2 = _vec2;
		real norm = _norm;
		for (size_t i = range.begin(); i != range.end(); ++i) {
		
			// get the element-wise difference; store the maximum
			real diff = (real) fabs(VECTOR(vec1, i) - VECTOR(vec2, i));
			if (diff > norm) {
				norm = diff;
			}
			
		}
		_norm = norm;
	}
	
	/**
	 * Splitting (TBB) constructor
	 */
	NormDiff(NormDiff& other, split):
		_vec1(other._vec1), _vec2(other._vec2), _norm(0) { }
	
	/**
	 * Joiner (TBB).
	 */
	void join(const NormDiff& other) {
		_norm = std::max(_norm, other.getNorm());
	}
	
	NormDiff(Vector vec1, Vector vec2): _vec1(vec1), _vec2(vec2), _norm(0) { }

};

/*****************************************************************************/

void Cowichan::product(Matrix matrix, Vector actual, Vector candidate, real* e) {
	
	// compute the vector based on the candidate solution.
	Vector computed = NEW_VECTOR(real);
	Product::exec(matrix, candidate, computed);
	
	// compute the error between the actual and computed vectors.

  >>> The difference to be computed is between result of gauss and result
  >>> of sor.

	*e = NormDiff::exec(actual, computed, NELTS);
	
}

