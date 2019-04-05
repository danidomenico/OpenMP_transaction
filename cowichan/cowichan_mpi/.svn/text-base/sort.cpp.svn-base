/**
 * \file cowichan_mpi/sort.cpp
 * \brief Implementation of MPI sorting algorithms.
 */

#include "sort.hpp"

namespace cowichan_mpi
{

index_t getNumRemainingNodes(const mpi::communicator& world, index_t level)
{
  index_t total_nodes = world.size();
  index_t level_nodes = power2(level);

  if (level_nodes > total_nodes) {
    return 0;
  }

  return total_nodes - level_nodes;
}

index_t power2(index_t n)
{
  const index_t max_n = sizeof(index_t) * 8 - 2;

  if (n > max_n) {
    n = max_n;
  }

  index_t result = 1;

  for (index_t i = 0; i < n; i++) {
    result *= 2;
  }

  return result;
}

void quick_sort(const mpi::communicator& world, WeightedPointVector vector,
    index_t len, index_t level, index_t owner)
{
  if (len > QUICK_SORT_CUTOFF) {
    WeightedPoint tmp;
  
    // use median of three
    if (vector[len / 2] < vector[0]) {
      // order 0 and len / 2
      tmp = vector[0];
      vector[0] = vector[len / 2];
      vector[len / 2] = tmp;
    }

    if (vector[len - 1] < vector[0]) {
      // order 0 and len - 1
      tmp = vector[0];
      vector[0] = vector[len - 1];
      vector[len - 1] = tmp;
    }
    
    if (vector[len - 1] < vector[len / 2]) {
      // order len / 2 and len - 1
      tmp = vector[len / 2];
      vector[len / 2] = vector[len - 1];
      vector[len - 1] = tmp;
    }
    
    index_t pivotNewIndex = quick_sort_partition(vector, len, len / 2);

    index_t nodesLeft = getNumRemainingNodes(world, level);

    if (nodesLeft > 1)
    {
      // branch more
      index_t rank = world.rank();

      index_t leftOwner = owner + power2(level);

      if (leftOwner >= world.size()) {
        leftOwner = owner;
      }

      index_t multiple = power2(level + 1);

      if ((rank >= leftOwner) && ((rank - leftOwner) % multiple == 0)) {
        quick_sort(world, vector, pivotNewIndex, level + 1, leftOwner);
      }
      
      if ((rank >= owner) && ((rank - owner) % multiple == 0)) {
        quick_sort(world, &vector[pivotNewIndex + 1], len - pivotNewIndex - 1,
            level + 1, owner);
      }

      if (owner != leftOwner)
      {
        // send results from the leftOwner to owner
        if (rank == leftOwner) {
          world.isend((int)owner, 0, vector, (int)pivotNewIndex);
        }
        else if (rank == owner) {
          world.recv((int)leftOwner, 0, vector, (int)pivotNewIndex);
        }
      }
    }
    else
    {
      // no more branching
      quick_sort(world, vector, pivotNewIndex, level + 1, owner);
      quick_sort(world, &vector[pivotNewIndex + 1], len - pivotNewIndex - 1,
          level + 1, owner);
    }
  }
  else if (len > 1)
  {
    // insertion sort
    for (index_t i = 1; i < len; i++)
    {
      WeightedPoint value = vector[i];
      index_t j = i - 1;
      
      while ((j >= 0) && (value < vector[j]))
      {
        vector[j + 1] = vector[j];
        j--;
      }
      
      vector[j + 1] = value;
    }
  }
}

index_t quick_sort_partition(WeightedPointVector vector, index_t len,
    index_t pivotIndex)
{
  WeightedPoint pivot = vector[pivotIndex];
  
  vector[pivotIndex] = vector[len - 1];

  index_t left = 0;
  index_t right = len - 2;

  for (;;)
  {
    // median of three guarantees that left and right will
    // never get out of bounds
  
    while (vector[left] < pivot)
    {
      left++;
    }
  
    while(pivot < vector[right])
    {
      right--;
    }
    
    // left points to an element greater or equal to the pivot
    // right points to an element less than or equal to the pivot
    
    if(left < right) {
      // swap left and right
      WeightedPoint tmp = vector[left];
      vector[left++] = vector[right];
      vector[right--] = tmp;
    }
    else {
      // move left to len - 1
      // put pivot to left
      vector[len - 1] = vector[left];
      vector[left] = pivot;
      return left;
    }
  }
  
}

}

