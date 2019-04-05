/*==============================================================*/
/* bar/hdr/proto.c : barrier prototypes				*/
/*==============================================================*/

/*--------------------------------------------------------------*/
/* applications							*/
/*--------------------------------------------------------------*/

void
elastic(
  int		tid,			/* own ID */
  pt1D		cities,			/* cities to link */
  int		n_cities,		/* number of cities */
  pt1D		net,			/* net to link with */
  int		n_net,			/* number of net points */
  int		iters,			/* total iterations */
  int		relax			/* relaxation interval */
);
void
gauss(
  int		tid,			/* own ID */
  real2D	matrix,			/* to solve */
  real1D	vector,			/* target vector */
  real1D	answer,			/* solution found */
  int		n			/* size */
);
void
half(
  int		tid,			/* own ID */
  int2D		matrix,			/* to shuffle */
  int		nr,			/* row size */
  int		nc			/* column size */
);
void
invperc(
  int2D		matrix,			/* matrix to invade */
  bool2D	mask,			/* mask to fill */
  int		nr,			/* row size */
  int		nc,			/* column size */
  real		fraction		/* how much to fill */
);
void
life(
  int		tid,			/* own ID */
  bool2D	world,			/* to evolve */
  int		nr,			/* row size */
  int		nc,			/* column size */
  int		iters			/* number of iterations */
);
void
mandel(
  int		tid,			/* own ID */
  int2D		matrix,			/* to fill */
  int		nr,			/* row size */
  int		nc,			/* column size */
  real		base_x,			/* lower left corner */
  real		base_y,			/* lower left corner */
  real		ext_x,			/* extent */
  real		ext_y			/* extent */
);
void
mandelSetup(
  void
);
void
mandelTakedown(
  void
);
void
norm(
  int		tid,			/* own ID */
  pt1D		vec,			/* points to normalize */
  int		n			/* length of vector */
);
void
outer(
  int		tid,			/* own ID */
  pt1D		ptVec,			/* vector of points */
  real2D	matrix,			/* matrix to fill */
  real1D	realVec,		/* vector to fill */
  int		n			/* size */
);
void
product(
  int		tid,			/* own ID */
  real2D	matrix,			/* to multiply by */
  real1D	vector,			/* to be multiplied */
  real1D	result,			/* result of multiply */
  int		nr,			/* row size */
  int		nc			/* column size */
);
void
randmat(
  int		tid,			/* own ID */
  int2D		matrix,			/* to ill */
  int		nr,			/* row size */
  int		nc,			/* column size */
  int		limit,			/* value limit */
  int		seed			/* RNG seed */
);
void
sor(
  int		tid,			/* own ID */
  real2D	matrix,			/* to solve */
  real1D	vector,			/* target vector */
  real1D	answer,			/* solution found */
  int		n,			/* size */
  real		tol			/* tolerance on result */
);
void
thresh(
  int		tid,			/* own ID */
  int2D		matrix,			/* to threshold */
  bool2D	mask,			/* threshold mask */
  int		nr,			/* row size */
  int		nc,			/* column size */
  real		fraction		/* how much to keep */
);
void
vecdiff(
  int		tid,			/* own ID */
  real1D	left,			/* left vector */
  real1D	right,			/* right vector */
  int		n,			/* vector lengths */
  real	      * diff			/* norm-1 difference */
);
void
winnow(
  int		tid,			/* own ID */
  int2D		matrix,			/* point values */
  bool2D	mask,			/* suitable points */
  int		nr,			/* row size */
  int		nc,			/* column size */
  pt1D		pt,			/* points to create */
  int		npt			/* number of points */
);

/*--------------------------------------------------------------*/
/* utilities							*/
/*--------------------------------------------------------------*/

#if SUNOS5
int
fetchInc(
  mutex_t     * mutex,			/* lock */
  int	      * counter			/* counter */
);
#elif NUMA
int
fetchInc(
  MUTEXTYPE	mutex,			/* lock */ 
  int	      * counter			/* counter */
);
#endif
void
cpReal1D(
  int		tid,			/* own ID */
  real1D	src,			/* source vector */
  real1D	dst,			/* destination matrix */
  int		n			/* size */
);
void
cpReal2D(
  int		tid,			/* own ID */
  real2D	src,			/* source matrix */
  real2D	dst,			/* destination matrix */
  int		nr,			/* row size */
  int		nc			/* column size */
);
void
randStateInit(
  int		seed,			/* RNG seed */
  int		width,			/* number of participants */
  int	      * state,			/* per-thread state vector */
  int	      * aPrime,			/* new multiplicative */
  int	      * cPrime			/* new additive value */
);
void
redPt1DPos(
  int		tid,			/* own ID */
  pt1D		vec,			/* vector of points */
  int		n,			/* number of points */
  pt	      * ptMin,			/* minimum location */
  pt	      * ptMax			/* maximum location */
);
int
scanIntSum(
  int		tid,			/* own ID */
  int	      * vec,			/* to sum */
  int		len			/* vector length */
);
void
thr_bar(
  int		tid			/* own ID */
);
#if SUNOS5
void *
thr_end(
  void	      * argsThr[]		/* thread arguments */
);
#elif NUMA
void
thr_end(
  void	      * argsThr[]		/* thread arguments */
);
#endif
void
thr_grp(
  thr_f		fxn,			/* function to run */
  void	      * args[]			/* shared function arguments */
);
int
thr_idGet(
  void
);
int
thr_idSet(
  void
);
