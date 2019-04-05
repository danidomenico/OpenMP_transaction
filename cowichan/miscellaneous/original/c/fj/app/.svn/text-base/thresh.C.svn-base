/*==============================================================*/
/* fj/app/thresh.c : forkjoin thresh implementation		*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

static thr_f	thresh_thr_hist;
static thr_f	thresh_thr_mask;
static thr_f	thresh_thr_max;

/*
 * @ thresh : do histogram thresholding
 * > none
 * + create mask
 */

void
thresh(
  int2D		matrix,			/* to threshold */
  bool2D	mask,			/* threshold mask */
  int		nr,			/* row size */
  int		nc,			/* column size */
  real		fraction		/* how much to keep */
){
  void	      * args[6];
  int		vMaxThr[MAXPAR];	/* per-thread max value */
  int         * hist;			/* histogram */
  int		i, t;			/* loop indices */
  int		vMax;			/* max value in matrix */
  int		retain;			/* selection */
#if GRAPHICS
  int		gfxCount = 0;		/* number of times graphics called */
#endif

  /* pack shared arguments */
  TP_any(args, 0, matrix);
  TP_any(args, 1, mask);
  TP_any(args, 2, nr);
  TP_any(args, 3, nc);

  /* find max value in matrix */
  TP_any(args, 4, vMaxThr);
  thr_grp(thresh_thr_max, ParWidth, args);
  vMax = vMaxThr[0];
  for (i=1; i<ParWidth; i++){
    if (vMax < vMaxThr[i]){
      vMax = vMaxThr[i];
    }
  }
#if GRAPHICS
  gfx_thresh(gfxCount++, matrix, mask, nr, nc, hist, vMax);
#endif

  /* count */
  ALLOC(hist, int, ParWidth * (vMax+1), "thresh", "histogram");
  TP_any(args, 4, hist);
  TP_any(args, 5, vMax);
  thr_grp(thresh_thr_hist, ParWidth, args);
  for (t=1; t<ParWidth; t++){
    for (i=0; i<=vMax; i++){
      hist[i] += hist[(t * (vMax + 1)) + i];
    }
  }

  /* include */
  retain = (int)(fraction * nc * nr);
  for (i = vMax; ((i >= 0) && (retain > 0)); i--) {
    retain -= hist[i];
  }
  retain = i;

  /* threshold */
  TP_any(args, 4, retain);
  thr_grp(thresh_thr_mask, ParWidth, args);
#if GRAPHICS
  gfx_thresh(gfxCount++, matrix, mask, nr, nc, hist, vMax);
#endif

  FREE(hist);

  /* return */
}

/*--------------------------------------------------------------*/
/* threading functions						*/
/*--------------------------------------------------------------*/

/*
 * @ thresh_thr_hist : compute portion of matrix value histogram
 * > NULL
 * + fill section of histogram
 */

static THR_DEF
thresh_thr_hist(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  int2D	      * matrix;			/* matrix to threshold */
  bool2D      * mask;			/* mask created */
  int		nr, nc;			/* row/col size */
  int	      * hist;			/* histogram */
  int		vMax;			/* maximum value in matrix */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		i, r, c;		/* indices */

  /* setup */
  matrix = TG_int2D(argsThr, 0);
  mask   = TG_bool2D(argsThr, 1);
  nr     = TG_int(argsThr, 2);
  nc     = TG_int(argsThr, 3);
  hist   = (int *)TG_void_p(argsThr, 4);
  vMax   = TG_int(argsThr, 5);
  tid    = TA_get_id(argsThr);
  nt     = TA_get_n(argsThr);

  /* initialize own section of histogram */
  hist += tid * (vMax+1);
  for (i=0; i<=vMax; i++){
    hist[i] = 0;
  }

  /* work */
  if (sch_work(nt, tid, 0, nr, &lo, &hi, &str)){
    for (r=lo; r<hi; r+=str){
      for (c=0; c<nc; c++){
	hist[(*matrix)[r][c]]++;
      }
    }
  }

  THR_END(argsThr);
}

/*
 * @ thresh_thr_max : find maximum value in matrix
 * > NULL
 * + fill entry in max value vector
 */

static THR_DEF
thresh_thr_max(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  int2D	      * matrix;			/* matrix to threshold */
  bool2D      * mask;			/* mask created */
  int		nr, nc;			/* row/col size */
  int	      * vMaxThr;		/* per-thread maximum value */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		r, c;			/* indices */

  /* setup */
  matrix  = TG_int2D(argsThr, 0);
  mask    = TG_bool2D(argsThr, 1);
  nr      = TG_int(argsThr, 2);
  nc      = TG_int(argsThr, 3);
  vMaxThr = (int *)TG_void_p(argsThr, 4);
  tid     = TA_get_id(argsThr);
  nt      = TA_get_n(argsThr);

  /* work */
  if (sch_work(nt, tid, 0, nr, &lo, &hi, &str)){
    vMaxThr[tid] = 0;
    for (r=lo; r<hi; r+=str){
      for (c=0; c<nc; c++){
	if (vMaxThr[tid] < (*matrix)[r][c]){
	  vMaxThr[tid] = (*matrix)[r][c];
	}
      }
    }
  }

  THR_END(argsThr);
}

/*
 * @ thresh_thr_mask : fill portion of mask
 * > NULL
 * + fill portion of mask
 */

static THR_DEF
thresh_thr_mask(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  int2D	      * matrix;			/* matrix to threshold */
  bool2D      * mask;			/* mask created */
  int		nr, nc;			/* row/col size */
  int		lim;			/* limiting value */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		r, c;			/* indices */

  /* setup */
  matrix = TG_int2D(argsThr, 0);
  mask   = TG_bool2D(argsThr, 1);
  nr     = TG_int(argsThr, 2);
  nc     = TG_int(argsThr, 3);
  lim    = TG_int(argsThr, 4);
  tid    = TA_get_id(argsThr);
  nt     = TA_get_n(argsThr);

  /* work */
  if (sch_work(nt, tid, 0, nr, &lo, &hi, &str)){
    for (r=lo; r<hi; r+=str){
      for (c=0; c<nc; c++){
	(*mask)[r][c] = (*matrix)[r][c] > lim;
      }
    }
  }

  THR_END(argsThr);
}
