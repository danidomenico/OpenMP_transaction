/*
	This module converts a matrix of integer values to a vector of points, rep-
	resented as x and y coordinates. Its inputs are:

	matrix: an integer matrix, whose values are used as masses.
	mask: a Boolean matrix showing which points are eligible for consideration.
	nrows, ncols: the number of rows and columns in the matrix.
	nelts: the number of points to select.

	Its output is:

	points: a vector of (x, y) points.

	Each location where mask is true becomes a candidate point, with a weight
	equal to the integer value in matrix at that location and x and y coordinates
	equal to its row and column indices. These candidate points are then sorted
	into increasing order by weight, and nelts evenly-spaced points selected to
	create the result vector.
*/
#include "cowichan.hpp"

class ValueSelector {
private:

	IntMatrix _candidates;
	BoolMatrix _mask;
	
	RealList values;

public:

	/**
	 * Standard constructor
	 */
	ValueSelector(IntMatrix candidates, BoolMatrix mask):
		_candidates(candidates), _mask(mask) { }

	/**
	 * Add candidate values to the value list based on mask (TBB).
	 */
	void operator()(const Range2D& range) {
		
		// bring pointers into cache
		const BoolMatrix mask = _mask;
		const IntMatrix candidates = _candidates;
		const Range& rows = range.rows();
		const Range& cols = range.cols();
		
		// add candidate values marked as good by the mask.
		for (size_t y = rows.begin(); y != rows.end(); ++y) {
			for (size_t x = cols.begin(); x != cols.end(); ++x) {
				
				if (MATRIX_RECT(mask, y, x)) {
					add(MATRIX_RECT(candidates, y, x));
				}
				
			}
		}
		
	}
	
	/**
	 * Splitting (TBB) constructor.
	 */
	ValueSelector(ValueSelector& other, split):
		_candidates(other._candidates), _mask(other._mask) { }
	
	/**
	 * Joiner (TBB).
	 */
	void join(const ValueSelector& other) {
		add(other.values);
	}
	
	/**
	 * Gets the list of selected values.
	 */
	RealList getValues() const {
		return values;
	}
		
private:

	/**
	 * Inserts a value into the values list, in correct order (given that the
	 * list is already sorted). It's O(log n)!
	 */
	void add(const real& newValue) {
		RealList::iterator pos = std::lower_bound(values.begin(), values.end(), newValue);
		values.insert(pos, newValue);
	}
	
	/**
	 * Merges another already-sorted list with this ValueSelector's list.
	 */
	void add(const RealList& other) {

		RealList replacement;
		RealList::const_iterator i, j;		

		// reserve space in the replacement vector for the current and new values		
		replacement.reserve(values.size() + other.size());

		// iterate over both collections, adding the smaller element to the new
		// real list every time we must make a comparison. In the end, all of
		// the elements are added to replacement.
		for (i = values.begin(), j = other.begin();;) {
		
			if (i == values.end() && j == other.end()) return;
			if (i == values.end()) {
				replacement.push_back(*j); ++j;
			} else if (j == other.end()) {
				replacement.push_back(*i); ++i;
			} else {
				if (*i < *j) {
					replacement.push_back(*i); ++i;
				} else {
					replacement.push_back(*j); ++j;
				}
			}
		
		}
		
		// replace the old list with the new one.
		values = replacement;
		
	}
	
};

/*****************************************************************************/

class PointCreator {
private:

	RealList* values;
	PointList points;

public:

	/**
	 * Perform weighted point selection.
	 * @return a list of numPoints points.
	 */
	static void perform(int numPoints, IntMatrix candidates, BoolMatrix mask, PointList* points) {
	
		// extract candidates from the matrix
		ValueSelector vc(candidates, mask);
		parallel_reduce(Range2D(0, Cowichan::NROWS, 0, Cowichan::NCOLS), vc,
			auto_partitioner());
		
    >>> I am having a hard time understanding how this performs the desired
    >>> function. It seems to me that x and y coordinates for the resulting
    >>> points are created using the weights (matrix values) only rather than
    >>> the row and column indices of the matrix.

		// we can only create as many points as we have pairs of values.
		if (numPoints < vc.getValues().size()) {
			numPoints = vc.getValues().size();
		}
		
		// pair them together to create points
		PointCreator pc(vc.getValues());
		parallel_reduce(Range(0, numPoints), pc, auto_partitioner());
		
		// return those gathered points
		points->reserve(pc.getPoints().size());
		for (PointList::const_iterator it = pc.getPoints().begin(); it != pc.getPoints().end(); ++it) {
			points->push_back(*it);
		}

	}

public:

	/**
	 * Standard constructor.
	 */
	PointCreator(RealList sv) {
		values = new RealList(sv);
	}

	/**
	 * Combine values pairwise to create a list of points (TBB).
	 */
	void operator()(const blocked_range<size_t>& range) {

		const RealList& v = *values;
		for (size_t i = range.begin(); i != range.end(); ++i) {
			points.push_back(Point(v[i*2], v[(i*2)+1]));
		}

	}

	/**
	 * Splitting (TBB) constructor.
	 */
	PointCreator(PointCreator& other, split):
		values(other.values) { }
	
	/**
	 * Joiner (TBB).
	 */	
	void join(const PointCreator& other) {
		std::copy(other.points.begin(), other.points.end(), std::back_inserter(points));	
//		points.insert(points.end(), other.points.begin(), other.points.end());
	}
	
	/**
	 * Gets the list of points.
	 */
	PointList getPoints() const {
		return points;
	}

};

/*****************************************************************************/

void Cowichan::winnow(IntMatrix matrix, BoolMatrix mask, PointList** points) {

	// get a list of points!
	*points = new PointList();

  >>> Naming: here, it's called matrix, inside perform it's called candidates.

	PointCreator::perform(NELTS, matrix, mask, *points);
	
}

