/*==============================================================*/
/* bar/app/vecdiff.c : barrier vecdiff implementation		*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*--------------------------------------------------------------*/
/* private data structures					*/
/*--------------------------------------------------------------*/

static real	Diff;			/* overall difference */
static real	DiffThr[MAXPAR];	/* per-thread differences */

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

/*
 * @ vecdiff : do body of vector difference
 * > none
 * + find norm-1 vector difference
 */

void
vecdiff(
  int		tid,			/* own ID */
  real1D	left,			/* left vector */
  real1D	right,			/* right vector */
  int		n,			/* vector lengths */
  real	      * diffPtr			/* norm-1 difference */
){
  int		lo, hi, str;		/* work controls */
  int		i;			/* loop index */
  real		d;			/* difference */
  bool		work;			/* do useful work? */
#if GRAPHICS
  int		gfxCount = 0;
#endif

  /* reduce sections */
  if ((work = sch_work(ParWidth, tid, 0, n, &lo, &hi, &str))){
    DiffThr[tid] = -1.0;
    for (i=lo; i<hi; i+=str){
      d = (real)fabs((double)(left[i] - right[i]));
      if (d > DiffThr[tid]) DiffThr[tid] = d;
    }
  }
  thr_bar(tid);

  /* overall reduction */
  if (MASTER(tid)){
    Diff = DiffThr[0];
    for (i=1; i<ParWidth; i++){
      if (Diff < DiffThr[i]) Diff = DiffThr[i];
    }
  }
  thr_bar(tid);

#if GRAPHICS
  if (MASTER(tid)){
    gfx_vecdiff(gfxCount++, left, right, n, Diff);
  }
  thr_bar(tid);
#endif

  /* record result in all threads (not just those doing work) */
  *diffPtr = Diff;

  /* return */
}
