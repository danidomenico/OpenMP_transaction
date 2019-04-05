/*==============================================================*/
/* generic/main/product.c : generic product driver		*/
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
  static char * context = "main(product)";
  real2D	matrix;			/* matrix to multiply by */
  real1D	vector;			/* vector to multiply */
  real1D	result;			/* result */
  int		n, nr, nc;		/* sizes */
  char	      * infnMat = NULL;		/* input matrix file name */
  char	      * infnVec = NULL;		/* input vector file name */
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
      gfx_open(app_product, arg_gfxCtrl(context, argc, argv, argd+1, argv[argd]));
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
  io_rdReal2D(context, infnMat, matrix, &nr, &nc);
  io_rdReal1D(context, infnVec, vector, &n);
  CHECK(nc == n,
	fail(context, "matrix/vector size mis-match",
	     "number of columns in matrix", "%d", nc,
	     "size of vector", "%d", n, NULL));

  /* run */
  product(matrix, vector, result, nr, nc);

  /* takedown */
  io_wrReal1D(context, outfn, result, nr);

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
