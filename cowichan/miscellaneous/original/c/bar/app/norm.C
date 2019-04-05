/*==============================================================*/
/* bar/app/norm.c : barrier norm implementation			*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*
 * @ norm : body of coordinate normalization
 * > none
 * + normalize point coordinates
 */

void
norm(
  int		tid,			/* own ID */
  pt1D		vec,			/* points to normalize */
  int		n			/* length of vector */
){
  pt		ptMin, ptMax;		/* pseudo-points */
  real		sclX, sclY;		/* scaling factors */
  int		i;			/* loop index */
  int		lo, hi, str;		/* work controls */
#if GRAPHICS
  int		gfxCount = 0;
#endif

  /* work */
  redPt1DPos(tid, vec, n, &ptMin, &ptMax);
  if (sch_work(ParWidth, tid, 0, n, &lo, &hi, &str)){
    /* scaling factors */
    sclX = (ptMax.x == ptMin.x) ? 0.0 : 1/(ptMax.x - ptMin.x);
    sclY = (ptMax.y == ptMin.y) ? 0.0 : 1/(ptMax.y - ptMin.y);
    /* scale */
    for (i=lo; i<hi; i+=str){
      vec[i].x = sclX * (vec[i].x - ptMin.x);
      vec[i].y = sclY * (vec[i].y - ptMin.y);
    }
  }
  thr_bar(tid);

#if GRAPHICS
  if (MASTER(tid)){
    gfx_norm(gfxCount++, vec, n);
  }
  thr_bar(tid);
#endif

  /* return */
}
