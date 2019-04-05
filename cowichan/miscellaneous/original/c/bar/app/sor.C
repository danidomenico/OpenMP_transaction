/*==============================================================*/
/* bar/app/sor.c : barrier sor implementation			*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*--------------------------------------------------------------*/
/* private data structures					*/
/*--------------------------------------------------------------*/

static real	DMaxThr[MAXPAR];	/* per-thread max diff */
static real	DMax;			/* actual max diff */

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

/*
 * @ sor : do body of successive over-relaxation
 * > none
 * + relax matrix to find solution
 */

void
sor(
  int		tid,			/* own ID */
  real2D	matrix,			/* to solve */
  real1D	vector,			/* target vector */
  real1D	answer,			/* solution found */
  int		n,			/* size */
  real		tol			/* tolerance on answer */
){
  int		lo, hi, str;		/* work controls */
  bool		work;			/* do useful work? */
  int		r, c, t;		/* indices */
  real1D	sums;			/* per-row sums */
  real		old, d;			/* temporaries */
  int		i;			/* loop index */
#if GRAPHICS
  int		gfxCount = 0;
#endif

  /* more setup */
  if (MASTER(tid)){
    for (i=0; i<n; i++){
      answer[i] = 1.0;
    }
    DMax = 2 * tol;			/* to forestall early exit */
  }
  thr_bar(tid);

#if GRAPHICS
  if (MASTER(tid)){
    gfx_sor(gfxCount++, matrix, vector, answer, n);
  }
  thr_bar(tid);
#endif

  /* work */
  work = sch_work(ParWidth, tid, 0, n, &lo, &hi, &str);
  for (t=0; (t<SOR_MAX_ITERS) && (DMax >= tol); t++){
    if (work){
      for (r=lo; r<hi; r+=str){
	/* compute sum */
	sums[r] = 0.0;
	for (c=0; c<r; c++){
	  sums[r] += matrix[r][c]*answer[c];
	}
	for (c=r+1; c<n; c++){
	  sums[r] += matrix[r][c]*answer[c];
	}
      }
    }
    thr_bar(tid);
    if (work){
      for (r=lo; r<hi; r+=str){
	/* compute difference */
	DMaxThr[tid] = 0.0;
	old = answer[r];
	answer[r] = (1.0-SOR_OMEGA)*old + SOR_OMEGA*(vector[r]-sums[r])/matrix[r][r];
	d = (real)fabs((double)(old - answer[r]));
	if (d > DMaxThr[tid]) DMaxThr[tid] = d;
      }
    }
    thr_bar(tid);
    /* compute overall difference */
    if (MASTER(tid)){
      DMax = DMaxThr[0];
      for (i=1; i<ParWidth; i++){
	if (DMaxThr[i] > DMax) DMax = DMaxThr[i];
      }
    }
    thr_bar(tid);
#if GRAPHICS
    if (MASTER(tid)){
      gfx_sor(gfxCount++, matrix, vector, answer, n);
    }
    thr_bar(tid);
#endif
  }

  /* return */
}
