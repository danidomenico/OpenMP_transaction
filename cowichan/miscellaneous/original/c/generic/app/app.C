/*==============================================================*/
/* generic/app/app.c : generic support routines			*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*
 * @ cpReal1D : copy real vector
 * > none
 * + copy vector
 */

void
cpReal1D(
  real1D	src,			/* source vector */
  real1D	dst,			/* destination matrix */
  int		n			/* size */
){
  int		i;			/* loop index */

  ASSERT(src != NULL);
  ASSERT(dst != NULL);
  for (i=0; i<n; i++){
    dst[i] = src[i];
  }
  /* return */
}

/*
 * @ cpReal2D : copy real matrix
 * > none
 * + copy matrix
 */

void
cpReal2D(
  real2D	src,			/* source matrix */
  real2D	dst,			/* destination matrix */
  int		nr,			/* row size */
  int		nc			/* column size */
){
  int		r, c;			/* loop indices */

  ASSERT(src != NULL);
  ASSERT(dst != NULL);
  for (r=0; r<nr; r++){
    for (c=0; c<nc; c++){
      dst[r][c] = src[r][c];
    }
  }
  /* return */
}

/*
 * @ redPt1DPos : find min/max point positions
 * > none
 * + fill arguments
 */

void
redPt1DPos(
  pt1D		vec,			/* vector of points */
  int		n,			/* number of points */
  pt	      * ptMin,			/* minimum location */
  pt	      * ptMax			/* maximum location */
){
  int		i;

  ASSERT(ptMin != NULL);
  ASSERT(ptMax != NULL);

  ptMin->x = vec[0].x; ptMin->y = vec[0].y;
  ptMax->x = vec[0].x; ptMax->y = vec[0].y;
  for (i=1; i<n; i++){
    if (vec[i].x < ptMin->x) ptMin->x = vec[i].x;
    if (vec[i].x > ptMax->x) ptMax->x = vec[i].x;
    if (vec[i].y < ptMin->y) ptMin->y = vec[i].y;
    if (vec[i].y > ptMax->y) ptMax->y = vec[i].y;
  }
  ptMin->w = 0; ptMax->w = 0;

  /* return */
}
