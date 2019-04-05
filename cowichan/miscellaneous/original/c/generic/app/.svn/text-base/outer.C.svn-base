/*==============================================================*/
/* generic/app/outer.c : generic outer implementation		*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*
 * @ outer : calculate outer product matrix
 * > none
 * + fill matrix
 */

void
outer(
  pt1D		ptVec,			/* vector of points */
  real2D	matrix,			/* matrix to fill */
  real1D	realVec,		/* vector to fill */
  int		n			/* size */
){
  real		d;			/* distance between points */
  real		dMax = -1.0;		/* maximum distance */
  int		r, c;			/* loop indices */
#if GRAPHICS
  int		gfxCount = 0;
#endif

  /* all elements except matrix diagonal */
  for (r=0; r<n; r++){
    realVec[r] = ptMag(&(ptVec[r]));
    for (c=0; c<r; c++){
      d = ptDist(&(ptVec[r]), &(ptVec[c]));
      if (d > dMax){
	dMax = d;
      }
      matrix[r][c] = matrix[c][r] = d;
    }
#if GRAPHICS
    gfx_outer(gfxCount++, ptVec, matrix, realVec, n);
#endif
  }

  /* matrix diagonal */
  dMax *= n;
  for (r=0; r<n; r++){
    matrix[r][r] = dMax;
  }
#if GRAPHICS
    gfx_outer(gfxCount++, ptVec, matrix, realVec, n);
#endif

  /* return */
}
