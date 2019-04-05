/**
 * \file cowichan_mpi/outer.cpp
 * \brief MPI outer product implementation.
 * \see CowichanMPI::outer
 */

#include "cowichan_mpi.hpp"
void CowichanMPI::outer(PointVector points, Matrix matrix, Vector vector)
{
  index_t  lo, hi;    /* work controls */
  index_t  r, c;      /* loop indices */
  real    d;      /* distance */
  real d_max_local = -1.0; // maximum distance
  real d_max; // maximum distance
  bool    work;      /* do useful work? */
  index_t i, j;

  /* all elements except matrix diagonal */
  work = get_block (world, 0, n, &lo, &hi);
  if (work) {
    for (r = lo; r < hi; r++) {
      vector[r] = Point::distance(points[r], Point());
      for (c = 0; c < r; c++) {
        d = Point::distance(points[r], points[c]);
        if (d > d_max_local) {
          d_max_local = d;
        }
        // fill columns 0 to r only
        MATRIX(matrix, r, c) = d;
      }
    }
  }

  // reduce to maximum d's
  all_reduce (world, d_max_local, d_max, mpi::maximum<real>());
  
  /* matrix diagonal */
  d = d_max * n;
  if (work) {
    for (r = lo; r < hi; r++) {
      MATRIX(matrix, r, r) = d;
    }
  }

  // broadcast matrix, realVec
  for (i = 0; i < world.size (); i++) {
    if (get_block (world, 0, n, &lo, &hi, i)) {
      broadcast (world, &vector[lo], (int)(hi - lo), (int)i);
      // broadcast row by row since n may be smaller than MAXEXT
      for (j = lo; j < hi; j++) {
        broadcast (world, &MATRIX(matrix, j, 0), (int)n, (int)i);
      }
    }
  }

  // fill in the rest to make symmetric matrix
  for (r = 0; r < n; r++) {
    for (c = 0; c < r; c++) {
      MATRIX(matrix, c, r) = MATRIX(matrix, r, c);
    }
  }

  /* return */  
}

