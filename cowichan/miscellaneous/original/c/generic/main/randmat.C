/*==============================================================*/
/* generic/main/randmat.c : generic randmat driver		*/
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
  static char * context = "main(randmat)";
  int2D		matrix;			/* matrix to fill */
  int		nr, nc;			/* matrix size */
  int		limit;			/* limit to values */
  int		seed;			/* RNG seed */
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
  CHECK(0 < limit,
	fail(context, "non-positive value limit",
	     "limit", "%d", limit, NULL));
  CHECK(0 < seed,
	fail(context, "non-positive RNG seed",
	     "seed value", "%d", seed, NULL));

  /* run */
  randmat(matrix, nr, nc, limit, seed);

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
