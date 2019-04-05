/*==============================================================*/
/* fj/app/product.c : forkjoin product implementation		*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

static thr_f	product_thr;

/*
 * @ product : do matrix-vector product
 * > none
 * + fill result vector
 */

void
product(
  real2D	matrix,			/* to multiply by */
  real1D	vector,			/* to be multiplied */
  real1D	result,			/* result of multiply */
  int		nr,			/* row size */
  int		nc			/* column size */
){
  void	      * args[5];		/* user argument vector */
#if GRAPHICS
  int		gfxCount = 0;
#endif

  TP_any(args, 0, matrix);
  TP_any(args, 1, vector);
  TP_any(args, 2, result);
  TP_any(args, 3, nr);
  TP_any(args, 4, nc);
  thr_grp(product_thr, ParWidth, args);
#if GRAPHICS
  gfx_product(gfxCount++, matrix, vector, result, nr, nc);
#endif

  /* return */
}

/*--------------------------------------------------------------*/
/* threading functions						*/
/*--------------------------------------------------------------*/

/*
 * @ product_thr : threaded product calculation
 * > NULL
 * + update result
 */

static THR_DEF
product_thr(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  real2D      * matrix;			/* to multiply by */
  real1D      * vector;			/* to be multiplied */
  real1D      * result;			/* result of multiply */
  int		nr, nc;			/* row/col size */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		r, c;			/* loop indices */

  /* setup */
  matrix = TG_real2D(argsThr, 0);
  vector = TG_real1D(argsThr, 1);
  result = TG_real1D(argsThr, 2);
  nr     = TG_int(argsThr, 3);
  nc     = TG_int(argsThr, 4);
  tid    = TA_get_id(argsThr);
  nt     = TA_get_n(argsThr);

  /* work */
  if (sch_work(nt, tid, 0, nr, &lo, &hi, &str)){
    for (r=lo; r<hi; r+=str){
      (*result)[r] = (*matrix)[r][0] * (*vector)[0];
      for (c=1; c<nc; c++){
	(*result)[r] += (*matrix)[r][c] * (*vector)[c];
      }
    }
  }

  THR_END(argsThr);
}
