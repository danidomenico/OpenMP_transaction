/*==============================================================*/
/* bar/app/app.c : barrier support routines			*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*--------------------------------------------------------------*/
/* global variables						*/
/*--------------------------------------------------------------*/

#if NUMA
BARDEC(GlobalBar);
#endif
dataDist_e	DataDist = dataDist_block; /* data/work distribution */
int		ParWidth = MAXPAR;	/* parallel width */

/*--------------------------------------------------------------*/
/* private data structures					*/
/*--------------------------------------------------------------*/

#if SUNOS5
static mutex_t	BarDown[MAXPAR];	/* down-going barrier locks */
static mutex_t	BarUp[MAXPAR];		/* up-going barrier locks */
static thread_key_t	IdKey;		/* key for thread IDs */
static mutex_t	IdMutex;		/* lock on IDs */
static int	IdNext = 0;		/* next ID */
static int	IdVec[MAXPAR];		/* ID values */
static cond_t	LiveCond;		/* cond var on # live */
static mutex_t	LiveMutex;		/* lock on # live */
static int	LiveNum;		/* number of live children */
#endif

/*--------------------------------------------------------------*/
/* public utility functions					*/
/*--------------------------------------------------------------*/

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
 * @ cpReal1D : copy real vector
 * > none
 * + copy vector
 */

void
cpReal1D(
  int		tid,			/* own ID */
  real1D	src,			/* source vector */
  real1D	dst,			/* destination matrix */
  int		n			/* size */
){
  int		lo, hi, str;		/* work controls */
  int		i;			/* loop index */

  ASSERT(src != NULL);
  ASSERT(dst != NULL);

  if ((sch_work(ParWidth, tid, 0, n, &lo, &hi, &str))){
    for (i=lo; i<hi; i+=str){
      dst[i] = src[i];
    }
  }
  thr_bar(tid);

  /* return */
}

/*
 * @ cpReal2D : copy real matrix
 * > none
 * + copy matrix
 */

void
cpReal2D(
  int		tid,			/* own ID */
  real2D	src,			/* source matrix */
  real2D	dst,			/* destination matrix */
  int		nr,			/* row size */
  int		nc			/* column size */
){
  int		lo, hi, str;		/* work controls */
  int		r, c;			/* loop indices */

  ASSERT(src != NULL);
  ASSERT(dst != NULL);

  if ((sch_work(ParWidth, tid, 0, nr, &lo, &hi, &str))){
    for (r=lo; r<hi; r+=str){
      for (c=0; c<nc; c++){
	dst[r][c] = src[r][c];
      }
    }
  }
  thr_bar(tid);

  /* return */
}

/*
 * @ redPt1DPos : find min/max point positions
 * > none
 * + fill arguments
 */

void
redPt1DPos(
  int		tid,			/* own ID */
  pt1D		vec,			/* vector of points */
  int		n,			/* number of points */
  pt	      * ptMin,			/* minimum location */
  pt	      * ptMax			/* maximum location */
){
  static pt	ptMinThr[MAXPAR];	/* minima */
  static pt	ptMaxThr[MAXPAR];	/* maxima */
  int		lo, hi, str;		/* controls */
  int		i;			/* loop index */

  ASSERT(ptMin != NULL);
  ASSERT(ptMax != NULL);

  if (sch_work(ParWidth, tid, 0, n, &lo, &hi, &str)){
    /* partial reduction */
    ptMinThr[tid] = vec[lo];
    ptMaxThr[tid] = vec[lo];
    for (i=lo+str; i<hi; i+=str){
      if (vec[i].x < ptMinThr[tid].x) ptMinThr[tid].x = vec[i].x;
      if (vec[i].x > ptMaxThr[tid].x) ptMaxThr[tid].x = vec[i].x;
      if (vec[i].y < ptMinThr[tid].y) ptMinThr[tid].y = vec[i].y;
      if (vec[i].y > ptMaxThr[tid].y) ptMaxThr[tid].y = vec[i].y;
    }
  } else {
    ptMinThr[tid] = vec[0];
    ptMaxThr[tid] = vec[0];
  }
  thr_bar(tid);

  /* whole reduction */
  if (MASTER(tid)){
    for (i=1; i<ParWidth; i++){
      if (ptMinThr[i].x < ptMinThr[0].x) ptMinThr[0].x = ptMinThr[i].x;
      if (ptMinThr[i].y < ptMinThr[0].y) ptMinThr[0].y = ptMinThr[i].y;
      if (ptMaxThr[i].x > ptMaxThr[0].x) ptMaxThr[0].x = ptMaxThr[i].x;
      if (ptMaxThr[i].y > ptMaxThr[0].y) ptMaxThr[0].y = ptMaxThr[i].y;
    }
  }
  thr_bar(tid);

  *ptMin = ptMinThr[0];
  *ptMax = ptMaxThr[0];

  /* return */
}

/*
 * @ scanIntSum : upward exclusive prefix sum of integer vector
 * > overall total
 * + do prefix sum
 */

int
scanIntSum(
  int		tid,			/* own ID */
  int	      * vec,			/* to sum */
  int		len			/* vector length */
){
  static int	sumThr[MAXPAR];		/* partial sums */
  static int	sumShared;		/* final result */
  int		i, tmp, sum;		/* for summing */
  int		lo, hi, str;		/* work controls */
  bool		work;			/* do useful work? */

  /* prefix */
  if ((work = sch_block(ParWidth, tid, 0, len, &lo, &hi, &str))){
    sumThr[tid] = 0;
    for (i=lo; i<hi; i+=str){
      tmp = vec[i];
      vec[i] = sumThr[tid];
      sumThr[tid] += tmp;
    }
  } else {
    sumThr[tid] = 0;
  }
  thr_bar(tid);

  /* smear */
  if (MASTER(tid)){
    sumShared = 0;
    for (i=0; i<ParWidth; i++){
      tmp = sumThr[i];
      sumThr[i] = sumShared;
      sumShared += tmp;
    }
  }
  thr_bar(tid);

  /* suffix */
  if (work){
    if (tid > 0){
      sum = sumThr[tid];
      for (i=lo; i<hi; i+=str){
	vec[i] += sum;
      }
    }
  }
  thr_bar(tid);

  return sumShared;
}

/*--------------------------------------------------------------*/
/* public threading functions					*/
/*--------------------------------------------------------------*/

/*
 * @ thr_bar : barrier synchronization
 * > none
 * + synchronize callers
 */

void
thr_bar(
  int		tid			/* own ID */
){
#if SUNOS5
  int		tidLeft, tidRight;	/* child IDs */
  int		tidParent;		/* parent's ID */

  /* initialization */
  tidLeft   = 2*tid + 1;
  tidRight  = 2*tid + 2;
  tidParent = tid/2;

  /* upwward */
  if (tidLeft < ParWidth)  ASSERT(mutex_lock(&(BarUp[tidLeft])) == 0);
  if (tidRight < ParWidth) ASSERT(mutex_lock(&(BarUp[tidRight])) == 0);
  if (tid > 0)		   ASSERT(mutex_unlock(&(BarUp[tid])) == 0);

  /* downward */
  if (tid > 0)		   ASSERT(mutex_lock(&(BarDown[tid])) == 0);
  if (tidLeft < ParWidth)  ASSERT(mutex_unlock(&(BarDown[tidLeft])) == 0);
  if (tidRight < ParWidth) ASSERT(mutex_unlock(&(BarDown[tidRight])) == 0);

#elif NUMA

  BARRIER(GlobalBar, ParWidth);

#endif

  /* return */
}

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
  /* decrement live count, and possibly signal root */
  ASSERT(mutex_lock(&LiveMutex) == 0);
  ASSERT(LiveNum > 0);
  LiveNum -= 1;
  if (LiveNum == 0){
    cond_signal(&LiveCond);
  }
  mutex_unlock(&LiveMutex);

  /* exit */
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
 * @ thr_grp : create group of children
 * > none
 * + create children
 */

void
thr_grp(
  thr_f		fxn,			/* function to run */
  void	      * args[]			/* shared function arguments */
){
  int		i;			/* loop index */

  /* argument consistency */
  ASSERT((0 < ParWidth) && (ParWidth <= MAXPAR));
  ASSERT(fxn != NULL);

#if SUNOS5
  /* initialize barrier locks */
  for (i=0; i<ParWidth; i++){
    ASSERT(mutex_init(&(BarDown[i]), USYNC_THREAD, NULL) == 0);
    ASSERT(mutex_lock(&(BarDown[i])) == 0);
    ASSERT(mutex_init(&(BarUp[i]), USYNC_THREAD, NULL) == 0);
    ASSERT(mutex_lock(&(BarUp[i])) == 0);
  }

  /* initialize ID key */
  ASSERT(mutex_init(&IdMutex, USYNC_THREAD, NULL) == 0);
  ASSERT(thr_keycreate(&IdKey, NULL) == 0);
  for (i=0; i<ParWidth; i++){
    IdVec[i] = i;
  }

  /* initialize live child count */
  ASSERT(mutex_init(&LiveMutex, USYNC_THREAD, NULL) == 0);
  ASSERT(cond_init(&LiveCond, USYNC_THREAD, NULL) == 0);
  LiveNum = ParWidth;
#endif

  /* create threads one by one */
  for (i=0; i<ParWidth; i++){
#if SUNOS5
    ASSERT(thr_create(NULL, 0, fxn, (void *)args, THR_DETACHED, NULL) == 0);
#elif NUMA
    CREATEARG(fxn, (void *)args);
#endif
  }

#if SUNOS5
  /* wait for children to complete and tidy up */
  mutex_lock(&LiveMutex);
  while (LiveNum > 0){
    cond_wait(&LiveCond, &LiveMutex);
  }
  mutex_unlock(&LiveMutex);
  for (i=0; i<ParWidth; i++){
    ASSERT(mutex_destroy(&(BarDown[i])) == 0);
    ASSERT(mutex_destroy(&(BarUp[i])) == 0);
  }
  ASSERT(mutex_destroy(&IdMutex) == 0);
#elif NUMA
  WAIT_FOR_END(ParWidth);
#endif

  /* return */
}

/*
 * @ thr_idSet : set own ID
 * > ID
 * + set up ID
 */

int
thr_idSet(
  void
){
  int		i;			/* local index */

#if SUNOS5
  i = fetchInc(&IdMutex, &IdNext);
  ASSERT(thr_setspecific(IdKey, &(IdVec[i])) == 0);
#elif NUMA
  GET_PID(i);
#endif

  return i;
}
