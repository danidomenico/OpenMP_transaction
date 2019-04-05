/*==============================================================*/
/* generic/app/winnow.c : generic winnow implementation		*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*--------------------------------------------------------------*/
/* private function prototypes					*/
/*--------------------------------------------------------------*/

static int
winnow_redBool2DCount(
  bool2D	mask,			/* to reduce */
  int		nr,			/* row size */
  int		nc			/* column size */
);

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

/*
 * @ winnow : do point winnowing
 * > none
 * + create vector of points
 */

void
winnow(
  int2D		matrix,			/* point values */
  bool2D	mask,			/* suitable points */
  int		nr,			/* row size */
  int		nc,			/* column size */
  pt1D		pt,			/* points to create */
  int		npt			/* number of points */
){
  pt1DX		ptVec;			/* temp point vector */
  int		r, c, i, j, len;	/* indices and number of points */
  int		stride;			/* selection stride */
#if GRAPHICS
  int		gfxCount;
#endif

#if GRAPHICS
  gfx_winnow(gfxCount++, matrix, mask, pt, nr, nc, npt);
#endif

  /* fill temporary vector */
  len = winnow_redBool2DCount(mask, nr, nc);
  ASSERT(len >= npt);
  i = 0;
  for (r=0; r<nr; r++){
    for (c=0; c<nc; c++){
      if (mask[r][c]){
	ptVec[i].x = r;
	ptVec[i].y = c;
	ptVec[i].w = matrix[r][c];
	i++;
      }
    }
  }

  /* sort */
  ptSort(ptVec, len);

  /* copy over points */
  stride = len / npt;
  for (i=npt-1, j=len-1; i>=0; i--, j-=stride){
    pt[i] = ptVec[j];
  }

#if GRAPHICS
  gfx_winnow(gfxCount++, matrix, mask, pt, nr, nc, npt);
#endif

  /* return */
}

/*--------------------------------------------------------------*/
/* private functions						*/
/*--------------------------------------------------------------*/

/*
 * @ winnow_redBool2DCount : count true elements in mask
 * > count
 */

static int
winnow_redBool2DCount(
  bool2D	mask,			/* to reduce */
  int		nr,			/* row size */
  int		nc			/* column size */
){
  int		r, c, sum = 0;		/* indices and result */

  for (r=0; r<nr; r++){
    for (c=0; c<nc; c++){
      if (mask[r][c]) sum++;
    }
  }

  return sum;
}
