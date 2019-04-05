/*==============================================================*/
/* fj/app/sor.c : forkjoin sor implementation			*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

static thr_f	sor_thr_diff;
static thr_f	sor_thr_sum;

/*
 * @ sor : do successive over-relaxation
 * > none
 * + relax matrix to find solution
 */

void
sor(
  real2D	matrix,			/* to solve */
  real1D	vector,			/* target vector */
  real1D	answer,			/* solution found */
  int		n,			/* size */
  real		tol			/* tolerance on answer */
){
  void	      * args[7];
  int		i, t;			/* indices */
  real		diffThr[MAXPAR];	/* per-thread differences */
  real1D	sums;			/* element-wise sums */
  real		dMax;			/* maximum difference */
#if GRAPHICS
  int		gfxCount = 0;
#endif

  /* initialize */
  for (i=0; i<n; i++){
    answer[i] = 1.0;
  }
  dMax = 2 * tol;			/* to forestall early exit */

  /* pack arguments once */
  TP_any(args, 0, matrix);
  TP_any(args, 1, vector);
  TP_any(args, 2, answer);
  TP_any(args, 3, n);
  TP_any(args, 4, &tol);
  TP_any(args, 5, diffThr);
  TP_any(args, 6, sums);

#if GRAPHICS
  gfx_sor(gfxCount++, matrix, vector, answer, n);
#endif

  for (t=0; (t<SOR_MAX_ITERS) && (dMax >= tol); t++){
    /* compute sum */
    thr_grp(sor_thr_sum, ParWidth, args);
    /* compute difference */
    thr_grp(sor_thr_diff, ParWidth, args);
    dMax = diffThr[0];
    for (i=1; i<ParWidth; i++){
      if (diffThr[i] > dMax) dMax = diffThr[i];
    }
#if GRAPHICS
    gfx_sor(gfxCount++, matrix, vector, answer, n);
#endif
  }

  /* return */
}

/*--------------------------------------------------------------*/
/* threading functions						*/
/*--------------------------------------------------------------*/

/*
 * @ sor_thr_diff : update answer elements, calculating maximum per-thread difference
 * > NULL
 * + update answer elements and fill diffs element
 */

static THR_DEF
sor_thr_diff(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  real2D      * matrix;			/* matrix to solve */
  real1D      * vector;			/* target vector */
  real1D      * answer;			/* solution */
  int		n;			/* actual number of elements */
  real		tol;			/* tolerance */
  real1D      * sums;			/* elementwise sums */
  real	      * diffThr;		/* threadwise differences */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		r;			/* index */
  real		old, diff;		/* temporaries */

  /* setup */
  matrix  = TG_real2D(argsThr, 0);
  vector  = TG_real1D(argsThr, 1);
  answer  = TG_real1D(argsThr, 2);
  n       = TG_int(argsThr, 3);
  tol     = *TG_real_p(argsThr, 4);
  diffThr = (real *)TG_void_p(argsThr, 5);
  sums    = TG_real1D(argsThr, 6);
  tid     = TA_get_id(argsThr);
  nt      = TA_get_n(argsThr);

  /* work */
  if (sch_work(nt, tid, 0, n, &lo, &hi, &str)){
    diffThr[tid] = 0.0;
    for (r=lo; r<=hi; r+=str){
      old = (*answer)[r];
      (*answer)[r] = (1.0-SOR_OMEGA)*old + SOR_OMEGA*((*vector)[r]-(*sums)[r])/(*matrix)[r][r];
      diff = (real)fabs((double)(old - (*answer)[r]));
      if (diff > diffThr[tid]){
	diffThr[tid] = diff;
      }
    }
  }

  THR_END(argsThr);
}
    
/*
 * @ sor_thr_sum : sum off-diagonal matrix elements
 * > NULL
 * + sum off-diagonal matrix elements
 */

static THR_DEF
sor_thr_sum(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  real2D      * matrix;			/* matrix to solve */
  real1D      * vector;			/* target vector */
  real1D      * answer;			/* solution */
  int		n;			/* actual number of elements */
  real		tol;			/* tolerance */
  real1D      * sums;			/* elementwise sums */
  real	      * diffThr;		/* threadwise differences */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		r, c;			/* indices */

  /* setup */
  matrix  = TG_real2D(argsThr, 0);
  vector  = TG_real1D(argsThr, 1);
  answer  = TG_real1D(argsThr, 2);
  n       = TG_int(argsThr, 3);
  tol     = *TG_real_p(argsThr, 4);
  diffThr = (real *)TG_void_p(argsThr, 5);
  sums    = TG_real1D(argsThr, 6);
  tid     = TA_get_id(argsThr);
  nt      = TA_get_n(argsThr);

  /* work */
  if (sch_work(nt, tid, 0, n, &lo, &hi, &str)){
    for (r=lo; r<=hi; r+=str){
      (*sums)[r] = 0.0;
      for (c=0; c<r; c++){
	(*sums)[r] += (*matrix)[r][c]*(*answer)[c];
      }
      for (c=r+1; c<n; c++){
	(*sums)[r] += (*matrix)[r][c]*(*answer)[c];
      }
    }
  }

  THR_END(argsThr);
}
