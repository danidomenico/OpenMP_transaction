/**
 * \file cowichan_mpi/sor.cpp
 * \brief MPI successive over-relaxation implementation.
 * \see CowichanMPI::sor
 */

#include "cowichan_mpi.hpp"
void CowichanMPI::sor(Matrix matrix, Vector target, Vector solution)
{
  index_t lo, hi;
  index_t i, blo, bhi;
  index_t r, c, t;
  real sum, old, dmax, dmax_local, d;
  bool work;

  // initialize
  for (r = 0; r < n; r++){
    solution[r] = 1.0;
  }
  dmax = (real)(2 * SOR_TOLERANCE); // to forestall early exit

  // work
  work = get_block (world, 0, n, &lo, &hi);
  for (t = 0; (t < SOR_MAX_ITERS) && (dmax >= SOR_TOLERANCE); t++) {
    dmax_local = 0.0;
    if (work) {
      // compute sum_local
      for (r = lo; r < hi; r++) {
        sum = 0.0;
        for (c = 0; c < r; c++) {
          sum += MATRIX(matrix, r, c) * solution[c];
        }
        for (c = r + 1; c < n; c++) {
          sum += MATRIX(matrix, r, c) * solution[c];
        }

        // compute difference
        old = solution[r];
        solution[r] = (real)((1.0 - SOR_OMEGA) * old
          + SOR_OMEGA * (target[r] - sum) / MATRIX(matrix, r, r));
        d = (real)fabs((double)(old - solution[r]));
        if (d > dmax_local) {
          dmax_local = d;
        }
      }
    }
    // broadcast next answer
    for (i = 0; i < world.size (); i++) {
      if (get_block (world, 0, n, &blo, &bhi, i)) {
        broadcast (world, &solution[blo], (int)(bhi - blo), (int)i);
      }
    }

    // compute maximum difference
    all_reduce (world, dmax_local, dmax, mpi::maximum<real>());

  }

  /* return */  
}

