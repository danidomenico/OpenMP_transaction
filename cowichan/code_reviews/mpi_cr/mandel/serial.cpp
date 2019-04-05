/**
 * Serial implementation of Mandelbrot set
 *
 * \file serial.cpp
 * \author Andrew Borzenko
 * \date 02-09-09
 */

#include "../include/main.h"
#include "serial.h"

// public

void
mandel(
  int2D*		matrix,			/* to fill */
  int		nr,			/* row size */
  int		nc,			/* column size */
  real		base_x,			/* lower left corner */
  real		base_y,			/* lower left corner */
  real		ext_x,			/* extent */
  real		ext_y			/* extent */
){
  int		r, c;			/* row and column indices */
  real		dx, dy;			/* per-step deltas */
#if GRAPHICS
  int		gfxCount = 0;		/* number of times graphics called */
#endif

  dx = ext_x / (nr - 1);
  dy = ext_y / (nc - 1);

  for (r = 0; r < nr; r++) {
    for (c = 0; c < nc; c++) {
      matrix[r][c] = mandel_calc (base_x + (r * dx), base_y + (c * dy));
    }
  }
#if GRAPHICS
  gfx_mandel(gfxCount++, matrix, nr, nc);
#endif

  /* return */
}


// private

int mandel_calc(
  real		x,			/* x coordinate */
  real		y			/* y coordinate */
){
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
