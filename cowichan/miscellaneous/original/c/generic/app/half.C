/*==============================================================*/
/* generic/app/half.c : generic half implementation		*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*
 * @ half : do halving shuffle
 * > none
 * + shuffle rows and columns
 */

void
half(
  int2D		matrix,			/* to shuffle */
  int		nr,			/* row size */
  int		nc			/* column size */
){
  int		r, c, i;		/* loop indices */
  int1D		tmp;			/* temporary */
#if GRAPHICS
  int		gfxCount = 0;
#endif

#if GRAPHICS
  gfx_half(gfxCount++, matrix, nr, nc);
#endif

  /* rows */
  for (r=0; r<nr; r++){
    for (c=1, i=0; c<nc; c+=2, i++){
      tmp[i] = matrix[r][c];
    }
    for (c=0, i=0; c<(nc+1)/2; c++, i+=2){
      matrix[r][c] = matrix[r][i];
    }
    for (c=(nc+1)/2, i=0; c<nc; c++, i++){
      matrix[r][c] = tmp[i];
    }
  }

#if GRAPHICS
  gfx_half(gfxCount++, matrix, nr, nc);
#endif

  /* columns */
  for (c=0; c<nc; c++){
    for (r=1, i=0; r<nr; r+=2, i++){
      tmp[i] = matrix[r][c];
    }
    for (r=0, i=0; r<(nr+1)/2; r++, i+=2){
      matrix[r][c] = matrix[i][c];
    }
    for (r=(nr+1)/2, i=0; r<nr; r++, i++){
      matrix[r][c] = tmp[i];
    }
  }

#if GRAPHICS
  gfx_half(gfxCount++, matrix, nr, nc);
#endif

  /* return */
}
