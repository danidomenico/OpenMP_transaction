/**
 * Parallel implementation of outer product matrix
 *
 * \file parallel.cpp
 * \author Andrew Borzenko
 * \date 02-27-09
 */

#include "../include/main.h"
#include "parallel.h"

/*
 * @ outer : body of outer product matrix calculation
 * > none
 * + fill matrix
 */

void
outer_mpi(mpi::communicator world,
  pt1D*		ptVec,			/* vector of points */
  real2D*	matrix,			/* matrix to fill */
  real1D*	realVec,		/* vector to fill */
  int		n			/* size */
){
  int		lo, hi;		/* work controls */
  int		r, c;			/* loop indices */
  real		d;			/* distance */
  real d_max_local = -1.0; // maximum distance
  real d_max; // maximum distance
  bool		work;			/* do useful work? */
  int i, j;

  /* all elements except matrix diagonal */
  work = get_block_rows_mpi (world, 0, n, &lo, &hi);
  if (work) {
    for (r = lo; r < hi; r++) {
      realVec[r] = ptMag(&(ptVec[r]));
      for (c = 0; c < r; c++) {
        d = ptDist (&(ptVec[r]), &(ptVec[c]));
        if (d > d_max_local) {
          d_max_local = d;
        }
        // fill columns 0 to r only
        matrix[r][c] = d;
      }
    }
  }

  // reduce to maximum d's
  all_reduce (world, d_max_local, d_max, mpi::maximum<real>());
  
  /* matrix diagonal */
  d = d_max * n;
  if (work) {
    for (r = lo; r < hi; r++) {
      matrix[r][r] = d;
    }
  }

  // broadcast matrix, realVec
  for (i = 0; i < world.size (); i++) {
    if (get_block_rows_mpi (world, 0, n, &lo, &hi, i)) {
      broadcast (world, &realVec[lo], hi - lo, i);
      // broadcast row by row since n may be smaller than MAXEXT
      for (j = lo; j < hi; j++) {
        broadcast (world, matrix[j], n, i);
      }
    }
  }

  // fill in the rest to make symmetric matrix
  for (r = 0; r < n; r++) {
    for (c = 0; c < r; c++) {
      matrix[c][r] = matrix[r][c];
    }
  }

  /* return */
}
