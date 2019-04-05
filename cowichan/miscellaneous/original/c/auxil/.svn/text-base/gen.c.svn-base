/*==============================================================*/
/* aux/gen.c : auxiliary input file generator			*/
/*==============================================================*/

#include "specific.h"

/*--------------------------------------------------------------*/
/* private definitions and types				*/
/*--------------------------------------------------------------*/

#define APP_SCL 0x0000FFFF

/*--------------------------------------------------------------*/
/* private function prototypes					*/
/*--------------------------------------------------------------*/

static void
gen_i2D_b(
  char	      * outfn,			/* output file name */
  int		nr,			/* number of rows */
  int		nc,			/* number of columns */
  real		fraction		/* fraction to fill */
);
static void
gen_i2D(
  char	      * outfn,			/* output file name */
  int		nr,			/* number of rows */
  int		nc,			/* number of columns */
  int		limit			/* upper value (exclusive) */
);
static void
gen_p1D(
  char	      * outfn,			/* output file name */
  real		limit,			/* limiting value */
  int		n			/* number to generate */
);
static void
gen_r1D(
  char	      * outfn,			/* output file name */
  int		n,			/* number of rows and columns */
  real		limit			/* upper value (exclusive) */
);
static void
gen_r2D(
  char	      * outfn,			/* output file name */
  int		n,			/* number of rows and columns */
  real		limit			/* upper value (exclusive) */
);
static void
usage(
  void
);

/*--------------------------------------------------------------*/
/* main driver							*/
/*--------------------------------------------------------------*/

int
main(
  int		argc,			/* argument count */
  char	     ** argv			/* argument vector */
){
  static char * context = "main(gen)";
  int		argd = 1;		/* argument index */
  char	      * outfn = NULL;		/* output file */
  int		seed = -1;		/* RNG seed */
  int		limInt = -1;		/* integer upper limit */
  real		limReal = -1.0;		/* real upper limit */
  int		nr = -1, nc = -1;	/* number of rows and columns */
  int		n = -1;			/* number to generate */
  real		fraction = -1.0;	/* fraction to fill */
  ft_e		ft = ft_e_sz;		/* operation type */

  /* user wants help */
  if (argc == 1){
    usage();
    exit(0);
  }

  /* arguments */
  while (argd < argc){
    ASSERT(argv[argd][0] == '-');
    switch(argv[argd][1]){
     case 'I' :			       /* value limit */
      limInt = arg_int(context, argc, argv, argd+1, argv[argd]);
      argd += 2;
      break;
     case 'R' :			       /* real value limit */
      limReal = arg_real(context, argc, argv, argd+1, argv[argd]);
      argd += 2;
      break;
     case 'u' :			       /* unformatted I/O */
      io_init(FALSE);
      break;
     case 'n' :			       /* number of elements */
      n = arg_int(context, argc, argv, argd+1, argv[argd]);
      argd += 2;
      CHECK((0 < n) && (n <= MAXEXT),
	    fail(context, "illegal size specifier", "size", "%d", n, NULL));
      break;
     case 'o' :			       /* output file */
      outfn = arg_str(context, argc, argv, argd+1, argv[argd]);
      argd += 2;
      break;
     case 'f' :			       /* fraction */
      fraction = arg_real(context, argc, argv, argd+1, argv[argd]);
      argd += 2;
      break;
     case 'r' :			       /* RNG seed */
      seed = arg_int(context, argc, argv, argd+1, argv[argd]);
      argd += 2;
      break;
     case 's' :			       /* sizes */
      nr = arg_int(context, argc, argv, argd+1, argv[argd]);
      nc = arg_int(context, argc, argv, argd+2, argv[argd]);
      argd += 3;
      CHECK((0 < nr) && (nr <= MAXEXT) && (0 < nc) && (nc <= MAXEXT),
	    fail(context, "illegal size specifier(s)",
		 "row size", "%d", nr,
		 "column size", "%d", nc, NULL));
      break;
     case 't' :			       /* type */
      ft = aux_str2Ft(arg_str(context, argc, argv, argd+1, argv[argd]));
      argd += 2;
      break;
     default :			       /* unknown */
      fail(context, "unknown flag", "flag", "%s", argv[argd], NULL);
      break;
    }
  }

  /* check */
  CHECK(ft < ft_e_sz,
	fail(context, "file type not set", NULL));

  /* processing */
  RAND_INIT(seed);
  switch(ft){
   case ft_i2D :
    CHECK((nr > 0) && (nc > 0),
	  fail(context, "row/col size not set for integer matrix", NULL));
    CHECK(limInt > 0,
	  fail(context, "integer limit not set for integer matrix", NULL));
    gen_i2D(outfn, nr, nc, limInt);
    break;
   case ft_i2D_b :
    CHECK((nr > 0) && (nc > 0),
	  fail(context, "row/col size not set for boolean matrix", NULL));
    CHECK(fraction > 0.0,
	  fail(context, "fraction not set for boolean matrix", NULL));
    gen_i2D_b(outfn, nr, nc, fraction);
    break;
   case ft_p1D :
    CHECK(n > 0,
	  fail(context, "number not set for point vector", NULL));
    CHECK(limReal > 0.0,
	  fail(context, "real limit not set for point vector", NULL));
    gen_p1D(outfn, limReal, n);
    break;
   case ft_r1D :
    CHECK(n > 0,
	  fail(context, "number not set for real vector", NULL));
    CHECK(limReal > 0.0,
	  fail(context, "real limit not set for real vector", NULL));
    gen_r1D(outfn, n, limReal);
    break;
   case ft_r2D :
    CHECK(n > 0,
	  fail(context, "number not set for real matrix", NULL));
    CHECK(limReal > 0.0,
	  fail(context, "real limit not set for real matrix", NULL));
    gen_r2D(outfn, n, limReal);
    break;
   default :
    ASSERT(FALSE);
    break;
  }

  exit(0);
}

/*--------------------------------------------------------------*/
/* private functions						*/
/*--------------------------------------------------------------*/

/*
 * @ gen_i2D_b : generate random Boolean matrix
 * > none
 * + generate matrix and write to file
 */

static void
gen_i2D_b(
  char	      * outfn,			/* output file name */
  int		nr,			/* number of rows */
  int		nc,			/* number of columns */
  real		fraction		/* fraction to fill */
){
  static char * context = "gen_i2D_b";
  int2D		matrix;			/* to fill */
  int		r, c;			/* loop indices */
  int		upper = fraction * APP_SCL;

  ASSERT(nr > 0);
  ASSERT(nc > 0);

  for (r=0; r<nr; r++){
    for (c=0; c<nc; c++){
      matrix[r][c] = (RAND_VAL() & APP_SCL) <= upper;
    }
  }

  io_wrInt2D(context, outfn, matrix, nr, nc);

  /* return */
}

/*
 * @ gen_i2D : generate random integer matrix
 * > none
 * + generate matrix and write to file
 */

static void
gen_i2D(
  char	      * outfn,			/* output file name */
  int		nr,			/* number of rows */
  int		nc,			/* number of columns */
  int		limit			/* upper value (exclusive) */
){
  static char * context = "gen_i2D";
  int2D		matrix;			/* to fill */
  int		r, c;			/* loop indices */

  ASSERT(nr > 0);
  ASSERT(nc > 0);

  for (r=0; r<nr; r++){
    for (c=0; c<nc; c++){
      matrix[r][c] = RAND_VAL() % limit;
    }
  }

  io_wrInt2D(context, outfn, matrix, nr, nc);

  /* return */
}

/*
 * @ gen_p1D : initialize a points vector randomly
 * > none
 * + generate points and write to file
 */

static void
gen_p1D(
  char	      * outfn,			/* output file name */
  real		limit,			/* limiting value */
  int		n			/* number to generate */
){
  static char * context = "gen_p1D";
  pt1D		points;			/* points generated */
  real		scale;			/* scaling factor */
  int		i;			/* loop index */

  scale = limit/(real)INT_MAX;
  for (i=0; i<n; i++){
    points[i].x = (real)RAND_VAL() * scale;
    points[i].y = (real)RAND_VAL() * scale;
    points[i].w = 1;
  }
  io_wrPt1D(context, outfn, points, n);

  /* return */
}

/*
 * @ gen_r1D : generate random real vector
 * > none
 * + generate vector and write to file
 */

static void
gen_r1D(
  char	      * outfn,			/* output file name */
  int		n,			/* number of rows and columns */
  real		limit			/* upper value (exclusive) */
){
  static char * context = "gen_r1D";
  real1D	vector;			/* to fill */
  int		i;			/* loop indices */
  real		limScale;		/* for conversion */

  ASSERT(n > 0);

  limScale = (real)((double)limit/(double)INT_MAX);
  for (i=0; i<n; i++){
    vector[i] = RAND_VAL() * limScale;
  }

  io_wrReal1D(context, outfn, vector, n);

  /* return */
}

/*
 * @ gen_r2D : generate random real matrix
 * > none
 * + generate matrix and write to file
 */

static void
gen_r2D(
  char	      * outfn,			/* output file name */
  int		n,			/* number of rows and columns */
  real		limit			/* upper value (exclusive) */
){
  static char * context = "gen_r2D";
  real2D	matrix;			/* to fill */
  int		r, c;			/* loop indices */
  real		limScale;		/* for conversion */
  real		valMax = -1.0;		/* maximum value seen */

  ASSERT(n > 0);

  limScale = (real)((double)limit/(double)INT_MAX);
  for (r=0; r<n; r++){
    for (c=0; c<r; c++){
      if (r != c){
	matrix[r][c] = matrix[c][r] = RAND_VAL() * limScale;
	if (matrix[r][c] > valMax){
	  valMax = matrix[r][c];
	}
      }
    }
  }

  valMax *= n;
  for (r=0; r<n; r++){
    matrix[r][r] = valMax;
  }

  io_wrReal2D(context, outfn, matrix, n, n);

  /* return */
}

/*
 * @ usage : print usage
 * > none
 */

static void
usage(
  void
){
  printf("gen\n");
  printf("\t-I val                      : integer limit\n");
  printf("\t-R val                      : real limit\n");
  printf("\t-f fraction (0..1)          : approximate filling percentage\n");
  printf("\t-n number                   : number of vector elements\n");
  printf("\t-o filename                 : output file name\n");
  printf("\t-r seed                     : random number generation seed\n");
  printf("\t-s rows cols                : matrix sizes\n");
  printf("\t-t {b2, i2, p1, r0, r1, r2} : object type\n");
  printf("\t-u                          : unformatted\n");
  /* return */
}
