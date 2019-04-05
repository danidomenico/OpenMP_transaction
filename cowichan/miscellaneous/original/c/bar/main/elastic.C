/*==============================================================*/
/* bar/elastic.c : barrier elastic driver			*/
/*==============================================================*/

#if NUMA
MAIN_ENV
#endif
#include "specific.h"

static thr_f	elastic_thr;

int
main(
  int		argc,			/* arg count */
  char	      * argv[]			/* arg vector */
){
  static char * context = "main(elastic)";
  pt1D		cities, net;		/* cities and net */
  int		n_cities, n_net;	/* cities/net sizes*/
  int		iters, relax;		/* control parameters */
  char	      * infn = NULL;		/* input file name */
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
     case 'E' :
      iters = arg_int(context, argc, argv, argd+1, argv[argd]);
      relax = arg_int(context, argc, argv, argd+2, argv[argd]);
      argd += 3;
      break;
#if GRAPHICS
     case 'g' :
      gfx_open(app_elastic, arg_gfxCtrl(context, argc, argv, argd+1, argv[argd]));
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
  CHECK(0 < iters,
	fail(context, "non-positive number of iterations",
	     "number of iterations", "%d", iters, NULL));
  CHECK((0 < relax) && (relax <= iters),
	fail(context, "illegal relaxation count",
	     "number of iterations", "%d", iters,
	     "relaxation count", "%d", relax, NULL));
  io_rdPt1D(context, infn, cities, &n_cities);
  ptNormChk(cities, n_cities);
  n_net = (int)(ELASTIC_RATIO * n_cities);
  CHECK(n_net <= MAXEXT,
	fail(context, "too many net points required",
	     "number of net points", "%d", n_net, NULL));

  /* run */
  TP_any(args, 0, cities);
  TP_any(args, 1, n_cities);
  TP_any(args, 2, net);
  TP_any(args, 3, n_net);
  TP_any(args, 4, iters);
  TP_any(args, 5, relax);
  thr_grp(elastic_thr, args);

  /* takedown */
  io_wrPt1D(context, outfn, net, n_net);

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
 * @ elastic_thr : do elastic net calculations
 * > NULL
 * + do calculations
 */

static THR_DEF
elastic_thr(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  pt1D	      * cities;			/* cities to link */
  int		n_cities;		/* number of cities */
  pt1D	      * net;			/* net to link with */
  int		n_net;			/* number of net points */
  int		iters;			/* total iterations */
  int		relax;			/* relaxation interval */
  int		tid;			/* ID */

  /* setup */
  cities   = TG_pt1D(argsThr, 0);
  n_cities = TG_int(argsThr, 1);
  net      = TG_pt1D(argsThr, 2);
  n_net    = TG_int(argsThr, 3);
  iters    = TG_int(argsThr, 4);
  relax    = TG_int(argsThr, 5);
  tid      = thr_idSet();

  /* work */
  elastic(tid, *cities, n_cities, *net, n_net, iters, relax);

  THR_END(argsThr);
}
