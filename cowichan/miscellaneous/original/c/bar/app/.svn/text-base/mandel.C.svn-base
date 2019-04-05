/*==============================================================*/
/* bar/app/mandel.c : barrier mandel implementation		*/
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

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

/*
 * @ mandel : body of Mandelbrot Set calculation
 * > none
 * + fill matrix
 */

void
mandel(
  int		tid,			/* own ID */
  int2D		matrix,			/* to shuffle */
  int		nr,			/* row size */
  int		nc,			/* column size */
  real		base_x,			/* lower left corner */
  real		base_y,			/* lower left corner */
  real		ext_x,			/* extent */
  real		ext_y			/* extent */
){
  int		r, c;			/* row and column indices */
  real		dx, dy;			/* per-step deltas */
#if GRAPHICS
  int		gfxCount = 0;		/* number of times graphics called */
#endif

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
      matrix[r][c] = mandel_calc(base_x+(r*dx), base_y+(c*dy));
    }
  }

#if GRAPHICS
  if (MASTER(tid)){
    gfx_mandel(gfxCount++, matrix, nr, nc);
  }
  thr_bar(tid);
#endif

  /* return */
}

/*
 * @ mandelSetup : preliminary function
 * > none
 * + set up for Mandelbrot Set
 */

void
mandelSetup(
  void
){
#if SUNOS5
  mutex_init(&Mutex, USYNC_THREAD, NULL);
#elif NUMA
  MUTEXINIT(Mutex);
#endif
  Counter = 0;
  /* return */
}

/*
 * @ mandelTakedown : preliminary function
 * > none
 * + take down after Mandelbrot Set
 */

void
mandelTakedown(
  void
){

#if SUNOS5
  mutex_destroy(&Mutex);
#elif NUMA
  MUTEXFREE(Mutex);
#endif
  /* return */
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
