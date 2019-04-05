/*==============================================================*/
/* fj/app/vecdiff.c : forkjoin vecdiff implementation		*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

static thr_f	vecdiff_thr;

/*
 * @ vecdiff : do vector difference
 * > none
 * + find norm-1 vector difference
 */

void
vecdiff(
  real1D	left,			/* left vector */
  real1D	right,			/* right vector */
  int		n,			/* vector lengths */
  real	      * diff			/* norm-1 difference */
){
  void	      * args[4];
  real		diffThr[MAXPAR];	/* per-thread differences */
  int		i;			/* loop index */
#if GRAPHICS
  int		gfxCount = 0;
#endif

  TP_any(args, 0, left);
  TP_any(args, 1, right);
  TP_any(args, 2, n);
  TP_any(args, 3, diffThr);
  thr_grp(vecdiff_thr, ParWidth, args);

  *diff = diffThr[0];
  for (i=1; i<ParWidth; i++){
    if (*diff < diffThr[i]) *diff = diffThr[i];
  }
#if GRAPHICS
  gfx_vecdiff(gfxCount++, left, right, n, *diff);
#endif

  /* return */
}

/*--------------------------------------------------------------*/
/* threading functions						*/
/*--------------------------------------------------------------*/

/*
 * @ vecdiff_thr : do partial vector difference
 */

static THR_DEF
vecdiff_thr(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  real1D      * left;			/* left vector */
  real1D      * right;			/* right vector */
  int		n;			/* vector lengths */
  real	      * diffThr;		/* per-thread differences */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		i;			/* loop index */
  real		d;			/* unit difference */

  /* setup */
  left    = TG_real1D(argsThr, 0);
  right   = TG_real1D(argsThr, 1);
  n       = TG_int(argsThr, 2);
  diffThr = (real *)TG_void_p(argsThr, 3);
  tid     = TA_get_id(argsThr);
  nt      = TA_get_n(argsThr);

  /* work */
  if (sch_work(nt, tid, 0, n, &lo, &hi, &str)){
    diffThr[tid] = 0.0;
    for (i=lo; i<hi; i+=str){
      d = (real)fabs((double)((*left)[i] - (*right)[i]));
      if (diffThr[tid] < d) diffThr[tid] = d;
    }
  }

  THR_END(argsThr);
}
