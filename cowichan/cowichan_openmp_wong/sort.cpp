/**
 * \file cowichan_openmp_wong/sort.cpp
 * \brief Implementation of OpenMP sorting algorithms.
 */

#include "sort.hpp"

namespace cowichan_openmp
{

//Versão sem OpenMP tasks, não alterada, pois o compilador suporta tasks
void histogram_sort(WeightedPointVector vector, index_t len)
{
  const index_t BUCKETS_PER_THREAD = 50;

  index_t num_threads = omp_get_max_threads();
  index_t num_buckets = BUCKETS_PER_THREAD * num_threads;

  // sort serially if array is too small
  if (len < num_buckets) {
    std::sort(vector, &vector[len]);
    return;
  }

  index_t i, j;

  // find min/max values
  INT_TYPE* minWeights = NULL;
  INT_TYPE* maxWeights = NULL;
  INT_TYPE minWeight, maxWeight;

  try {
    minWeights = NEW_VECTOR_SZ(INT_TYPE, num_threads);
    maxWeights = NEW_VECTOR_SZ(INT_TYPE, num_threads);
  }
  catch (...) {out_of_memory();}

#pragma omp parallel private(i)
  {
    index_t thread_num = omp_get_thread_num();
    minWeights[thread_num] = vector[0].weight;
    maxWeights[thread_num] = vector[0].weight;
#pragma omp for schedule(static)
    for (i = 1; i < len; i++) {
      if (minWeights[thread_num] > vector[i].weight) {
        minWeights[thread_num] = vector[i].weight;
      }
      else if (maxWeights[thread_num] < vector[i].weight) {
        maxWeights[thread_num] = vector[i].weight;
      }
    }
  }

  minWeight = minWeights[0];
  maxWeight = maxWeights[0];
  for (i = 1; i < num_threads; i++) {
    if (minWeight > minWeights[i]) {
      minWeight = minWeights[i];
    }
    else if (maxWeight < maxWeights[i]) {
      maxWeight = maxWeights[i];
    }
  }

  delete [] minWeights;
  delete [] maxWeights;

  // count number of elements in each bucket
  index_t** threadCounts = NULL;
  index_t* counts = NULL;
  index_t* offsets = NULL;

  try {
    counts = NEW_VECTOR_SZ(index_t, num_buckets);
    offsets = NEW_VECTOR_SZ(index_t, num_buckets + 1);
    threadCounts = NEW_VECTOR_SZ(index_t*, num_threads);
    for (i = 0; i < num_threads; i++) {
      threadCounts[i] = NEW_VECTOR_SZ(index_t, num_buckets);
    }
  }
  catch (...) {out_of_memory();}

#pragma omp parallel private(i)
  {
    index_t thread_num = omp_get_thread_num();
    for (i = 0; i < num_buckets; i++) {
      threadCounts[thread_num][i] = 0;
    }
#pragma omp for schedule(static)
    for (i = 0; i < len; i++) {
      index_t bucket = num_buckets * ((index_t)(vector[i].weight - minWeight))
          / ((index_t)(maxWeight - minWeight + 2));
      threadCounts[thread_num][bucket]++;
    }
  }

  for (i = 0; i < num_buckets; i++) {
    counts[i] = threadCounts[0][i];
    for (j = 1; j < num_threads; j++) {
      counts[i] += threadCounts[j][i];
    }
  }

  for (i = 0; i < num_threads; i++) {
    delete [] threadCounts[i];
  }
  delete [] threadCounts;

  // calculate offsets
  index_t offset = 0;
  index_t tmp;
  for (i = 0; i < num_buckets; i++) {
    tmp = counts[i];
    offsets[i] = counts[i] = offset;
    offset += tmp;
  }
  offsets[num_buckets] = len;

  // put elements into appropriate buckets by swapping
  // NOTE: not parallel, in-place
  WeightedPoint tmpPoint;
  index_t src, dest;
  index_t bucket;

  src = 0;
  while (src < len) {
    bucket = num_buckets * ((index_t)(vector[src].weight - minWeight))
        / ((index_t)(maxWeight - minWeight + 2));

    if ((src >= offsets[bucket]) && (src < offsets[bucket + 1])) {
      src++;
      continue;
    }

    dest = counts[bucket]++;

    tmpPoint = vector[dest];
    vector[dest] = vector[src];
    vector[src] = tmpPoint;
  }

  delete [] counts;

  // sort individual buckets
#pragma omp parallel for schedule(dynamic)
  for (i = 0; i < num_buckets; i++) {
    if (offsets[i] != offsets[i + 1]) {
      std::sort(&vector[offsets[i]], &vector[offsets[i + 1]]);
    }
  }

  delete [] offsets;
}

#if defined(LIN32) || defined(LIN64)
//Sempre vai ordenar por este método, pois o compilador suporta OpenMP tasks
void quick_sort(WeightedPointVector vector, index_t len)
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
#pragma omp task if(len > QUICK_SORT_TASK_CUTOFF)
    quick_sort(vector, pivotNewIndex);
#pragma omp task if(len > QUICK_SORT_TASK_CUTOFF)
    quick_sort(&vector[pivotNewIndex + 1], len - pivotNewIndex - 1);
#pragma omp taskwait
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

#endif

}

