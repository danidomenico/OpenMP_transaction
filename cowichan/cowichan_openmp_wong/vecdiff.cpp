/**
 * \file cowichan_openmp_wong/vecdiff.cpp
 * \brief OpenMP vecdiff implementation (transactional memory).
 * \see CowichanOpenMP::vecdiff
 */

#include "../cowichan_openmp/cowichan_openmp.hpp"

real CowichanOpenMP::vecdiff (Vector actual, Vector computed) {
    index_t i;
    real diff;
    real maxDiff;

    maxDiff = (real)fabs((double)(actual[0] - computed[0]));
    
    #pragma omp for schedule(static) private(diff)
    for (i = 1; i < n; i++) {
        diff = (real)fabs((double)(actual[i] - computed[i]));
		#pragma omp transaction
		{
			if (maxDiff < diff) {
				maxDiff = diff;
			}
		}
    }
    
    return maxDiff;
}

