/**
 * \file cowichan_openmp/sor.cpp
 * \brief OpenMP sor implementation.
 * \see CowichanOpenMP::sor
 */

#include "cowichan_openmp.hpp"

void CowichanOpenMP::sor (Matrix matrix, Vector target, Vector solution)
{
  index_t r, c;
  index_t t;
  real sum;
  real oldSolution;
  real diff, maxDiff;

  Vector maxDiffs = NULL;
  index_t num_threads = omp_get_max_threads();

  try {
    maxDiffs = NEW_VECTOR_SZ(real, num_threads);
  }
  catch (...) {out_of_memory();}

  // initialize
  for (r = 0; r < n; r++) {
    solution[r] = 1.0;
  }
  maxDiff = (real)(2 * SOR_TOLERANCE); // to forestall early exit

  for (t = 0; (t < SOR_MAX_ITERS) && (maxDiff >= SOR_TOLERANCE); t++) {

    maxDiff = 0.0;

#pragma omp parallel private(oldSolution, diff, sum, c) firstprivate(maxDiff)
    {
      index_t thread_num = omp_get_thread_num();

#pragma omp for schedule(static)
      for (r = 0; r < n; r++) {
        // compute sum
        sum = 0.0;
        for (c = 0; c < r; c++) {
          sum += MATRIX_SQUARE(matrix, r, c) * solution[c];
        }
        for (c = r + 1; c < n; c++) {
          sum += MATRIX_SQUARE(matrix, r, c) * solution[c];
        }

        // calculate new solution
        oldSolution = solution[r];
        solution[r] = (real)((1.0 - SOR_OMEGA) * oldSolution + SOR_OMEGA *
            (target[r] - sum) / MATRIX_SQUARE(matrix, r, r));

        // compute difference
        diff = (real)fabs((double)(oldSolution - solution[r]));
        if (diff > maxDiff){
          maxDiff = diff;
        }
      }

      maxDiffs[thread_num] = maxDiff;
    }

    maxDiff = maxDiffs[0];
    for (index_t i = 1; i < num_threads; i++) {
      if (maxDiff < maxDiffs[i]) {
        maxDiff = maxDiffs[i];
      }
    }
  }

  delete [] maxDiffs;
}

