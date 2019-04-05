/**
 * \file cowichan_lt/src/hull.hpp
 * \brief LinuxTuples iterative convex hull header file.
 * \see CowichanLinuxTuples::hull
 */

#ifndef __HULL_PRIVATE_HPP__
#define __HULL_PRIVATE_HPP__

	#include "tuple_common.hpp"
	#include "cowichan_lt.hpp"

	/**
	 * Indicator value to denote no valid point is stored.
	 */
	#define NO_POINT -1

	/**
	 * Performs an iterative convex hull in tuple space.
	 * Iterative convex hull means that the output contains the convex hull of
	 * all points, followed by the convex hull of those points not on the first
	 * convex hull, followed by the convex hull of those points not on the first
	 * or second, etc. etc. until no points remain. Thus the iterative convex hull
	 * can be seen as a spiralling shape, or a permutation of the input points.
	 */
	class LTHull: public TupleApplication {
	public:

		/**
		 * Returns the number of points in the convex hull.
		 */
		int getNumPoints();

		/**
		 * Constructor.
		 * \param numLeft the number of points left unmasked in the input.
		 */
		LTHull(size_t numLeft);

		/**
		 * The index of a point that is masked off (i.e. already selected) in the input.
		 * This is public so we can access it from CowichanLinuxTuples::hull(...).
		 */
		static const char* MASKED_POINT;

	protected:

		/**
		 * The number of points left unmasked in the input.
		 */
		size_t numLeft;

		void consumeInput();
		void work();
		void produceOutput();

		/**
		 * Generic request for work to be done.
		 */
		static const char* REQUEST;
		/**
		 * Specific work: min-max request over a length of points.
		 */
		static const char* REQUEST_MINMAX;
		/**
		 * Specific work: cross-product request over a length of points.
		 */
		static const char* REQUEST_CROSS;

		/**
		 * Synchronization lock (critical section).
		 */
		static const char* SYNCH_LOCK;
		/**
		 * The number of points that have been computed for.
		 */
		static const char* POINTS_DONE;

		/**
		 * The point with lowest x value, unmasked.
		 */
		static const char* MIN_X_POINT;
		/**
		 * The point with highest x value, unmasked.
		 */
		static const char* MAX_X_POINT;
		/**
		 * The cross product value of the "furthest" point.
		 */
		static const char* MAX_CROSS;
		/**
		 * The "furthest" point found.
		 */
		static const char* MAX_POINT;

		/**
		 * A point on the hull, ordered.
		 */
		static const char* HULL_POINT;
		/**
		 * The number of points in this convex hull.
		 */
		static const char* NUM_POINTS;

		/**
		 * Should the output producer run now?
		 */
		static const char* FLAG_OUTPUT;
		/**
		 * The min-max process mandated has been finished.
		 */
		static const char* FINISHED_MINMAX;

		/**
		 * Analogous to split in other versions of quickhull.
		 * split again based on the furthest point from the line denoted
		 * by the two points given (p1 and p2).
		 */
		void split(const index_t p1, const index_t p2, index_t *order);

		/**
		 * Initiate a tuple-space cross-product over multiple points.
		 */
		void computeCross(index_t p1, index_t p2, index_t* maxPoint, real* maxCross);

		/**
		 * Initiate a tuple-space min/max routine over multiple points.
		 */
		void computeMinMax(index_t* minPoint, index_t* maxPoint);

		/**
		 * Worker helper to service a min/max request.
		 */
		void serviceMinMaxRequest(tuple* gotten);

		/**
		 * Worker helper to server a cross-product request.
		 */
		void serviceCrossRequest(tuple* gotten);

		/**
		 * Is the given point "masked", i.e., should we skip it?
		 * \param position the point to check.
		 * \return true if the point is to be skipped, false otherwise.
		 */
		bool isMasked(index_t position);

		/**
		 * Mask the given point (make sure we skip it next time we do a convex hull
		 * computation).
		 * \param position the point to mask.
		 */
		void mask(index_t position);

	};

#endif

