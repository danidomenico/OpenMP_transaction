/**
 * Parallel implementation of vector norm
 *
 * \file parallel.cpp
 * \author Andrew Borzenko
 * \date 02-10-09
 */

#include "../include/main.h"
#include "parallel.h"

void norm_mpi (mpi::communicator world,
               pt1D* vec,      /* points to normalize */
               int  n)       /* length of vector */
{
  pt		ptMin, ptMax;		/* pseudo-points */
  real		sclX, sclY;		/* scaling factors */
  int		i;			/* loop index */
  int		lo, hi;		/* work controls */
  int rank;

  redPt1DPos(vec, n, &ptMin, &ptMax);
  if (get_block_rows_mpi (world, 0, n, &lo, &hi)) {
    /* scaling factors */
    sclX = (ptMax.x == ptMin.x) ? 0.0 : 1/(ptMax.x - ptMin.x);
    sclY = (ptMax.y == ptMin.y) ? 0.0 : 1/(ptMax.y - ptMin.y);
    /* scale */
    for (i = lo; i < hi; i++) {
      vec[i].x = sclX * (vec[i].x - ptMin.x);
      vec[i].y = sclY * (vec[i].y - ptMin.y);
    }
  }

  // broadcast normalized values
  for (i = 0; i < n; i++) {
    rank = get_block_rank_mpi (world, 0, n, i);
    broadcast (world, vec[i].x, rank);
    broadcast (world, vec[i].y, rank);
  }

  /* return */
}
