/*==============================================================*/
/* generic/app/product.c : generic product implementation	*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

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
  int		r, c;			/* row/column indices */
#if GRAPHICS
  int		gfxCount = 0;
#endif

  for (r=0; r<nr; r++){
    result[r] = matrix[r][0] * vector[0];
    for (c=1; c<nc; c++){
      result[r] += matrix[r][c] * vector[c];
    }
  }
#if GRAPHICS
  gfx_product(gfxCount++, matrix, vector, result, nr, nc);
#endif

  /* return */
}
