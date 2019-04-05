/*==============================================================*/
/* generic/main/chain.c : generic chained driver		*/
/*==============================================================*/

#if NUMA
MAIN_ENV
#endif
#include "specific.h"

/*--------------------------------------------------------------*/
/* private function prototypes					*/
/*--------------------------------------------------------------*/

static char *
mkfname(
  char	      * stem,			/* stem to name */
  char	      * addition,		/* new part of name */
  char	      * sequence,		/* how we got here */
  char	      * suffix			/* file type */
);

/*--------------------------------------------------------------*/
/* driver							*/
/*--------------------------------------------------------------*/

int
main(
  int		argc,			/* arg count */
  char	      * argv[]			/* arg vector */
){
  static char * context = "main(chain)";
  char	      * stem = NULL;		/* dump filename stem */
  char	      * suffix = NULL;		/* dump filename suffix */
  char	      * suff2 = NULL;		/* last half of suffix */
  int		nr, nc;			/* integer matrix sizes */
  int		n;			/* square matrix/vector size */
  real		base_x, base_y;		/* base of Mandelbrot */
  real		ext_x, ext_y;		/* extent of Mandelbrot */
  int		limit, seed;		/* randmat controls */
  real		fraction;		/* invperc/thresh filling */
  int		itersLife;		/* life iterations */
  int		itersElastic, relax;	/* elastic controls */
  int2D		i2D;			/* integer matrix */
  bool2D	b2D;			/* boolean matrix */
  pt1D		cities;			/* cities point vector */
  int		n_cities;		/* number of cities */
  pt1D		net;			/* net point vector */
  int		n_net;			/* number of net points */
  real2D	r2D_gauss;		/* real matrix for Gaussian */
  real2D	r2D_sor;		/* real matrix for SOR */
  real1D	r1D_gauss_v;		/* real vector input for Gaussian */
  real1D	r1D_sor_v;		/* real vector input for SOR */
  real1D	r1D_gauss_a;		/* real vector answer for Gaussian */
  real1D	r1D_sor_a;		/* real vector answer for SOR */
  real1D	r1D_gauss_c;		/* real vector check for Gaussian */
  real1D	r1D_sor_c;		/* real vector check for SOR */
  real		tol;			/* SOR tolerance */
  real		realDiff;		/* vector difference */
  bool		choicesSet = FALSE;	/* path choices set? */
  bool		doMandel = TRUE;	/* mandel vs. randmat */
  bool		doInvperc = TRUE;	/* invperc vs. thresholding */
  bool		doDump = FALSE;		/* dump intermediate results? */
  int		argd = 1;		/* argument index */

  /* arguments */
#if NUMA
  MAIN_INITENV(,32000000)
#endif
  while (argd < argc){
    CHECK(argv[argd][0] == '-',
	  fail(context, "bad argument", "index", "%d", argd, NULL));
    switch(argv[argd][1]){
     case 'E' :				/* elastic */
      itersElastic = arg_int(context, argc, argv, argd+1, argv[argd]);
      relax = arg_int(context, argc, argv, argd+2, argv[argd]);
      argd += 3;
      break;
     case 'F' :				/* fraction (invperc/thresh) */
      fraction = arg_real(context, argc, argv, argd+1, argv[argd]);
      argd += 2;
      break;
     case 'L' :				/* life */
      itersLife = arg_int(context, argc, argv, argd+1, argv[argd]);
      argd += 2;
      break;
     case 'M' :				/* mandel */
      base_x = arg_real(context, argc, argv, argd+1, argv[argd]);
      base_y = arg_real(context, argc, argv, argd+2, argv[argd]);
      ext_x  = arg_real(context, argc, argv, argd+3, argv[argd]);
      ext_y  = arg_real(context, argc, argv, argd+4, argv[argd]);
      argd += 5;
      break;
     case 'N' :				/* winnow */
      n_cities = arg_int(context, argc, argv, argd+1, argv[argd]);
      argd += 2;
      break;
     case 'R' :				/* randmat */
      limit = arg_int(context, argc, argv, argd+1, argv[argd]);
      seed  = arg_int(context, argc, argv, argd+2, argv[argd]);
      argd += 3;
      break;
     case 'S' :				/* matrix size */
      nr = arg_int(context, argc, argv, argd+1, argv[argd]);
      nc = arg_int(context, argc, argv, argd+2, argv[argd]);
      argd += 3;
      break;
     case 'T' :				/* SOR tolerance */
      tol = arg_real(context, argc, argv, argd+1, argv[argd]);
      argd += 2;
      break;
     case 'c' :				/* choice */
      CHECK(!choicesSet,
	    fail(context, "choices already set", NULL));
      suffix = arg_str(context, argc, argv, argd+1, argv[argd]);
      argd += 2;
      switch(suffix[0]){
       case 'i' :	doInvperc = TRUE;	break;
       case 't' :	doInvperc = FALSE;	break;
       default :
	fail(context, "unknown choice(s)", "choice", "%s", suffix, NULL);
      }
      switch(suffix[1]){
       case 'm' :	doMandel = TRUE;	break;
       case 'r' :	doMandel = FALSE;	break;
       default :
	fail(context, "unknown choice(s)", "choice", "%s", suffix, NULL);
      }
      suff2 = suffix+1;
      choicesSet = TRUE;
      break;
     case 'd' :				/* dump */
      doDump = TRUE;
      argd += 1;
      if ((argd < argc) && (argv[argd][0] != '-')){
        stem = arg_str(context, argc, argv, argd, argv[argd-1]);
        argd += 1;
      }
      break;
#if GRAPHICS
     case 'g' :
      gfx_open(app_chain, arg_gfxCtrl(context, argc, argv, argd+1, argv[argd]));
      argd += 2;
      break;
#endif
#if MIMD
     case 'p' :
      DataDist = arg_dataDist(context, argc, argv, argd+1, argv[argd]);
      ParWidth = arg_int(context, argc, argv, argd+2, argv[argd]);
      argd += 3;
      break;
#endif
     case 'u' :
      io_init(FALSE);
      argd += 1;
      break;
     default :
      fail(context, "unknown flag", "flag", "%s", argv[argd], NULL);
      break;
    }
  }
  CHECK(choicesSet,
	fail("context", "choices not set using -c flag", NULL));

  /* initialize */
#if MIMD
  sch_init(DataDist);
#endif

  /* mandel vs. randmat */
  if (doMandel){
    mandel(i2D, nr, nc, base_x, base_y, ext_x, ext_y);
    if (doDump) io_wrInt2D(context, mkfname(stem, NULL, suff2, "i2"), i2D, nr, nc);
  } else {
    randmat(i2D, nr, nc, limit, seed);
    if (doDump) io_wrInt2D(context, mkfname(stem, NULL, suff2, "i2"), i2D, nr, nc);
  }

  /* half */
  half(i2D, nr, nc);
  if (doDump) io_wrInt2D(context, mkfname(stem, "h", suff2, "i2"), i2D, nr, nc);

  /* invperc vs. thresh */
  if (doInvperc){
    invperc(i2D, b2D, nr, nc, fraction);
    if (doDump) io_wrBool2D(context, mkfname(stem, NULL, suffix, "b2"), b2D, nr, nc);
  } else {
    thresh(i2D, b2D, nr, nc, fraction);
    if (doDump) io_wrBool2D(context, mkfname(stem, NULL, suffix, "b2"), b2D, nr, nc);
  }

  /* life */
  life(b2D, nr, nc, itersLife);
  if (doDump) io_wrBool2D(context, mkfname(stem, "l", suffix, "b2"), b2D, nr, nc);

  /* winnow */
  winnow(i2D, b2D, nr, nc, cities, n_cities);
  if (doDump) io_wrPt1D(context, mkfname(stem, "w", suffix, "p1"), cities, n_cities);

  /* norm */
  norm(cities, n_cities);
  if (doDump) io_wrPt1D(context, mkfname(stem, "n", suffix, "p1"), cities, n_cities);

  /* elastic */
  n_net = (int)(ELASTIC_RATIO * n_cities);
  CHECK(n_net <= MAXEXT,
	fail(context, "too many net points required",
	     "number of net points", "%d", n_net, NULL));
  elastic(cities, n_cities, net, n_net, itersElastic, relax);
  if (doDump) io_wrPt1D(context, mkfname(stem, "e", suffix, "p1"), net, n_net);

  /* outer */
  n = n_net;
  outer(net, r2D_gauss, r1D_gauss_v, n);
  if (doDump){
    io_wrReal2D(context, mkfname(stem, "o", suffix, "r2"), r2D_gauss, n, n);
    io_wrReal1D(context, mkfname(stem, "o", suffix, "r1"), r1D_gauss_v, n);
  }

  cpReal2D(r2D_gauss, r2D_sor, n, n);
  cpReal1D(r1D_gauss_v, r1D_sor_v, n);

  /* gauss */
  gauss(r2D_gauss, r1D_gauss_v, r1D_gauss_a, n);
  if (doDump) io_wrReal1D(context, mkfname(stem, "g", suffix, "r1"), r1D_gauss_a, n);

  /* product (gauss) */
  product(r2D_gauss, r1D_gauss_a, r1D_gauss_c, n, n);
  if (doDump) io_wrReal1D(context, mkfname(stem, "pg", suffix, "r1"), r1D_gauss_c, n);

  /* sor */
  sor(r2D_sor, r1D_sor_v, r1D_sor_a, n, tol);
  if (doDump) io_wrReal1D(context, mkfname(stem, "s", suffix, "r1"), r1D_gauss_a, n);

  /* product (sor) */
  product(r2D_sor, r1D_sor_a, r1D_sor_c, n, n);
  if (doDump) io_wrReal1D(context, mkfname(stem, "ps", suffix, "r1"), r1D_gauss_c, n);

  /* difference */
  vecdiff(r1D_gauss_a, r1D_sor_a, n, &realDiff);
  if (doDump) io_wrReal0D(context, mkfname(stem, "v", suffix, "r0"), realDiff);

#if IEEE
  ieee_retrospective(stderr);
#endif
#if NUMA
  MAIN_END;
#endif

  return 0;
}

/*--------------------------------------------------------------*/
/* private functions						*/
/*--------------------------------------------------------------*/

/*
 * @ mkfname : make filename (or NULL if stem is NULL)
 * > filename or NULL
 */

static char *
mkfname(
  char	      * stem,			/* stem to name */
  char	      * addition,		/* new part of name */
  char	      * sequence,		/* how we got here */
  char	      * suffix			/* file type */
){
  char	      * fn = NULL;		/* filename created */
  int		len = 1;		/* length of new filename */

  ASSERT(stem != NULL);
  len += strlen(stem);
  if (addition != NULL){
    len += strlen(addition);
  }
  ASSERT(sequence != NULL);
  len += strlen(sequence);
  ASSERT(suffix != NULL);
  len += strlen(suffix) + 1;

  if ((fn = (char *)malloc(len)) == NULL){
    fail("mkfname", "malloc failed", NULL);
  }
  strcpy(fn, stem);
  if (addition != NULL) strcat(fn, addition);
  strcat(fn, sequence);
  strcat(fn, ".");
  strcat(fn, suffix);

  return fn;
}
