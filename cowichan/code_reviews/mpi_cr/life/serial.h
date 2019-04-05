/**
 * Serial implementation of conway's game of life
 *
 * \file serial.h
 * \author Andrew Borzenko
 * \date 01-26-09
 */

#pragma once
#ifndef LIFE_SERIAL_H
#define LIFE_SERIAL_H

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

/*
 * @ life : simulate Game of Life
 * > none
 * + evolve world
 */

void
life(
  bool2D*	world,			/* world to evolve */
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
life_one(
  bool2D*	world,			/* world to evolve */
  int2D*		count,			/* neighborhood counts */
  int		r,			/* this row */
  int		r_lo,			/* lower row */
  int		r_hi,			/* higher row */
  int		c,			/* this column */
  int		c_lo,			/* lower column */
  int		c_hi			/* higher column */
);

/*
 * @ life_row : count entire row
 * > none
 * + update counts
 */

void
life_row(
  bool2D*	world,			/* world to evolve */
  int2D*		count,			/* neighborhood counts */
  int		nr,			/* row size */
  int		nc,			/* column size */
  int		r,			/* this row */
  int		r_lo,			/* lower row */
  int		r_hi			/* higher row */
);

#endif /* LIFE_SERIAL_H */
