/**
 * \file cowichan_lt/src/gauss.hpp
 * \brief LinuxTuples gauss header file.
 * \see CowichanLinuxTuples::gauss
 */

#ifndef __GAUSS_PRIVATE_HPP__
#define __GAUSS_PRIVATE_HPP__

	#include "tuple_common.hpp"
	#include "cowichan_lt.hpp"

	/**
	 * Forward-elimination tuple application. Performs
	 * forward elimination on a square matrix in parallel.
	 */
	class LTForward: public TupleApplication {
	protected:

		void consumeInput();
		void work();
		void produceOutput();

		/**
		 * Number of rows computed.
		 */
		static const char* ROWS_DONE;
		/**
		 * Is forward-elimination complete?
		 */
		static const char* FORWARD_DONE;
		/**
		 * A matrix row computation request.
		 */
		static const char* REQUEST;

		/**
		 * The target vector stored in the tuple space.
		 */
		static const char* TARGET;
		/**
		 * One of the matrix rows, stored in the tuple space.
		 */
		static const char* MATRIX_ROW;

	};

#endif

