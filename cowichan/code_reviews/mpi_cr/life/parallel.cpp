/**
 * Parallel implementation of conway's game of life
 *
 * \file parallel.cpp
 * \author Andrew Borzenko
 * \date 01-26-09
 */

#if NUMA
EXTERN_ENV
#endif

#include "../include/main.h"
#include "parallel.h"

// public

void
life_mpi(
  mpi::communicator world,
  bool2D*	matrix,			/* world to evolve */
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
  gfx_life(gfxCount++, matrix, nr, nc);
#endif

  int		lo, hi;		/* work controls */
  bool		work;			/* useful work to do? */ 
  int is_alive = 1;
  int rank;

  count = new int2D[MAXEXT];

  // work
  work = get_block_rows_mpi (world, 0, nr, &lo, &hi);
  for (i=0; (i<iters) && is_alive; i++){
    // fill neighborhood counts
    if (work) {
      for (r = lo; r < hi; r++) {
        life_row_mpi(matrix, count, nr, nc, r,
          (nr + r - 1) % nr, (nr + r + 1) % nr);
      }
    }
    // broadcast counts
    for (r = 0; r < nr; r++) {
      rank = get_block_rank_mpi (world, 0, nr, r);
      broadcast (world, count[r], nc, rank);
    }
    // update cells
    alive = 0;
    if (work) {
      for (r = lo; r < hi; r++) {
        for (c=0; c<nc; c++) {
          if ((count[r][c] == 3) || ((count[r][c] == 2) && matrix[r][c])){ 
            matrix[r][c] = TRUE;
            alive++;
          }
          else {
            matrix[r][c] = FALSE;
          }
        }
      }
    }
    // broadcast matrix
    for (r = 0; r < nr; r++) {
      rank = get_block_rank_mpi (world, 0, nr, r);
      broadcast (world, matrix[r], nc, rank);
    }
    // is_alive is maximum of local alive's
    if (world.rank () == 0) {
      reduce (world, alive, is_alive, mpi::maximum<int>(), 0);
    }
    else {
      reduce (world, alive, mpi::maximum<int>(), 0);
    }
    broadcast (world, is_alive, 0);
    is_alive = 1;

#if GRAPHICS
    gfx_life(gfxCount++, matrix, nr, nc);
#endif
  }

  delete [] count;

  /* check */
  if (is_alive == 0){
    fail("life", "no cells left alive", "iteration", "%d", i, NULL);
  }

  /* return */
}

// private

void
life_one_mpi(
  bool2D*	matrix,			/* world to evolve */
  int2D*		count,			/* neighborhood counts */
  int		r,			/* this row */
  int		r_lo,			/* lower row */
  int		r_hi,			/* higher row */
  int		c,			/* this column */
  int		c_lo,			/* lower column */
  int		c_hi			/* higher column */
){
  count[r][c] = matrix[r_lo][c_lo] + matrix[r_lo][c] + matrix[r_lo][c_hi]
	      + matrix[r][c_lo]            +          matrix[r][c_hi]
	      + matrix[r_hi][c_lo] + matrix[r_hi][c] + matrix[r_hi][c_hi];
  /* return */
}

void
life_row_mpi(
  bool2D*	matrix,			/* world to evolve */
  int2D*		count,			/* neighborhood counts */
  int		nr,			/* row size */
  int		nc,			/* column size */
  int		r,			/* this row */
  int		r_lo,			/* lower row */
  int		r_hi			/* higher row */
){
  int		c;			/* column index */

  life_one_mpi(matrix, count, r, r_lo, r_hi, 0, nc-1, 1);
  for (c=1; c<(nc-1); c++){
    life_one_mpi(matrix, count, r, r_lo, r_hi, c, c-1, c+1);
  }
  life_one_mpi(matrix, count, r, r_lo, r_hi, nc-1, nc-2, 0);

  /* return */
}
