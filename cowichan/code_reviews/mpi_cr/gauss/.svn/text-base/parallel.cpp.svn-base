/**
 * Parallel implementation of generic gauss
 *
 * \file parallel.cpp
 * \author Andrew Borzenko
 * \date 02-03-09
 */

#include "../include/main.h"
#include "parallel.h"

void gauss_mpi (mpi::communicator world,
                real2D*	matrix,			/* to solve */
                real1D*	vector,			/* target vector */
                real1D*	answer,			/* solution found */
                int		n)
{
  bool		work;			/* work control */
  int		lo, hi;		/* work controls */
  int		r, c, k;		/* indices */
#if GRAPHICS
  int		gfxCount = 0;
#endif
  int rank;

  // forward elimination
  for (k=0; k<n; k++){
#if GRAPHICS
    if (MASTER(tid)){
      gfx_gauss(gfxCount++, matrix, vector, answer, n);
    }
    thr_bar(tid);
#endif

    // calculate pivots in k'th column
    if ((work = get_block_rows_mpi (world, k + 1, n, &lo, &hi))) {
      for (r = lo; r < hi; r++) {
        matrix[r][k] = matrix[r][k] / matrix[k][k];
      }
    }
    // broadcast rows
    for (r = k + 1; r < n; r++) {
      rank = get_block_rank_mpi (world, k + 1, n, r);
      //printf ("k is %d, n is %d, lo is %d, hi is %d, r is %d, rank is %d\n", k, n, lo, hi, r, rank);
      broadcast (world, matrix[r], n, rank);
    }

    // update elements below k'th row
    if (work) {
      for (r = lo; r < hi; r++) {
        for (c = k + 1; c < n; c++) {
          matrix[r][c] = matrix[r][c] - (matrix[r][k] * matrix[k][c]);
        }
      }
    }
    // broadcast rows
    for (r = k + 1; r < n; r++) {
      rank = get_block_rank_mpi (world, k + 1, n, r);
      broadcast (world, matrix[r], n, rank);
    }

    // update element of solution vector
    if (work) {
      for (r = lo; r < hi; r++) {
        vector[r] = vector[r] - (matrix[r][k] * vector[k]);
      }
    }
    // broadcast solution vector
    for (r = k + 1; r < n; r++) {
      rank = get_block_rank_mpi (world, k + 1, n, r);
      broadcast (world, vector[r], rank);
    }
  }

  // back substitution
  for (k=(n-1); k>=0; k--){
    // set this element
    answer[k] = vector[k] / matrix[k][k];
    
    // update other elements
    if (get_block_rows_mpi (world, 0, k, &lo, &hi)) {
      for (r = lo; r < hi; r++) {
	    vector[r] = vector[r] - (matrix[r][k] * answer[k]);
      }
    }
    // broadcast solution vector
    for (r = 0; r < k; r++) {
      rank = get_block_rank_mpi (world, 0, k, r);
      broadcast (world, vector[r], rank);
    }
  }

  /* return */
}
