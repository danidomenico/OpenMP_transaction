/*==============================================================*/
/* generic/main/winnow.c : generic winnow driver		*/
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

  /* arguments */
#if NUMA
  MAIN_INITENV(,32000000)
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
#if MIMD
     case 'p' :
      DataDist = arg_dataDist(context, argc, argv, argd+1, argv[argd]);
      ParWidth = arg_int(context, argc, argv, argd+2, argv[argd]);
      argd += 3;
      break;
#endif
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
#if MIMD
  sch_init(DataDist);
#endif
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
  winnow(matrix, mask, nr, nc, pt, npt);

  /* takedown */
  io_wrPt1D(context, outfn, pt, npt);

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
