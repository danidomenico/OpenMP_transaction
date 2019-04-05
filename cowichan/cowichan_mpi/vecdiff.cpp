/**
 * \file cowichan_mpi/vecdiff.cpp
 * \brief MPI vector difference implementation.
 * \see CowichanMPI::vecdiff
 */

#include "cowichan_mpi.hpp"
real CowichanMPI::vecdiff(Vector left, Vector right)
{
  index_t  lo, hi;    /* work controls */
  index_t  i;      /* loop index */
  real    d;      /* difference */
  real max_d;           // maximum difference

  max_d = (real) fabs(left[0] - right[0]);

  if (get_block (world, 0, n, &lo, &hi)) {
    for (i = lo; i < hi; i++) {
      d = (real) fabs(left[i] - right[i]);
      if (d > max_d) {
        max_d = d;
      }
    }
  }

  // reduce to maximum d's
  real diff;
  all_reduce (world, max_d, diff, mpi::maximum<real>());
  return diff;
}

