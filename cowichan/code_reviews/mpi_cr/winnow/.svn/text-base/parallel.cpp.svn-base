/**
 * Parallel implementation of weighted point selection
 *
 * \file parallel.cpp
 * \author Andrew Borzenko
 * \date 02-23-09
 */

#include "../include/main.h"
#include "parallel.h"

// private

static int1D*	Totals;		/* row totals */
static pt*	TmpPt;		/* temporary storage vector */
static pt*	TmpPt2;		/* another temporary storage vector */
static int*	Pivots;	/* pivot elements */
static int*	Counts;	/* number of elements in each interval */
static int*	Starts;	/* starts of sub-sections */
static int	Len;		/* number of points */
static int	NumSamples;	/* total number of samples */
static int	SectionSize;	/* size of individual sections */
static int	IntervalSize;	/* sampling interval */
static int	ParWidth_1;	/* number of threads - 1 */ 

static int*	SumProc;		/* partial sums */
static int	SumShared;		/* final result */

// public

void
winnow_mpi(mpi::communicator world,
  int2D*		matrix,			/* point values */
  bool2D*	mask,			/* suitable points */
  int		nr,			/* row size */
  int		nc,			/* column size */
  pt1D*		pts,			/* points to create */
  int		npt			/* number of points */
){
  int		i, j;			/* loop indices */
  int		sum, tmp;		/* for scanning */
#if GRAPHICS
  int		gfxCount = 0;
#endif
  int rank, size;

  rank = world.rank ();
  size = world.size ();
  if (size > 1) {
    ParWidth_1 = size - 1;

    Totals = new int1D[MAXEXT];
    TmpPt = new pt[MAXEXT * MAXEXT];
    TmpPt2 = new pt[MAXEXT * MAXEXT];

    Pivots = new int[size * size];
    Counts = new int[size * size];
    Starts = new int[size * size];

    SumProc = new int[size];

    /* pack points into temporary storage */

    winnow_count(world, mask, nr, nc);
    i = scanIntSum(world, Totals, nr);

    Len = i;

    /* set slice sizes */
    NumSamples   = size * (ParWidth_1);
    SectionSize  = INT_CEIL(Len, size);
    IntervalSize = INT_CEIL(SectionSize, ParWidth_1);

    ASSERT(Len >= npt);
    winnow_copy(world, matrix, mask, nr, nc);

    /* sort */
    if ((size == 1) || (Len < NumSamples)){
      if (rank == 0) {
        ptSort(TmpPt, Len);
        winnow_pack(pts, npt, TmpPt, Len, 1, 0);
      }
      // broadcast pts
      broadcast (world, pts, npt, 0);
    } else {
      /* sort sections and select P-1 pivot values */
#if defined(TEST_OUTPUT) || defined(TEST_TIME)
      printf ("Started stage 1\n");
#endif
      fflush (stdout);
      winnow_psrs_1 (world);
      if (rank == 0) {
        intSort(Pivots, NumSamples);
      }
      // broadcast Pivots
      broadcast (world, Pivots, NumSamples, 0);

      /* select P-1 pivot values from P*(P-1) pivot values */
      if (rank == 0) {
        for (i = 0, j = ParWidth_1 / 2; j < NumSamples; i++, j += size) {
          Pivots[i] = Pivots[j];
        }
        ASSERT(i == ParWidth_1);
      }
      // broadcast Pivots
      broadcast (world, Pivots, ParWidth_1, 0);

      /* count elements in processor intervals that belong in pivot intervals */
#if defined(TEST_OUTPUT) || defined(TEST_TIME)
      printf ("Started stage 2\n");
#endif
      fflush (stdout);
      winnow_psrs_2(world);
      /* scan number of elements in pivot intervals */
      if (rank == 0) {
        sum = 0;
        for (i = 0; i < size; i++) {
          for (j = 0; j < size * size; j += size) {
            tmp = Counts[i+j];
            Starts[i+j] = sum;
            sum += tmp;
          }
        }
      }
      // broadcast Starts
      broadcast (world, Starts, size * size, 0);

      /* copy values into pivot intervals */
#if defined(TEST_OUTPUT) || defined(TEST_TIME)
      printf ("Started stage 3\n");
#endif
      fflush (stdout);
      winnow_psrs_3(world);
      /* sort pivot intervals */
#if defined(TEST_OUTPUT) || defined(TEST_TIME)
      printf ("Started stage 4\n");
#endif
      fflush (stdout);
      winnow_psrs_4(world);
      /* copy selected points */
      winnow_pack(pts, npt, TmpPt2, Len, size, rank);

      // broadcast pts
      int		stride;			/* stride in source */
      stride = Len / npt;
      for (rank = 0; rank < size; rank++) {
        for (i = (npt - 1) - rank; i >= 0; i -= size) {
          broadcast (world, pts[i], rank);
        }
      }
    }

    delete [] Totals;
    delete [] TmpPt;
    delete [] TmpPt2;

    delete [] Pivots;
    delete [] Counts;
    delete [] Starts;

    delete [] SumProc;
  }
  else {
    pt1DX* ptVec;         // temp point vector
    int r, c, i, j, len;	// indices and number of points
    int stride;           // selection stride

    ptVec = new pt1DX[MAXEXT * MAXEXT];

    // fill temporary vector
    len = winnow_redBool2DCount_mpi(mask, nr, nc);
    ASSERT(len >= npt);
    i = 0;
    for (r=0; r<nr; r++){
      for (c=0; c<nc; c++){
        if (mask[r][c]){
          ptVec[i].x = r;
          ptVec[i].y = c;
          ptVec[i].w = matrix[r][c];
          i++;
        }
      }
    }

    // sort
    ptSort(ptVec, len);

    // copy over points
    stride = len / npt;
    for (i=npt-1, j=len-1; i>=0; i--, j-=stride){
      pts[i] = ptVec[j];
    }
  }

  /* return */
}

// private

/*
 * @ winnow_copy : copy selected points from rows
 * > none
 * + fill tmpPt
 */

void
winnow_copy(mpi::communicator world,
  int2D*		matrix,			/* matrix of values */
  bool2D*	mask,			/* mask on values */
  int		nr,			/* row size */
  int		nc			/* column size */
){
  int		lo, hi;		/* work descriptors */
  int		r, c, i;		/* loop and vector indices */

  /* work */
  if (get_block_rows_mpi (world, 0, nr, &lo, &hi)) {
    for (r = lo; r < hi; r++) {
      i = Totals[r];
      for (c = 0; c < nc; c++) {
        if (mask[r][c]) {
          TmpPt[i].x = r;
          TmpPt[i].y = c;
          TmpPt[i].w = matrix[r][c];
          i++;
        }
      }
    }
  }
  // broadcast TmpPt
  for (r = 0; r < nr - 1; r++) {
    i = get_block_rank_mpi (world, 0, nr, r);
    if (Totals[r + 1] - Totals[r] > 0) {
      broadcast (world, &TmpPt[Totals[r]], Totals[r + 1] - Totals[r], i);
    }
  }
  i = get_block_rank_mpi (world, 0, nr, r);
  if (nr * nc - Totals[r] > 0) {
    broadcast (world, &TmpPt[Totals[r]], nr * nc - Totals[r], i);
  }

  /* return */
}

/*
 * @ winnow_count : count selected points within rows
 * > none
 * + fill totals
 */

void
winnow_count(mpi::communicator world,
  bool2D*	mask,			/* mask on points */
  int		nr,			/* row size */
  int		nc			/* column size */
){
  int		lo, hi;		/* work descriptors */
  int		r, c;			/* loop indices */
  int i;

  /* work */
  if (get_block_rows_mpi (world, 0, nr, &lo, &hi)) {
    for (r = lo; r < hi; r++) {
      Totals[r] = 0;
      for (c = 0; c < nc; c++) {
        if (mask[r][c]) {
          Totals[r] += 1;
        }
      }
    }
  }
  // broadcast Totals
  for (i = 0; i < world.size (); i++) {
    if (get_block_rows_mpi (world, 0, nr, &lo, &hi, i)) {
      broadcast (world, &Totals[lo], hi - lo, i);
    }
  }

  /* return */
}

/*
 * @ winnow_pack : compact points (serially)
 * > none
 * + pack points
 */

void
winnow_pack(
  pt1D*		ptDst,			/* to pack into */
  int		nDst,			/* number of points */
  pt1D*		ptSrc,			/* to pull from */
  int		nSrc,			/* number of tmps */
  int		nt,			/* number of threads */
  int		rank
){
  int		i, j;			/* loop indices */
  int		stride;			/* stride in source */

  stride = nSrc / nDst;
  for (i=(nDst-1)-rank, j=(nSrc-1)-(rank*stride);
       i>=0;
       i-=nt, j-=(stride*nt))
  {
    ptDst[i] = ptSrc[j];
  }
  /* return */
}

/*
 * @ winnow_psrs_1 : first pass of PSRS
 * > none
 * + sort subsections, choose pivot elements
 */

void
winnow_psrs_1(mpi::communicator world
){
  int		lo, hi;			/* work descriptors */
  int		i;			/* indices */
  int		offset;			/* to ensure even sampling */
  int		dex;			/* total index */

  int rank, size;

  rank = world.rank ();
  size = world.size ();

  /* do own scheduling */
  offset = IntervalSize / 2;
  winnow_sched (rank, &lo, &hi);

  /* sort */
  ptSort(TmpPt + lo, hi - lo);

  // broadcast TmpPt
  for (i = 0; i < size; i++) {
    winnow_sched (i, &lo, &hi);
    broadcast (world, &TmpPt[lo], hi - lo, i);
  }

  /* pack pivots */
  for (i = 0; i < ParWidth_1; i++) {
    dex = lo + offset + (i*IntervalSize);
    if (dex < hi){
      Pivots[(rank * ParWidth_1)+i] = TmpPt[dex].w;
    } else {
      Pivots[(rank * ParWidth_1)+i] = TmpPt[hi-1].w;
    }
  }

  // broadcast Pivots
  for (rank = 0; rank < size; rank++) {
    for (i = 0; i < ParWidth_1; i++) {
      winnow_sched (i, &lo, &hi);
      broadcast (world, Pivots[(rank * ParWidth_1)+i], rank);
    }
  }

  /* return */
}

/*
 * @ winnow_psrs_2 : second phase of PSRS
 * > none
 * + count elements in each subsection
 */

void
winnow_psrs_2(mpi::communicator world
){
  int		lo, hi;			/* own interval */
  int		i;			/* index into array */
  int		dexPivot, dexCount;	/* indices into pivots and counts */
  int		limCount;		/* limit on dexCount */
  int rank, size;

  rank = world.rank ();
  size = world.size ();

  /* do own scheduling */
  winnow_sched(rank, &lo, &hi);

  /* count elements */
  dexPivot = 0;
  dexCount = rank * size;
  limCount = (rank + 1) * size;
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

  // broadcast Counts
  for (i = 0; i < size; i++) {
    broadcast (world, &Counts[i * size], size, i);
  }

  /* return */
}

/*
 * @ winnow_psrs_3 : third pass of PSRS
 * > none
 * + copy elements into correct section
 */

void
winnow_psrs_3(mpi::communicator world
){
  int		lo, hi;			/* own source section boundaries */
  int		dexSrc, dexDst;		/* source and destination indices */
  int		dexCtrl;		/* index into control structures */
  int rank, size;
  int i;

  rank = world.rank ();
  size = world.size ();

  /* do own scheduling */
  winnow_sched(rank, &lo, &hi);

  /* work */
  dexSrc = lo;
  for (dexCtrl = rank * size; dexCtrl < (rank + 1) * size; dexCtrl++) {
    for (dexDst = Starts[dexCtrl];
         dexDst < Starts[dexCtrl] + Counts[dexCtrl];
         dexDst++) {
      TmpPt2[dexDst] = TmpPt[dexSrc];
      dexSrc++;
    }
  }
  ASSERT(dexSrc == hi);

  // broadcast TmpPt2
  for (i = 0; i < size; i++) {
    for (dexCtrl = i * size; dexCtrl < (i + 1) * size; dexCtrl++) {
      if (Counts[dexCtrl] > 0) {
        broadcast (world, &TmpPt2[Starts[dexCtrl]], Counts[dexCtrl], i);
      }
    }
  }

  /* return */
}

/*
 * @ winnow_psrs_4 : fourth phase of PSRS
 * > none
 * + sort elements in subsections
 */

void
winnow_psrs_4(mpi::communicator world
){
  int		secBase, secLen;	/* base of own section and its length */
  int rank, size;
  int i;

  rank = world.rank ();
  size = world.size ();

  secBase = Starts[rank];
  if (rank < ParWidth_1){
    secLen = Starts[rank + 1] - secBase;
  } else {
    secLen = Len - secBase;
  }
  ptSort(TmpPt2 + secBase, secLen);

  // broadcast TmpPt2
  for (i = 0; i < size; i++) {
    secBase = Starts[i];
    if (i < ParWidth_1){
      secLen = Starts[i + 1] - secBase;
    } else {
      secLen = Len - secBase;
    }
    if (secLen > 0) {
      broadcast (world, TmpPt2 + secBase, secLen, i);
    }
  }

  /* return */
}
/*
 * @ winnow_sched : do scheduling for PSRS
 * > none
 * + set limit parameters
 */

void
winnow_sched(int rank,
  int	      * start,			/* start of own interval */
  int	      * end			/* end of own interval */
){
  *start = rank * SectionSize;
  *end = (rank + 1) * SectionSize;
  if (*end > Len){
    *end = Len;
  }
  /* return */
}

/*
 * @ scanIntSum : upward exclusive prefix sum of integer vector
 * > overall total
 * + do prefix sum
 */

int
scanIntSum(mpi::communicator world,
  int	      * vec,			/* to sum */
  int		len			/* vector length */
){
  int		i, tmp, sum;		/* for summing */
  int		lo, hi;		/* work controls */
  bool		work;			/* do useful work? */
  int rank, size;
  int blo, bhi;

  rank = world.rank ();
  size = world.size ();

  /* prefix */
  if ((work = get_block_rows_mpi (world, 0, len, &lo, &hi))) {
    SumProc[rank] = 0;
    for (i = lo; i < hi; i++) {
      tmp = vec[i];
      vec[i] = SumProc[rank];
      SumProc[rank] += tmp;
    }
  }
  else {
    SumProc[rank] = 0;
  }
  // broadcast SumProc, vec
  for (i = 0; i < size; i++) {
    broadcast (world, SumProc[i], i);
    if(get_block_rows_mpi (world, 0, len, &blo, &bhi, i)) {
      broadcast (world, &vec[blo], bhi - blo, i);
    }
  }

  /* smear */
  if (rank == 0) {
    SumShared = 0;
    for (i = 0; i < size; i++){
      tmp = SumProc[i];
      SumProc[i] = SumShared;
      SumShared += tmp;
    }
  }
  // broadcast SumShared, SumProc
  broadcast (world, SumProc, size, 0);
  broadcast (world, SumShared, 0);

  /* suffix */
  if (work) {
    if (rank > 0){
      sum = SumProc[rank];
      for (i = lo; i < hi; i++) {
        vec[i] += sum;
      }
    }
  }
  // broadcast vec
  for (i = 1; i < size; i++) {
    if (get_block_rows_mpi (world, 0, len, &blo, &bhi, i)) {
      broadcast (world, &vec[blo], bhi - blo, i);
    }
  }

  return SumShared;
}

int winnow_redBool2DCount_mpi(bool2D* mask, // to reduce
                              int nr,       // row size
                              int nc)       // column size
{
  int		r, c, sum = 0;		/* indices and result */

  for (r=0; r<nr; r++){
    for (c=0; c<nc; c++){
      if (mask[r][c]) sum++;
    }
  }

  return sum;
}
