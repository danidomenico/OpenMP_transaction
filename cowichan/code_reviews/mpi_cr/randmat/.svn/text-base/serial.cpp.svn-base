/**
 * Serial implementation of random matrix generation
 *
 * \file serial.cpp
 * \author Andrew Borzenko
 * \date 01-26-09
 */

#include "../include/main.h"
#include "serial.h"

// public

void
randmat(
  int2D*		matrix,			/* to fill */
  int		nr,			/* row size */
  int		nc,			/* column size */
  int		limit,			/* value limit */
  int		seed			/* RNG seed */
){
  int		r, c;			/* loop indices */
  int		v = seed % RAND_M;	/* random value */
#if GRAPHICS
  int		gfxCount = 0;
#endif

  for (r=0; r<nr; r++){
    for (c=0; c<nc; c++){
      matrix[r][c] = v % limit;
      v = (RAND_A * v + RAND_C) % RAND_M;
    }
  }

#if GRAPHICS
  gfx_randmat(gfxCount++, matrix, nr, nc);
#endif

  /* return */
}
