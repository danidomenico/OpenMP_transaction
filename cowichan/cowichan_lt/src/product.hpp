/**
 * \file cowichan_lt/src/product.hpp
 * \brief LinuxTuples product header file.
 * \see CowichanLinuxTuples::product
 */

#ifndef __PRODUCT_PRIVATE_HPP__
#define __PRODUCT_PRIVATE_HPP__

	#include "tuple_common.hpp"
	#include "cowichan_lt.hpp"

	/**
	 * Performs the product of a matrix and a vector in tuple-space.
	 */
	class LTProduct: public TupleApplication {
	protected:
		void consumeInput();
		void work();
		void produceOutput();
	};

#endif

