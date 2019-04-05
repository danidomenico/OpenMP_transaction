/**
 * Fully non-parallel invasion percolation...
 */
#include "cowichan.hpp"
#include <queue>

class PercPoint {
public:

	Point point;
	PercPoint(Point point): point(point) { }

	// we want to extract lowest values.
	bool operator<(const PercPoint &other) const {
		return value() > other.value();
	}

	uint value() const {
		return MATRIX_RECT(matrix, (int)point.y, (int)point.x);
	}

public:

	static IntMatrix matrix;
	
};

IntMatrix PercPoint::matrix = NULL;

/*****************************************************************************/

void Cowichan::invperc(IntMatrix matrix, BoolMatrix* mask) {
	
	PercPoint pp(Point(0,0));
	
	// create a mask matrix; fill it with false.
	*mask = NEW_MATRIX_RECT(bool);
	std::fill_n(*mask, NROWS * NCOLS, false);
	
	// set the matrix we are working with
	PercPoint::matrix = matrix;
	
	// "seed" with the middle value; start a priority queue.
	std::vector<PercPoint> points;
	points.push_back(Point((int) (NROWS / 2), (int) (NCOLS / 2)));
	std::make_heap(points.begin(), points.end());

	// perform invasion percolation NFILL times.
	int r, c;
	for (int it = 0; it < NFILL; ++it) {

		// get the highest-priority point that hasn't already
		// been filled.
		do {
			std::pop_heap(points.begin(), points.end());
			pp = points.back();
			points.pop_back();
			r = pp.point.y;
			c = pp.point.x;			
		} while (MATRIX_RECT(*mask, r, c)); // find a free one

		// fill it.
		MATRIX_RECT(*mask, r, c) = true;

		// add all of its neighbours to the party...
		
		// top neighbour
		if (r > 0) {
			points.push_back(Point(c, r-1));
			push_heap(points.begin(), points.end());
		}
		
		// bottom neighbour
		if (r < (NROWS - 1)) {
			points.push_back(Point(c, r+1));
			push_heap(points.begin(), points.end());
		}
		
		// left neighbour
		if (c > 0) {
			points.push_back(Point(c-1, r));
			push_heap(points.begin(), points.end());
		}
		
		// right neighbour
		if (c < (NCOLS - 1)) {
			points.push_back(Point(c+1, r));
			push_heap(points.begin(), points.end());
		}
		
	}
	
}

