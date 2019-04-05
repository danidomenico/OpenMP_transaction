/**
 * \file cowichan_lt/src/winnow.hpp
 * \brief LinuxTuples winnow header file.
 * \see CowichanLinuxTuples::winnow
 */

#ifndef __WINNOW_PRIVATE_HPP__
#define __WINNOW_PRIVATE_HPP__

	#include "tuple_common.hpp"
	#include "cowichan_lt.hpp"

	/**
	 * Performs the weighted point selection in tuple space.
	 * Uses a property of tuple-space which means no sorting needs to occur!
	 */
	class LTWinnow: public TupleApplication {
	protected:

		void consumeInput();
		void work();
		void produceOutput();

		/**
		 * Figure out the next point we can pull out of tuple space.
		 * \param order the weight of the next point to pull out.
		 * \return the point with the smallest weight w such that w >= order.
		 */
		Point nextWeightedPoint(INT_TYPE* order);

		/**
		 * Synchronization lock (critical section).
		 */
		static const char* SYNCH_LOCK;
		/**
		 * Request to perform computation on a matrix row.
		 */
		static const char* REQUEST;
		/**
		 * A weighted point in the tuple space.
		 */
		static const char* WEIGHTED_POINT;
		/**
		 * The number of weighted points in the tuple space.
		 */
		static const char* COUNT;
		/**
		 * The number of rows that have had their computation finish.
		 */
		static const char* ROWS_DONE;

	};

#endif

