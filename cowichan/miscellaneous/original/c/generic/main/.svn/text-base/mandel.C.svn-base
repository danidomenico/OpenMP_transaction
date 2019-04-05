/*==============================================================*/
/* generic/main/mandel.c : generic mandel driver		*/
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
  static char * context = "main(mandel)";
  int2D		matrix;			/* matrix to fill */
  int		nr, nc;			/* matrix size */
  real		base_x, base_y;		/* base point */
  real		ext_x, ext_y;		/* extent */
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
#if MIMD
     case 'p' :
      DataDist = arg_dataDist(context, argc, argv, argd+1, argv[argd]);
      ParWidth = arg_int(context, argc, argv, argd+2, argv[argd]);
      argd += 3;
      break;
#endif
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
  CHECK((0 < nr) && (nr <= MAXEXT) && (0 < nc) && (nc <= MAXEXT),
	fail(context, "illegal matrix size(s)",
	     "row extent", "%d", nr,
	     "column extent", "%d", nc, NULL));
  CHECK((0.0 < ext_x) && (0.0 < ext_y),
	fail(context, "illegal extent(s)",
	     "X extent", "%f", (double)ext_x,
	     "Y extent", "%f", (double)ext_y, NULL));

  /* run */
  mandel(matrix, nr, nc, base_x, base_y, ext_x, ext_y);

  /* takedown */
  io_wrInt2D(context, outfn, matrix, nr, nc);

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
