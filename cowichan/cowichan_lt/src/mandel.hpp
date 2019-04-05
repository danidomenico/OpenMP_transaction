/**
 * \file cowichan_lt/src/mandel.hpp
 * \brief LinuxTuples mandelbrot set header file.
 * \see CowichanLinuxTuples::mandel
 */

#ifndef __MANDEL_PRIVATE_HPP__
#define __MANDEL_PRIVATE_HPP__

	#include "tuple_common.hpp"
	#include "cowichan_lt.hpp"

	/**
	 * Calculates the mandelbrot set on a row-by-row basis.
	 * Uses LinuxTuples to do the heavy lifting.
	 */
	class LTMandel: public TupleApplication {
	protected:
		void consumeInput();
		void work();
		void produceOutput();

		/**
		 * Computes a part of the mandelbrot set.
		 * \param x x co-ordinate to compute for.
		 * \param y y co-ordinate to compute for.
		 * \return the computed value at (x,y) on the mandelbrot set.
		 */
		int mandelCalc(real x, real y);
	};

#endif

