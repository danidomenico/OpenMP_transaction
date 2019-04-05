/*==============================================================*/
/* generic/app/norm.c : generic norm implementation		*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*
 * @ norm : do coordinate normalization
 * > none
 * + normalize point coordinates
 */

void
norm(
  pt1D		vec,			/* points to normalize */
  int		n			/* length of vector */
){
  pt		ptMin, ptMax;		/* pseudo-points */
  real		sclX, sclY;		/* scaling factors */
  int		i;			/* loop index */
#if GRAPHICS
  int		gfxCount = 0;
#endif

  /* scaling factors */
  redPt1DPos(vec, n, &ptMin, &ptMax);
  sclX = (ptMax.x == ptMin.x) ? 0.0 : 1/(ptMax.x - ptMin.x);
  sclY = (ptMax.y == ptMin.y) ? 0.0 : 1/(ptMax.y - ptMin.y);

  /* scale */
  for (i=0; i<n; i++){
    vec[i].x = sclX * (vec[i].x - ptMin.x);
    vec[i].y = sclY * (vec[i].y - ptMin.y);
  }

#if GRAPHICS
  gfx_norm(gfxCount++, vec, n);
#endif

  /* return */
}
