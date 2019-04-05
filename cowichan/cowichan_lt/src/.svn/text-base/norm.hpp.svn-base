/**
 * \file cowichan_lt/src/norm.hpp
 * \brief LinuxTuples norm header file.
 * \see CowichanLinuxTuples::norm
 */

#ifndef __NORM_PRIVATE_HPP__
#define __NORM_PRIVATE_HPP__

	#include "tuple_common.hpp"
	#include "cowichan_lt.hpp"

	/**
	 * Calculates the minimum/maximum point in the point cloud,
	 * so that the other points can be later normalized. Keeps those
	 * values in the tuple space so they can be used later.
	 */
	class LTBounds: public TupleApplication {
	protected:

		void consumeInput();
		void work();
		void produceOutput();

		/**
		 * Synchronization lock (critical section).
		 */
		static const char* SYNCH_LOCK;
		/**
		 * Number of points that have been processed.
		 */
		static const char* POINTS_DONE;
		/**
		 * The smallest point (in x and y) found
		 */
		static const char* MIN_POINT;
		/**
		 * The largest point (in x and y) found
		 */
		static const char* MAX_POINT;

	};

	/**
	 * Re-scales the points in the cloud so that they all
	 * lay in the unit square.
	 */
	class LTNorm: public TupleApplication {
	protected:

		void consumeInput();
		void work();
		void produceOutput();

		/**
		 * The smallest point (in x and y) found
		 */
		static const char* MIN_POINT;
		/**
		 * The largest point (in x and y) found
		 */
		static const char* MAX_POINT;

	};

#endif

