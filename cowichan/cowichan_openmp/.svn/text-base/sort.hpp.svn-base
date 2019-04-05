/**
 * \file cowichan_openmp/sort.hpp
 * \brief Sorting algorithms that use OpenMP.
 *
 * Two algorithms are included:
 * <OL>
 * <LI> Parallel Histogram Sort - similar to bucket sort, but does not require
 * additional space for buckets (in-place). </LI>
 * <LI> Parallel Quick Sort - requires OpenMP 3.0</LI>
 * </OL>
 */

#ifndef __sort_hpp__
#define __sort_hpp__

#include "cowichan_openmp.hpp"

namespace cowichan_openmp
{

/**
 * This performs parallel histogram sort.
 * \param vector vector to sort.
 * \param len vector length.
 */
void histogram_sort(WeightedPointVector vector, index_t len);

// OpenMP tasks are not supported by msvc yet
#if defined(LIN32) || defined(LIN64)

/**
 * Quick sort cutoff (becomes insertion sort) - must be > 1
 */
#define QUICK_SORT_CUTOFF 100

/**
 * Parallel sort cutoff (no more tasks generated).
 */
#define QUICK_SORT_TASK_CUTOFF 1000

/**
 * This performs parallel quick sort.
 * \param vector vector to sort.
 * \param len vector length.
 */
void quick_sort(WeightedPointVector vector, index_t len);

/**
 * Partitions vector into points less than and greater than the pivot.
 * \param vector vector to partition.
 * \param len vector length.
 * \param pivotIndex index to partition around.
 * \return New pivot index.
 */
index_t quick_sort_partition(WeightedPointVector vector, index_t len,
    index_t pivotIndex);
    
#endif

}

#endif

