/*==============================================================*/
/* bar/norm.c : barrier norm driver				*/
/*==============================================================*/

#if NUMA
MAIN_ENV
#endif
#include "specific.h"

static thr_f	norm_thr;

int
main(
  int		argc,			/* arg count */
  char	      * argv[]			/* arg vector */
){
  static char * context = "main(norm)";
  pt1D		vec;			/* point vector */
  int		n;			/* number of points */
  char	      * infn = NULL;		/* input file name */
  char	      * outfn = NULL;		/* output file name */
  int		argd = 1;		/* argument index */
  void	      * args[2];

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
      gfx_open(app_norm, arg_gfxCtrl(context, argc, argv, argd+1, argv[argd]));
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
  io_rdPt1D(context, infn, vec, &n);

  /* run */
  TP_any(args, 0, vec);
  TP_any(args, 1, n);
  thr_grp(norm_thr, args);

  /* takedown */
  io_wrPt1D(context, outfn, vec, n);

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
 * @ norm_thr : threaded coordinate normalization
 * > NULL
 * + modify coordinates
 */

static THR_DEF
norm_thr(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  pt1D	      * vec;			/* points to normalize */
  int		n;			/* length of vector */
  int		tid;			/* ID */

  /* setup */
  vec = TG_pt1D(argsThr, 0);
  n   = TG_int(argsThr, 1);
  tid = thr_idSet();

  norm(tid, *vec, n);

  THR_END(argsThr);
}
