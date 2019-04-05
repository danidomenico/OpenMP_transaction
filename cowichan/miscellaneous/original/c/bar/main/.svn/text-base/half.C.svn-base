/*==============================================================*/
/* bar/half.c : barrier half driver				*/
/*==============================================================*/

#if NUMA
MAIN_ENV
#endif
#include "specific.h"

static thr_f	half_thr;

int
main(
  int		argc,			/* arg count */
  char	      * argv[]			/* arg vector */
){
  static char * context = "main(half)";
  int2D		matrix;			/* matrix to shuffle */
  int		nr, nc;			/* sizes */
  char	      * infn = NULL;		/* input file name */
  char	      * outfn = NULL;		/* output file name */
  int		argd = 1;		/* argument index */
  void	      * args[3];

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
      gfx_open(app_half, arg_gfxCtrl(context, argc, argv, argd+1, argv[argd]));
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
  io_rdInt2D(context, infn, matrix, &nr, &nc);

  /* run */
  TP_any(args, 0, matrix);
  TP_any(args, 1, nr);
  TP_any(args, 2, nc);
  thr_grp(half_thr, args);

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
 * @ half_thr : do halving
 * > NULL
 * + do halving
 */

static THR_DEF
half_thr(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  int2D	      * matrix;			/* matrix to halve */
  int		nr, nc;			/* row/col size */
  int		tid;			/* ID */

  /* setup */
  matrix = TG_int2D(argsThr, 0);
  nr     = TG_int(argsThr, 1);
  nc     = TG_int(argsThr, 2);
  tid    = thr_idSet();

  half(tid, *matrix, nr, nc);

  THR_END(argsThr);
}
