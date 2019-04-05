/*==============================================================*/
/* generic/app/sor.c : generic sor implementation		*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

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
  int		r, c, t;		/* indices */
  real		sum, old, dMax, d;	/* temporaries */
#if GRAPHICS
  int		gfxCount = 0;
#endif

  /* initialize */
  for (r=0; r<n; r++){
    answer[r] = 1.0;
  }
  dMax = 2 * tol;			/* to forestall early exit */

#if GRAPHICS
  gfx_sor(gfxCount++, matrix, vector, answer, n);
#endif

  for (t=0; (t<SOR_MAX_ITERS) && (dMax >= tol); t++){
    dMax = 0.0;
    for (r=0; r<n; r++){
      /* compute sum */
      sum = 0.0;
      for (c=0; c<r; c++){
	sum += matrix[r][c]*answer[c];
      }
      for (c=r+1; c<n; c++){
	sum += matrix[r][c]*answer[c];
      }
    
      /* compute difference */
      old = answer[r];
      answer[r] = (1.0-SOR_OMEGA)*old + SOR_OMEGA*(vector[r]-sum)/matrix[r][r];
      d = (real)fabs((double)(old - answer[r]));
      if (d > dMax){
	dMax = d;
      }
    }
#if GRAPHICS
    gfx_sor(gfxCount++, matrix, vector, answer, n);
#endif
  }
  /* return */
}
