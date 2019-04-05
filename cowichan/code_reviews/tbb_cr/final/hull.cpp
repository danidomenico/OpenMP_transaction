#include "cowichan.hpp"

/**
 * Computes the maximum signed distance from a point to a line, given
 * the line and list of points.
 */
class MaximumDistance {
public:

	/**
	 * Computes the cross product of the vectors (l1,l2) and (l1,p).
	 */
	static inline real cross(const Point& l1, const Point& l2, const Point& p) {
		return (l1.x - p.x) * (l2.y - p.y) -
			   (l1.y - p.y) * (l2.x - p.x);
	}

private:

	const PointList& points;
	const Point &p1, &p2;
	Point maxPoint;
	real maxCross;

public:

	MaximumDistance(const PointList& points, const Point& p1, const Point& p2):
		points(points), p1(p1), p2(p2), maxCross(-numeric_limits<real>::infinity())
		{ }

	/**
	 * Gets the point with maximum signed distance (if it has already been
	 * calculated).
	 */	
	Point getPoint() const {
		return maxPoint;
	}
	
	/**
	 * Gets the signed distance to that point.
	 */
	real getDistance() const {
		return maxCross;
	}
	
	/**
	 * Calculates the Point with maximum signed distance.
	 */
	void operator()(const blocked_range<size_t>& range) {

		// compute the signed distances from the line for each point in the range.
		for (int i = range.begin(); i < range.end(); ++i) {
			real currentCross = cross(p1, p2, points[i]);
			if (currentCross > maxCross) {
				maxPoint = points[i];
				maxCross = currentCross;
			}
		}
		
	}

	/**
	 * Splitting (TBB) constructor
	 */
	MaximumDistance(MaximumDistance& other, split):
		points(other.points), maxCross(other.maxCross), p1(other.p1), p2(other.p2)
		{ }

	/**
	 * Joiner (TBB).
	 */
	void join(const MaximumDistance& other) {
		if (other.maxCross > maxCross) {
			maxPoint = other.maxPoint;
			maxCross = other.maxCross;
		}
	}

};

/**
 * Computes the convex hull of a set of points, returning a set of points in
 * counter-clockwise order. The method used is known as parallel quickhull, and
 * it is described at:
 * 
 * http://www.cs.cmu.edu/~scandal/cacm/node10.html
 * http://www.cs.princeton.edu/~ah/alg_anim/version1/QuickHull.html
 *
 * The code used in this program is based on the ideas presented there.
 */
class QuickHull {
private:

	PointList& hull;
	const PointList& points;

public:

	QuickHull(const PointList& points, PointList& hull):
		points(points), hull(hull)
		{ }

	/**
	 * Splits the recursion space to find members of the convex hull.
	 */
	void split(const PointList& points, const Point p1, const Point p2) {

		// use TBB to find the point with maximal signed distance from the line (p1,p2)
		MaximumDistance md(points, p1, p2);
		parallel_reduce(blocked_range<size_t>(0, points.size()), md, auto_partitioner());

		// is there a point in the positive half-space?
		// if so, it has maximal distance, and we must recurse based on that point.
		if (md.getDistance() > 0.0) {
				
			// recurse on the new set with the given far point
			split(points, p1, md.getPoint());
			split(points, md.getPoint(), p2);
			return;
		
		} 
		
		// otherwise, it's not on the right side; we don't need to split anymore.
		// this is because all points are inside the hull when we use this half-space.
		// add the first point and return.
		hull.push_back(p1);
	
	}

	/**
	 * Calculate the convex hull of points using the QuickHull method.
	 * @return PointList whose members are the points of the convex hull.
	 */
	void convexHull() {
	
		PointList::const_iterator minPoint = points.begin();
		PointList::const_iterator maxPoint = points.begin();	
	
		// figure out the points with minimum and maximum x values

    >>> Could this be done in parallel too?

		for (PointList::const_iterator it = points.begin(); it != points.end(); ++it) {
		
			if (minPoint->x > it->x) minPoint = it;
			if (maxPoint->x < it->x) maxPoint = it;
		
		}
	
		// use these as initial pivots
		split(points, *minPoint, *maxPoint);
		split(points, *maxPoint, *minPoint);
	
	}

public:

	/**
	 * Computes and returns the convex hull for the given set of points (TBB).
	 */
	static void perform(const PointList& points, PointList& hull) {
		QuickHull qh(points, hull);
		qh.convexHull();
	}

};

/*****************************************************************************/

void Cowichan::hull(PointList* pointsIn, PointList** pointsOut) {

	*pointsOut = new PointList();
	
	// compute the convex hull of the points.
	QuickHull::perform(*pointsIn, **pointsOut);
	
}

