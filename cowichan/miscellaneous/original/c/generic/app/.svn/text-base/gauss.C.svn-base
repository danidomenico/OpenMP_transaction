/*==============================================================*/
/* generic/app/gauss.c : generic gauss implementation		*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

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
  int		r, c, k;		/* indices */
#if GRAPHICS
  int		gfxCount = 0;
#endif

  /* forward elimination */
  for (k=0; k<n; k++){
#if GRAPHICS
    gfx_gauss(gfxCount++, matrix, vector, answer, n);
#endif
    /* calculate pivots in k'th column */
    for (r=k+1; r<n; r++){
      matrix[r][k] = matrix[r][k]/matrix[k][k];
    }
    /* update elements below k'th row */
    for (r=k+1; r<n; r++){
      for (c=k+1; c<n; c++){
	matrix[r][c] = matrix[r][c] - (matrix[r][k] * matrix[k][c]);
      }
    }
    /* update element of solution vector */
    for (r=k+1; r<n; r++){
      vector[r] = vector[r] - (matrix[r][k] * vector[k]);
    }
  }

  /* back substitution */
  for (k=(n-1); k>=0; k--){
    answer[k] = vector[k]/matrix[k][k];
    for (r=k-1; r>=0; r--){
      vector[r] = vector[r] - (matrix[r][k] * answer[k]);
    }
  }

  /* return */
}
