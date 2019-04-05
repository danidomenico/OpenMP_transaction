/*==============================================================*/
/* generic/main/sor.c : generic sor driver			*/
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
  static char * context = "main(sor)";
  real2D	matrix;			/* matrix to solve */
  real1D	vector, answer;		/* input and answer vectors */
  int		n, nr, nc;		/* sizes */
  real		tol;			/* tolerance on result */
  char	      * infnMat = NULL;		/* matrix file name */
  char	      * infnVec = NULL;		/* vector file name */
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
     case 'T' :
       tol = arg_real(context, argc, argv, argd+1, argv[argd]);
       argd += 2;
       break;
#if GRAPHICS
     case 'g' :
      gfx_open(app_sor, arg_gfxCtrl(context, argc, argv, argd+1, argv[argd]));
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
      infnVec = arg_str(context, argc, argv, argd+2, argv[argd]);
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
  CHECK(0.0 < tol,
	fail(context, "non-positive tolerance",
	     "tolerance", "%f", (double)tol, NULL));
  io_rdReal2D(context, infnMat, matrix, &nr, &nc);
  CHECK(nr == nc,
	fail(context, "non-square matrix",
	     "file name", "%s", infnMat, NULL));
  io_rdReal1D(context, infnVec, vector, &n);
  CHECK(n == nr,
	fail(context, "vector size does not match matrix size",
	     "matrix file name", "%s", infnMat,
	     "vector file name", "%s", infnVec, NULL));

  /* run */
  sor(matrix, vector, answer, n, tol);

  /* takedown */
  io_wrReal1D(context, outfn, answer, n);

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
