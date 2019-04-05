/*==============================================================*/
/* bar/app/product.c : barrier product implementation		*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*
 * @ product : do body of matrix-vector product
 * > none
 * + fill result vector
 */

void
product(
  int		tid,			/* own ID */
  real2D	matrix,			/* to multiply by */
  real1D	vector,			/* to be multiplied */
  real1D	result,			/* result of multiply */
  int		nr,			/* row size */
  int		nc			/* column size */
){
  int		lo, hi, str;		/* work controls */
  int		r, c;			/* loop indices */
#if GRAPHICS
  int		gfxCount = 0;
#endif
  
  /* work */
  if (sch_work(ParWidth, tid, 0, nr, &lo, &hi, &str)){
    for (r=lo; r<hi; r+=str){
      result[r] = matrix[r][0] * vector[0];
      for (c=1; c<nc; c++){
	result[r] += matrix[r][c] * vector[c];
      }
    }
  }
  thr_bar(tid);

#if GRAPHICS
  if (MASTER(tid)){
    gfx_product(gfxCount++, matrix, vector, result, nr, nc);
  }
  thr_bar(tid);
#endif

  /* return */
}
