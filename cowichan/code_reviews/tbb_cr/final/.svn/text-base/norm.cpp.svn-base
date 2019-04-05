#include "cowichan.hpp"

/**
 * This class performs point normalization -- points are put onto the unit
 * square. 
 */
class Normalizer {
public:
	
	PointList& points;

public:
	
	/**
	 * Performs the minimum and maximum computations.
	 */
	class MinMaxReducer {
	private:

		Normalizer* parent;
		Point _min, _max;

	public:

		/**
		 * Initialise min and max with max and min, respectively, so that we
		 * don't have to "special-case" the first iteration.
		 */
		MinMaxReducer(Normalizer* parent):
			parent(parent), _min(Point::maximum), _max(Point::minimum) { }
	
		Point getMinimum() const {
			return _min;
		}
	
		Point getMaximum() const {
			return _max;
		}
	
		/**
		 * Calculates the minimum and maximum co-ordinates over the given array
		 * range.
		 */
		void operator()(const Range& range) {

			// get pointers locally.
			Point min = _min;
			Point max = _max;

			// calculate the minimum and maximum co-ordinates over the range.
			for (size_t i = range.begin(); i != range.end(); ++i) {
				min.x = std::min(parent->points[i].x, min.x);
				min.y = std::min(parent->points[i].y, min.y);
				max.x = std::max(parent->points[i].x, max.x);
				max.y = std::max(parent->points[i].y, max.y);
			}
			
			// refresh member variables.
			_min = min;
			_max = max;
			
		}
	
		/**
		 * Splitting (TBB) constructor
		 */
		MinMaxReducer(MinMaxReducer& other, split):
			parent(other.parent), _min(Point::maximum), _max(Point::minimum) { }
	
		/**
		 * Joiner (TBB).
		 */
		void join(const MinMaxReducer& other) {
			_min.x = std::min(other._min.x, _min.x);
			_min.y = std::min(other._min.y, _min.y);
			_max.x = std::max(other._max.x, _max.x);
			_max.y = std::max(other._max.y, _max.y);
		}
		
	};

	/** 
	 * Performs the re-normalization procedure.
	 */
	class Computer {
	public:
		
		Normalizer* parent;
		Point min, max;

		Computer(Normalizer* parent, Point min, Point max):
			parent(parent), min(min), max(max) { }

		void operator()(const Range& range) const {

			// normalize the points that lie in the given range.
			real xfactor = 1.0 / (max.x - min.x);

      >>> This does not check that max.x is not equal to min.x, which
      >>> can happen.

			real yfactor = 1.0 / (max.y - min.y);
			for (size_t i = range.begin(); i != range.end(); ++i) {

        >>> I am not sure why size_t is used here. size_t is for result of
        >>> sizeof operator

				parent->points[i].x = (parent->points[i].x - min.x) * xfactor;
				parent->points[i].y = (parent->points[i].y - min.y) * yfactor;

			}
			
		}
		
	};
	
	MinMaxReducer minmax;
	Computer* computer;

public:

	Normalizer(PointList& points):
		minmax(this), computer(NULL), points(points)
		{ }

	~Normalizer() {
		if (computer != NULL) {
			delete computer;
		}
	}
		
	/**
	 * In-place normalizes the incoming co-ordinates onto the unit square.
	 */
	static void perform(PointList* points) {
		
		Normalizer norm(*points);
				
		// first compute the statistics, and then use the min/max to compute the new points.
		norm.computeStats(points->size());
		parallel_for(Range(0, points->size()), (*norm.computer), auto_partitioner());
		
		// the points and transformed in-place, so don't return anything.
		return;
		
	}

private:

	/**
	 * Calls the stats-gathering reducer and creates a normalizer based on it.
	 */
	void computeStats(size_t size) {
		
		parallel_reduce(Range(0, size), minmax, auto_partitioner());
		computer = new Computer(this, minmax.getMinimum(), minmax.getMaximum());
		
	}
	
};

/*****************************************************************************/

void Cowichan::norm(PointList* pointsIn, PointList** pointsOut) {

	*pointsOut = pointsIn;
	Normalizer::perform(pointsIn);

}

