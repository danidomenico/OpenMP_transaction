/**
 * Parallel implementation of invasion percolation
 *
 * \file parallel.cpp
 * \author Andrew Borzenko
 * \date 02-28-09
 */

#include "../include/main.h"
#include "parallel.h"

// public

void
invperc_mpi (mpi::communicator world,
  int2D*		matrix,			/* matrix to invade */
  bool2D*	mask,			/* mask to fill */
  int		nr,			/* row size */
  int		nc,			/* column size */
  real		fraction		/* how much to fill */
){
  int		r, c;			/* row and column indices */
  int		num, i;			/* filling index */
  int min_r, min_c; // row and column of minimum value
  int min_v; // minimum value

  // init
  num = (int)(fraction * nr * nc);

  mask[nr / 2][nc / 2] = TRUE;

  // fill
  for (i = 0; i < num; i++) {
    min_v = -1;
    for (r = 0; r < nr; r++) {
      for (c = 0; c < nc; c++) {
        if ((mask[r][c] != TRUE) && (is_connected (mask, r, c, nr, nc))) {
          if ((matrix[r][c] < min_v) || (min_v == -1)) {
            min_r = r;
            min_c = c;
            min_v = matrix[r][c];
          }
        }
      }
    }
    if (min_v != -1) {
      mask[min_r][min_c] = TRUE;
    }
  }

  /* return */
}

// private

bool is_connected (bool2D* mask, int r, int c, int nr, int nc)
{
  if ((r - 1 >= 0) && (mask[r - 1][c])) {
    return true;
  }
  else if ((r + 1 < nr) && (mask[r + 1][c])) {
    return true;
  }
  else if ((c - 1 >= 0) && (mask[r][c - 1])) {
    return true;
  }
  else if ((c + 1 < nc) && (mask[r][c + 1])) {
    return true;
  }
  return false;
}
