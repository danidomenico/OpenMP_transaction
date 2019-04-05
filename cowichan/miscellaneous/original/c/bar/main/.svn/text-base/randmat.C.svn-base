/*==============================================================*/
/* bar/main/randmat.c : barrier randmat driver			*/
/*==============================================================*/

#if NUMA
MAIN_ENV
#endif
#include "specific.h"

static thr_f	randmat_thr;

int
main(
  int		argc,			/* arg count */
  char	      * argv[]			/* arg vector */
){
  static char * context = "main(randmat)";
  int2D		matrix;			/* matrix to fill */
  int		nr, nc;			/* matrix size */
  int		limit;			/* limit to values */
  int		seed;			/* RNG seed */
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
     case 'R' :
       limit = arg_int(context, argc, argv, argd+1, argv[argd]);
       seed = arg_int(context, argc, argv, argd+2, argv[argd]);
       argd += 3;
       break;
     case 'S' :
      nr = arg_int(context, argc, argv, argd+1, argv[argd]);
      nc = arg_int(context, argc, argv, argd+2, argv[argd]);
      argd += 3;
      break;
#if GRAPHICS
     case 'g' :
      gfx_open(app_randmat, arg_gfxCtrl(context, argc, argv, argd+1, argv[argd]));
      argd += 2;
      break;
#endif
     case 'p' :
      DataDist = arg_dataDist(context, argc, argv, argd+1, argv[argd]);
      ParWidth = arg_int(context, argc, argv, argd+2, argv[argd]);
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
  CHECK(0 < limit,
	fail(context, "non-positive value limit",
	     "limit", "%d", limit, NULL));
  CHECK(0 < seed,
	fail(context, "non-positive RNG seed",
	     "seed value", "%d", seed, NULL));

  /* run */
  TP_any(args, 0, matrix);
  TP_any(args, 1, nr);
  TP_any(args, 2, nc);
  TP_any(args, 3, limit);
  TP_any(args, 4, seed);
  thr_grp(randmat_thr, args);

  /* takedown */
  io_wrInt2D(context, outfn, matrix, nr, nc);

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
 * @ randmat_thr : threaded random matrix generation
 * > NULL
 * + update matrix
 */

static THR_DEF
randmat_thr(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  int2D	      * matrix;			/* matrix to fill */
  int		nr, nc;			/* size */
  int		limit, seed;		/* RNG controls */
  int		tid;			/* ID */

  /* setup */
  matrix = TG_int2D(argsThr, 0);
  nr     = TG_int(argsThr, 1);
  nc     = TG_int(argsThr, 2);
  limit  = TG_int(argsThr, 3);
  seed   = TG_int(argsThr, 4);
  tid    = thr_idSet();

  randmat(tid, *matrix, nr, nc, limit, seed);

  THR_END(argsThr);
}
