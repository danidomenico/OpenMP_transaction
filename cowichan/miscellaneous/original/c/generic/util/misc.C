/*==============================================================*/
/* generic/util/misc.c : generic miscellaneous utilities	*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "util.h"

/*--------------------------------------------------------------*/
/* private definitions						*/
/*--------------------------------------------------------------*/

#define SWAP_I_2(w_, l_, h_, t_)				\
t_ = w_[l_], w_[l_] = w_[h_], w_[h_] = t_

#define SWAP_I_3(w_, a_, b_, c_, t_)				\
t_ = w_[a_], w_[a_] = w_[b_], w_[b_] = w_[c_], w_[c_] = t_

#define SWAP_PT_2(p_, l_, h_, t_)				\
t_ = p_[l_], p_[l_] = p_[h_], p_[h_] = t_

#define SWAP_PT_3(p_, a_, b_, c_, t_)				\
t_ = p_[a_], p_[a_] = p_[b_], p_[b_] = p_[c_], p_[c_] = t_

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

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
 * @ intSortChk : check that integers are sorted
 * > none
 * + fail if integers not sorted
 */

void
intSortChk(
  int	      * vec,			/* to check */
  int		len			/* length */
){
  int		i;			/* loop index */
  for (i=1; i<len; i++){
    CHECK(vec[i] >= vec[i-1],
	  fail("intSortChk", "vector unordered",
	       "index", "%d", i, NULL));
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

/*
 * @ ptNormChk : check that point vector is normalized
 * > none
 * + fail if any coordinate outside [0..1]x[0..1]
 */

void
ptNormChk(
  pt1D		pt,			/* point vector */
  int		n			/* length */
){
  int		i;			/* loop index */

  for (i=0; i<n; i++){
    CHECK((0.0 <= pt[i].x) && (pt[i].x <= 1.0) &&
	  (0.0 <= pt[i].y) && (pt[i].y <= 1.0),
	  fail("ptNormChk", "unnormalized point",
	       "index", "%d", i, NULL));
  }
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
 * @ ptSortChk : check that points are sorted
 * > none
 * + fail if points not sorted
 */

void
ptSortChk(
  pt	      * ptVec,			/* to check */
  int		len			/* length */
){
  int		i;			/* loop index */
  for (i=1; i<len; i++){
    CHECK(ptCmp(&(ptVec[i]), &(ptVec[i-1])) >= 0,
	  fail("ptSortChk", "vector unordered",
	       "index", "%d", i, NULL));
  }
  /* return */
}

/*
 * @ randStateInit : initialize parallel random state vector
 * > none
 * + fill vector and calculate constants
 */

void
randStateInit(
  int		seed,			/* RNG seed */
  int		width,			/* number of participants */
  int	      * state,			/* per-thread state vector */
  int	      * aPrime,			/* new multiplicative */
  int	      * cPrime			/* new additive value */
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
 * @ redReal1DExt : find extrema in real vector
 * > range
 * + set lo and hi values
 */

real
redReal1DExt(
  real1D	vec,			/* to reduce */
  int		n,			/* size */
  real	      * lo,			/* low value */
  real	      * hi			/* high value */
){
  int		i;			/* loop index */

  ASSERT(lo != NULL);
  ASSERT(hi != NULL);

  *lo = *hi = vec[0];
  for (i=1; i<n; i++){
    if (vec[i] < *lo) *lo = vec[i];
    if (vec[i] > *hi) *hi = vec[i];
  }

  return *hi - *lo;
}

/*
 * @ redReal2DExt : find extrema in real matrix
 * > range
 * + set lo and hi values
 */

real
redReal2DExt(
  real2D	mat,			/* to reduce */
  int		nr,			/* row size */
  int		nc,			/* column size */
  real	      * lo,			/* low value */
  real	      * hi			/* high value */
){
  int		r, c;			/* loop indices */

  ASSERT(lo != NULL);
  ASSERT(hi != NULL);

  *lo = *hi = mat[0][0];
  for (r=0; r<nr; r++){
    for (c=0; c<nc; c++){
      if (mat[r][c] < *lo) *lo = mat[r][c];
      if (mat[r][c] > *hi) *hi = mat[r][c];
    }
  }

  return *hi - *lo;
}
