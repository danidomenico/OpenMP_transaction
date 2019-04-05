/**
 * \file cowichan_lt/src/outer.hpp
 * \brief LinuxTuples outer product header file.
 * \see CowichanLinuxTuples::outer
 */

#ifndef __OUTER_PRIVATE_HPP__
#define __OUTER_PRIVATE_HPP__

	#include "tuple_common.hpp"
	#include "cowichan_lt.hpp"

	/**
	 * Performs the outer product with LinuxTuples.
	 */
	class LTOuter: public TupleApplication {
	protected:

		void consumeInput();
		void work();
		void produceOutput();

		/**
		 * Synchronization lock (critical section).
		 */
		static const char* SYNCH_LOCK;
		/**
		 * Request to compute for one vector.
		 */
		static const char* REQUEST;

		/**
		 * The maximum length of any vector.
		 */
		static const char* MAX_DISTANCE;
		/**
		 * An entry in the matrix, to be stored in the output.
		 */
		static const char* MATRIX_ENTRY;

	};

#endif

