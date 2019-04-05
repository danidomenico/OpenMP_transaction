/*==============================================================*/
/* fj/app/randmat.c : forkjoin randmat implementation		*/
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
/* private function prototypes					*/
/*--------------------------------------------------------------*/

static thr_f	randmat_thr;

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
  int2D		matrix,			/* to fill */
  int		nr,			/* row size */
  int		nc,			/* column size */
  int		limit,			/* value limit */
  int		seed			/* RNG seed */
){
  void	      * args[5];
#if GRAPHICS
  int		gfxCount = 0;
#endif

  /* set up */
  randStateInit(seed, ParWidth, state, &aPrime, &cPrime);

  /* run */
  TP_any(args, 0, matrix);
  TP_any(args, 1, nr);
  TP_any(args, 2, nc);
  TP_any(args, 3, limit);
  TP_any(args, 4, seed);
  thr_grp(randmat_thr, ParWidth, args);

#if GRAPHICS
  gfx_randmat(gfxCount++, matrix, nr, nc);
#endif

  /* return */
}

/*--------------------------------------------------------------*/
/* threading functions						*/
/*--------------------------------------------------------------*/

/*
 * @ randmat_thr : fill matrix
 * > none
 * + update values in matrix
 */

static THR_DEF
randmat_thr(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  int2D	      * matrix;			/* to fill */
  int		nr, nc;			/* sizes */
  int		limit, seed;		/* RNG controls */
  int		i;			/* loop index */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */

  /* setup */
  matrix = TG_int2D(argsThr, 0);
  nr     = TG_int(argsThr, 1);
  nc     = TG_int(argsThr, 2);
  limit  = TG_int(argsThr, 3);
  seed   = TG_int(argsThr, 4);
  tid    = TA_get_id(argsThr);
  nt     = TA_get_n(argsThr);

  /* special scheduling */
  if (sch_cyclic(nt, tid, 0, nr*nc, &lo, &hi, &str)){
    for (i=lo; i<hi; i+=str){
      (*matrix)[i/nr][i%nr] = state[tid] % limit;
      state[tid] = (aPrime * state[tid] + cPrime) % RAND_M;
    }
  }

  THR_END(argsThr);
}
