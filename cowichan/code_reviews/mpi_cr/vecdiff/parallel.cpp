/**
 * Parallel implementation of vector difference
 *
 * \file parallel.cpp
 * \author Andrew Borzenko
 * \date 02-27-09
 */

#include "../include/main.h"
#include "parallel.h"

/*
 * @ vecdiff : do vector difference
 * > none
 * + find norm-1 vector difference
 */

void
vecdiff_mpi(mpi::communicator world,
  real1D*	left,			/* left vector */
  real1D*	right,			/* right vector */
  int		n,			/* vector lengths */
  real	      * diff			/* norm-1 difference */
){
  int		lo, hi;		/* work controls */
  int		i;			/* loop index */
  real		d;			/* difference */
  real max_d;           // maximum difference

  max_d = (real) fabs ((double) (left[0] - right[0]));

  if (get_block_rows_mpi (world, 0, n, &lo, &hi)) {
    for (i = lo; i < hi; i++) {
      d = (real) fabs ((double) (left[i] - right[i]));
      if (d > max_d) {
        max_d = d;
      }
    }
  }

  // reduce to maximum d's
  all_reduce (world, max_d, *diff, mpi::maximum<real>());

  /* return */
}
