/*==============================================================*/
/* bar/winnow.c : barrier winnow driver				*/
/*==============================================================*/

#if NUMA
MAIN_ENV
#endif
#include "specific.h"

static thr_f	winnow_thr;

int
main(
  int		argc,			/* arg count */
  char	      * argv[]			/* arg vector */
){
  static char * context = "main(winnow)";
  int2D		matrix;			/* matrix of values */
  bool2D	mask;			/* mask on values */
  int		nr, nc, nrM, ncM;	/* sizes */
  pt1D		pt;			/* resulting point vector */
  int		npt;			/* number of points to keep */
  char	      * infnMat = NULL;		/* input matrix file name */
  char	      * infnMask = NULL;	/* input mask file name */
  char	      * outfn = NULL;		/* output file name */
  int		argd = 1;		/* argument index */
  void	      * args[6];

  /* arguments */
#if NUMA
  MAIN_INITENV(,32000000)
  BARINIT(GlobalBar);
#endif
  while (argd < argc){
    CHECK(argv[argd][0] == '-',
	  fail(context, "bad argument", "index", "%d", argd, NULL));
    switch(argv[argd][1]){
     case 'N' :
      npt = arg_int(context, argc, argv, argd+1, argv[argd]);
      argd += 2;
      break;
#if GRAPHICS
     case 'g' :
      gfx_open(app_winnow, arg_gfxCtrl(context, argc, argv, argd+1, argv[argd]));
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
      infnMask = arg_str(context, argc, argv, argd+2, argv[argd]);
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
  CHECK(npt > 0,
	fail(context, "non-positive number of points requested",
	     "number of points", "%d", npt, NULL));
  io_rdInt2D(context, infnMat, matrix, &nr, &nc);
  io_rdBool2D(context, infnMask, mask, &nrM, &ncM);
  CHECK((nr == nrM) && (nc == ncM),
	fail(context, "matrix/mask size mismatch",
	     "matrix file", "%s", infnMat,
	     "mask file", "%s", infnMask, NULL));

  /* run */
  TP_any(args, 0, matrix);
  TP_any(args, 1, mask);
  TP_any(args, 2, nr);
  TP_any(args, 3, nc);
  TP_any(args, 4, pt);
  TP_any(args, 5, npt);
  thr_grp(winnow_thr, args);

  /* takedown */
  io_wrPt1D(context, outfn, pt, npt);

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
 * @ winnow_thr : threaded point winnowing
 * > NULL
 * + sort and select
 */

static THR_DEF
winnow_thr(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  int2D	      * matrix;			/* point values */
  bool2D      * mask;			/* suitable points */
  int		nr, nc;			/* row size */
  pt1D	      * pt;			/* points to create */
  int		npt;			/* number of points */
  int		tid;			/* ID */

  /* setup */
  matrix = TG_int2D(argsThr, 0);
  mask   = TG_bool2D(argsThr, 1);
  nr     = TG_int(argsThr, 2);
  nc     = TG_int(argsThr, 3);
  pt     = TG_pt1D(argsThr, 4);
  npt    = TG_int(argsThr, 5);
  tid    = thr_idSet();

  winnow(tid, *matrix, *mask, nr, nc, *pt, npt);

  THR_END(argsThr);
}
