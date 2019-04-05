/*==============================================================*/
/* bar/app/life.c : barrier life implementation			*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*--------------------------------------------------------------*/
/* private data structures					*/
/*--------------------------------------------------------------*/

static int2D	Count;			/* neighborhood counts */

/*--------------------------------------------------------------*/
/* private function prototypes					*/
/*--------------------------------------------------------------*/

static void
life_one(
  bool2D	world,			/* world to evolve */
  int		r,			/* this row */
  int		r_lo,			/* lower row */
  int		r_hi,			/* higher row */
  int		c,			/* this column */
  int		c_lo,			/* lower column */
  int		c_hi			/* higher column */
);
static void
life_row(
  bool2D	world,			/* world to evolve */
  int		nr,			/* row size */
  int		nc,			/* column size */
  int		r,			/* this row */
  int		r_lo,			/* lower row */
  int		r_hi			/* higher row */
);

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

/*
 * @ life : body of Game of Life simulation
 * > none
 * + evolve world
 */

void
life(
  int		tid,			/* own ID */
  bool2D	world,			/* world to evolve */
  int		nr,			/* row size */
  int		nc,			/* column size */
  int		iters			/* number of iterations */
){
  int		lo, hi, str;		/* work controls */
  int		r, c;			/* row/column indices */
  int		i;			/* iteration index */
  bool		work;			/* useful work to do? */
#if GRAPHICS
  int		gfxCount = 0;
#endif

#if GRAPHICS
  if (MASTER(tid)){
    gfx_life(gfxCount++, world, nr, nc);
  }
#endif

  /* work */
  work = sch_work(ParWidth, tid, 0, nr, &lo, &hi, &str);
  for (i=0; i<iters; i++){
    /* fill neighborhood counts */
    if (work){
      for (r=lo; r<hi; r+=str){
	life_row(world, nr, nc, r, (nr+r-1)%nr, (nr+r+1)%nr);
      }
    }
    thr_bar(tid);
    /* update cells */
    if (work){
      for (r=lo; r<hi; r+=str){
	for (c=0; c<nc; c++){
	  if ((Count[r][c] == 3) || ((Count[r][c] == 2) && world[r][c])){
	    world[r][c] = TRUE;
	  } else {
	    world[r][c] = FALSE;
	  }
	}
      }
    }
    thr_bar(tid);
#if GRAPHICS
    if (MASTER(tid)){
      gfx_life(gfxCount++, world, nr, nc);
    }
    thr_bar(tid);
#endif
  }

  /* return */
}

/*--------------------------------------------------------------*/
/* private functions						*/
/*--------------------------------------------------------------*/

/*
 * @ life_one : update count for single cell
 * > none
 * + update count
 */

static void
life_one(
  bool2D	world,			/* world to evolve */
  int		r,			/* this row */
  int		r_lo,			/* lower row */
  int		r_hi,			/* higher row */
  int		c,			/* this column */
  int		c_lo,			/* lower column */
  int		c_hi			/* higher column */
){
  Count[r][c] = world[r_lo][c_lo] + world[r_lo][c] + world[r_lo][c_hi]
	      + world[r][c_lo]            +          world[r][c_hi]
	      + world[r_hi][c_lo] + world[r_hi][c] + world[r_hi][c_hi];
  /* return */
}

/*
 * @ life_row : count entire row
 * > none
 * + update counts
 */

static void
life_row(
  bool2D	world,			/* world to evolve */
  int		nr,			/* row size */
  int		nc,			/* column size */
  int		r,			/* this row */
  int		r_lo,			/* lower row */
  int		r_hi			/* higher row */
){
  int		c;			/* column index */

  life_one(world, r, r_lo, r_hi, 0, nc-1, 1);
  for (c=1; c<(nc-1); c++){
    life_one(world, r, r_lo, r_hi, c, c-1, c+1);
  }
  life_one(world, r, r_lo, r_hi, nc-1, nc-2, 0);

  /* return */
}
