/**
 * \file cowichan_openmp/vecdiff.cpp
 * \brief OpenMP vecdiff implementation.
 * \see CowichanOpenMP::vecdiff
 */

#include "cowichan_openmp.hpp"

real CowichanOpenMP::vecdiff (Vector actual, Vector computed)
{
  index_t i;
  real diff;
  real maxDiff;

  Vector maxDiffs = NULL;
  index_t num_threads = omp_get_max_threads();

  try {
    maxDiffs = NEW_VECTOR_SZ(real, num_threads);
  }
  catch (...) {out_of_memory();}

#pragma omp parallel private(diff, maxDiff)
  {
    index_t thread_num = omp_get_thread_num();
    maxDiff = (real)fabs((double)(actual[0] - computed[0]));
#pragma omp for schedule(static)
    for (i = 1; i < n; i++) {
      diff = (real)fabs((double)(actual[i] - computed[i]));
      if (maxDiff < diff) {
        maxDiff = diff;
      }
    }
    maxDiffs[thread_num] = maxDiff;
  }

  maxDiff = maxDiffs[0];
  for (i = 1; i < num_threads; i++) {
    if (maxDiff < maxDiffs[i]) {
      maxDiff = maxDiffs[i];
    }
  }

  delete [] maxDiffs;

  return maxDiff;
}

