/**
 * \file cowichan_lt/src/life.hpp
 * \brief LinuxTuples conway's game of life header file.
 * \see CowichanLinuxTuples::life
 */

#ifndef __LIFE_PRIVATE_HPP__
#define __LIFE_PRIVATE_HPP__

	#include "tuple_common.hpp"
	#include "cowichan_lt.hpp"

	/**
	 * Cellular automata done in LinuxTuples.
	 */
	class LTLife: public TupleApplication {
	protected:
		void consumeInput();
		void work();
		void produceOutput();

		index_t sumNeighbours(index_t y, index_t x);
	};

#endif

