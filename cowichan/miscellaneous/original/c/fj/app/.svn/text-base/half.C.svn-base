/*==============================================================*/
/* fj/app/half.c : forkjoin half implementation			*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

static thr_f	half_thr_col;
static thr_f	half_thr_row;

/*
 * @ half : do halving shuffle
 * > none
 * + shuffle rows and columns
 */

void
half(
  int2D		matrix,			/* to shuffle */
  int		nr,			/* row size */
  int		nc			/* column size */
){
  void	      * args[3];		/* user argument vector */
#if GRAPHICS
  int		gfxCount = 0;
#endif

  TP_any(args, 0, matrix);
  TP_any(args, 1, nr);
  TP_any(args, 2, nc);

#if GRAPHICS
  gfx_half(gfxCount++, matrix, nr, nc);
#endif

  thr_grp(half_thr_row, ParWidth, args);

#if GRAPHICS
  gfx_half(gfxCount++, matrix, nr, nc);
#endif

  thr_grp(half_thr_col, ParWidth, args);

#if GRAPHICS
  gfx_half(gfxCount++, matrix, nr, nc);
#endif

  /* return */
}

/*--------------------------------------------------------------*/
/* threading functions						*/
/*--------------------------------------------------------------*/

/*
 * @ half_thr_col : shuffle by columns
 */

static THR_DEF
half_thr_col(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  int2D	      * matrix;			/* matrix to halve */
  int		nr, nc;			/* row/col size */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		i, r, c;		/* indices */
  int1D		tmp;			/* for shuffling */

  /* setup */
  matrix = TG_int2D(argsThr, 0);
  nr     = TG_int(argsThr, 1);
  nc     = TG_int(argsThr, 2);
  tid    = TA_get_id(argsThr);
  nt     = TA_get_n(argsThr);

  /* columns */
  if (sch_work(nt, tid, 0, nr, &lo, &hi, &str)){
    for (c=lo; c<hi; c+=str){
      for (r=1, i=0; r<nr; r+=2, i++){
	tmp[i] = (*matrix)[r][c];
      }
      for (r=0, i=0; r<(nr+1)/2; r++, i+=2){
	(*matrix)[r][c] = (*matrix)[i][c];
      }
      for (r=(nr+1)/2, i=0; r<nr; r++, i++){
	(*matrix)[r][c] = tmp[i];
      }
    }
  }

  THR_END(argsThr);
}

/*
 * @ half_thr_row : halving by rows
 */

static THR_DEF
half_thr_row(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  int2D	      * matrix;			/* matrix to halve */
  int		nr, nc;			/* row/col size */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		i, r, c;		/* indices */
  int1D		tmp;			/* for shuffling */

  /* setup */
  matrix = TG_int2D(argsThr, 0);
  nr     = TG_int(argsThr, 1);
  nc     = TG_int(argsThr, 2);
  tid    = TA_get_id(argsThr);
  nt     = TA_get_n(argsThr);

  /* rows */
  if (sch_work(nt, tid, 0, nc, &lo, &hi, &str)){
    for (r=lo; r<hi; r+=str){
      for (c=1, i=0; c<nc; c+=2, i++){
	tmp[i] = (*matrix)[r][c];
      }
      for (c=0, i=0; c<(nc+1)/2; c++, i+=2){
	(*matrix)[r][c] = (*matrix)[r][i];
      }
      for (c=(nc+1)/2, i=0; c<nc; c++, i++){
	(*matrix)[r][c] = tmp[i];
      }
    }
  }

  THR_END(argsThr);
}
