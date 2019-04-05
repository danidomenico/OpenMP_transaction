/**
 * Utility functions
 *
 * \file util.cpp
 * \author Andrew Borzenko
 * \date 01-26-09
 */

#include "../include/main.h"


#define SWAP_I_2(w_, l_, h_, t_)				\
t_ = w_[l_], w_[l_] = w_[h_], w_[h_] = t_

#define SWAP_I_3(w_, a_, b_, c_, t_)				\
t_ = w_[a_], w_[a_] = w_[b_], w_[b_] = w_[c_], w_[c_] = t_ 

#define SWAP_PT_2(p_, l_, h_, t_)				\
t_ = p_[l_], p_[l_] = p_[h_], p_[h_] = t_

#define SWAP_PT_3(p_, a_, b_, c_, t_)				\
t_ = p_[a_], p_[a_] = p_[b_], p_[b_] = p_[c_], p_[c_] = t_ 


/*
 * @ fail : generic failure handler
 * > none (never returns)
 */

void
fail(
  char	      * caller,			/* calling function */
  char	      * descrip,		/* error description */
  ...					/* other things to print */
){
  va_list	stackPtr;		/* stack pointer */
  char	      * msg;			/* error string */
  char	      * fmt;			/* error format */
  int		i_val;			/* integer value */
  real		r_val;			/* real value (NOTE TYPE) */
  char	      * s_val;			/* string value */

  ASSERT(caller  != NULL);
  ASSERT(descrip != NULL);

  fflush(stdout);
  fflush(stderr);
  fprintf(stderr, "ERROR (%s): %s\n", caller, descrip);

  va_start(stackPtr, descrip);
  while ((msg = va_arg(stackPtr, char*)) != NULL){
    if ((fmt = va_arg(stackPtr, char*)) == NULL){
      fprintf(stderr, "fail: null format for %s\n", msg);
      exit(1);
    } else if ((fmt[0] != '%') || (fmt[1] == '\0')){
      fprintf(stderr, "fail: bad format string \"%s\" for %s\n", fmt, msg);
      exit(1);
    } else {
      switch(fmt[1]){
	case 'd' :
	  i_val = va_arg(stackPtr, int);
	  fprintf(stderr, "%s: %d\n", msg, i_val);
	  break;
	case 'r' :
	  r_val = va_arg(stackPtr, real);
	  fprintf(stderr, "%s: ", msg);
	  fprintf(stderr, FMT_REAL_WR, r_val);
	  break;
	case 's' :
	  s_val = va_arg(stackPtr, char*);
	  if (s_val == NULL){
	    fprintf(stderr, "%s: <<NULL>>\n", msg);
	  } else {
	    fprintf(stderr, "%s: \"%s\"\n", msg, s_val);
	  }
	  break;
	case 'x' :
	  i_val = va_arg(stackPtr, int);
	  fprintf(stderr, "%s: %08x\n", msg, i_val);
	  break;
	default :
	  fprintf(stderr, "fail: unrecognized format \"%s\"\n", fmt);
	  exit(1);
      }
      fflush(stderr);
    }
  }
  va_end(stackPtr);
  fflush(stderr);

  exit(1);

  /* return */
}

void print_matrix (bool2D* matrix, int nr, int nc)
{
  int i, j;
  for (i = 0; i < nr; i++)
  {
    for (j = 0; j < nc; j++)
    {
      if (matrix[i][j] == 0) {
        printf ("0");
      }
      else {
        printf ("1");
      }
    }
    printf ("\n");
  }
}

void print_matrix (int2D* matrix, int nr, int nc)
{
  int i, j;
  for (i = 0; i < nr; i++)
  {
    for (j = 0; j < nc; j++)
    {
      printf ("%d\t", matrix[i][j]);
    }
    printf ("\n");
  }
}

void print_matrix (real2D* matrix, int nr, int nc)
{
  int i, j;
  for (i = 0; i < nr; i++)
  {
    for (j = 0; j < nc; j++)
    {
      printf ("%lg\t", matrix[i][j]);
    }
    printf ("\n");
  }
}

void print_matrix (bool1DX* matrix, int nr, int nc)
{
  int i, j;
  for (i = 0; i < nr; i++)
  {
    for (j = 0; j < nc; j++)
    {
      if (matrix[i * nc + j] == 0) {
        printf ("0");
      }
      else {
        printf ("1");
      }
    }
    printf ("\n");
  }
}

void print_matrix (int1DX* matrix, int nr, int nc)
{
  int i, j;
  for (i = 0; i < nr; i++)
  {
    for (j = 0; j < nc; j++)
    {
      printf ("%d\t", matrix[i * nc + j]);
    }
    printf ("\n");
  }
}

void print_matrix (real1DX* matrix, int nr, int nc)
{
  int i, j;
  for (i = 0; i < nr; i++)
  {
    for (j = 0; j < nc; j++)
    {
      printf ("%lg\t", matrix[i * nc + j]);
    }
    printf ("\n");
  }
}

void print_vector (bool1D* vector, int nr)
{
  int i;
  for (i = 0; i < nr; i++)
  {
    if (vector[i] == 0) {
      printf ("0\n");
    }
    else {
      printf ("1\n");
    }
  }
}

void print_vector (int1D* vector, int nr)
{
  int i;
  for (i = 0; i < nr; i++)
  {
    printf ("%d\n", vector[i]);
  }
}

void print_vector (real1D* vector, int nr)
{
  int i;
  for (i = 0; i < nr; i++)
  {
    printf ("%lg\n", vector[i]);
  }
}

void print_vector (pt1D* vector, int nr)
{
  int i;
  for (i = 0; i < nr; i++)
  {
    printf ("(%lg, %lg) = %d\n", vector[i].x, vector[i].y, vector[i].w);
  }
}

void print_points (pt1D* vector, int nr, int limit)
{
  int x, y, i;
  for (x = 0; x < limit; x++) {
    for (y = 0; y < limit; y++) {
      for (i = 0; i < nr; i++) {
        if ((vector[i].x == x) && (vector[i].y == y)) {
          printf ("%d\t", vector[i].w);
          break;
        }
      }
      if (i == nr) {
        printf ("-\t");
      }
    }
    printf ("\n");
  }
}

/**
 * Assign elements to this process
 *
 * \param world [in] Communicator
 * \param lo [in] Low element
 * \param hi [in] High element
 * \param start [out] Start element
 * \param end [out] End element
 * \return Returns whether at least one element is assigned
 */
bool get_block_rows_mpi (mpi::communicator world, int lo, int hi,
                         int* start, int* end)
{
  int size = world.size ();
  int rank = world.rank ();
  
  int nl;	   /* number of elements */
  int num;	 /* number to do */
  int extra; /* spillage */

  nl    = hi - lo;
  num   = nl / size;
  extra = nl % size;

  if ((nl <= 0) || (rank >= nl)) {
    /* do nothing */
    *start = 0;
    *end = -1;
  }
  else {
    /* do share of work */
    if (rank < extra){
      num += 1;
      *start = lo + rank * num;
    } else {
      *start = lo + (extra * (num + 1)) + ((rank - extra) * num);
    }
    *end = *start + num;
  }

  return (*end != -1);
}

/**
 * Assign elements to this process
 *
 * \param world [in] Communicator
 * \param lo [in] Low element
 * \param hi [in] High element
 * \param start [out] Start element
 * \param end [out] End element
 * \param rank [in] Process rank
 * \return Returns whether at least one element is assigned
 */
bool get_block_rows_mpi (mpi::communicator world, int lo, int hi,
                         int* start, int* end, int rank)
{
  int size = world.size ();
  
  int nl;	   /* number of elements */
  int num;	 /* number to do */
  int extra; /* spillage */

  nl    = hi - lo;
  num   = nl / size;
  extra = nl % size;

  if ((nl <= 0) || (rank >= nl)) {
    /* do nothing */
    *start = 0;
    *end = -1;
  }
  else {
    /* do share of work */
    if (rank < extra){
      num += 1;
      *start = lo + rank * num;
    } else {
      *start = lo + (extra * (num + 1)) + ((rank - extra) * num);
    }
    *end = *start + num;
  }

  return (*end != -1);
}

/**
 * Return which process is working on element
 *
 * \param world [in] Communicator
 * \param lo [in] Low element
 * \param hi [in] High element
 * \param element [in] Element
 * \return Returns process number assigned to element
 */
int get_block_rank_mpi (mpi::communicator world, int lo, int hi,
                        int element)
{
  int size = world.size ();
  int rank;

  int nl;	   /* number of elements */
  int num;	 /* number to do */
  int extra; /* spillage */

  nl    = hi - lo;
  num   = nl / size;
  extra = nl % size;

  if (element < lo + extra * (num + 1)) {
    rank = (element - lo) / (num + 1);
  }
  else {
    rank = (element - lo - extra * (num + 1)) / num + extra;
  }

  return rank;
}

/**
 * Assign elements to this process
 *
 * \param world [in] Communicator
 * \param lo [in] Low element
 * \param hi [in] High element
 * \param start [out] Start element
 * \param end [out] End element
 * \param stride [out] Element stride
 * \return Returns whether at least one element is assigned
 */
bool get_cyclic_rows_mpi(mpi::communicator world, int lo, int hi,
                         int* start, int* end, int* stride)
{
  int size = world.size ();
  int rank = world.rank ();
  
  int		nl;			/* number of loops */

  nl = hi - lo;

  if ((nl <= 0) || (rank >= nl)){		/* do nothing */
    *start = 0;
    *end = -1;
    *stride = 1;
  } else {				/* do share of work */
    *start  = lo + rank;
    *end    = hi;
    *stride = size;
  }

  return (*end != -1);
}

/**
 * Return which process is working on element
 *
 * \param world [in] Communicator
 * \param lo [in] Low element
 * \param hi [in] High element
 * \param element [in] Element
 * \return Returns process number assigned to element
 */
int get_cyclic_rank_mpi (mpi::communicator world, int lo, int hi,
                        int element)
{
  int size = world.size ();
  int rank;

  int nl;	   /* number of rows */

  nl    = hi - lo;
  rank = (element - lo) % size;

  return rank;
}

/*
 * @ randStateInit : initialize parallel random state vector
 * > none
 * + fill vector and calculate constants
 */

void
randStateInit(
  unsigned int		seed,			/* RNG seed */
  int		width,			/* number of participants */
  unsigned int	      * state,			/* per-thread state vector */
  unsigned int	      * aPrime,			/* new multiplicative */
  unsigned int	      * cPrime			/* new additive value */
){
  int		i;			/* loop index */

  state[0] = seed % RAND_M;
  *aPrime = RAND_A;
  *cPrime = 1;
  for (i=1; i<width; i++){
    state[i] = (RAND_A * state[i-1] + RAND_C) % RAND_M;
    *cPrime = (*cPrime + *aPrime) % RAND_M;
    *aPrime = (*aPrime * RAND_A) % RAND_M;
  }
  *cPrime = (*cPrime * RAND_C) % RAND_M;

  /* return */
}

/*
 * @ redPt1DPos : find min/max point positions
 * > none
 * + fill arguments
 */

void
redPt1DPos(
  pt1D*		vec,			/* vector of points */
  int		n,			/* number of points */
  pt	      * ptMin,			/* minimum location */
  pt	      * ptMax			/* maximum location */
){
  int		i;

  ASSERT(ptMin != NULL);
  ASSERT(ptMax != NULL);

  ptMin->x = vec[0].x; ptMin->y = vec[0].y;
  ptMax->x = vec[0].x; ptMax->y = vec[0].y;
  for (i=1; i<n; i++){
    if (vec[i].x < ptMin->x) ptMin->x = vec[i].x;
    if (vec[i].x > ptMax->x) ptMax->x = vec[i].x;
    if (vec[i].y < ptMin->y) ptMin->y = vec[i].y;
    if (vec[i].y > ptMax->y) ptMax->y = vec[i].y;
  }
  ptMin->w = 0; ptMax->w = 0;

  /* return */
}

/*
 @ ptSort : sort points by weight
 > none
 + sort points
 */

void
ptSort(
  pt	      * ptVec,			/* points to sort */
  int		len			/* length of vectors */
){
  int		i, j;			/* indices into vectors */
  int		loop;			/* partitioning control */
  pt		pivot;			/* partitioning value (can't be pointer!) */
  pt		tmp;			/* swapping temporary */

  if (len <= 1){
    /* skip */
  } else if (len == 2){
    if (ptCmp(&(ptVec[1]), &(ptVec[0])) < 0){
      SWAP_PT_2(ptVec, 0, 1, tmp);
    }
  } else if (len == 3){
    if (ptCmp(&(ptVec[1]), &(ptVec[0])) < 0){
      if (ptCmp(&(ptVec[0]), &(ptVec[2])) < 0){
	SWAP_PT_2(ptVec, 0, 1, tmp);
      } else if (ptCmp(&(ptVec[1]), &(ptVec[2])) < 0){
	SWAP_PT_3(ptVec, 0, 1, 2, tmp);
      } else {
	SWAP_PT_2(ptVec, 0, 2, tmp);
      }
    } else if (ptCmp(&(ptVec[2]), &(ptVec[0])) < 0){
      SWAP_PT_3(ptVec, 2, 1, 0, tmp);
    } else if (ptCmp(&(ptVec[2]), &(ptVec[1])) < 0){
      SWAP_PT_2(ptVec, 1, 2, tmp);
    }
  } else {
    i = 0;
    j = len - 1;
    pivot = ptVec[(i+j)/2];
    loop = TRUE;
    while (loop){
      while (ptCmp(&(ptVec[i]), &pivot) < 0) i++;
      while (ptCmp(&pivot, &(ptVec[j])) < 0) j--;
      if (i <= j){
	SWAP_PT_2(ptVec, i, j, tmp);
	i++; j--;
      }
      loop = (i <= j);
    }
    if (1 <= j){
      ptSort(ptVec, j+1);
    }
    if (i < len-1){
      ptSort(ptVec+i, len-i);
    }
  }

  /* return */
}

/*
 * @ ptCmp : compare two points (weight, then x, then y)
 * > -1, 0, or 1 (a<b, a==b, a>b)
 */

int
ptCmp(
  pt	      * left,			/* left point */
  pt	      * right			/* right point */
){
  ASSERT(left != NULL);
  ASSERT(right != NULL);
  if		(left->w < right->w)	return -1;
  else if	(left->w > right->w)	return  1;
  else if	(left->x < right->x)	return -1;
  else if	(left->x > right->x)	return  1;
  else if	(left->y < right->y)	return -1;
  else if	(left->y > right->y)	return  1;
  else					return  0;
}

/*
 * @ intSort : sort integers
 * > none
 * + sort vector of integers
 */

void
intSort(
  int	      * vec,			/* to sort */
  int		len			/* length */
){
  int		tmp;			/* for swapping */
  int		pivot;			/* pivot value */
  int		i, j;			/* indices */
  int		loop;			/* loop control */

  if (len <= 1){
    /* skip */
  } else if (len == 2){
    if (vec[1] < vec[0]){
      SWAP_I_2(vec, 0, 1, tmp);
    }
  } else if (len == 3){
    if (vec[1] < vec[0]){
      if (vec[0] < vec[2]){
	SWAP_I_2(vec, 0, 1, tmp);
      } else if (vec[1] < vec[2]){
	SWAP_I_3(vec, 0, 1, 2, tmp);
      } else {
	SWAP_I_2(vec, 0, 2, tmp);
      }
    } else if (vec[2] < vec[0]){
      SWAP_I_3(vec, 2, 1, 0, tmp);
    } else if (vec[2] < vec[1]){
      SWAP_I_2(vec, 1, 2, tmp);
    }
  } else {
    i = 0;
    j = len - 1;
    pivot = vec[(i+j)/2];
    loop = TRUE;
    while (loop){
      while (vec[i] < pivot) i++;
      while (vec[j] > pivot) j--;
      if (i <= j){
	SWAP_I_2(vec, i, j, tmp);
	i++; j--;
      }
      loop = (i <= j);
    }
    if (1 <= j){
      intSort(vec, j+1);
    }
    if (i < len-1){
      intSort(vec+i, len-i);
    }
  }

  /* return */
}

/*
 * @ ptDist : Euclidean distance between two points
 * > distance
 */

real
ptDist(
  pt	      * left,			/* left point */
  pt	      * right			/* right point */
){
  double	dx, dy;			/* components */

  ASSERT(left != NULL);
  ASSERT(right != NULL);

  dx = (double)(left->x - right->x);
  dy = (double)(left->y - right->y);

  return (real)sqrt(dx*dx + dy*dy);
}

/*
 * @ ptMag : distance of point from origin
 * > distance
 */

real
ptMag(
  pt	      * p			/* point */
){
  return (real)sqrt((double)((p->x*p->x) + (p->y*p->y)));
}

INT64 get_ticks ()
{
  INT64 count;
#if defined(WIN32)   // Windows
  if (! QueryPerformanceCounter((LARGE_INTEGER *) &count)) {
    count = GetTickCount (); // ms
  }
#else                // Linux
  tms tm;
  count = times (&tm);
#endif               // end of WIN32/Linux definitions
  return count;
}

INT64 get_freq ()
{
  INT64 freq;
#if defined(WIN32)   // Windows
  if (! QueryPerformanceFrequency((LARGE_INTEGER *) &freq)) {
    freq = 1000; // ms
  }
#else                // Linux
  freq = sysconf (_SC_CLK_TCK);
#endif               // end of WIN32/Linux definitions
  return freq;
}

void print_elapsed_time (INT64 start, INT64 end)
{
  INT64 freq = get_freq ();
  printf ("Elapsed time: %lg seconds\n", ((double) (end - start)) /
                                         ((double) freq));
}
