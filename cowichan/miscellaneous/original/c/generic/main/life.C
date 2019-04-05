/*==============================================================*/
/* generic/main/life.c : generic life driver			*/
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
  static char * context = "main(life)";
  bool2D	world;			/* world to evolve */
  int		nr, nc;			/* matrix size */
  int		iters;			/* number of iterations */
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
  CHECK(0 < iters,
	fail(context, "non-positive number of iterations",
	     "number of iterations", "%d", iters, NULL));
  io_rdBool2D(context, infn, world, &nr, &nc);

  /* run */
  life(world, nr, nc, iters);

  /* takedown */
  io_wrBool2D(context, outfn, world, nr, nc);

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
