/**
 * Parallel implementation of successive over-relaxation
 *
 * \file parallel.cpp
 * \author Andrew Borzenko
 * \date 03-02-09
 */

#include "../include/main.h"
#include "parallel.h"

/*
 * @ sor : do body of successive over-relaxation
 * > none
 * + relax matrix to find solution
 */

void sor_mpi (mpi::communicator world,
              real2D*	matrix,			/* to solve */
              real1D*	vector,			/* target vector */
              real1D*	answer,			/* solution found */
              int		n,			/* size */
              real		tol)			/* tolerance on answer */
{
  int lo, hi;
  int i, blo, bhi;
  int r, c, t;
  real sum, old, dmax, dmax_local, d;
  bool work;

  // initialize
  for (r = 0; r < n; r++){
    answer[r] = 1.0;
  }
  dmax = 2 * tol; // to forestall early exit

  // work
  work = get_block_rows_mpi (world, 0, n, &lo, &hi);
  for (t = 0; (t < SOR_MAX_ITERS) && (dmax >= tol); t++) {
    dmax_local = 0.0;
    if (work) {
      // compute sum_local
      for (r = lo; r < hi; r++) {
        sum = 0.0;
        for (c = 0; c < r; c++) {
          sum += matrix[r][c] * answer[c];
        }
        for (c = r + 1; c < n; c++) {
          sum += matrix[r][c] * answer[c];
        }

        // compute difference
        old = answer[r];
        answer[r] = (1.0 - SOR_OMEGA) * old
          + SOR_OMEGA * (vector[r] - sum) / matrix[r][r];
        d = (real)fabs((double)(old - answer[r]));
        if (d > dmax_local) {
          dmax_local = d;
        }
      }
    }
    // broadcast next answer
    for (i = 0; i < world.size (); i++) {
      if (get_block_rows_mpi (world, 0, n, &blo, &bhi, i)) {
        broadcast (world, &answer[blo], bhi - blo, i);
      }
    }

    // compute maximum difference
    all_reduce (world, dmax_local, dmax, mpi::maximum<real>());

  }

  /* return */
}
