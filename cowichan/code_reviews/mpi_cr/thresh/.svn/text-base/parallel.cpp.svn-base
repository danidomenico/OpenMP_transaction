/**
 * Parallel implementation of histogram thresholding
 *
 * \file parallel.cpp
 * \author Andrew Borzenko
 * \date 03-02-09
 */

#include "../include/main.h"
#include "parallel.h"

/*
 * @ thresh : do body of histogram thresholding
 * > none
 * + create mask
 */

void thresh_mpi (mpi::communicator world,
                 int2D*		matrix,			/* to threshold */
                 bool2D*	mask,			/* threshold mask */
                 int		nr,			/* row size */
                 int		nc,			/* column size */
                 real		fraction)		/* how much to keep */
{
  int		lo, hi;		/* work controls */
  int		i, r, c;		/* loop indices */
  bool		work;			/* do useful work? */
  int	      * hist_local;		/* own histogram section */
  int	retain; // number to retain
  int vmax_local;
  int vmax;
  int* hist;

  // more setup
  retain = (int)(fraction * nc * nr);

  // any useful work to do?
  work = get_block_rows_mpi (world, 0, nr, &lo, &hi);

  // find max value in matrix
  vmax_local = 0;
  if (work) {
    for (r = lo; r < hi; r++) {
      for (c = 0; c < nc; c++) {
        if (vmax_local < matrix[r][c]) {
          vmax_local = matrix[r][c];
        }
      }
    }
  }
  // reduce to maximum
  all_reduce (world, vmax_local, vmax, mpi::maximum<int> ());

  hist = new int[vmax + 1];
  hist_local = new int[vmax + 1];

  // initialize own portion of histogram
  for (i = 0; i <= vmax; i++) {
    hist_local[i] = 0;
  }

  // count
  if (work) {
    for (r = lo; r < hi; r++) {
      for (c = 0; c < nc; c++) {
        hist_local[matrix[r][c]]++;
      }
    }
  }

  // calculate retention
  for (i = 0; i <= vmax; i++) {
    all_reduce (world, hist_local[i], hist[i], sum<int> ());
  }
  for (i = vmax; ((i >= 0) && (retain > 0)); i--) {
    retain -= hist[i];
  }
  retain = i;

  // threshold
  if (work) {
    for (r = lo; r < hi; r++) {
      for (c = 0; c < nc; c++) {
        mask[r][c] = matrix[r][c] > retain;
      }
    }
  }

  // broadcast mask
  for (i = 0; i < world.size (); i++) {
    if (get_block_rows_mpi (world, 0, nr, &lo, &hi, i)) {
      broadcast (world, mask[lo], (hi - lo) * nc, i);
    }
  }

  // takedown
  delete [] hist;
  delete [] hist_local;

  /* return */
}
