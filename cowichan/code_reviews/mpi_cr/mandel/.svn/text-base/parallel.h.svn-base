/**
 * Parallel implementation of Mandelbrot set
 *
 * \file parallel.h
 * \author Andrew Borzenko
 * \date 02-09-09
 */

#pragma once
#ifndef MANDEL_PARALLEL_H
#define MANDEL_PARALLEL_H

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

/*
 * @ mandel : calculate Mandelbrot Set
 * > none
 * + fill matrix
 */

void mandel_mpi (mpi::communicator world,
                 int2D*		matrix,			/* to fill */
                 int		nr,			/* row size */
                 int		nc,			/* column size */
                 real		base_x,			/* lower left corner */
                 real		base_y,			/* lower left corner */
                 real		ext_x,			/* extent */
                 real		ext_y);			/* extent */


/*--------------------------------------------------------------*/
/* private functions						*/
/*--------------------------------------------------------------*/

int mandel_calc_mpi (real		x,			/* x coordinate */
                     real		y);			/* y coordinate */

#endif /* MANDEL_PARALLEL_H */
