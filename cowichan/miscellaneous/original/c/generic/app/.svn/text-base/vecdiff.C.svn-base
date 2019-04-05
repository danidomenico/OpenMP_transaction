/*==============================================================*/
/* generic/app/vecdiff.c : generic vecdiff implementation	*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*
 * @ vecdiff : do vector difference
 * > none
 * + find norm-1 vector difference
 */

void
vecdiff(
  real1D	left,			/* left vector */
  real1D	right,			/* right vector */
  int		n,			/* vector lengths */
  real	      * diff			/* norm-1 difference */
){
  int		i;			/* loop index */
  real		d;			/* difference */
#if GRAPHICS
  int		gfxCount = 0;
#endif

  *diff = (real)fabs((double)(left[0] - right[0]));
  for (i=1; i<n; i++){
    d = (real)fabs((double)(left[i] - right[i]));
    if (*diff < d){
      *diff = d;
    }
  }
#if GRAPHICS
  gfx_vecdiff(gfxCount++, left, right, n, *diff);
#endif

  /* return */
}
