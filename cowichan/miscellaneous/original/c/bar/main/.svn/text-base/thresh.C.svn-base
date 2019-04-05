/*==============================================================*/
/* bar/thresh.c : barrier thresh driver				*/
/*==============================================================*/

#if NUMA
MAIN_ENV
#endif
#include "specific.h"

static thr_f	thresh_thr;

int
main(
  int		argc,			/* arg count */
  char	      * argv[]			/* arg vector */
){
  static char * context = "main(thresh)";
  int2D		matrix;			/* values matrix */
  bool2D	mask;			/* result mask */
  int		nr, nc;			/* matrix size */
  real		fraction;		/* how much to keep */
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
     case 'F' :
      fraction = arg_real(context, argc, argv, argd+1, argv[argd]);
      argd += 2;
      break;
#if GRAPHICS
     case 'g' :
      gfx_open(app_thresh, arg_gfxCtrl(context, argc, argv, argd+1, argv[argd]));
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
  TP_any(args, 1, mask);
  TP_any(args, 2, nr);
  TP_any(args, 3, nc);
  TP_any(args, 4, &fraction);
  thr_grp(thresh_thr, args);

  /* takedown */
  io_wrBool2D(context, outfn, mask, nr, nc);

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
 * @ thresh_thr : threaded histogram thresholding
 * > NULL
 * + create mask
 */

static THR_DEF
thresh_thr(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  int2D	      * matrix;			/* to threshold */
  bool2D      * mask;			/* threshold mask */
  int		nr, nc;			/* row size */
  real		fraction;		/* filling fraction */
  int		tid;			/* own ID */

  /* setup */
  matrix   = TG_int2D(argsThr, 0);
  mask     = TG_bool2D(argsThr, 1);
  nr       = TG_int(argsThr, 2);
  nc       = TG_int(argsThr, 3);
  fraction = *TG_real_p(argsThr, 4);
  tid      = thr_idSet();

  thresh(tid, *matrix, *mask, nr, nc, fraction);

  THR_END(argsThr);
}
