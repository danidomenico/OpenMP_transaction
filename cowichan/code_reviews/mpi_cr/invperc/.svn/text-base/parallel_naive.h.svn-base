/**
 * Parallel implementation of invasion percolation
 *
 * \file parallel.h
 * \author Andrew Borzenko
 * \date 02-28-09
 */

#pragma once
#ifndef PARALLEL_H
#define PARALLEL_H

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

void
invperc_mpi (mpi::communicator world,
  int2D*		matrix,			/* matrix to invade */
  bool2D*	mask,			/* mask to fill */
  int		nr,			/* row size */
  int		nc,			/* column size */
  real		fraction		/* how much to fill */
);

/*--------------------------------------------------------------*/
/* private functions						*/
/*--------------------------------------------------------------*/

bool is_connected (bool2D* mask, int r, int c, int nr, int nc);

#endif /* PARALLEL_H */
