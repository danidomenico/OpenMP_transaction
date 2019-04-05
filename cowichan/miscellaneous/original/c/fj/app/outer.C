/*==============================================================*/
/* fj/app/outer.c : forkjoin outer implementation		*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

static thr_f	outer_thr_diag;
static thr_f	outer_thr_nonDiag;

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
  void	      * args[5];		/* user argument vector */
  real		distThr[MAXPAR];	/* per-thread max dist */
  real		dMax = -1.0;		/* maximum distance */
  int		i;			/* loop index */

  /* pack common arguments */
  TP_any(args, 0, ptVec);
  TP_any(args, 1, matrix);
  TP_any(args, 2, realVec);
  TP_any(args, 3, n);

  /* all elements except matrix diagonal */
  TP_any(args, 4, distThr);
  thr_grp(outer_thr_nonDiag, ParWidth, args);

  /* reduce and scale */
  for (i=0; i<ParWidth; i++){
    if (distThr[i] > dMax){
      dMax = distThr[i];
    }
  }
  dMax *= n;

  /* diagonal elements */
  TP_any(args, 4, &dMax);
  thr_grp(outer_thr_diag, ParWidth, args);

  /* return */
}

/*--------------------------------------------------------------*/
/* threading functions						*/
/*--------------------------------------------------------------*/

/*
 * @ outer_thr_diag : handle diagonal elements of matrix
 * > NULL
 * + fill diagonal of matrix
 */

static THR_DEF
outer_thr_diag(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  pt1D	      * ptVec;			/* vector of points */
  real2D      * matrix;			/* matrix to fill */
  real1D      * realVec;		/* vector to fill */
  int		n;			/* size */
  real		dMax;			/* maximum distance */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		i;			/* loop index */

  /* setup */
  ptVec   = TG_pt1D(argsThr, 0);
  matrix  = TG_real2D(argsThr, 1);
  realVec = TG_real1D(argsThr, 2);
  n       = TG_int(argsThr, 3);
  dMax    = *TG_real_p(argsThr, 4);
  tid     = TA_get_id(argsThr);
  nt      = TA_get_n(argsThr);

  /* work */
  if (sch_work(nt, tid, 0, n, &lo, &hi, &str)){
    for (i=lo; i<hi; i+=str){
      (*matrix)[i][i] = dMax;
    }
  }

  THR_END(argsThr);
}

/*
 * @ outer_thr_nonDiag : handle off-diagonal elements of matrix
 * > NULL
 * + fill off-diagonal of matrix, and all of vector
 */

static THR_DEF
outer_thr_nonDiag(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  pt1D	      * ptVec;			/* vector of points */
  real2D      * matrix;			/* matrix to fill */
  real1D      * realVec;		/* vector to fill */
  int		n;			/* size */
  real	      * distThr;		/* per-thread max distance */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  real		d;			/* point-to-point distance */
  int		r, c;			/* loop indices */

  /* setup */
  ptVec   = TG_pt1D(argsThr, 0);
  matrix  = TG_real2D(argsThr, 1);
  realVec = TG_real1D(argsThr, 2);
  n       = TG_int(argsThr, 3);
  distThr = (real *)TG_void_p(argsThr, 4);
  tid     = TA_get_id(argsThr);
  nt      = TA_get_n(argsThr);

  /* work */
  if (sch_work(nt, tid, 0, n, &lo, &hi, &str)){
    distThr[tid] = 0.0;
    for (r=lo; r<hi; r+=str){
      (*realVec)[r] = ptMag(&((*ptVec)[r]));
      for (c=0; c<r; c++){
	d = ptDist(&((*ptVec)[r]), &((*ptVec)[c]));
	if (d > distThr[tid]){
	  distThr[tid] = d;
	}
	(*matrix)[r][c] = d;
	(*matrix)[c][r] = d;
      }
    }
  }

  THR_END(argsThr);
}
