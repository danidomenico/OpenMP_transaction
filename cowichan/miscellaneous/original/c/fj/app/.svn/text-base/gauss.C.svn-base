/*==============================================================*/
/* fj/app/gauss.c : forkjoin gauss implementation		*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

static thr_f	gauss_thr_backSub;
static thr_f	gauss_thr_calcPivots;
static thr_f	gauss_thr_updateMatrix;
static thr_f	gauss_thr_updateVector;

/*
 * @ gauss : do Gaussian elimination
 * > none
 * + decompose matrix and fill in answer vector
 */

void
gauss(
  real2D	matrix,			/* to solve */
  real1D	vector,			/* target vector */
  real1D	answer,			/* solution found */
  int		n			/* size */
){
  void	      * args[5];
  int		k;			/* outer loop index */
#if GRAPHICS
  int		gfxCount = 0;
#endif

  /* pack matrix, vector, answer, and n once */
  TP_any(args, 0, matrix);
  TP_any(args, 1, vector);
  TP_any(args, 2, answer);
  TP_any(args, 3, n);

  /* forward elimination */
  for (k=0; k<n; k++){
#if GRAPHICS
    gfx_gauss(gfxCount++, matrix, vector, answer, n);
#endif
    /* marshal loop index */
    TP_any(args, 4, k);
    /* calcualte pivots in k'th column */
    thr_grp(gauss_thr_calcPivots, ParWidth, args);
    /* update elements below k'th row */
    thr_grp(gauss_thr_updateMatrix, ParWidth, args);
    /* update elements of solution vector */
    thr_grp(gauss_thr_updateVector, ParWidth, args);
  }

  /* back substitution */
  for (k=(n-1); k>=0; k--){
    TP_any(args, 4, k);
    answer[k] = vector[k]/matrix[k][k];
    thr_grp(gauss_thr_backSub, ParWidth, args);
  }

  /* return */
}

/*--------------------------------------------------------------*/
/* threading functions						*/
/*--------------------------------------------------------------*/

/*
 * @ gauss_thr_backSub : do back-substitution
 * > NULL
 * + update vector elements
 */

static THR_DEF
gauss_thr_backSub(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  real2D      * matrix;			/* matrix to solve */
  real1D      * vector;			/* target vector */
  real1D      * answer;			/* solution */
  int		n, k;			/* size and outer loop index */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		r;			/* loop index */

  /* setup */
  matrix = TG_real2D(argsThr, 0);
  vector = TG_real1D(argsThr, 1);
  answer = TG_real1D(argsThr, 2);
  n      = TG_int(argsThr, 3);
  k      = TG_int(argsThr, 4);
  tid    = TA_get_id(argsThr);
  nt     = TA_get_n(argsThr);

  /* work */
  if (sch_work(nt, tid, 0, n, &lo, &hi, &str)){
    for (r=lo; r<hi; r+=str){
      (*vector)[r] = (*vector)[r] - ((*matrix)[r][k] * (*answer)[k]);
    }
  }

  THR_END(argsThr);
}

/*
 * @ gauss_thr_calcPivots : calculate pivot elements
 * > NULL
 * + update matrix elements with pivot value
 */

static THR_DEF
gauss_thr_calcPivots(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  real2D      * matrix;			/* matrix to solve */
  real1D      * vector;			/* target vector */
  real1D      * answer;			/* solution */
  int		n, k;			/* size and outer loop index */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		r;			/* loop index */

  /* setup */
  matrix = TG_real2D(argsThr, 0);
  vector = TG_real1D(argsThr, 1);
  answer = TG_real1D(argsThr, 2);
  n      = TG_int(argsThr, 3);
  k      = TG_int(argsThr, 4);
  tid    = TA_get_id(argsThr);
  nt     = TA_get_n(argsThr);

  /* work */
  if (sch_work(nt, tid, k+1, n, &lo, &hi, &str)){
    for (r=lo; r<hi; r+=str){
      (*matrix)[r][k] = (*matrix)[r][k]/(*matrix)[k][k];
    }
  }

  THR_END(argsThr);
}

/*
 * @ gauss_thr_updateMatrix : calculate pivot elements
 * > NULL
 * + update matrix elements with pivot value
 */

static THR_DEF
gauss_thr_updateMatrix(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  real2D      * matrix;			/* matrix to solve */
  real1D      * vector;			/* target vector */
  real1D      * answer;			/* solution */
  int		n, k;			/* size and outer loop index */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		r, c;			/* loop indices */

  /* setup */
  matrix = TG_real2D(argsThr, 0);
  vector = TG_real1D(argsThr, 1);
  answer = TG_real1D(argsThr, 2);
  n      = TG_int(argsThr, 3);
  k      = TG_int(argsThr, 4);
  tid    = TA_get_id(argsThr);
  nt     = TA_get_n(argsThr);

  /* work */
  if (sch_work(nt, tid, k+1, n, &lo, &hi, &str)){
    for (r=lo; r<hi; r+=str){
      for (c=k+1; c<n; c++){
	(*matrix)[r][c] = (*matrix)[r][c] - ((*matrix)[r][k] * (*matrix)[k][c]);
      }
    }
  }

  THR_END(argsThr);
}

/*
 * @ gauss_thr_updateVector : calculate pivot elements
 * > NULL
 * + update matrix elements with pivot value
 */

static THR_DEF
gauss_thr_updateVector(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  real2D      * matrix;			/* matrix to solve */
  real1D      * vector;			/* target vector */
  real1D      * answer;			/* solution */
  int		n, k;			/* size and outer loop index */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		r;			/* loop index */

  /* setup */
  matrix = TG_real2D(argsThr, 0);
  vector = TG_real1D(argsThr, 1);
  answer = TG_real1D(argsThr, 2);
  n      = TG_int(argsThr, 3);
  k      = TG_int(argsThr, 4);
  tid    = TA_get_id(argsThr);
  nt     = TA_get_n(argsThr);

  /* work */
  if (sch_work(nt, tid, k+1, n, &lo, &hi, &str)){
    for (r=lo; r<hi; r+=str){
      (*vector)[r] = (*vector)[r] - ((*matrix)[r][k] * (*vector)[k]);
    }
  }

  THR_END(argsThr);
}
