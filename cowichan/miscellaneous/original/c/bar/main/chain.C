/*==============================================================*/
/* bar/main/chain.c : barrier chained driver			*/
/*==============================================================*/

#if NUMA
MAIN_ENV
#endif
#include "specific.h"

/*--------------------------------------------------------------*/
/* global data (too much to be parameters)			*/
/*--------------------------------------------------------------*/

static char   * stem = NULL;		/* dump filename stem */
static char   * suffix = NULL;		/* dump filename suffix */
static char   * suff2 = NULL;		/* last half of suffix */
static int	nr, nc;			/* integer matrix sizes */
static int	n;			/* square matrix/vector size */
static real	base_x, base_y;		/* base of Mandelbrot */
static real	ext_x, ext_y;		/* extent of Mandelbrot */
static int	limit, seed;		/* randmat controls */
static real	fraction;		/* invperc/thresh filling */
static int	itersLife;		/* life iterations */
static int	itersElastic, relax;	/* elastic controls */
static int2D	i2D;			/* integer matrix */
static bool2D	b2D;			/* boolean matrix */
static pt1D	cities;			/* cities point vector */
static int	n_cities;		/* number of cities */
static pt1D	net;			/* net point vector */
static int	n_net;			/* number of net points */
static real2D	r2D_gauss;		/* real matrix for Gaussian */
static real2D	r2D_sor;		/* real matrix for SOR */
static real1D	r1D_gauss_v;		/* real vector input for Gaussian */
static real1D	r1D_sor_v;		/* real vector input for SOR */
static real1D	r1D_gauss_a;		/* real vector answer for Gaussian */
static real1D	r1D_sor_a;		/* real vector answer for SOR */
static real1D	r1D_gauss_c;		/* real vector check for Gaussian */
static real1D	r1D_sor_c;		/* real vector check for SOR */
static real	tol;			/* SOR tolerance */
static real	realDiff;		/* vector difference */
static bool	doMandel = TRUE;	/* mandel vs. randmat */
static bool	doInvperc = TRUE;	/* invperc vs. thresholding */
static bool	doDump = FALSE;		/* dump intermediate results? */

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
static void
mainBody(
  int		tid			/* own ID */
);
static thr_f	main_thr;

/*--------------------------------------------------------------*/
/* driver							*/
/*--------------------------------------------------------------*/

int
main(
  int		argc,			/* arg count */
  char	      * argv[]			/* arg vector */
){
  static char * context = "main(chain)";
  bool		choicesSet = FALSE;	/* path choices set? */
  int		argd = 1;		/* argument index */

  /* arguments */
#if NUMA
  MAIN_INITENV(,32000000)
  BARINIT(GlobalBar);
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
     case 'p' :
      DataDist = arg_dataDist(context, argc, argv, argd+1, argv[argd]);
      ParWidth = arg_int(context, argc, argv, argd+2, argv[argd]);
      argd += 3;
      break;
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

  sch_init(DataDist);
  thr_grp(main_thr, NULL);

#if IEEE
  ieee_retrospective(stderr);
#endif
#if NUMA
  BARFREE(GlobalBar);
  MAIN_END;
#endif

  return 0;
}

/*--------------------------------------------------------------*/
/* private functions						*/
/*--------------------------------------------------------------*/

/*
 * @ mainBody : body of main
 * > none
 * + do calculations
 */

static void
mainBody(
  int		tid			/* own ID */
){
  static char * context = "main(chain)body";
  /* mandel vs. randmat */
  if (doMandel){
    mandelSetup();
    mandel(tid, i2D, nr, nc, base_x, base_y, ext_x, ext_y);
    mandelTakedown();
    if (doDump){
      if (MASTER(tid)){
	io_wrInt2D(context, mkfname(stem, NULL, suff2, "i2"), i2D, nr, nc);
      }
      thr_bar(tid);
    }
  } else {
    randmat(tid, i2D, nr, nc, limit, seed);
    thr_bar(tid);
    if (doDump){
      if (MASTER(tid)){
	io_wrInt2D(context, mkfname(stem, NULL, suff2, "i2"), i2D, nr, nc);
      }
      thr_bar(tid);
    }
  }

  /* half */
  half(tid, i2D, nr, nc);
  if (doDump){
    if (MASTER(tid)){
      io_wrInt2D(context, mkfname(stem, "h", suff2, "i2"), i2D, nr, nc);
    }
    thr_bar(tid);
  }

  /* invperc vs. thresh */
  if (doInvperc){
    if (MASTER(tid)){
      invperc(i2D, b2D, nr, nc, fraction);
    }
    thr_bar(tid);
    if (doDump){
      if (MASTER(tid)){
	io_wrBool2D(context, mkfname(stem, NULL, suffix, "b2"), b2D, nr, nc);
      }
      thr_bar(tid);
    }
  } else {
    thresh(tid, i2D, b2D, nr, nc, fraction);
    if (doDump){
      if (MASTER(tid)){
	io_wrBool2D(context, mkfname(stem, NULL, suffix, "b2"), b2D, nr, nc);
      }
      thr_bar(tid);
    }
  }

  /* life */
  life(tid, b2D, nr, nc, itersLife);
  if (doDump){
    if (MASTER(tid)){
      io_wrBool2D(context, mkfname(stem, "l", suffix, "b2"), b2D, nr, nc);
    }
    thr_bar(tid);
  }

  /* winnow */
  winnow(tid, i2D, b2D, nr, nc, cities, n_cities);
  if (doDump){
    if (MASTER(tid)){
      io_wrPt1D(context, mkfname(stem, "w", suffix, "p1"), cities, n_cities);
    }
    thr_bar(tid);
  }

  /* norm */
  norm(tid, cities, n_cities);
  if (doDump){
    if (MASTER(tid)){
      io_wrPt1D(context, mkfname(stem, "n", suffix, "p1"), cities, n_cities);
    }
    thr_bar(tid);
  }

  /* elastic */
  if (MASTER(tid)){
    n_net = (int)(ELASTIC_RATIO * n_cities);
    CHECK(n_net <= MAXEXT,
	  fail(context, "too many net points required",
	       "number of net points", "%d", n_net, NULL));
  }
  thr_bar(tid);
  elastic(tid, cities, n_cities, net, n_net, itersElastic, relax);
  if (doDump){
    if (MASTER(tid)){
      io_wrPt1D(context, mkfname(stem, "e", suffix, "p1"), net, n_net);
    }
    thr_bar(tid);
  }

  /* outer */
  if (MASTER(tid)){
    n = n_net;
  }
  thr_bar(tid);
  outer(tid, net, r2D_gauss, r1D_gauss_v, n);
  if (doDump){
    if (MASTER(tid)){
      io_wrReal2D(context, mkfname(stem, "o", suffix, "r2"), r2D_gauss, n, n);
      io_wrReal1D(context, mkfname(stem, "o", suffix, "r1"), r1D_gauss_v, n);
    }
    thr_bar(tid);
  }

  cpReal2D(tid, r2D_gauss, r2D_sor, n, n);
  cpReal1D(tid, r1D_gauss_v, r1D_sor_v, n);

  /* gauss */
  gauss(tid, r2D_gauss, r1D_gauss_v, r1D_gauss_a, n);
  if (doDump){
    if (MASTER(tid)){
      io_wrReal1D(context, mkfname(stem, "g", suffix, "r1"), r1D_gauss_a, n);
    }
    thr_bar(tid);
  }

  /* product (gauss) */
  product(tid, r2D_gauss, r1D_gauss_a, r1D_gauss_c, n, n);
  if (doDump){
    if (MASTER(tid)){
      io_wrReal1D(context, mkfname(stem, "pg", suffix, "r1"), r1D_gauss_c, n);
    }
    thr_bar(tid);
  }

  /* sor */
  sor(tid, r2D_sor, r1D_sor_v, r1D_sor_a, n, tol);
  if (doDump){
    if (MASTER(tid)){
      io_wrReal1D(context, mkfname(stem, "s", suffix, "r1"), r1D_gauss_a, n);
    }
    thr_bar(tid);
  }

  /* product (sor) */
  product(tid, r2D_sor, r1D_sor_a, r1D_sor_c, n, n);
  if (doDump){
    if (MASTER(tid)){
      io_wrReal1D(context, mkfname(stem, "ps", suffix, "r1"), r1D_gauss_c, n);
    }
    thr_bar(tid);
  }

  /* difference */
  vecdiff(tid, r1D_gauss_a, r1D_sor_a, n, &realDiff);
  if (doDump){
    if (MASTER(tid)){
      io_wrReal0D(context, mkfname(stem, "v", suffix, "r0"), realDiff);
    }
    thr_bar(tid);
  }

  /* return */
}

/*
 * @ main_thr : initiate main threading
 * > NULL
 * + initiate threading
 */
  
static THR_DEF
main_thr(
  void	      * argVoid
){
  mainBody(thr_idSet());
  THR_END(argVoid);
}

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

