/*==============================================================*/
/* bar/vecdiff.c : barrier vecdiff driver			*/
/*==============================================================*/

#if NUMA
MAIN_ENV
#endif
#include "specific.h"

static thr_f	vecdiff_thr;

int
main(
  int		argc,			/* arg count */
  char	      * argv[]			/* arg vector */
){
  static char * context = "main(vecdiff)";
  real1D	left, right;		/* vectors */
  int		n_left, n_right;	/* sizes */
  real		diff;			/* difference */
  char	      * infnLeft = NULL;	/* left vector file name */
  char	      * infnRight = NULL;	/* right vector file name */
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
      gfx_open(app_vecdiff, arg_gfxCtrl(context, argc, argv, argd+1, argv[argd]));
      argd += 2;
      break;
#endif
     case 'p' :
      DataDist = arg_dataDist(context, argc, argv, argd+1, argv[argd]);
      ParWidth = arg_int(context, argc, argv, argd+2, argv[argd]);
      argd += 3;
      break;
     case 'i' :
      infnLeft = arg_str(context, argc, argv, argd+1, argv[argd]);
      infnRight = arg_str(context, argc, argv, argd+2, argv[argd]);
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
  io_rdReal1D(context, infnLeft, left, &n_left);
  io_rdReal1D(context, infnRight, right, &n_right);
  CHECK(n_left == n_right,
	fail(context, "vector size mis-match",
	     "left size", "%d", n_left,
	     "right size", "%d", n_right, NULL));

  /* run */
  TP_any(args, 0, left);
  TP_any(args, 1, right);
  TP_any(args, 2, n_left);
  TP_any(args, 3, &diff);
  thr_grp(vecdiff_thr, args);

  /* takedown */
  io_wrReal0D(context, outfn, diff);

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
 * @ vecdiff_thr : threaded vector difference
 * > NULL
 * + calculate part of difference
 */

static THR_DEF
vecdiff_thr(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  real1D      * left;			/* left vector */
  real1D      * right;			/* right vector */
  int		n;			/* vector length */
  real	      * diffPtr;		/* where answer goes */
  int		tid;			/* ID */

  /* setup */
  left    = TG_real1D(argsThr, 0);
  right   = TG_real1D(argsThr, 1);
  n       = TG_int(argsThr, 2);
  diffPtr = TG_real_p(argsThr, 3);
  tid     = thr_idSet();

  vecdiff(tid, *left, *right, n, diffPtr);

  THR_END(argsThr);
}
