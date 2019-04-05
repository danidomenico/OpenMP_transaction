/**
 * \file cowichan_mpi/gauss.cpp
 * \brief MPI gauss implementation.
 * \see CowichanMPI::gauss
 */

#include "cowichan_mpi.hpp"

void CowichanMPI::gauss (Matrix matrix, Vector target, Vector solution)
{
  index_t i, j, k;
  index_t jlo, jhi;
  index_t p;

  index_t num_processes = world.size();

  // forward elimination
  for (i = 0; i < n; i++) {
    // get row with maximum column i
    index_t max = i;
    for (j = i + 1; j < n; j++) {
      if (fabs(MATRIX(matrix, j, i)) > fabs(MATRIX(matrix, max, i))) {
        max = j;
      }
    }

    real tmp;
    // swap max row with row i
    for (j = i; j < n; j++) {
      tmp = MATRIX(matrix, i, j);
      MATRIX(matrix, i, j) = MATRIX(matrix, max, j);
      MATRIX(matrix, max, j) = tmp;
    }
    tmp = target[i];
    target[i] = target[max];
    target[max] = tmp;

    // eliminate i-th column in j-th row
    real column_i = MATRIX(matrix, i, i);

    if (get_block (world, i + 1, n, &jlo, &jhi))
    {
      for (j = jlo; j < jhi; j++) {
        real factor = -(MATRIX(matrix, j, i) / column_i);
        for (k = n - 1; k >= i; k--) {
          MATRIX(matrix, j, k) += MATRIX(matrix, i, k) * factor;
        }
        target[j] += target[i] * factor;
      }
    }

    for (p = 0; p < num_processes; p++)
    {
      if (get_block (world, i + 1, n, &jlo, &jhi, p))
      {
        broadcast (world, &MATRIX(matrix, jlo, 0), (int)((jhi - jlo) * n),
            (int)p);
        broadcast (world, &target[jlo], (int)(jhi - jlo), (int)p);
      }
    }
  }

  // back substitution
  for (k = (n - 1); k >= 0; k--) {
    solution[k] = target[k] / MATRIX_SQUARE(matrix, k, k);
    for (i = k - 1; i >= 0; i--) {
      target[i] = target[i] - (MATRIX_SQUARE(matrix, i, k) * solution[k]);
    }
  }
}

