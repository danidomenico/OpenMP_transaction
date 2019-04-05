/*==============================================================*/
/* fj/app/app.c : forkjoin support routines			*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*--------------------------------------------------------------*/
/* global variables						*/
/*--------------------------------------------------------------*/

dataDist_e	DataDist = dataDist_block; /* data/work distribution */
int		ParWidth = MAXPAR;	/* parallel width */

/*--------------------------------------------------------------*/
/* private function prototypes					*/
/*--------------------------------------------------------------*/

static thr_f	cpReal1D_thr;
static thr_f	cpReal2D_thr;
static thr_f	redPt1DPos_thr;
static thr_f	scanIntSum_thr_prefix;
static thr_f	scanIntSum_thr_suffix;

/*--------------------------------------------------------------*/
/* public utility functions					*/
/*--------------------------------------------------------------*/

/*
 * @ cpReal1D : copy real vector
 * > none
 * + copy vector
 */

void
cpReal1D(
  real1D	src,			/* source vector */
  real1D	dst,			/* destination matrix */
  int		n			/* size */
){
  void	      * args[3];

  ASSERT(src != NULL);
  ASSERT(dst != NULL);

  TP_any(args, 0, src);
  TP_any(args, 1, dst);
  TP_any(args, 2, n);
  thr_grp(cpReal1D_thr, ParWidth, args);

  /* return */
}

/*
 * @ cpReal2D : copy real matrix
 * > none
 * + copy matrix
 */

void
cpReal2D(
  real2D	src,			/* source matrix */
  real2D	dst,			/* destination matrix */
  int		nr,			/* row size */
  int		nc			/* column size */
){
  void	      * args[4];

  ASSERT(src != NULL);
  ASSERT(dst != NULL);

  TP_any(args, 0, src);
  TP_any(args, 1, dst);
  TP_any(args, 2, nr);
  TP_any(args, 3, nc);
  thr_grp(cpReal2D_thr, ParWidth, args);

  /* return */
}

/*
 * @ fetchInc : fetch and increment by unity
 * > old value
 * + increment counter
 */

#if SUNOS5
int
fetchInc(
  mutex_t     * mutex,			/* lock */
  int	      * counter			/* counter */
){
  int		result;			/* value returned */

  ASSERT(mutex != NULL);
  ASSERT(counter != NULL);

  mutex_lock(mutex);
  result = *counter;
  *counter += 1;
  mutex_unlock(mutex);

  return result;
}
#elif NUMA
int
fetchInc(
  MUTEXTYPE	mutex,			/* lock */ 
  int	      * counter			/* counter */
){
  int		result;			/* value returned */

  ASSERT(counter != NULL);

  MUTEXLOCK(mutex);
  result = *counter;
  *counter += 1;
  MUTEXUNLOCK(mutex);

  return result;
}
#endif

/*
 * @ redPt1DPos : find min/max point positions
 * > none
 * + fill arguments
 */

void
redPt1DPos(
  pt1D		vec,			/* vector of points */
  int		n,			/* number of points */
  pt	      * ptMin,			/* minimum location */
  pt	      * ptMax			/* maximum location */
){
  void	      * args[4];
  pt		ptMinThr[MAXPAR];	/* per-thread minima */
  pt		ptMaxThr[MAXPAR];	/* per-thread maxima */
  int		i;			/* loop index */

  ASSERT(ptMin != NULL);
  ASSERT(ptMax != NULL);

  TP_any(args, 0, vec);
  TP_any(args, 1, n);
  TP_any(args, 2, ptMinThr);
  TP_any(args, 3, ptMaxThr);
  thr_grp(redPt1DPos_thr, ParWidth, args);

  *ptMin = ptMinThr[0];
  *ptMax = ptMaxThr[0];
  for (i=1; i<ParWidth; i++){
    if (ptMinThr[i].x < ptMin->x) ptMin->x = ptMinThr[i].x;
    if (ptMaxThr[i].x > ptMax->x) ptMax->x = ptMaxThr[i].x;
    if (ptMinThr[i].y < ptMin->y) ptMin->y = ptMinThr[i].y;
    if (ptMaxThr[i].y > ptMax->y) ptMax->y = ptMaxThr[i].y;
  }

  /* return */
}

/*
 * @ scanIntSum : upward exclusive prefix sum of integer vector
 * > overall total
 * + do prefix sum
 */

int
scanIntSum(
  int	      * vec,			/* to sum */
  int		len			/* vector length */
){
  void	      * args[3];
  int		tmpThr[MAXPAR];		/* temporary limits */
  int		i, tmp, sum;		/* intermediate sum */

  TP_any(args, 0, vec);
  TP_any(args, 1, len);
  TP_any(args, 2, tmpThr);

  thr_grp(scanIntSum_thr_prefix, ParWidth, args);

  sum = 0;
  for (i=0; i<ParWidth; i++){
    tmp = tmpThr[i];
    tmpThr[i] = sum;
    sum += tmp;
  }

  thr_grp(scanIntSum_thr_suffix, ParWidth, args);

  return sum;
}

/*--------------------------------------------------------------*/
/* public threading functions					*/
/*--------------------------------------------------------------*/

/*
 * @ thr_end : exit thread (from child's point of view)
 * > none
 * + exit thread
 */

#if SUNOS5
void *
thr_end(
  void	      * argsThr[]		/* thread arguments */
){
  ASSERT(argsThr != NULL);
  ASSERT(argsThr[TA_LIVE] != NULL);

  ASSERT(argsThr[TA_MUTEX] != NULL);
  mutex_lock((mutex_t *)argsThr[TA_MUTEX]);

  ASSERT(*((int *)argsThr[TA_LIVE]) > 0);
  *((int *)argsThr[TA_LIVE]) -= 1;

  if (*((int *)argsThr[TA_LIVE]) == 0){
    ASSERT(argsThr[TA_COND] != NULL);
    cond_signal((cond_t *)argsThr[TA_COND]);
  }

  mutex_unlock((mutex_t *)argsThr[TA_MUTEX]);

  thr_exit(NULL);			/* real exit point */
  return NULL;				/* shouldn't ever be executed */
}
#elif NUMA
void
thr_end(
  void	      * argsThr[]		/* thread arguments */
){
  /* empty */
}
#endif

/*
 * @ thr_grp : fork-join group of children
 * > none
 * + create children
 */

void
thr_grp(
  thr_f		fxn,			/* function to run */
  int		n,			/* number to create */
  void	      * argsFxn[]		/* shared function arguments */
){
  void	      * argsThr[MAXPAR][TA_SZ];	/* thread arguments */
  int		i;			/* loop index */
#if SUNOS5
  int		live;			/* number of live threads */
  mutex_t	mutex;			/* lock on counts */
  cond_t	cond;			/* conditional lock */
#elif NUMA
#endif

  /* argument consistency */
  ASSERT((0 < n) && (n <= MAXPAR));
  ASSERT(fxn != NULL);

#if SUNOS5
  /* initialize mutex and condition variable, and count of live threads */
  ASSERT(mutex_init(&mutex, USYNC_THREAD, NULL) == 0);
  ASSERT(cond_init(&cond, USYNC_THREAD, 0) == 0);
  live = n;
#elif NUMA
#endif

  /* create threads one by one */
  for (i=0; i<n; i++){
    argsThr[i][TA_ID]    = (void *)i;
    argsThr[i][TA_N]     = (void *)n;
    argsThr[i][TA_USR]   = argsFxn;
#if SUNOS5
    argsThr[i][TA_LIVE]  = (void *)&live;
    argsThr[i][TA_MUTEX] = &mutex;
    argsThr[i][TA_COND]  = &cond;
#elif NUMA
#endif
#if SUNOS5
    ASSERT(thr_create(NULL, 0, fxn, (void *)(argsThr[i]), THR_DETACHED, NULL) == 0);
#elif NUMA
    CREATEARG(fxn, (void *)(argsThr[i]));
#endif
  }

#if SUNOS5
  /* wait for children to complete, then tidy up */
  mutex_lock(&mutex);
  while (live > 0){
    cond_wait(&cond, &mutex);
  }
  mutex_unlock(&mutex);
  mutex_destroy(&mutex);
  cond_destroy(&cond);
#elif NUMA
  /* wait on children */
  WAIT_FOR_END(n);
#endif

  /* return */
}

/*--------------------------------------------------------------*/
/* private threading functions					*/
/*--------------------------------------------------------------*/

/*
 * @ cpReal1D_thr : threaded real vector copy
 * > NULL
 * + copy vector elements
 */

static THR_DEF
cpReal1D_thr(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  real1D      * dst;			/* target vector */
  real1D      * src;			/* source vector */
  int		n;			/* size */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		i;			/* loop index */

  /* setup */
  src = TG_real1D(argsThr, 0);
  dst = TG_real1D(argsThr, 1);
  n   = TG_int(argsThr, 2);
  tid = TA_get_id(argsThr);
  nt  = TA_get_n(argsThr);

  /* work */
  if (sch_work(nt, tid, 0, n, &lo, &hi, &str)){
    for (i=lo; i<hi; i+=str){
      (*dst)[i] = (*src)[i];
    }
  }

  THR_END(argsThr);
}

/*
 * @ cpReal2D_thr : threaded real matrix copy
 * > NULL
 * + copy matrix elements
 */

static THR_DEF
cpReal2D_thr(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  real2D      * dst;			/* target vector */
  real2D      * src;			/* source vector */
  int		nr, nc;			/* sizes */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		r, c;			/* loop indices */

  /* setup */
  src = TG_real2D(argsThr, 0);
  dst = TG_real2D(argsThr, 1);
  nr  = TG_int(argsThr, 2);
  nc  = TG_int(argsThr, 3);
  tid = TA_get_id(argsThr);
  nt  = TA_get_n(argsThr);

  /* work */
  if (sch_work(nt, tid, 0, nr, &lo, &hi, &str)){
    for (r=lo; r<hi; r+=str){
      for (c=0; c<nc; c++){
	(*dst)[r][c] = (*src)[r][c];
      }
    }
  }

  THR_END(argsThr);
}

/*
 * @ redPt1DPos_thr : threaded point location reduction
 * > NULL
 * + reduce positions
 */

static THR_DEF
redPt1DPos_thr(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  pt1D	      * vec;			/* vector of points */
  int		n;			/* number of points */
  pt	      * ptMinThr;		/* per-thread minima */
  pt	      * ptMaxThr;		/* per-thread maxima */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		i;			/* loop index */

  /* setup */
  vec      = TG_pt1D(argsThr, 0);
  n        = TG_int(argsThr, 1);
  ptMinThr = (pt *)TG_void_p(argsThr, 2);
  ptMaxThr = (pt *)TG_void_p(argsThr, 3);
  tid      = TA_get_id(argsThr);
  nt       = TA_get_n(argsThr);

  /* work */
  if (sch_work(nt, tid, 0, n, &lo, &hi, &str)){
    ptMinThr[tid] = (*vec)[lo];
    ptMaxThr[tid] = (*vec)[lo];
    for (i=lo+str; i<hi; i+=str){
      if ((*vec)[i].x < ptMinThr[tid].x) ptMinThr[tid].x = (*vec)[i].x;
      if ((*vec)[i].x > ptMaxThr[tid].x) ptMaxThr[tid].x = (*vec)[i].x;
      if ((*vec)[i].y < ptMinThr[tid].y) ptMinThr[tid].y = (*vec)[i].y;
      if ((*vec)[i].y > ptMaxThr[tid].y) ptMaxThr[tid].y = (*vec)[i].y;
    }
  } else {
    ptMinThr[tid] = (*vec)[0];
    ptMaxThr[tid] = (*vec)[0];
  }

  THR_END(argsThr);
}

/*
 * @ scanIntSum_thr_prefix : first pass of parallel prefix
 * > NULL
 * + calculate partial sum
 */

static THR_DEF
scanIntSum_thr_prefix(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  int1D	      * vec;			/* vector to scan */
  int		n;			/* vector length */
  int	      * tmpThr;			/* partial sums */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		i;			/* for summing */

  /* setup */
  vec    = TG_int1D(argsThr, 0);
  n      = TG_int(argsThr, 1);
  tmpThr = (int *)TG_void_p(argsThr, 2);
  tid    = TA_get_id(argsThr);
  nt     = TA_get_n(argsThr);

  /* work */
  if (sch_block(nt, tid, 0, n, &lo, &hi, &str)){
    ASSERT(str == 1);
    tmpThr[tid] = (*vec)[lo];
    for (i=lo+str; i<hi; i+=str){
      tmpThr[tid] += (*vec)[i];
    }
  } else {
    tmpThr[tid] = 0;
  }

  THR_END(argsThr);
}

/*
 * @ scanIntSum_thr_suffix : suffix pass of parallel suffix
 * > NULL
 * + calculate partial sums
 */

static THR_DEF
scanIntSum_thr_suffix(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  int1D	      * vec;			/* vector to scan */
  int		n;			/* vector length */
  int	      * tmpThr;			/* partial sums */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		i, tmp, sum;		/* for summing */

  /* setup */
  vec    = TG_int1D(argsThr, 0);
  n      = TG_int(argsThr, 1);
  tmpThr = (int *)TG_void_p(argsThr, 2);
  tid    = TA_get_id(argsThr);
  nt     = TA_get_n(argsThr);

  /* work */
  if (sch_block(nt, tid, 0, n, &lo, &hi, &str)){
    ASSERT(str == 1);
    sum = tmpThr[tid];
    for (i=lo; i<hi; i+=str){
      tmp = (*vec)[i];
      (*vec)[i] = sum;
      sum += tmp;
    }
  }

  THR_END(argsThr);
}
