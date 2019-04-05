/*==============================================================*/
/* bar/app/half.c : barrier half implementation			*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*
 * @ half : body of halving shuffle
 * > none
 * + shuffle rows and columns
 */

void
half(
  int		tid,			/* own ID */
  int2D		matrix,			/* to shuffle */
  int		nr,			/* row size */
  int		nc			/* column size */
){
  int		lo, hi, str;		/* work controls */
  int		r, c, i;		/* loop indices */
  int1D		tmp;			/* temporary */
#if GRAPHICS
  int		gfxCount = 0;
#endif

#if GRAPHICS
  if (MASTER(tid)){
    gfx_half(gfxCount++, matrix, nr, nc);
  }
  thr_bar(tid);
#endif

  /* work */
  if (sch_work(ParWidth, tid, 0, nr, &lo, &hi, &str)){
    /* rows */
    for (r=lo; r<hi; r+=str){
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
    thr_bar(tid);
    if (MASTER(tid)){
      gfx_half(gfxCount++, matrix, nr, nc);
    }
#endif
  }
  thr_bar(tid);

  if (sch_work(ParWidth, tid, 0, nc, &lo, &hi, &str)){
    /* columns */
    for (c=lo; c<hi; c+=str){
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
    thr_bar(tid);
    if (MASTER(tid)){
      gfx_half(gfxCount++, matrix, nr, nc);
    }
#endif
  }
  thr_bar(tid);

  /* return */
}
