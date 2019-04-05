
/*==============================================================*/
/* bar/product.c : barrier product driver			*/
/*==============================================================*/

#if NUMA
MAIN_ENV
#endif
#include "specific.h"

static thr_f	product_thr;

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
#if GRAPHICS
     case 'g' :
      gfx_open(app_product, arg_gfxCtrl(context, argc, argv, argd+1, argv[argd]));
      argd += 2;
      break;
#endif
     case 'p' :
      DataDist = arg_dataDist(context, argc, argv, argd+1, argv[argd]);
      ParWidth = arg_int(context, argc, argv, argd+2, argv[argd]);
      argd += 3;
      break;
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
  sch_init(DataDist);
  io_rdReal2D(context, infnMat, matrix, &nr, &nc);
  io_rdReal1D(context, infnVec, vector, &n);
  CHECK(nc == n,
	fail(context, "matrix/vector size mis-match",
	     "number of columns in matrix", "%d", nc,
	     "size of vector", "%d", n, NULL));

  /* run */
  TP_any(args, 0, matrix);
  TP_any(args, 1, vector);
  TP_any(args, 2, result);
  TP_any(args, 3, nr);
  TP_any(args, 4, nc);
  thr_grp(product_thr, args);

  /* takedown */
  io_wrReal1D(context, outfn, result, nr);

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
 * @ product_thr : threaded matrix-vector product
 * > NULL
 * + update portion of result vector
 */

static THR_DEF
product_thr(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  real2D      * matrix;			/* to multiply by */
  real1D      * vector;			/* to be multiplied */
  real1D      * result;			/* result of multiply */
  int		nr, nc;			/* sizes */
  int		tid;			/* ID */

  /* setup */
  matrix = TG_real2D(argsThr, 0);
  vector = TG_real1D(argsThr, 1);
  result = TG_real1D(argsThr, 2);
  nr     = TG_int(argsThr, 3);
  nc     = TG_int(argsThr, 4);
  tid    = thr_idSet();

  product(tid, *matrix, *vector, *result, nr, nc);

  THR_END(argsThr);
}
