/*==============================================================*/
/* generic/hdr/generic.h : generic definitions			*/
/*==============================================================*/

/*--------------------------------------------------------------*/
/* miscellaneous definitions					*/
/*--------------------------------------------------------------*/

/* Booleans */

#define TRUE 1
#define FALSE 0

/* applications */

typedef enum {				/* 			       (gfx) */
  app_chain,				/* chained			(+) */
  app_elastic,				/* elastic net			(+) */
  app_gauss,				/* Gaussian elimination		(+) */
  app_half,				/* halving shuffle		(+) */
  app_invperc,				/* invasion percolation		(+) */
  app_life,				/* Game of Life			(+) */
  app_mandel,				/* Mandelbrot Set		(+) */
  app_norm,				/* coordinate normalization	(-) */
  app_outer,				/* outer product		(-) */
  app_product,				/* matrix-vector product	(-) */
  app_randmat,				/* random matrix generation	(+) */
  app_sor,				/* successive over-relaxation	(-) */
  app_thresh,				/* histogram thresholding	(+) */
  app_vecdiff,				/* vector difference		(-) */
  app_winnow,				/* point winnowing		(+) */
  app_any,				/* none of the above		(-) */
  app_e_sz				/* size of enumeration */
} app_e;

/* data (and work) distributions */

typedef enum {
  dataDist_block,			/* blockwise */
  dataDist_cyclic,			/* cyclic */
  dataDist_e_sz				/* size of enumeration */
} dataDist_e;

/* graphics controls */

typedef enum {
  gfxCtrl_off,				/* no graphics */
  gfxCtrl_on,				/* full-speed graphics */
  gfxCtrl_pause,			/* pause per frame */
  gfxCtrl_e_sz				/* size of enumeration */
} gfxCtrl_e;

/*--------------------------------------------------------------*/
/* parameters							*/
/*--------------------------------------------------------------*/

#define ELASTIC_ALPHA  0.20
#define ELASTIC_BETA   2.00
#define ELASTIC_K_INIT 0.15
#define ELASTIC_RADIUS 0.10
#define ELASTIC_RATIO  2.5
#define ELASTIC_RATE   0.98

#define MANDEL_INFINITY 2.0
#define MANDEL_MAX_ITER 150

#define RAND_A  1291
#define RAND_C   917
#define RAND_M 56197

#define SOR_MAX_ITERS 1000000
#define SOR_OMEGA 1.25

/*--------------------------------------------------------------*/
/* functional macros						*/
/*--------------------------------------------------------------*/

/* assertion */

#define ASSERT(cond_)\
  if (!(cond_)){\
    fprintf(stderr, "\tassertion failed in %s at %d\n", __FILE__, __LINE__);\
    exit(1);\
  }

/* check condition and fail if false */

#define CHECK(cond_, action_) if (!(cond_)){action_;}

/* integer ceiling */

#define INT_CEIL(num_, denom_) ((num_ + denom_ - 1) / denom_)

/*--------------------------------------------------------------*/
/* system-dependent initialization and termination		*/
/*--------------------------------------------------------------*/

#if SUNOS5
# define MAIN_INIT() /**/
# define MAIN_TERM() /**/
#endif

/*--------------------------------------------------------------*/
/* global data structures					*/
/*--------------------------------------------------------------*/

extern bool
(*sch_work)(
  int		n,			/* number of threads */
  int		i,			/* this thread's ID */
  int		base,			/* base of loop section */
  int		lim,			/* limit of loop section */
  int	      * start,			/* loop start */
  int	      * end,			/* loop end */
  int	      * stride			/* loop stride */
);

/*--------------------------------------------------------------*/
/* utility function prototypes					*/
/*--------------------------------------------------------------*/

/*
 * arg.c
 */

dataDist_e
arg_dataDist(
  char	      * caller,			/* calling function */
  int		argc,			/* argument count */
  char	      * argv[],			/* argument vector */
  int		argd,			/* current argument index */
  char	      * flag			/* argument flag */
);
real
arg_frac(
  char	      * caller,			/* calling function */
  int		argc,			/* argument count */
  char	      * argv[],			/* argument vector */
  int		argd,			/* current argument index */
  char	      * flag			/* argument flag */
);
gfxCtrl_e
arg_gfxCtrl(
  char	      * caller,			/* calling function */
  int		argc,			/* argument count */
  char	      * argv[],			/* argument vector */
  int		argd,			/* current argument index */
  char	      * flag			/* argument flag */
);
int
arg_int(
  char	      * caller,			/* calling function */
  int		argc,			/* argument count */
  char	      * argv[],			/* argument vector */
  int		argd,			/* current argument index */
  char	      * flag			/* argument flag */
);
real
arg_real(
  char	      * caller,			/* calling function */
  int		argc,			/* argument count */
  char	      * argv[],			/* argument vector */
  int		argd,			/* current argument index */
  char	      * flag			/* argument flag */
);
char *
arg_str(
  char	      * caller,			/* calling function */
  int		argc,			/* argument count */
  char	      * argv[],			/* argument vector */
  int		argd,			/* current argument index */
  char	      * flag			/* argument flag */
);

/*
 * io.c
 */

void
io_init(
  int		formatted		/* use formatted I/O? */
);
extern void (*io_rdBool2D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  bool2D	mask,			/* matrix to read */
  int	      * rptr,			/* row size pointer */
  int	      * cptr			/* column size pointer */
);
extern void (*io_rdInt2D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  int2D		matrix,			/* matrix to read */
  int	      * rptr,			/* row size pointer */
  int	      * cptr			/* column size pointer */
);
extern void (*io_rdPt1D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  pt1D		vector,			/* matrix to read */
  int	      * nptr			/* size pointer */
);
extern void (*io_rdReal0D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  real	      * val			/* value read */
);
extern void (*io_rdReal1D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  real1D	vector,			/* matrix to read */
  int	      * nptr			/* size pointer */
);
extern void (*io_rdReal2D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  real2D	matrix,			/* matrix to read */
  int	      * rptr,			/* row size pointer */
  int	      * cptr			/* column size pointer */
);
extern void (*io_wrBool2D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of output file */
  bool2D	mask,			/* matrix to write */
  int		nr,			/* row size */
  int		nc			/* column size */
);
extern void (*io_wrInt2D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of output file */
  int2D		matrix,			/* matrix to write */
  int		nr,			/* row size */
  int		nc			/* column size */
);
extern void (*io_wrPt1D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of output file */
  pt1D		vector,			/* matrix to write */
  int		n			/* size */
);
extern void (*io_wrReal0D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  real		val			/* value to write */
);
extern void (*io_wrReal1D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of output file */
  real1D	vector,			/* matrix to write */
  int		n			/* size */
);
extern void (*io_wrReal2D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of output file */
  real2D	matrix,			/* matrix to write */
  int		nr,			/* row size */
  int		nc			/* column size */
);

/*
 * misc.c
 */

void
fail(
  char	      * caller,			/* calling function */
  char	      * descrip,		/* error description */
  ...					/* other things to print */
);
void
intSort(
  int	      * vec,			/* to sort */
  int		len			/* length */
);
void
intSortChk(
  int	      * vec,			/* to check */
  int		len			/* length */
);
int
ptCmp(
  pt	      * left,			/* left point */
  pt	      * right			/* right point */
);
real
ptDist(
  pt	      * left,			/* left point */
  pt	      * right			/* right point */
);
real
ptMag(
  pt	      * p			/* point */
);
void
ptNormChk(
  pt	      * ptVec,			/* points to check */
  int		len			/* length of vector */
);
void
ptSort(
  pt	      * ptVec,			/* points to sort */
  int		len			/* length of vectors */
);
void
ptSortChk(
  pt	      * ptVec,			/* points to check */
  int		len			/* length of vectors */
);
void
randStateInit(
  int		seed,			/* RNG seed */
  int		width,			/* number of participants */
  int	      * state,			/* per-thread state vector */
  int	      * aPrime,			/* new multiplicative */
  int	      * cPrime			/* new additive value */
);
real
redReal1DExt(
  real1D	vec,			/* to reduce */
  int		n,			/* size */
  real	      * lo,			/* low value */
  real	      * hi			/* high value */
);
real
redReal2DExt(
  real2D	mat,			/* to reduce */
  int		nr,			/* row size */
  int		nc,			/* column size */
  real	      * lo,			/* low value */
  real	      * hi			/* high value */
);

/*
 * sch.c
 */

bool
sch_block(
  int		n,			/* number of threads */
  int		i,			/* this thread's ID */
  int		base,			/* base of loop section */
  int		lim,			/* limit of loop section */
  int	      * start,			/* loop start */
  int	      * end,			/* loop end */
  int	      * stride			/* loop stride */
);
bool
sch_cyclic(
  int		n,			/* number of threads */
  int		i,			/* this thread's ID */
  int		base,			/* base of loop section */
  int		lim,			/* limit of loop section */
  int	      * start,			/* loop start */
  int	      * end,			/* loop end */
  int	      * stride			/* loop stride */
);
void
sch_init(
  dataDist_e	dd			/* data distribution */
);

/*
 * str.c
 */

char *
str_app2Str(
  app_e		app			/* to convert */
);
char *
str_dataDist2Str(
  dataDist_e	dd			/* to convert */
);
char *
str_gfxCtrl2Str(
  gfxCtrl_e	gc			/* to convert */
);
app_e
str_str2app(
  char	      * str			/* to convert */
);
dataDist_e
str_str2dataDist(
  char	      * str			/* to convert */
);
gfxCtrl_e
str_str2gfxCtrl(
  char	      * str			/* to convert */
);
