/*==============================================================*/
/* generic/hdr/proto.h : generic prototypes			*/
/*==============================================================*/

/*--------------------------------------------------------------*/
/* applications							*/
/*--------------------------------------------------------------*/

void
elastic(
  pt1D		cities,			/* cities to link */
  int		n_cities,		/* number of cities */
  pt1D		net,			/* net to link with */
  int		n_net,			/* number of net points */
  int		iters,			/* total iterations */
  int		relax			/* relaxation interval */
);
void
gauss(
  real2D	matrix,			/* to solve */
  real1D	vector,			/* target vector */
  real1D	answer,			/* solution found */
  int		n			/* size */
);
void
half(
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
  bool2D	world,			/* to evolve */
  int		nr,			/* row size */
  int		nc,			/* column size */
  int		iters			/* number of iterations */
);
void
mandel(
  int2D		matrix,			/* to fill */
  int		nr,			/* row size */
  int		nc,			/* column size */
  real		base_x,			/* lower left corner */
  real		base_y,			/* lower left corner */
  real		ext_x,			/* extent */
  real		ext_y			/* extent */
);
void
norm(
  pt1D		vec,			/* points to normalize */
  int		n			/* length of vector */
);
void
outer(
  pt1D		ptVec,			/* vector of points */
  real2D	matrix,			/* matrix to fill */
  real1D	realVec,		/* vector to fill */
  int		n			/* size */
);
void
product(
  real2D	matrix,			/* to multiply by */
  real1D	vector,			/* to be multiplied */
  real1D	result,			/* result of multiply */
  int		nr,			/* row size */
  int		nc			/* column size */
);
void
randmat(
  int2D		matrix,			/* to ill */
  int		nr,			/* row size */
  int		nc,			/* column size */
  int		limit,			/* value limit */
  int		seed			/* RNG seed */
);
void
sor(
  real2D	matrix,			/* to solve */
  real1D	vector,			/* target vector */
  real1D	answer,			/* solution found */
  int		n,			/* size */
  real		tol			/* tolerance on result */
);
void
thresh(
  int2D		matrix,			/* to threshold */
  bool2D	mask,			/* threshold mask */
  int		nr,			/* row size */
  int		nc,			/* column size */
  real		fraction		/* how much to keep */
);
void
vecdiff(
  real1D	left,			/* left vector */
  real1D	right,			/* right vector */
  int		n,			/* vector lengths */
  real	      * diff			/* norm-1 difference */
);
void
winnow(
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

void
cpReal1D(
  real1D	src,			/* source vector */
  real1D	dst,			/* destination matrix */
  int		n			/* size */
);
void
cpReal2D(
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
  pt1D		vec,			/* vector of points */
  int		n,			/* number of points */
  pt	      * ptMin,			/* minimum location */
  pt	      * ptMax			/* maximum location */
);
