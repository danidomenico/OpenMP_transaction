/**
 * Parallel implementation of Mandelbrot set
 *
 * \file parallel.cpp
 * \author Andrew Borzenko
 * \date 02-09-09
 */

#include "../include/main.h"
#include "parallel.h"

// public

void mandel_mpi (mpi::communicator world,
                 int2D*		matrix,			/* to fill */
                 int		nr,			/* row size */
                 int		nc,			/* column size */
                 real		base_x,			/* lower left corner */
                 real		base_y,			/* lower left corner */
                 real		ext_x,			/* extent */
                 real		ext_y)			/* extent */
{
  int		r, c;			/* row and column indices */
  real		dx, dy;			/* per-step deltas */
#if GRAPHICS
  int		gfxCount = 0;		/* number of times graphics called */
#endif

  int row_count = 0;
  int i;
  mpi::status status;
  int source;
  const int WORK_REQUEST_TAG = 0;
  const int WORK_RESPONSE_TAG = 1;
  const int NO_MORE_WORK = -1;
  int processed_rows = 0;

  dx = ext_x / (nr - 1);
  dy = ext_y / (nc - 1);

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
        world.recv (mpi::any_source, r + 1, matrix[r], nc);
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
            matrix[r][c] = mandel_calc_mpi (base_x + (r * dx), base_y + (c * dy));
          }
          processed_rows++;
          // send results
          world.isend (0, r + 1, matrix[r], nc);
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
      broadcast (world, matrix[r], nc, 0);
    }
  }
  else {
    for (r = 0; r < nr; r++) {
      for (c = 0; c < nc; c++) {
        matrix[r][c] = mandel_calc_mpi (base_x + (r * dx), base_y + (c * dy));
      }
    }
  }

#if GRAPHICS
  gfx_mandel(gfxCount++, matrix, nr, nc);
#endif

  /* return */
}


// private

int mandel_calc_mpi (real		x,			/* x coordinate */
                     real		y)			/* y coordinate */
{
  real		r    = 0.0, i  = 0.0;	/* real and imaginary parts */
  real		rs   = 0.0, is = 0.0; 	/* " ", squared */
  int		iter = 0;		/* number of iterations */

  do {
    i = (2.0 * r * i) + x;
    r = (rs - is) + y;
    iter++;
    rs = r * r;
    is = i * i;
  } while ((iter < MANDEL_MAX_ITER) && ((rs + is) < MANDEL_INFINITY));

  return iter;
}
