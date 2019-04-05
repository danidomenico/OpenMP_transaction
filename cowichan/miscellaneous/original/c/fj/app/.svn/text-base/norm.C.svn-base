/*==============================================================*/
/* fj/app/norm.c : forkjoin norm implementation			*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

static thr_f	norm_thr;

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
  void	      * args[5];
  pt		ptMin, ptMax;		/* pseudo-points */
  real		sclX, sclY;		/* scaling factors */
#if GRAPHICS
  int		gfxCount = 0;
#endif

  /* scaling factors */
  redPt1DPos(vec, n, &ptMin, &ptMax);
  sclX = (ptMax.x == ptMin.x) ? 0.0 : 1/(ptMax.x - ptMin.x);
  sclY = (ptMax.y == ptMin.y) ? 0.0 : 1/(ptMax.y - ptMin.y);

  /* work */
  TP_any(args, 0, vec);
  TP_any(args, 1, n);
  TP_any(args, 2, &sclX);
  TP_any(args, 3, &sclY);
  TP_any(args, 4, &ptMin);
  thr_grp(norm_thr, ParWidth, args);

#if GRAPHICS
  gfx_norm(gfxCount++, vec, n);
#endif

  /* return */
}

/*--------------------------------------------------------------*/
/* threading functions						*/
/*--------------------------------------------------------------*/

/*
 * @ norm_thr : normalize part of points
 * > NULL
 * + update point positions
 */

static THR_DEF
norm_thr(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  pt1D	      * vec;			/* points to normalize */
  int		n;			/* length of vector */
  real		sclX, sclY;		/* scaling factors */
  pt	      * ptMin;			/* low point */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		i;			/* loop index */

  /* setup */
  vec   = TG_pt1D(argsThr, 0);
  n     = TG_int(argsThr, 1);
  sclX  = *TG_real_p(argsThr, 2);
  sclY  = *TG_real_p(argsThr, 3);
  ptMin = (pt *)TG_void_p(argsThr, 4);
  tid   = TA_get_id(argsThr);
  nt    = TA_get_n(argsThr);

  /* scale */
  if (sch_work(nt, tid, 0, n, &lo, &hi, &str)){
    for (i=lo; i<hi; i+=str){
      (*vec)[i].x = sclX * ((*vec)[i].x - ptMin->x);
      (*vec)[i].y = sclY * ((*vec)[i].y - ptMin->y);
    }
  }

  THR_END(argsThr);
}
