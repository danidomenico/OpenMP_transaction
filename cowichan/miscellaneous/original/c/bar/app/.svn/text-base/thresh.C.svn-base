/*==============================================================*/
/* bar/app/thresh.c : barrier thresh implementation		*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*--------------------------------------------------------------*/
/* private data structures					*/
/*--------------------------------------------------------------*/

static int    * Hist;			/* histogram */
static int	Retain;			/* number to retain */
static int	VMaxThr[MAXPAR];	/* partial matrix maxima */
static int	VMax;			/* actual matrix maximum */

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

/*
 * @ thresh : do body of histogram thresholding
 * > none
 * + create mask
 */

void
thresh(
  int		tid,			/* own ID */
  int2D		matrix,			/* to threshold */
  bool2D	mask,			/* threshold mask */
  int		nr,			/* row size */
  int		nc,			/* column size */
  real		fraction		/* how much to keep */
){
  int		lo, hi, str;		/* work controls */
  int		i, t, r, c;		/* loop indices */
  bool		work;			/* do useful work? */
  int	      * histThr;		/* own histogram section */
#if GRAPHICS
  int		gfxCount = 0;		/* number of times graphics called */
#endif

  /* more setup */
  if (MASTER(tid)){
    Retain = (int)(fraction * nc * nr);
  }
  thr_bar(tid);

  /* any useful work to do? */
  work = sch_work(ParWidth, tid, 0, nr, &lo, &hi, &str);

  /* find max value in matrix */
  VMaxThr[tid] = 0;
  if (work){
    for (r=lo; r<hi; r+=str){
      for (c=0; c<nc; c++){
	if (VMaxThr[tid] < matrix[r][c]) VMaxThr[tid] = matrix[r][c];
      }
    }
  }
  thr_bar(tid);

  /* initialize histogram */
  if (MASTER(tid)){
    /* find real maximum */
    VMax = VMaxThr[0];
    for (i=1; i<ParWidth; i++){
      if (VMaxThr[i] > VMax) VMax = VMaxThr[i];
    }
    /* allocate and initialize histogram */
    ALLOC(Hist, int, ParWidth * (VMax+1), "thresh", "histogram");
  }
  thr_bar(tid);

#if GRAPHICS
  if (MASTER(tid)){
    gfx_thresh(gfxCount++, matrix, mask, nr, nc, Hist, VMax);
  }
  thr_bar(tid);
#endif

  /* initialize own portion of histogram */
  histThr = Hist + (tid * (VMax + 1));
  for (i=0; i<=VMax; i++){
    histThr[i] = 0;
  }

  /* count */
  if (work){
    for (r=lo; r<hi; r+=str){
      for (c=0; c<nc; c++){
	histThr[matrix[r][c]]++;
      }
    }
  }
  thr_bar(tid);

  /* calculate retention */
  if (MASTER(tid)){
    for (t=1; t<ParWidth; t++){
      for (i=0; i<=VMax; i++){
	Hist[i] += Hist[(t * (VMax+1)) + i];
      }
    }
    for (i = VMax; ((i >= 0) && (Retain > 0)); i--) {
      Retain -= Hist[i];
    }
    Retain = i;
  }
  thr_bar(tid);

  /* threshold */
  if (work){
    for (r=lo; r<hi; r+=str){
      for (c=0; c<nc; c++){
	mask[r][c] = matrix[r][c] > Retain;
      }
    }
  }
  thr_bar(tid);

#if GRAPHICS
  if (MASTER(tid)){
    gfx_thresh(gfxCount++, matrix, mask, nr, nc, Hist, VMax);
  }
  thr_bar(tid);
#endif

  /* takedown */
  if (MASTER(tid)){
    FREE(Hist);
  }
  thr_bar(tid);

  /* return */
}
