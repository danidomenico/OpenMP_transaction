/*==============================================================*/
/* bar/gauss.c : barrier gauss driver				*/
/*==============================================================*/

#if NUMA
MAIN_ENV
#endif
#include "specific.h"

static thr_f	gauss_thr;

int
main(
  int		argc,			/* arg count */
  char	      * argv[]			/* arg vector */
){
  static char * context = "main(gauss)";
  real2D	matrix;			/* matrix to solve */
  real1D	vector, answer;		/* input and answer vectors */
  int		n, nr, nc;		/* sizes */
  char	      * infnMat = NULL;		/* matrix file name */
  char	      * infnVec = NULL;		/* vector file name */
  char	      * outfn = NULL;		/* output file name */
  int		argd = 1;		/* argument index */
  void	      * args[4];

  /* arguments */
#if NUMA
  MAIN_INITENV(,32000000)
  BARINIT(GlobalBar);
#endif
  while (argd < argc){
    CHECK(argv[argd][0] == '-',
	  fail(context, "bad argument", "index", "%d", argd, NULL));
    switch(argv[argd][1]){
#if GRAPHICS
     case 'g' :
      gfx_open(app_gauss, arg_gfxCtrl(context, argc, argv, argd+1, argv[argd]));
      argd += 2;
      break;
#endif
     case 'p' :
      DataDist = arg_dataDist(context, argc, argv, argd+1, argv[argd]);
      ParWidth = arg_int(context, argc, argv, argd+2, argv[argd]);
      argd += 3;
      break;
     case 'i' :
      infnMat = arg_str(context, argc, argv, argd+1, argv[argd]);
      infnVec = arg_str(context, argc, argv, argd+2, argv[argd]);
      argd += 3;
      break;
     case 'o' :
      outfn = arg_str(context, argc, argv, argd+1, argv[argd]);
      argd += 2;
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

  /* setup */
  sch_init(DataDist);
  io_rdReal2D(context, infnMat, matrix, &nr, &nc);
  CHECK(nr == nc,
	fail(context, "non-square matrix",
	     "file name", "%s", infnMat, NULL));
  io_rdReal1D(context, infnVec, vector, &n);
  CHECK(n == nr,
	fail(context, "vector size does not match matrix size",
	     "matrix file name", "%s", infnMat,
	     "vector file name", "%s", infnVec, NULL));

  /* run */
  TP_any(args, 0, matrix);
  TP_any(args, 1, vector);
  TP_any(args, 2, answer);
  TP_any(args, 3, n);
  thr_grp(gauss_thr, args);

  /* takedown */
  io_wrReal1D(context, outfn, answer, n);

#if GRAPHICS
  gfx_close();
#endif
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
/* threading functions						*/
/*--------------------------------------------------------------*/

/*
 * @ gauss_thr : threaded Gaussian elimination
 * > NULL
 * + update matrix and vector
 */

static THR_DEF
gauss_thr(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  real2D      * matrix;			/* to solve */
  real1D      * vector;			/* target vector */
  real1D      * answer;			/* solution found */
  int		n;			/* size */
  int		tid;			/* ID */

  /* setup */
  matrix = TG_real2D(argsThr, 0);
  vector = TG_real1D(argsThr, 1);
  answer = TG_real1D(argsThr, 2);
  n      = TG_int(argsThr, 3);
  tid    = thr_idSet();

  gauss(tid, *matrix, *vector, *answer, n);

  THR_END(argsThr);
}
