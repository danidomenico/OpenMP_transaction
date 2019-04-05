/*==============================================================*/
/* bar/app/randmat.c : barrier randmat implementation		*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*--------------------------------------------------------------*/
/* shared data structures					*/
/*--------------------------------------------------------------*/

static int	state[MAXPAR];		/* random state vector */
static int	aPrime, cPrime;		/* modified constants */

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

/*
 * @ randmat : do random matrix generation
 * > none
 * + fill matrix
 */

void
randmat(
  int		tid,			/* own ID */
  int2D		matrix,			/* to fill */
  int		nr,			/* row size */
  int		nc,			/* column size */
  int		limit,			/* value limit */
  int		seed			/* RNG seed */
){
  int		i;			/* loop index */
  int		lo, hi, str;		/* work controls */
#if GRAPHICS
  int		gfxCount = 0;
#endif

  /* set up */
  if (MASTER(tid)){
    randStateInit(seed, ParWidth, state, &aPrime, &cPrime);
  }
  thr_bar(tid);

  /* special scheduling */
  if (sch_cyclic(ParWidth, tid, 0, nr*nc, &lo, &hi, &str)){
    for (i=lo; i<hi; i+=str){
      matrix[i/nr][i%nr] = state[tid] % limit;
      state[tid] = (aPrime * state[tid] + cPrime) % RAND_M;
    }
  }
  thr_bar(tid);

#if GRAPHICS
  if (MASTER(tid)){
    gfx_randmat(gfxCount++, matrix, nr, nc);
  }
  thr_bar(tid);
#endif

  /* return */
}
