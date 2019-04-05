/**
 * Parallel implementation of halving shuffle
 *
 * \file parallel.cpp
 * \author Andrew Borzenko
 * \date 02-27-09
 */

#include "../include/main.h"
#include "parallel.h"

/*
 * @ half : body of halving shuffle
 * > none
 * + shuffle rows and columns
 */

void
half_mpi (mpi::communicator world,
  int2D*		matrix,			/* to shuffle */
  int		nr,			/* row size */
  int		nc			/* column size */
){
  int		lo, hi;		/* work controls */
  int		r, c, i;		/* loop indices */
  int1D*		tmp;			/* temporary */

  tmp = new int1D[MAXEXT];

  // work
  if (get_block_rows_mpi (world, 0, nr, &lo, &hi)) {
    /* rows */
    for (r = lo; r < hi; r++) {
      for (c = 1, i = 0; c < nc; c += 2, i++) {
        tmp[i] = matrix[r][c];
      }
      for (c = 0, i = 0; c < (nc + 1) / 2; c++, i += 2) {
        matrix[r][c] = matrix[r][i];
      }
      for (c = (nc + 1) / 2, i = 0; c < nc; c++, i++) {
        matrix[r][c] = tmp[i];
      }
    }
  }
  
  // broadcast matrix
  for (i = 0; i < world.size (); i++) {
    if (get_block_rows_mpi (world, 0, nr, &lo, &hi, i)) {
      broadcast (world, matrix[lo], (hi - lo) * nc, i);
    }
  }

  if (get_block_rows_mpi (world, 0, nc, &lo, &hi)) {
    /* columns */
    for (c = lo; c < hi; c++) {
      for (r = 1, i = 0; r < nr; r += 2, i++) {
        tmp[i] = matrix[r][c];
      }
      for (r = 0, i = 0; r < (nr + 1) / 2; r++, i += 2) {
        matrix[r][c] = matrix[i][c];
      }
      for (r = (nr + 1) / 2, i = 0; r < nr; r++, i++) {
        matrix[r][c] = tmp[i];
      }
    }
  }

  // broadcast matrix
  for (i = 0; i < world.size (); i++) {
    if (get_block_rows_mpi (world, 0, nc, &lo, &hi, i)) {
      for (c = lo; c < hi; c++) {
        for (r = 0; r < nr; r++) {
          broadcast (world, matrix[r][c], i);
        }
      }
    }
  }

  delete [] tmp;

  /* return */
}
