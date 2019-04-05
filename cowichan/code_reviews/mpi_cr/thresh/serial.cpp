/**
 * Serial implementation of histogram thresholding
 *
 * \file serial.cpp
 * \author Andrew Borzenko
 * \date 03-02-09
 */

#include "../include/main.h"
#include "serial.h"

// public

/*
 * @ thresh : do histogram thresholding
 * > none
 * + create mask
 */

void
thresh(
  int2D*		matrix,			/* to threshold */
  bool2D*	mask,			/* threshold mask */
  int		nr,			/* row size */
  int		nc,			/* column size */
  real		fraction		/* how much to keep */
){
  int         * hist;			/* histogram */
  int		i, r, c;		/* loop indices */
  int		vMax;			/* max value in matrix */
  int		retain;			/* selection */
#if GRAPHICS
  int		gfxCount = 0;		/* number of times graphics called */
#endif

  /* find max value in matrix */
  vMax = 0;
  for (r=0; r<nr; r++){
    for (c=0; c<nc; c++){
      if (vMax < matrix[r][c]){
	vMax = matrix[r][c];
      }
    }
  }

  /* initialize histogram */
  hist = new int[vMax + 1];
  for (i=0; i<=vMax; i++){
    hist[i] = 0;
  }

#if GRAPHICS
  gfx_thresh(gfxCount++, matrix, mask, nr, nc, hist, vMax);
#endif

  /* count */
  for (r=0; r<nr; r++){
    for (c=0; c<nc; c++){
      hist[matrix[r][c]]++;
    }
  }

  /* include */
  retain = (int)(fraction * nc * nr);
  for (i = vMax; ((i >= 0) && (retain > 0)); i--) {
    retain -= hist[i];
  }
  retain = i;

  /* threshold */
  for (r=0; r<nr; r++){
    for (c=0; c<nc; c++){
      mask[r][c] = matrix[r][c] > retain;
    }
  }

#if GRAPHICS
  gfx_thresh(gfxCount++, matrix, mask, nr, nc, hist, vMax);
#endif

  delete [] hist;

  /* return */
}
