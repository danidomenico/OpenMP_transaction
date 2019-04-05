/*==============================================================*/
/* bar/mandel.c : barrier mandel driver				*/
/*==============================================================*/

#if NUMA
MAIN_ENV
#endif
#include "specific.h"

static thr_f	mandel_thr;

int
main(
  int		argc,			/* arg count */
  char	      * argv[]			/* arg vector */
){
  static char * context = "main(mandel)";
  int2D		matrix;			/* matrix to fill */
  int		nr, nc;			/* matrix size */
  real		base_x, base_y;		/* base point */
  real		ext_x, ext_y;		/* extent */
  char	      * outfn = NULL;		/* output file name */
  int		argd = 1;		/* argument index */
  void	      * args[7];

  /* arguments */
#if NUMA
  MAIN_INITENV(,32000000)
  BARINIT(GlobalBar);
#endif
  while (argd < argc){
    CHECK(argv[argd][0] == '-',
	  fail(context, "bad argument", "index", "%d", argd, NULL));
    switch(argv[argd][1]){
     case 'M' :
      base_x = arg_real(context, argc, argv, argd+1, argv[argd]);
      base_y = arg_real(context, argc, argv, argd+2, argv[argd]);
      ext_x  = arg_real(context, argc, argv, argd+3, argv[argd]);
      ext_y  = arg_real(context, argc, argv, argd+4, argv[argd]);
      argd += 5;
      break;
     case 'S' :
      nr = arg_int(context, argc, argv, argd+1, argv[argd]);
      nc = arg_int(context, argc, argv, argd+2, argv[argd]);
      argd += 3;
      break;
#if GRAPHICS
     case 'g' :
      gfx_open(app_mandel, arg_gfxCtrl(context, argc, argv, argd+1, argv[argd]));
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
  CHECK((0 < nr) && (nr <= MAXEXT) && (0 < nc) && (nc <= MAXEXT),
	fail(context, "illegal matrix size(s)",
	     "row extent", "%d", nr,
	     "column extent", "%d", nc, NULL));
  CHECK((0.0 < ext_x) && (0.0 < ext_y),
	fail(context, "illegal extent(s)",
	     "X extent", "%f", (double)ext_x,
	     "Y extent", "%f", (double)ext_y, NULL));

  /* run */
  mandelSetup();
  TP_any(args, 0, matrix);
  TP_any(args, 1, nr);
  TP_any(args, 2, nc);
  TP_any(args, 3, &base_x);
  TP_any(args, 4, &base_y);
  TP_any(args, 5, &ext_x);
  TP_any(args, 6, &ext_y);
  thr_grp(mandel_thr, args);
  mandelTakedown();

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
 * @ mandel_thr : worker to calculate Mandelbrot Set
 * > NULL
 * + fill matrix
 */

static THR_DEF
mandel_thr(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  int2D	      * matrix;			/* to shuffle */
  int		nr, nc;			/* row/col size */
  real		base_x, base_y;		/* lower left corner */
  real		ext_x, ext_y;		/* extent */
  int		tid;			/* own ID */

  /* setup */
  matrix = TG_int2D(argsThr, 0);
  nr     = TG_int(argsThr, 1);
  nc     = TG_int(argsThr, 2);
  base_x = *TG_real_p(argsThr, 3);
  base_y = *TG_real_p(argsThr, 4);
  ext_x  = *TG_real_p(argsThr, 5);
  ext_y  = *TG_real_p(argsThr, 6);
  tid    = thr_idSet();

  mandel(tid, *matrix, nr, nc, base_x, base_y, ext_x, ext_y);

  THR_END(argsThr);
}
