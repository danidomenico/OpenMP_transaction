/*==============================================================*/
/* fj/app/winnow.c : forkjoin winnow implementation		*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*--------------------------------------------------------------*/
/* private definitions						*/
/*--------------------------------------------------------------*/

#define WDEX_MATRIX 0
#define WDEX_MASK   1
#define WDEX_NR     2
#define WDEX_NC     3
#define WDEX_PT     4
#define WDEX_NPT    5
#define WDEX_SZ     6

#define MP_2 (MAXPAR*MAXPAR)

/*--------------------------------------------------------------*/
/* private data structures					*/
/*--------------------------------------------------------------*/

static int1D	Totals;		/* row totals */
static pt1DX	TmpPt;		/* temporary storage vector */
static pt1DX	TmpPt2;		/* another temporary storage vector */
static int	Pivots[MP_2];	/* pivot elements */
static int	Counts[MP_2];	/* number of elements in each interval */
static int	Starts[MP_2];	/* starts of sub-sections */
static int	Len;		/* number of points */
static int	NumSamples;	/* total number of samples */
static int	SectionSize;	/* size of individual sections */
static int	IntervalSize;	/* sampling interval */
static int	ParWidth_1;	/* number of threads - 1 */

/*--------------------------------------------------------------*/
/* private function prototypes					*/
/*--------------------------------------------------------------*/

static thr_f	winnow_thr_copy;
static thr_f	winnow_thr_count;
static thr_f	winnow_thr_pack;
static thr_f	winnow_thr_psrs_1;
static thr_f	winnow_thr_psrs_2;
static thr_f	winnow_thr_psrs_3;
static thr_f	winnow_thr_psrs_4;
static void
winnow_pack(
  pt1D		ptDst,			/* to pack into */
  int		nDst,			/* number of points */
  pt1D		ptSrc,			/* to pull from */
  int		nSrc,			/* number of tmps */
  int		nt,			/* number of threads */
  int		tid			/* own ID */
);
static void
winnow_sched(
  int		tid,			/* caller thread ID */
  int	      * start,			/* start of own interval */
  int	      * end			/* end of own interval */
);

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

/*
 * @ winnow : do point winnowing
 * > none
 * + create vector of points
 */

void
winnow(
  int2D		matrix,			/* point values */
  bool2D	mask,			/* suitable points */
  int		nr,			/* row size */
  int		nc,			/* column size */
  pt1D		pt,			/* points to create */
  int		npt			/* number of points */
){
  void	      * args[WDEX_SZ];
  int		sum, tmp;		/* for scanning */
  int		i, j;			/* loop indices */
#if GRAPHICS
  int		gfxCount;
#endif

#if GRAPHICS
  gfx_winnow(gfxCount++, matrix, mask, pt, nr, nc, npt);
#endif

  /* pack shared arguments once */
  TP_any(args, WDEX_MATRIX, matrix);
  TP_any(args, WDEX_MASK, mask);
  TP_any(args, WDEX_NR, nr);
  TP_any(args, WDEX_NC, nc);
  TP_any(args, WDEX_PT, pt);
  TP_any(args, WDEX_NPT, npt);

  /* pack points into temporary storage */
  thr_grp(winnow_thr_count, ParWidth, args);
  Len = scanIntSum(Totals, nr);
  NumSamples = ParWidth * (ParWidth - 1);
  ASSERT(Len >= npt);
  thr_grp(winnow_thr_copy, ParWidth, args);

  /* sort */
  if ((ParWidth == 1) || (Len < NumSamples)){
    ptSort(TmpPt, Len);
    winnow_pack(pt, npt, TmpPt, Len, 1, 0);
  } else {
    /* set slice sizes */
    ParWidth_1   = ParWidth - 1;
    SectionSize  = INT_CEIL(Len, ParWidth);
    IntervalSize = INT_CEIL(SectionSize, ParWidth_1);
    /* sort sections and select P-1 pivot values */
    thr_grp(winnow_thr_psrs_1, ParWidth, args);
    intSort(Pivots, NumSamples);
    /* select P-1 pivot values from P*(P-1) pivot values */
    for (i=0, j=ParWidth_1/2; j<NumSamples; i++, j+=ParWidth){
      Pivots[i] = Pivots[j];
    }
    ASSERT(i == ParWidth_1);
    /* count elements in processor intervals that belong in pivot intervals */
    thr_grp(winnow_thr_psrs_2, ParWidth, args);
    /* scan number of elements in pivot intervals */
    sum = 0;
    for (i=0; i<ParWidth; i++){
      for (j=0; j<ParWidth*ParWidth; j+=ParWidth){
	tmp = Counts[i+j];
	Starts[i+j] = sum;
	sum += tmp;
      }
    }
    /* copy values into pivot intervals */
    thr_grp(winnow_thr_psrs_3, ParWidth, args);
    /* sort pivot intervals */
    thr_grp(winnow_thr_psrs_4, ParWidth, args);
    /* copy selected points */
    thr_grp(winnow_thr_pack, ParWidth, args);
  }

#if GRAPHICS
  gfx_winnow(gfxCount++, matrix, mask, pt, nr, nc, npt);
#endif

  /* return */
}

/*--------------------------------------------------------------*/
/* threading functions						*/
/*--------------------------------------------------------------*/

/*
 * @ winnow_thr_copy : copy selected points from rows
 * > NULL
 * + fill tmpPt
 */

static THR_DEF
winnow_thr_copy(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  int2D       * matrix;			/* matrix of weights */
  bool2D      * mask;			/* selection mask */
  int		nr, nc;			/* sizes */
  int		tid, nt;		/* thread ID & num threads */
  int		lo, hi, str;		/* work descriptors */
  int		r, c, i;		/* loop and vector indices */

  /* setup */
  matrix = TG_int2D(argsThr, WDEX_MATRIX);
  mask   = TG_bool2D(argsThr, WDEX_MASK);
  nr     = TG_int(argsThr, WDEX_NR);
  nc     = TG_int(argsThr, WDEX_NC);
  tid    = TA_get_id(argsThr);
  nt     = TA_get_n(argsThr);

  /* work */
  if (sch_work(nt, tid, 0, nr, &lo, &hi, &str)){
    for (r=lo; r<hi; r+=str){
      i = Totals[r];
      for (c=0; c<nc; c++){
	if ((*mask)[r][c]){
	  TmpPt[i].x = r;
	  TmpPt[i].y = c;
	  TmpPt[i].w = (*matrix)[r][c];
	  i++;
	}
      }
    }
  }

  THR_END(argsThr); 
}

/*
 * @ winnow_thr_count : count selected points within rows
 * > NULL
 * + fill totals
 */

static THR_DEF
winnow_thr_count(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  bool2D      * mask;			/* selection mask */
  int		nr, nc;			/* sizes */
  int		tid, nt;		/* thread ID & num threads */
  int		lo, hi, str;		/* work descriptors */
  int		r, c;			/* loop indices */

  /* setup */
  mask   = TG_bool2D(argsThr, WDEX_MASK);
  nr     = TG_int(argsThr, WDEX_NR);
  nc     = TG_int(argsThr, WDEX_NC);
  tid    = TA_get_id(argsThr);
  nt     = TA_get_n(argsThr);

  /* work */
  if (sch_work(nt, tid, 0, nr, &lo, &hi, &str)){
    for (r=lo; r<hi; r+=str){
      Totals[r] = 0;
      for (c=0; c<nc; c++){
	if ((*mask)[r][c]){
	  Totals[r] += 1;
	}
      }
    }
  }

  THR_END(argsThr); 
}

/*
 * @ winnow_thr_pack : pack selected points
 * > NULL
 * + pack points
 */

static THR_DEF
winnow_thr_pack(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  int		tid, nt;		/* thread ID & num threads */
  pt1D	      * pt;			/* to pack into */
  int		npt;			/* number of points */

  /* setup */
  pt  = TG_pt1D(argsThr, WDEX_PT);
  npt = TG_int(argsThr, WDEX_NPT);
  tid = TA_get_id(argsThr);
  nt  = TA_get_n(argsThr);

  /* work */
  winnow_pack(*pt, npt, TmpPt2, Len, nt, tid);

  THR_END(argsThr); 
}

/*
 * @ winnow_thr_psrs_1 : first pass of PSRS
 * > NULL
 * + sort subsections, choose pivot elements
 */

static THR_DEF
winnow_thr_psrs_1(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  int		tid, nt;		/* thread ID & num threads */
  int		lo, hi;			/* work descriptors */
  int		i;			/* indices */
  int		offset;			/* to ensure even sampling */
  int		dex;			/* total index */

  /* do own scheduling */
  tid = TA_get_id(argsThr);
  nt  = TA_get_n(argsThr);
  offset = IntervalSize / 2;

  /* work */
  winnow_sched(tid, &lo, &hi);
  /* sort */
  ptSort(TmpPt + lo, hi - lo);
  /* pack pivots */
  for (i=0; i<ParWidth_1; i++){
    dex = lo + offset + (i*IntervalSize);
    if (dex < hi){
      Pivots[(tid*ParWidth_1)+i] = TmpPt[dex].w;
    } else {
      Pivots[(tid*ParWidth_1)+i] = TmpPt[hi-1].w;
    }
  }

  THR_END(argsThr); 
}

/*
 * @ winnow_thr_psrs_2 : second phase of PSRS
 * > NULL
 * + count elements in each subsection
 */

static THR_DEF
winnow_thr_psrs_2(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  int		tid, nt;		/* own ID and # threads */
  int		lo, hi;			/* own interval */
  int		i;			/* index into array */
  int		dexPivot, dexCount;	/* indices into pivots and counts */
  int		limCount;		/* limit on dexCount */

  /* do own scheduling */
  tid = TA_get_id(argsThr);
  nt  = TA_get_n(argsThr);

  /* count elements */
  winnow_sched(tid, &lo, &hi);
  dexPivot = 0;
  dexCount = tid * nt;
  limCount = (tid + 1) * nt;
  Counts[dexCount] = 0;
  for (i=lo; (i<hi) && (dexCount < limCount-1); i++){
    /* into current bucket */
    if (TmpPt[i].w < Pivots[dexPivot]){
      Counts[dexCount]++;
    }
    /* move on to next bucket */
    else {
      while ((dexPivot < ParWidth_1) && (Pivots[dexPivot] <= TmpPt[i].w)){
	dexPivot++;
	dexCount++;
	ASSERT(dexCount < limCount);
	Counts[dexCount] = 0;
      }
      /* account for this element */
      if (dexCount == limCount-1){
	Counts[dexCount] = (hi - i);
      } else {
	Counts[dexCount] = 1;
      }
    }
  }
  /* tidy up end */
  dexCount++;
  while (dexCount < limCount){
    Counts[dexCount] = 0;
    dexCount++;
  }

  THR_END(argsThr); 
}

/*
 * @ winnow_thr_psrs_3 : third pass of PSRS
 * > NULL
 * + copy elements into correct section
 */

static THR_DEF
winnow_thr_psrs_3(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  int		tid, nt;		/* thread ID & num threads */
  int		lo, hi;			/* own source section boundaries */
  int		dexSrc, dexDst;		/* source and destination indices */
  int		dexCtrl;		/* index into control structures */

  /* do own scheduling */
  tid = TA_get_id(argsThr);
  nt  = TA_get_n(argsThr);

  /* work */
  winnow_sched(tid, &lo, &hi);
  dexSrc = lo;
  for (dexCtrl = tid*nt; dexCtrl<(tid+1)*nt; dexCtrl++){
    for (dexDst = Starts[dexCtrl];
	 dexDst < Starts[dexCtrl] + Counts[dexCtrl];
	 dexDst++){
      TmpPt2[dexDst] = TmpPt[dexSrc];
      dexSrc++;
    }
  }
  ASSERT(dexSrc == hi);

  THR_END(argsThr); 
}

/*
 * @ winnow_thr_psrs_4 : fourth phase of PSRS
 * > NULL
 * + sort elements in subsections
 */

static THR_DEF
winnow_thr_psrs_4(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  int		tid, nt;		/* thread ID & num threads */
  int		secBase, secLen;	/* base of own section and its length */

  tid     = TA_get_id(argsThr);
  nt      = TA_get_n(argsThr);
  secBase = Starts[tid];
  if (tid < nt-1){
    secLen = Starts[tid+1] - secBase;
  } else {
    secLen = Len - secBase;
  }
  ptSort(TmpPt2+secBase, secLen);

  THR_END(argsThr); 
}

/*--------------------------------------------------------------*/
/* private functions						*/
/*--------------------------------------------------------------*/

/*
 * @ winnow_pack : compact points (serially)
 * > none
 * + pack points
 */

static void
winnow_pack(
  pt1D		ptDst,			/* to pack into */
  int		nDst,			/* number of points */
  pt1D		ptSrc,			/* to pull from */
  int		nSrc,			/* number of tmps */
  int		nt,			/* number of threads */
  int		tid			/* own ID */
){
  int		i, j;			/* loop indices */
  int		stride;			/* stride in source */

  stride = nSrc / nDst;
  for (i=(nDst-1)-tid, j=(nSrc-1)-(tid*stride);
       i>=0;
       i-=nt, j-=(stride*nt)){
    ptDst[i] = ptSrc[j];
  }
  /* return */
}

/*
 * @ winnow_sched : do scheduling for PSRS
 * > none
 * + set limit parameters
 */

static void
winnow_sched(
  int		tid,			/* caller thread ID */
  int	      * start,			/* start of own interval */
  int	      * end			/* end of own interval */
){
  *start = tid * SectionSize;
  *end = (tid + 1) * SectionSize;
  if (*end > Len){
    *end = Len;
  }
  /* return */
}
