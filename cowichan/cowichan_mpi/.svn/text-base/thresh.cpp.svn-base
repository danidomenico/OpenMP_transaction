/**
 * \file cowichan_mpi/thresh.cpp
 * \brief MPI thresh implementation.
 * \see CowichanMPI::thresh
 */

#include "cowichan_mpi.hpp"

namespace cowichan_mpi
{

/**
 * \brief Calculates sum of two elements.
 */
template<typename T>
struct sum {

  /**
   * Add two elements.
   * \param a element 1.
   * \param b element 2.
   * \return The sum of element 1 and 2.
   */
  T operator()(T a, T b)
  {
    return a + b;
  }
};

}

/*****************************************************************************/

void CowichanMPI::thresh(IntMatrix matrix, BoolMatrix mask)
{
  index_t lo, hi; // work controls
  index_t i, r, c; // loop indices
  bool work; // do useful work?
  index_t* hist_local; // own histogram section
  index_t retain; // number to retain
  INT_TYPE vmax_local;
  INT_TYPE vmax;
  index_t* hist;

  // more setup
  retain = (index_t)(threshPercent * nc * nr);

  // any useful work to do?
  work = get_block (world, 0, nr, &lo, &hi);

  // find max value in matrix
  vmax_local = 0;
  if (work) {
    for (r = lo; r < hi; r++) {
      for (c = 0; c < nc; c++) {
        if (vmax_local < MATRIX_RECT(matrix, r, c)) {
          vmax_local = MATRIX_RECT(matrix, r, c);
        }
      }
    }
  }
  // reduce to maximum
  all_reduce (world, vmax_local, vmax, mpi::maximum<INT_TYPE> ());

  try {
    hist = new index_t[vmax + 1];
    hist_local = new index_t[vmax + 1];
  }
  catch (...) {out_of_memory();}

  // initialize own portion of histogram
  for (i = 0; i <= (index_t)vmax; i++) {
    hist_local[i] = 0;
  }

  // count
  if (work) {
    for (r = lo; r < hi; r++) {
      for (c = 0; c < nc; c++) {
        hist_local[MATRIX_RECT(matrix, r, c)]++;
      }
    }
  }

  // calculate retention
  for (i = 0; i <= (index_t)vmax; i++) {
    all_reduce (world, hist_local[i], hist[i], sum<index_t> ());
  }
  for (i = vmax; ((i >= 0) && (retain > 0)); i--) {
    retain -= hist[i];
  }
  retain = i;

  // threshold
  if (work) {
    for (r = lo; r < hi; r++) {
      for (c = 0; c < nc; c++) {
        MATRIX_RECT(mask, r, c) = (index_t)MATRIX_RECT(matrix, r, c) > retain;
      }
    }
  }

  // broadcast mask
  for (i = 0; i < world.size (); i++) {
    if (get_block (world, 0, nr, &lo, &hi, i)) {
      broadcast (world, &MATRIX_RECT(mask, lo, 0), (int)((hi - lo) * nc),
          (int)i);
    }
  }

  // takedown
  delete [] hist;
  delete [] hist_local;
}

