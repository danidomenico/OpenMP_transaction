/**
 * \file cowichan_mpi/product.cpp
 * \brief MPI matrix product implementation.
 * \see CowichanMPI::product
 */

#include "cowichan_mpi.hpp"
void CowichanMPI::product(Matrix matrix, Vector vector, Vector result)
{
  index_t  lo, hi;    /* work controls */
  index_t  r, c;    /* loop indices */ 
  int rank;

  // work
  if (get_block (world, 0, nr, &lo, &hi)) {

    for (r = lo; r < hi; r ++) {
      result[r] = MATRIX(matrix, r, 0) * vector[0];
      for (c = 1; c < nc; c++) {
        result[r] += MATRIX(matrix, r, c) * vector[c];
      }
    }

  }

  // broadcast result
  for (rank = 0; rank < world.size (); rank++) {
    if (get_block (world, 0, nr, &lo, &hi, rank)) {
      broadcast (world, &result[lo], (int)(hi - lo), rank);
    }
  }

}

