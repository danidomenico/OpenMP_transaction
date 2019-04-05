/*==============================================================*/
/* bar/outer.c : barrier outer driver				*/
/*==============================================================*/

#if NUMA
MAIN_ENV
#endif
#include "specific.h"

static thr_f	outer_thr;

int
main(
  int		argc,			/* arg count */
  char	      * argv[]			/* arg vector */
){
  static char * context = "main(outer)";
  pt1D		ptVec;			/* point vector */
  real2D	matrix;			/* matrix to create */
  real1D	realVec;		/* vector to create */
  int		n;			/* size */
  char	      * infn = NULL;		/* input file name */
  char	      * outfnMat = NULL;	/* output matrix file name */
  char	      * outfnVec = NULL;	/* output vector file name */
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
      gfx_open(app_outer, arg_gfxCtrl(context, argc, argv, argd+1, argv[argd]));
      argd += 2;
      break;
#endif
     case 'p' :
      DataDist = arg_dataDist(context, argc, argv, argd+1, argv[argd]);
      ParWidth = arg_int(context, argc, argv, argd+2, argv[argd]);
      argd += 3;
      break;
     case 'i' :
      infn = arg_str(context, argc, argv, argd+1, argv[argd]);
      argd += 2;
      break;
     case 'o' :
      outfnMat = arg_str(context, argc, argv, argd+1, argv[argd]);
      outfnVec = arg_str(context, argc, argv, argd+2, argv[argd]);
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

  /* setup */
  sch_init(DataDist);
  io_rdPt1D(context, infn, ptVec, &n);

  /* run */
  TP_any(args, 0, ptVec);
  TP_any(args, 1, matrix);
  TP_any(args, 2, realVec);
  TP_any(args, 3, n);
  thr_grp(outer_thr, args);

  /* takedown */
  io_wrReal2D(context, outfnMat, matrix, n, n);
  io_wrReal1D(context, outfnVec, realVec, n);

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
 * @ outer_thr : threaded outer product
 * > NULL
 * + do part of outer product calculations
 */

static THR_DEF
outer_thr(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  pt1D	      * ptVec;			/* vector of points */
  real2D      * matrix;			/* matrix to fill */
  real1D      * realVec;		/* vector to fill */
  int		n;			/* size */
  int		tid;			/* ID */

  /* setup */
  ptVec   = TG_pt1D(argsThr, 0);
  matrix  = TG_real2D(argsThr, 1);
  realVec = TG_real1D(argsThr, 2);
  n       = TG_int(argsThr, 3);
  tid     = thr_idSet();

  outer(tid, *ptVec, *matrix, *realVec, n);

  THR_END(argsThr);
}
