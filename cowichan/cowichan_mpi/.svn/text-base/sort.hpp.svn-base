/**
 * \file cowichan_mpi/sort.hpp
 * \brief Sorting algorithms that use MPI.
 *
 * One algorithms is included:
 * <OL>
 * <LI> Parallel Quick Sort</LI>
 * </OL>
 */

#ifndef __sort_hpp__
#define __sort_hpp__

#include "cowichan_mpi.hpp"

namespace cowichan_mpi
{

/**
 * Quick sort cutoff (becomes insertion sort) - must be > 1
 */
#define QUICK_SORT_CUTOFF 100

/**
 * Get number of remaining nodes, i.e. not doing useful work.
 * \param world global communicator.
 * \param level current recursion depth.
 * \return Number of remaining nodes.
 */
index_t getNumRemainingNodes(const mpi::communicator& world, index_t level);

/**
 * Calculates a power of 2.
 * \param n power is limited by sizeof(index_t) * 8 - 2 to prevent overflow.
 * \return 2^n.
 */
index_t power2(index_t n);

/**
 * This performs parallel quick sort.
 * \param world global communicator.
 * \param vector vector to sort.
 * \param len vector length.
 * \param level depth of recursion.
 * \param owner rank of the owner of this sort.
 */
void quick_sort(const mpi::communicator& world, WeightedPointVector vector,
    index_t len, index_t level = 0, index_t owner = 0);

/**
 * Partitions vector into points less than and greater than the pivot.
 * \param vector vector to partition.
 * \param len vector length.
 * \param pivotIndex index to partition around.
 * \return New pivot index.
 */
index_t quick_sort_partition(WeightedPointVector vector, index_t len,
    index_t pivotIndex);

}

#endif

