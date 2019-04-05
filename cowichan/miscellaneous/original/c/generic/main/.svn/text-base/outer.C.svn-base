/*==============================================================*/
/* generic/main/outer.c : generic outer driver			*/
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
  static char * context = "main(outer)";
  pt1D		ptVec;			/* point vector */
  real2D	matrix;			/* matrix to create */
  real1D	realVec;		/* vector to create */
  int		n;			/* size */
  char	      * infn = NULL;		/* input file name */
  char	      * outfnMat = NULL;	/* output matrix file name */
  char	      * outfnVec = NULL;	/* output vector file name */
  int		argd = 1;		/* argument index */

  /* arguments */
#if NUMA
  MAIN_INITENV(,32000000)
#endif
  while (argd < argc){
    CHECK(argv[argd][0] == '-',
	  fail(context, "bad argument", "index", "%d", argd, NULL));
    switch(argv[argd][1]){
#if GRAPHICS
     case 'g' :
      gfx_open(app_outer, arg_gfxCtrl(context, argc, argv, argd+1, argv[argd]));
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
     case 'o' :
      outfnMat = arg_str(context, argc, argv, argd+1, argv[argd]);
      outfnVec = arg_str(context, argc, argv, argd+2, argv[argd]);
      argd += 3;
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
  io_rdPt1D(context, infn, ptVec, &n);

  /* run */
  outer(ptVec, matrix, realVec, n);

  /* takedown */
  io_wrReal2D(context, outfnMat, matrix, n, n);
  io_wrReal1D(context, outfnVec, realVec, n);

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
