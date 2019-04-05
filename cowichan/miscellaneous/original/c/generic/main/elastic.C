/*==============================================================*/
/* generic/main/elastic.c : generic elastic driver		*/
/*==============================================================*/

#if NUMA
MAIN_ENV
#endif
#include "specific.h"

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

  /* arguments */
#if NUMA
  MAIN_INITENV(,32000000)
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
#if MIMD
     case 'p' :
      DataDist = arg_dataDist(context, argc, argv, argd+1, argv[argd]);
      ParWidth = arg_int(context, argc, argv, argd+2, argv[argd]);
      argd += 3;
      break;
#endif
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
#if MIMD
  sch_init(DataDist);
#endif
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
  elastic(cities, n_cities, net, n_net, iters, relax);

  /* takedown */
  io_wrPt1D(context, outfn, net, n_net);

#if GRAPHICS
  gfx_close();
#endif
#if IEEE
  ieee_retrospective(stderr);
#endif
#if NUMA
  MAIN_END;
#endif

  return 0;
}
