/*==============================================================*/
/* generic/main/invperc.c : generic invperc driver		*/
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
  static char * context = "main(invperc)";
  int2D		matrix;			/* values matrix */
  bool2D	mask;			/* result mask */
  int		nr, nc;			/* matrix size */
  real		fraction;		/* how much to fill */
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
     case 'F' :
      fraction = arg_real(context, argc, argv, argd+1, argv[argd]);
      argd += 2;
      break;
#if GRAPHICS
     case 'g' :
      gfx_open(app_invperc, arg_gfxCtrl(context, argc, argv, argd+1, argv[argd]));
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
  io_rdInt2D(context, infn, matrix, &nr, &nc);

  /* run */
  invperc(matrix, mask, nr, nc, fraction);

  /* takedown */
  io_wrBool2D(context, outfn, mask, nr, nc);

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
