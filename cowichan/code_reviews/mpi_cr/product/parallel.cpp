/**
 * Parallel implementation of matrix-vector product
 *
 * \file parallel.cpp
 * \author Andrew Borzenko
 * \date 02-02-09
 */

#include "../include/main.h"
#include "parallel.h"

void product_mpi (mpi::communicator world,
                  real2D* matrix,           /* to multiply by */
                  real1D* vector,          /* to be multiplied */
                  real1D* result,          /* result of multiply */
                  int   nr,                /* row size */
                  int		nc)                /* column size */
{
  int		lo, hi;		/* work controls */
  int		r, c;			/* loop indices */ 
  int rank;

  // work
  if (get_block_rows_mpi (world, 0, nr, &lo, &hi)) {

    for (r = lo; r < hi; r ++) {
      result[r] = matrix[r][0] * vector[0];
      for (c = 1; c < nc; c++) {
        result[r] += matrix[r][c] * vector[c];
      }
    }

  }

  // broadcast result
  for (rank = 0; rank < world.size (); rank++) {
    if (get_block_rows_mpi (world, 0, nr, &lo, &hi, rank)) {
      broadcast (world, &result[lo], hi - lo, rank);
    }
  }

}
