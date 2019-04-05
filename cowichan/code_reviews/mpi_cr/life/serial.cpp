/**
 * Serial implementation of conway's game of life
 *
 * \file serial.cpp
 * \author Andrew Borzenko
 * \date 01-26-09
 */

#if NUMA
EXTERN_ENV
#endif

#include "../include/main.h"
#include "serial.h"

// public

void
life(
  bool2D*	world,			/* world to evolve */
  int		nr,			/* row size */
  int		nc,			/* column size */
  int		iters			/* number of iterations */
){
  int2D*		count;			/* neighborhood counts */
  int		i;			/* iteration index */
  int		r, c;			/* row/column indices */
  int		alive = nr * nc;	/* number alive */
#if GRAPHICS
  int		gfxCount = 0;
#endif

#if GRAPHICS
  gfx_life(gfxCount++, world, nr, nc);
#endif

  count = new int2D[MAXEXT];

  for (i=0; (i<iters) && alive; i++){
    /* fill neighborhood counts */
    life_row(world, count, nr, nc, 0, nr-1, 1);
    for (r=1; r<(nr-1); r++){
      life_row(world, count, nr, nc, r, r-1, r+1);
    }
    life_row(world, count, nr, nc, nr-1, nr-2, 0);
    /* update cells */
    alive = 0;
    for (r=0; r<nr; r++){
      for (c=0; c<nc; c++){
	if ((count[r][c] == 3) || ((count[r][c] == 2) && world[r][c])){
	  world[r][c] = TRUE;
	  alive += 1;
	} else {
	  world[r][c] = FALSE;
	}
      }
    }
#if GRAPHICS
    gfx_life(gfxCount++, world, nr, nc);
#endif
  }

  delete [] count;

  /* check */
  if (alive == 0){
    fail("life", "no cells left alive", "iteration", "%d", i, NULL);
  }

  /* return */
}

// private

void
life_one(
  bool2D*	world,			/* world to evolve */
  int2D*		count,			/* neighborhood counts */
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

void
life_row(
  bool2D*	world,			/* world to evolve */
  int2D*		count,			/* neighborhood counts */
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
