/*==============================================================*/
/* fj/app/life.c : forkjoin life implementation			*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*--------------------------------------------------------------*/
/* private function prototypes					*/
/*--------------------------------------------------------------*/

static void
life_one(
  bool2D	world,			/* world to evolve */
  int2D		count,			/* neighborhood counts */
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
  int2D		count,			/* neighborhood counts */
  int		nr,			/* row size */
  int		nc,			/* column size */
  int		r,			/* this row */
  int		r_lo,			/* lower row */
  int		r_hi			/* higher row */
);
static thr_f	life_thr_count;
static thr_f	life_thr_update;

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

/*
 * @ life : simulate Game of Life
 * > none
 * + evolve world
 */

void
life(
  bool2D	world,			/* world to evolve */
  int		nr,			/* row size */
  int		nc,			/* column size */
  int		iters			/* number of iterations */
){
  void	      * args[4];
  int2D		count;			/* neighborhood counts */
  int		i;			/* iteration index */
#if GRAPHICS
  int		gfxCount = 0;
#endif

#if GRAPHICS
  gfx_life(gfxCount++, world, nr, nc);
#endif

  /* fill parameters once */
  TP_any(args, 0, world);
  TP_any(args, 1, count);
  TP_any(args, 2, nr);
  TP_any(args, 3, nc);

  /* evolve */
  for (i=0; i<iters; i++){
    /* fill neighborhood counts */
    thr_grp(life_thr_count, ParWidth, args);
    /* update cells */
    thr_grp(life_thr_update, ParWidth, args);
#if GRAPHICS
    gfx_life(gfxCount++, world, nr, nc);
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
  int2D		count,			/* neighborhood counts */
  int		r,			/* this row */
  int		r_lo,			/* lower row */
  int		r_hi,			/* higher row */
  int		c,			/* this column */
  int		c_lo,			/* lower column */
  int		c_hi			/* higher column */
){
  count[r][c] = world[r_lo][c_lo] + world[r_lo][c] + world[r_lo][c_hi]
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
  int2D		count,			/* neighborhood counts */
  int		nr,			/* row size */
  int		nc,			/* column size */
  int		r,			/* this row */
  int		r_lo,			/* lower row */
  int		r_hi			/* higher row */
){
  int		c;			/* column index */

  life_one(world, count, r, r_lo, r_hi, 0, nc-1, 1);
  for (c=1; c<(nc-1); c++){
    life_one(world, count, r, r_lo, r_hi, c, c-1, c+1);
  }
  life_one(world, count, r, r_lo, r_hi, nc-1, nc-2, 0);

  /* return */
}

/*
 * @ life_thr_count : count neighbors
 * > NULL
 * + update counts
 */

static THR_DEF
life_thr_count(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  bool2D      * world;			/* world to evolve */
  int2D	      * count;			/* neighborhood counts */
  int		nr, nc;			/* row/col size */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		r;			/* loop index */

  /* setup */
  world = TG_bool2D(argsThr, 0);
  count = TG_int2D(argsThr, 1);
  nr    = TG_int(argsThr, 2);
  nc    = TG_int(argsThr, 3);
  tid   = TA_get_id(argsThr);
  nt    = TA_get_n(argsThr);

  /* work */
  if (sch_work(nt, tid, 0, nr, &lo, &hi, &str)){
    for (r=lo; r<hi; r+=str){
      life_row(*world, *count, nr, nc, r, (nr+r-1)%nr, (r+1)%nr);
    }
  }

  THR_END(argsThr);
}

/*
 * @ life_thr_update : update world
 * > NULL
 * + update world
 */

static THR_DEF
life_thr_update(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  bool2D      * world;			/* world to evolve */
  int2D	      * count;			/* neighborhood counts */
  int		nr, nc;			/* row/col size */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		r, c;			/* loop indices */

  /* setup */
  world = TG_bool2D(argsThr, 0);
  count = TG_int2D(argsThr, 1);
  nr    = TG_int(argsThr, 2);
  nc    = TG_int(argsThr, 3);
  tid   = TA_get_id(argsThr);
  nt    = TA_get_n(argsThr);

  /* work */
  if (sch_work(nt, tid, 0, nr, &lo, &hi, &str)){
    for (r=lo; r<hi; r+=str){
      for (c=0; c<nc; c++){
	if (((*count)[r][c] == 3) || (((*count)[r][c] == 2) && (*world)[r][c])){
	  (*world)[r][c] = TRUE;
	} else {
	  (*world)[r][c] = FALSE;
	}
      }
    }
  }

  THR_END(argsThr);
}
