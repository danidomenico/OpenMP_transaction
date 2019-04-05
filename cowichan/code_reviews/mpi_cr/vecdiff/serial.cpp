/**
 * Serial implementation of vector difference
 *
 * \file serial.cpp
 * \author Andrew Borzenko
 * \date 02-27-09
 */

#include "../include/main.h"
#include "serial.h"

// public

/*
 * @ vecdiff : do vector difference
 * > none
 * + find norm-1 vector difference
 */

void
vecdiff(
  real1D*	left,			/* left vector */
  real1D*	right,			/* right vector */
  int		n,			/* vector lengths */
  real	      * diff			/* norm-1 difference */
){
  int		i;			/* loop index */
  real		d;			/* difference */
#if GRAPHICS
  int		gfxCount = 0;
#endif

  *diff = (real)fabs((double)(left[0] - right[0]));
  for (i=1; i<n; i++){
    d = (real)fabs((double)(left[i] - right[i]));
    if (*diff < d){
      *diff = d;
    }
  }
#if GRAPHICS
  gfx_vecdiff(gfxCount++, left, right, n, *diff);
#endif

  /* return */
}
