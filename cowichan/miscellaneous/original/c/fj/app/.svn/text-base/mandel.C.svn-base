/*==============================================================*/
/* fj/app/mandel.c : forkjoin mandel implementation		*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*--------------------------------------------------------------*/
/* private data structures					*/
/*--------------------------------------------------------------*/

#if SUNOS5
mutex_t		Mutex;			/* to control work */
#elif NUMA
MUTEXDEC(Mutex);
#endif

int		Counter = 0;		/* shared row index */

/*--------------------------------------------------------------*/
/* private function prototypes					*/
/*--------------------------------------------------------------*/

static int
mandel_calc(
  real		x,			/* x coord */
  real		y			/* y coord */
);
static thr_f	mandel_thr;

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

/*
 * @ mandel : calculate Mandelbrot Set
 * > none
 * + fill matrix
 */

void
mandel(
  int2D		matrix,			/* to shuffle */
  int		nr,			/* row size */
  int		nc,			/* column size */
  real		base_x,			/* lower left corner */
  real		base_y,			/* lower left corner */
  real		ext_x,			/* extent */
  real		ext_y			/* extent */
){
  void	      * args[7];
#if GRAPHICS
  int		gfxCount = 0;		/* number of times graphics called */
#endif

#if SUNOS5
  mutex_init(&Mutex, USYNC_THREAD, NULL);
#elif NUMA
  MUTEXINIT(Mutex);
#endif
  Counter = 0;

  TP_any(args, 0, matrix);
  TP_any(args, 1, nr);
  TP_any(args, 2, nc);
  TP_any(args, 3, &base_x);
  TP_any(args, 4, &base_y);
  TP_any(args, 5, &ext_x);
  TP_any(args, 6, &ext_y);
  thr_grp(mandel_thr, ParWidth, args);

#if SUNOS5
  mutex_destroy(&Mutex);
#elif NUMA
  MUTEXFREE(Mutex);
#endif

#if GRAPHICS
  gfx_mandel(gfxCount++, matrix, nr, nc);
#endif

  /* return */
}

/*--------------------------------------------------------------*/
/* threading functions						*/
/*--------------------------------------------------------------*/

/*
 * @ mandel_thr : worker to calculate Mandelbrot Set
 * > NULL
 * + fill matrix
 */

static THR_DEF
mandel_thr(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  int2D	      * matrix;			/* to shuffle */
  int		nr, nc;			/* row/col size */
  real		base_x, base_y;		/* lower left corner */
  real		ext_x, ext_y;		/* extent */
  int		r, c;			/* row and column indices */
  real		dx, dy;			/* per-step deltas */

  /* setup */
  matrix = TG_int2D(argsThr, 0);
  nr     = TG_int(argsThr, 1);
  nc     = TG_int(argsThr, 2);
  base_x = *TG_real_p(argsThr, 3);
  base_y = *TG_real_p(argsThr, 4);
  ext_x  = *TG_real_p(argsThr, 5);
  ext_y  = *TG_real_p(argsThr, 6);

  /* initialize constants */
  dx = ext_x / (nr - 1);
  dy = ext_y / (nc - 1);

  /* work */
#if SUNOS5
  while ((r = fetchInc(&Mutex, &Counter)) < nr)
#elif NUMA
  while ((r = fetchInc(Mutex, &Counter)) < nr)
#endif
  {
    for (c=0; c<nc; c++){
      (*matrix)[r][c] = mandel_calc(base_x+(r*dx), base_y+(c*dy));
    }
  }

  THR_END(argsThr);
}

/*--------------------------------------------------------------*/
/* private functions						*/
/*--------------------------------------------------------------*/

static int mandel_calc(
  real		x,			/* x coordinate */
  real		y			/* y coordinate */
){
  real		r    = 0.0, i  = 0.0;	/* real and imaginary parts */
  real		rs   = 0.0, is = 0.0; 	/* " ", squared */
  int		iter = 0;		/* number of iterations */

  do {
    i = (2.0 * r * i) + x;
    r = (rs - is) + y;
    iter++;
    rs = r * r;
    is = i * i;
  } while ((iter < MANDEL_MAX_ITER) && ((rs + is) < MANDEL_INFINITY));

  return iter;
}
