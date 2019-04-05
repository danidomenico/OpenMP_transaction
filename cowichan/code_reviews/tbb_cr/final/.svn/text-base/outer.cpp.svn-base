/**
	This module turns a vector containing point positions into a dense, symmet-
	ric, diagonally dominant matrix by calculating the distances between each
	pair of points. It also constructs a real vector whose values are the distance
	of each point from the origin. Inputs are:

	points: a vector of (x, y) points, where x and y are the point’s position.
	nelts: the number of points in the vector, and the size of the matrix along
		   each axis.

	Its outputs are:

	matrix: a real matrix, whose values are filled with inter-point distances.
	vector: a real vector, whose values are filled with origin-to-point distances.

	Each matrix element Mi,j such that i != j is given the value di,j, the Eu-
	clidean distance between point i and point j. The diagonal values Mi,i are
	then set to nelts times the maximum off-diagonal value to ensure that the
	matrix is diagonally dominant. The value of the vector element vi is set to
	the distance of point i from the origin, which is given by sqrt(xi^2 + yi^2).
 */
#include "cowichan.hpp"
 
class PointDistances {
	
	Matrix _matrix;
	Vector _vector;
	PointList *_points;
	
public:

	static void perform(PointList* points, Matrix matrix, Vector vector) {
		PointDistances dist(points, matrix, vector);
		parallel_for(Range(0, Cowichan::NELTS), dist, 
			auto_partitioner());
	}
	
public:

	PointDistances(PointList *points, Matrix matrix, Vector vector):
		_points(points), _matrix(matrix), _vector(vector) { }

	/**
	 * Calculates inter-point distances on the given range.
	 */
	void operator()(const Range& rows) const {
		
		PointList& points = *_points;
		Matrix matrix = _matrix;
		Vector vector = _vector;
		
		for (size_t y = rows.begin(); y != rows.end(); ++y) {
			
			// compute distances from points to origin
			VECTOR(vector, y) = Point::distance(points[y], Point::origin);
		
			// compute distances between points,
			for (size_t x = 0; x < Cowichan::NELTS; ++x) {

        >>> Checking if x != y here is probably not going to speed up
        >>> the computation since it is true for vast majority of elements.

				if (x != y) {
					// and only for the non-diagonal elements.
					MATRIX(matrix, y, x) = Point::distance(points[x], points[y]);
				} else {
					MATRIX(matrix, y, x) = 0.0;
				}
			}			
		}
	}
	
};


/**
 * Performs the maximum computation.
 */
class MaxReducer {
private:

	Matrix _image;
	real _max;
	
public:

	static real perform(Matrix matrix) {
		MaxReducer reducer(matrix);
		parallel_reduce(Range2D(0, Cowichan::NELTS, 0, Cowichan::NELTS), reducer,
			auto_partitioner());
		return reducer.getMaximum();
	}
	
public:

	/**
	 * Initialise max with the lowest possible value.
	 */
	MaxReducer(Matrix image):
		_image(image), _max(MINIMUM_REAL) { }

	real getMaximum() const {
		return _max;
	}

	/**
	 * Calculates the maximum value over the given range.
	 */
	void operator()(const Range2D& range) {

		Matrix image = _image;
		real max = _max;
		
		const Range& rows = range.rows();
		const Range& cols = range.cols();
		
		for (size_t y = rows.begin(); y != rows.end(); ++y) {
			for (size_t x = cols.begin(); x != cols.end(); ++x) {
				max = std::max(max, MATRIX(image, y, x));
			}
		}
		
		_max = max;
		
	}
	
	/**
	 * Splitting (TBB) constructor
	 */
	MaxReducer(MaxReducer& other, split):
		_image(other._image), _max(MINIMUM_REAL) { }

	/**
	 * Joiner (TBB).
	 */
	void join(const MaxReducer& other) {
		_max = std::max(other._max, _max);
	}
	
};

/**
 * Makes a given (NELTS) matrix diagonally dominant by modifying its diagonal elements.
 */
class MakeDominant {

  >>> Writing a whole class about 30 lines long to set matrix diagonal
  >>> values seems clumsy to me, but i guess that's how TBB works.

	Matrix _matrix;
	const real value;
	
public:

	static void perform(Matrix matrix, real value) {
		MakeDominant dom(matrix, value);
		parallel_for(Range(0, Cowichan::NELTS), dom, auto_partitioner());
	}

public:

	MakeDominant(Matrix matrix, real value):
		_matrix(matrix), value(value) { }
	
	/**
	 * Sets diagonal elements to a given constant.
	 */	
	void operator()(const Range& rows) const {
		Matrix matrix = _matrix;
		
		for (size_t i = rows.begin(); i != rows.end(); ++i) {
			DIAG(matrix, i) = value;

      >>> DIAG is the same as MATRIX_SQUARE pretty much.

		}
	}
	
};

/*****************************************************************************/

void Cowichan::outer(PointList* points, Matrix* matrix, Vector* vector) {
	
	// allocate space for the output (NELTS square matrix and vector)
	*matrix = NEW_MATRIX_SQUARE(real);
	*vector = NEW_VECTOR(real);
	
	// figure out the matrix and vector; fix up the diagonal.
	PointDistances::perform(points, *matrix, *vector);
	real maxValue = MaxReducer::perform(*matrix);
	MakeDominant::perform(*matrix, maxValue);	
	
}

