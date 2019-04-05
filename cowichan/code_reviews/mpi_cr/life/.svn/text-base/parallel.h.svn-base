/**
 * Parallel implementation of conway's game of life
 *
 * \file parallel.h
 * \author Andrew Borzenko
 * \date 01-26-09
 */

#pragma once
#ifndef LIFE_PARALLEL_H
#define LIFE_PARALLEL_H

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

/*
 * @ life : simulate Game of Life
 * > none
 * + evolve world
 */

void
life_mpi(
  mpi::communicator world,
  bool2D*	matrix,			/* world to evolve */
  int		nr,			/* row size */
  int		nc,			/* column size */
  int		iters			/* number of iterations */
);

/*--------------------------------------------------------------*/
/* private functions						*/
/*--------------------------------------------------------------*/

/*
 * @ life_one : update count for single cell
 * > none
 * + update count (using fact that TRUE==1 and FALSE==0)
 */

void
life_one_mpi(
  bool2D*	matrix,			/* world to evolve */
  int2D*		count,			/* neighborhood counts */
  int		r,			/* this row */
  int		r_lo,			/* lower row */
  int		r_hi,			/* higher row */
  int		c,			/* this column */
  int		c_lo,			/* lower column */
  int		c_hi,			/* higher column */
  int   width
);

/*
 * @ life_row : count entire row
 * > none
 * + update counts
 */

void
life_row_mpi(
  bool2D*	matrix,			/* world to evolve */
  int2D*		count,			/* neighborhood counts */
  int		nr,			/* row size */
  int		nc,			/* column size */
  int		r,			/* this row */
  int		r_lo,			/* lower row */
  int		r_hi			/* higher row */
);

#endif /* LIFE_PARALLEL_H */
