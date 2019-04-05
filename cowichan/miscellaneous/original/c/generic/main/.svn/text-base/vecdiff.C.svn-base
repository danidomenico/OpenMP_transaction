/*==============================================================*/
/* generic/main/vecdiff.c : generic vecdiff driver		*/
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
  static char * context = "main(vecdiff)";
  real1D	left, right;		/* vectors */
  int		n_left, n_right;	/* sizes */
  real		diff;			/* difference */
  char	      * infnLeft = NULL;	/* left vector file name */
  char	      * infnRight = NULL;	/* right vector file name */
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
#if GRAPHICS
     case 'g' :
      gfx_open(app_vecdiff, arg_gfxCtrl(context, argc, argv, argd+1, argv[argd]));
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
      infnLeft = arg_str(context, argc, argv, argd+1, argv[argd]);
      infnRight = arg_str(context, argc, argv, argd+2, argv[argd]);
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
  io_rdReal1D(context, infnLeft, left, &n_left);
  io_rdReal1D(context, infnRight, right, &n_right);
  CHECK(n_left == n_right,
	fail(context, "vector size mis-match",
	     "left size", "%d", n_left,
	     "right size", "%d", n_right, NULL));

  /* run */
  vecdiff(left, right, n_left, &diff);

  /* takedown */
  io_wrReal0D(context, outfn, diff);

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
