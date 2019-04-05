/*==============================================================*/
/* bar/app/outer.c : barrier outer implementation		*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*--------------------------------------------------------------*/
/* private data structures					*/
/*--------------------------------------------------------------*/

static real	DMaxThr[MAXPAR];	/* maxima */

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

/*
 * @ outer : body of outer product matrix calculation
 * > none
 * + fill matrix
 */

void
outer(
  int		tid,			/* own ID */
  pt1D		ptVec,			/* vector of points */
  real2D	matrix,			/* matrix to fill */
  real1D	realVec,		/* vector to fill */
  int		n			/* size */
){
  int		lo, hi, str;		/* work controls */
  int		r, c;			/* loop indices */
  real		d;			/* distance */
  bool		work;			/* do useful work? */
#if GRAPHICS
  int		gfxCount = 0;
#endif

  /* all elements except matrix diagonal */
  if ((work = sch_work(ParWidth, tid, 0, n, &lo, &hi, &str))){
    for (r=lo; r<hi; r+=str){
      realVec[r] = ptMag(&(ptVec[r]));
      for (c=0; c<r; c++){
	d = ptDist(&(ptVec[r]), &(ptVec[c]));
	if (d > DMaxThr[tid]){
	  DMaxThr[tid] = d;
	}
	matrix[r][c] = matrix[c][r] = d;
      }
#if GRAPHICS
      if (MASTER(tid)){
	gfx_outer(gfxCount++, ptVec, matrix, realVec, n);
      }
#endif
    }
  }
  thr_bar(tid);

  /* find maximum */
  if (MASTER(tid)){
    for (r=1; r<ParWidth; r++){
      if (DMaxThr[r] > DMaxThr[0]) DMaxThr[0] = DMaxThr[r];
    }
  }
  thr_bar(tid);

  /* matrix diagonal */
  d = DMaxThr[0] * n;
  if (work){
    for (r=lo; r<hi; r+=str){
      matrix[r][r] = d;
    }
  }
  thr_bar(tid);

#if GRAPHICS
  if (MASTER(tid)){
    gfx_outer(gfxCount++, ptVec, matrix, realVec, n);
  }
  thr_bar(tid);
#endif

  /* return */
}
