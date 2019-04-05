/*==============================================================*/
/* bar/life.c : barrier life driver				*/
/*==============================================================*/

#if NUMA
MAIN_ENV
#endif
#include "specific.h"

static thr_f	life_thr;

int
main(
  int		argc,			/* arg count */
  char	      * argv[]			/* arg vector */
){
  static char * context = "main(life)";
  bool2D	world;			/* world to evolve */
  int		nr, nc;			/* matrix size */
  int		iters;			/* number of iterations */
  char	      * infn = NULL;		/* input file name */
  char	      * outfn = NULL;		/* output file name */
  int		argd = 1;		/* argument index */
  void	      * args[5];

  /* arguments */
#if NUMA
  MAIN_INITENV(,32000000)
  BARINIT(GlobalBar);
#endif
  while (argd < argc){
    CHECK(argv[argd][0] == '-',
	  fail(context, "bad argument", "index", "%d", argd, NULL));
    switch(argv[argd][1]){
     case 'L' :
      iters = arg_int(context, argc, argv, argd+1, argv[argd]);
      argd += 2;
      break;
#if GRAPHICS
     case 'g' :
      gfx_open(app_life, arg_gfxCtrl(context, argc, argv, argd+1, argv[argd]));
      argd += 2;
      break;
#endif
#if PARALLEL
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
  sch_init(DataDist);
  CHECK(0 < iters,
	fail(context, "non-positive number of iterations",
	     "number of iterations", "%d", iters, NULL));
  io_rdBool2D(context, infn, world, &nr, &nc);

  /* run */
  TP_any(args, 0, world);
  TP_any(args, 1, nr);
  TP_any(args, 2, nc);
  TP_any(args, 3, iters);
  thr_grp(life_thr, args);

  /* takedown */
  io_wrBool2D(context, outfn, world, nr, nc);

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
 * @ life_thr : threaded Game of Life
 * > NULL
 * + evolve
 */

static THR_DEF
life_thr(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  bool2D      * world;			/* world to evolve */
  int		nr, nc;			/* row/col size */
  int		iters;			/* number of iterations */
  int		tid;			/* ID */

  /* setup */
  world = TG_bool2D(argsThr, 0);
  nr    = TG_int(argsThr, 1);
  nc    = TG_int(argsThr, 2);
  iters = TG_int(argsThr, 3);
  tid   = thr_idSet();

  life(tid, *world, nr, nc, iters);

  THR_END(argsThr);
}
