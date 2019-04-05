/**
 * \file cowichan_mpi/mandel.cpp
 * \brief MPI mandelbrot set implementation.
 * \see CowichanMPI::mandel
 */

#include "cowichan_mpi.hpp"
void CowichanMPI::mandel(IntMatrix matrix) {
  index_t r, c; // row and column indices
  real dx, dy; // per-step deltas
  index_t row_count = 0;
  int i;
  mpi::status status;
  int source;
  const int WORK_REQUEST_TAG = 0;
  const int WORK_RESPONSE_TAG = 1;
  const int NO_MORE_WORK = -1;
  int processed_rows = 0;

  dx = mandelDx / (nc - 1);
  dy = mandelDy / (nr - 1);
  
  if (world.size () > 1) {
    if (world.rank () == 0) {
      // control process

      // send out work
      while (row_count < nr) {
        status = world.recv (mpi::any_source, WORK_REQUEST_TAG);
        source = status.source ();
        // send next row
        world.isend (source, WORK_RESPONSE_TAG, row_count);
        row_count++;
      }
      // send out no more work
      for (i = 1; i < world.size (); i++) {
        status = world.recv (mpi::any_source, WORK_REQUEST_TAG);
        source = status.source ();
        world.isend (source, WORK_RESPONSE_TAG, NO_MORE_WORK);
      }
      // receive results
      for (r = 0; r < nr; r++) {
        world.recv (mpi::any_source, (int)(r + 1), &MATRIX_RECT(matrix, r, 0),
            (int)nc);
      }
    }
    else {
      // work process
      while (true) {
        // request next row
        world.send (0, WORK_REQUEST_TAG);
        world.recv (0, WORK_RESPONSE_TAG, r);
        if (r != NO_MORE_WORK) {
          for (c = 0; c < nc; c++) {
            MATRIX_RECT(matrix, r, c) = mandel_calc (mandelX0 + (c * dx),
                mandelY0 + (r * dy));
          }
          processed_rows++;
          // send results
          world.isend (0, (int)(r + 1), &MATRIX_RECT(matrix, r, 0), (int)nc);
        }
        else {
          break;
        }
      }
#if defined(TEST_OUTPUT) || defined(TEST_TIME)
      printf ("processed rows: %d\n", processed_rows);
#endif
    }
    // broadcast matrix
    for (r = 0; r < nr; r++) {
      broadcast (world, &MATRIX_RECT(matrix, r, 0), (int)nc, 0);
    }
  }
  else {
    // compute serially, as we only have one process
    for (r = 0; r < nr; r++) {
      for (c = 0; c < nc; c++) {
        MATRIX_RECT(matrix, r, c) = mandel_calc (mandelX0 + (c * dx),
            mandelY0 + (r * dy));
      }
    }
  }

  /* return */
}

/*****************************************************************************/

namespace cowichan_mpi
{

INT_TYPE mandel_calc (real x, real y)
{
  real r = 0.0, i = 0.0; // real and imaginary parts
  real rs = 0.0, is = 0.0; // " ", squared
  INT_TYPE iter = 0; // number of iterations

  do {
    i = (((real)2.0) * r * i) + x;
    r = (rs - is) + y;
    iter++;
    rs = r * r;
    is = i * i;
  } while ((iter < MANDEL_MAX_ITER) && ((rs + is) < MANDEL_INFINITY));

  return iter;
}

}

