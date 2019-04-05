/**
 * \file cowichan_lt/src/half.hpp
 * \brief LinuxTuples halving shuffle header file.
 * \see CowichanLinuxTuples::half
 */

#ifndef __HALF_PRIVATE_HPP__
#define __HALF_PRIVATE_HPP__

	#include "tuple_common.hpp"
	#include "cowichan_lt.hpp"

	/**
	 * Performs the halving shuffle on an input matrix.
	 * Generates an output matrix using LinuxTuples.
	 */
	class LTHalf: public TupleApplication {
	protected:
		void consumeInput();
		void work();
		void produceOutput();

		/**
		 * A request to perform computation on a row of the matrix.
		 */
		static const char *REQUEST;

		/**
		 * Done with the halving shuffle.
		 */
		static const char *DONE;

	};

#endif

