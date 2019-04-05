/*==============================================================*/
/* bar/app/winnow.c : barrier winnow implementation		*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*--------------------------------------------------------------*/
/* private definitions						*/
/*--------------------------------------------------------------*/

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

static void
winnow_copy(
  int		tid,			/* own ID */
  int2D		matrix,			/* matrix of values */
  bool2D	mask,			/* mask on values */
  int		nr,			/* row size */
  int		nc			/* column size */
);
static void
winnow_count(
  int		tid,			/* own ID */
  bool2D	mask,			/* mask on points */
  int		nr,			/* row size */
  int		nc			/* column size */
);
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
winnow_psrs_1(
  int		tid			/* own ID */
);
static void
winnow_psrs_2(
  int		tid			/* own ID */
);
static void
winnow_psrs_3(
  int		tid			/* own ID */
);
static void
winnow_psrs_4(
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
 * @ winnow : do body of point winnowing
 * > none
 * + create vector of points
 */

void
winnow(
  int		tid,			/* own ID */
  int2D		matrix,			/* point values */
  bool2D	mask,			/* suitable points */
  int		nr,			/* row size */
  int		nc,			/* column size */
  pt1D		pt,			/* points to create */
  int		npt			/* number of points */
){
  int		i, j;			/* loop indices */
  int		sum, tmp;		/* for scanning */
#if GRAPHICS
  int		gfxCount = 0;
#endif

#if GRAPHICS
  if (MASTER(tid)){
    gfx_winnow(gfxCount++, matrix, mask, pt, nr, nc, npt);
  }
  thr_bar(tid);
#endif

  /* pack points into temporary storage */
  winnow_count(tid, mask, nr, nc);
  i = scanIntSum(tid, Totals, nr);
  if (MASTER(tid)){
    Len = i;
  }
  thr_bar(tid);
  /* set slice sizes */
  if (MASTER(tid)){
    NumSamples   = ParWidth * (ParWidth - 1);
    ParWidth_1   = ParWidth - 1;
    SectionSize  = INT_CEIL(Len, ParWidth);
    IntervalSize = INT_CEIL(SectionSize, ParWidth_1);
  }
  ASSERT(Len >= npt);
  winnow_copy(tid, matrix, mask, nr, nc);

  /* sort */
  if ((ParWidth == 1) || (Len < NumSamples)){
    if (MASTER(tid)){
      ptSort(TmpPt, Len);
      winnow_pack(pt, npt, TmpPt, Len, 1, 0);
    }
    thr_bar(tid);
  } else {
    /* sort sections and select P-1 pivot values */
    winnow_psrs_1(tid);
    if (MASTER(tid)){
      intSort(Pivots, NumSamples);
    }
    thr_bar(tid);
    /* select P-1 pivot values from P*(P-1) pivot values */
    if (MASTER(tid)){
      for (i=0, j=ParWidth_1/2; j<NumSamples; i++, j+=ParWidth){
	Pivots[i] = Pivots[j];
      }
      ASSERT(i == ParWidth_1);
    }
    thr_bar(tid);
    /* count elements in processor intervals that belong in pivot intervals */
    winnow_psrs_2(tid);
    /* scan number of elements in pivot intervals */
    if (MASTER(tid)){
      sum = 0;
      for (i=0; i<ParWidth; i++){
	for (j=0; j<ParWidth*ParWidth; j+=ParWidth){
	  tmp = Counts[i+j];
	  Starts[i+j] = sum;
	  sum += tmp;
	}
      }
    }
    thr_bar(tid);
    /* copy values into pivot intervals */
    winnow_psrs_3(tid);
    /* sort pivot intervals */
    winnow_psrs_4(tid);
    /* copy selected points */
    winnow_pack(pt, npt, TmpPt2, Len, ParWidth, tid);
  }

#if GRAPHICS
  if (MASTER(tid)){
    gfx_winnow(gfxCount++, matrix, mask, pt, nr, nc, npt);
  }
#endif

  /* return */
}

/*--------------------------------------------------------------*/
/* private functions						*/
/*--------------------------------------------------------------*/

/*
 * @ winnow_copy : copy selected points from rows
 * > none
 * + fill tmpPt
 */

static void
winnow_copy(
  int		tid,			/* own ID */
  int2D		matrix,			/* matrix of values */
  bool2D	mask,			/* mask on values */
  int		nr,			/* row size */
  int		nc			/* column size */
){
  int		lo, hi, str;		/* work descriptors */
  int		r, c, i;		/* loop and vector indices */

  /* work */
  if (sch_work(ParWidth, tid, 0, nr, &lo, &hi, &str)){
    for (r=lo; r<hi; r+=str){
      i = Totals[r];
      for (c=0; c<nc; c++){
	if (mask[r][c]){
	  TmpPt[i].x = r;
	  TmpPt[i].y = c;
	  TmpPt[i].w = matrix[r][c];
	  i++;
	}
      }
    }
  }
  thr_bar(tid);

  /* return */
}

/*
 * @ winnow_count : count selected points within rows
 * > none
 * + fill totals
 */

static void
winnow_count(
  int		tid,			/* own ID */
  bool2D	mask,			/* mask on points */
  int		nr,			/* row size */
  int		nc			/* column size */
){
  int		lo, hi, str;		/* work descriptors */
  int		r, c;			/* loop indices */

  /* work */
  if (sch_work(ParWidth, tid, 0, nr, &lo, &hi, &str)){
    for (r=lo; r<hi; r+=str){
      Totals[r] = 0;
      for (c=0; c<nc; c++){
	if (mask[r][c]){
	  Totals[r] += 1;
	}
      }
    }
  }
  thr_bar(tid);

  /* return */
}

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
 * @ winnow_psrs_1 : first pass of PSRS
 * > none
 * + sort subsections, choose pivot elements
 */

static void
winnow_psrs_1(
  int		tid			/* own ID */
){
  int		lo, hi;			/* work descriptors */
  int		i;			/* indices */
  int		offset;			/* to ensure even sampling */
  int		dex;			/* total index */

  /* do own scheduling */
  offset = IntervalSize / 2;
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
  thr_bar(tid);

  /* return */
}

/*
 * @ winnow_psrs_2 : second phase of PSRS
 * > none
 * + count elements in each subsection
 */

static void
winnow_psrs_2(
  int		tid			/* own ID */
){
  int		lo, hi;			/* own interval */
  int		i;			/* index into array */
  int		dexPivot, dexCount;	/* indices into pivots and counts */
  int		limCount;		/* limit on dexCount */

  /* do own scheduling */
  winnow_sched(tid, &lo, &hi);

  /* count elements */
  dexPivot = 0;
  dexCount = tid * ParWidth;
  limCount = (tid + 1) * ParWidth;
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
  thr_bar(tid);

  /* return */
}

/*
 * @ winnow_psrs_3 : third pass of PSRS
 * > none
 * + copy elements into correct section
 */

static void
winnow_psrs_3(
  int		tid			/* own ID */
){
  int		lo, hi;			/* own source section boundaries */
  int		dexSrc, dexDst;		/* source and destination indices */
  int		dexCtrl;		/* index into control structures */

  /* do own scheduling */
  winnow_sched(tid, &lo, &hi);

  /* work */
  dexSrc = lo;
  for (dexCtrl = tid*ParWidth; dexCtrl<(tid+1)*ParWidth; dexCtrl++){
    for (dexDst = Starts[dexCtrl];
	 dexDst < Starts[dexCtrl] + Counts[dexCtrl];
	 dexDst++){
      TmpPt2[dexDst] = TmpPt[dexSrc];
      dexSrc++;
    }
  }
  ASSERT(dexSrc == hi);
  thr_bar(tid);

  /* return */
}

/*
 * @ winnow_psrs_4 : fourth phase of PSRS
 * > none
 * + sort elements in subsections
 */

static void
winnow_psrs_4(
  int		tid			/* own ID */
){
  int		secBase, secLen;	/* base of own section and its length */

  secBase = Starts[tid];
  if (tid < ParWidth-1){
    secLen = Starts[tid+1] - secBase;
  } else {
    secLen = Len - secBase;
  }
  ptSort(TmpPt2+secBase, secLen);
  thr_bar(tid);

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
