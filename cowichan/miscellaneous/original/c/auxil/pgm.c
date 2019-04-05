/*==============================================================*/
/* aux/pgm.c : auxiliary Cowichan->pgm file converter		*/
/*==============================================================*/

#include "specific.h"

/*--------------------------------------------------------------*/
/* private definitions						*/
/*--------------------------------------------------------------*/

#define PGM_MAGIC "P2"

/*--------------------------------------------------------------*/
/* private function prototypes					*/
/*--------------------------------------------------------------*/

static void
pgm_i2D(
  FILE	      * ofp,			/* output file pointer */
  int		col,			/* number of colors to use */
  int2D		i2D,			/* matrix to write */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
);
static void
pgm_p1D(
  FILE	      * ofp,			/* output file pointer */
  int		col,			/* number of colors to use */
  pt1D		p1D,			/* vector to write */
  int		n,			/* number of elements */
  int		nr,			/* rows in output */
  int		nc			/* columns in output */
);
static void
pgm_r1D(
  FILE	      * ofp,			/* output file pointer */
  int		col,			/* number of colors to use */
  real1D	r1D,			/* vector to write */
  int		n			/* number of elements */
);
static void
pgm_r2D(
  FILE	      * ofp,			/* output file pointer */
  int		col,			/* number of colors to use */
  real2D	r2D,			/* matrix to write */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
);
static void
pgm_wr(
  FILE	      * ofp,			/* output file pointer */
  int2D		mat,			/* matrix to write */
  int		nr,			/* row size */
  int		nc,			/* column size */
  int		col			/* maximum color */
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
  static char * context = "main(pgm)";
  int		argd = 1;		/* argument index */
  char	      * infn = NULL;		/* input file name */
  char	      * outfn = NULL;		/* output file name */
  FILE	      * ofp = stdout;		/* output file */
  int2D		i2D;			/* integer/boolean matrix */
  pt1D		p1D;			/* point vector */
  real1D	r1D;			/* real vector */
  real2D	r2D;			/* real matrix */
  ft_e		ft = ft_e_sz;		/* file type */
  int		n, nr = 0, nc = 0;	/* sizes */
  int		col;			/* number of colors to use */

  /* user wants help */
  if (argc == 1){
    usage();
    exit(0);
  }

  /* arguments */
  while (argd < argc){
    ASSERT(argv[argd][0] == '-');
    switch(argv[argd][1]){
     case 'c' :				/* number of colors */
      col = arg_int(context, argc, argv, argd+1, argv[argd]);
      argd += 2;
      break;
     case 'i' :				/* input file */
      infn = arg_str(context, argc, argv, argd+1, argv[argd]);
      argd += 2;
      break;
     case 'o' :				/* output file */
      outfn = arg_str(context, argc, argv, argd+1, argv[argd]);
      argd += 2;
      break;
     case 's' :				/* size (for points) */
      nr = arg_int(context, argc, argv, argd+1, argv[argd]);
      nc = arg_int(context, argc, argv, argd+2, argv[argd]);
      argd += 3;
      break;
     case 't' :				/* type */
      ft = aux_str2Ft(arg_str(context, argc, argv, argd+1, argv[argd]));
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
  /* check */
  CHECK(ft < ft_e_sz,
	fail(context, "file type not set", NULL));
  CHECK(0 < col,
	fail(context, "non-positive number of colors given",
	     "number", "%d", col, NULL));
  if (outfn != NULL){
    CHECK((ofp = fopen(outfn, "w")) != NULL,
	  fail(context, "unable to open output file",
	       "output file name", "%s", outfn, NULL));
  }

  /* processing */
  switch(ft){
   case ft_i2D :
   case ft_i2D_b :
    io_rdInt2D(context, infn, i2D, &nr, &nc);
    pgm_i2D(ofp, col, i2D, nr, nc);
    break;
   case ft_p1D :
    io_rdPt1D(context, infn, p1D, &n);
    pgm_p1D(ofp, col, p1D, n, nr, nc);
    break;
   case ft_r1D :
    io_rdReal1D(context, infn, r1D, &n);
    pgm_r1D(ofp, col, r1D, n);
    break;
   case ft_r2D :
    io_rdReal2D(context, infn, r2D, &nr, &nc);
    pgm_r2D(ofp, col, r2D, nr, nc);
    break;
   default :
    ASSERT(FALSE);
    break;
  }

  /* takedown */
  if (ofp != stdout){
    fclose(ofp);
  }

  return 0;
}

/*--------------------------------------------------------------*/
/* private functions						*/
/*--------------------------------------------------------------*/

/*
 * @ pgm_i2D : write integer (or boolean) matrix
 * > none
 * + write matrix in PGM format
 */

static void
pgm_i2D(
  FILE	      * ofp,			/* output file pointer */
  int		col,			/* number of colors to use */
  int2D		i2D,			/* matrix to write */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
){
  static char * context = "pgm_i2D";
  int		iMin, iMax;		/* extrema */
  int		r, c;			/* row/column indices */
  int		range;			/* range of values */
  int		offset = 0;		/* shifting offset */

  /* scale if necessary */
  range = redInt2DExtrema(i2D, nr, nc, &iMin, &iMax);
  if (range <= col){
    if (iMin < 0){
      offset = - iMin;
    } else if (col <= iMax){
      offset = (col - iMax) - 1;
    }
  } else {
    fail(context, "values do not fit in color range",
	 "minimum value", "%d", iMin,
	 "maximum value", "%d", iMax,
	 "specified color range", "%d", col, NULL);
  }

  /* adjust matrix */
  if (offset != 0){
    for (r=0; r<nr; r++){
      for (c=0; c<nc; c++){
	i2D[r][c] += offset;
      }
    }
  }

  /* write */
  pgm_wr(ofp, i2D, nr, nc, range);

  /* return */
}

/*
 * @ pgm_p1D : write point vector
 * > none
 * + write points in PGM format
 */

static void
pgm_p1D(
  FILE	      * ofp,			/* output file pointer */
  int		col,			/* number of colors to use */
  pt1D		p1D,			/* vector to write */
  int		n,			/* number of elements */
  int		nr,			/* rows in output */
  int		nc			/* columns in output */
){
  static char * context = "pgm_p1D";
  int2D		mat;			/* output matrix */
  int		i, r, c;		/* vector/row/column indices */
  pt		pMin, pMax;		/* extrema */
  pt		scl;			/* scaling factors */
  int		iX, iY;			/* integer coordinates */
  int		mc;			/* maximum count */

  /* setup */
  CHECK((0 < nr) && (nr <= MAXEXT) && (0 < nc) && (nc <= MAXEXT),
	fail(context, "bad output size specification",
	     "nr", "%d", nr, "nc", "%d", nc, NULL));
  for (r=0; r<nr; r++){
    for (c=0; c<nc; c++){
      mat[r][c] = 0;
    }
  }

  /* find scaling factors */
  redPt1DExtrema(p1D, n, &pMin, &pMax);
  scl.x = (pMax.x > pMin.x) ? scl.x = (nr - 1) / (pMax.x - pMin.x) : 1.0;
  scl.y = (pMax.y > pMin.y) ? scl.y = (nc - 1) / (pMax.y - pMin.y) : 1.0;

  /* fill matrix */
  mc = 0;
  for (i=0; i<n; i++){
    iX = (p1D[i].x - pMin.x) * scl.x;
    iY = (p1D[i].y - pMin.y) * scl.y;
    if (mat[iX][iY] < col-1){
      mat[iX][iY] += 1;
      if (mat[iX][iY] > mc){
	mc = mat[iX][iY];
      }
    }
  }

  /* write matrix */
  pgm_wr(ofp, mat, nr, nc, mc);
  
  /* return */
}

/*
 * @ pgm_r1D : write real vector
 * > none
 * + write vector in PGM format
 */

static void
pgm_r1D(
  FILE	      * ofp,			/* output file pointer */
  int		col,			/* number of colors to use */
  real1D	r1D,			/* vector to write */
  int		n			/* number of elements */
){
  int2D		mat;			/* output matrix */
  int		i;			/* loop index */
  real		rMin, rMax;		/* extrema */
  real		range;			/* range of values */
  real		scl;			/* scaling factor */

  /* scale */
  range = redReal1DExtrema(r1D, n, &rMin, &rMax);
  scl = (rMax > rMin) ? col / (rMax - rMin) : 1.0;

  /* fill */
  for (i=0; i<n; i++){
    mat[0][i] = scl * (r1D[i] - rMin);
  }

  /* write matrix */
  pgm_wr(ofp, mat, 1, n, col);

  /* return */
}

/*
 * @ pgm_r2D : write real matrix
 * > none
 * + write matrix in PGM format
 */

static void
pgm_r2D(
  FILE	      * ofp,			/* output file pointer */
  int		col,			/* number of colors to use */
  real2D	r2D,			/* matrix to write */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
){
  int2D		mat;			/* output matrix */
  int		r, c;			/* loop indices */
  real		rMin, rMax;		/* extrema */
  real		range;			/* range of values */
  real		scl;			/* scaling factor */

  /* scale */
  range = redReal2DExtrema(r2D, nr, nc, &rMin, &rMax);
  scl = (rMax > rMin) ? col / (rMax - rMin) : 1.0;

  /* fill */
  for (r=0; r<nr; r++){
    for (c=0; c<nc; c++){
      mat[r][c] = scl * (r2D[r][c] - rMin);
    }
  }

  /* write matrix */
  pgm_wr(ofp, mat, nr, nc, col);

  /* return */
}

/*
 * @ pgm_wr : write in PGM format
 * > none
 * + write matrix
 */

static void
pgm_wr(
  FILE	      * ofp,			/* output file pointer */
  int2D		mat,			/* matrix to write */
  int		nr,			/* row size */
  int		nc,			/* column size */
  int		col			/* maximum color */
){
  int		r, c;			/* indices */

  ASSERT(ofp != NULL);

  fprintf(ofp, "%s\n", PGM_MAGIC);
  fprintf(ofp, "%d\t%d\t%d\n", nr, nc, col);
  for (r=0; r<nr; r++){
    for (c=0; c<nc; c++){
      fprintf(ofp, "%d\n", mat[r][c]);
    }
  }

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
  printf("pgm\n");
  printf("\t-c num                      : number of colors\n");
  printf("\t-i filename                 : input file name\n");
  printf("\t-o filename                 : output file name\n");
  printf("\t-s rows cols                : matrix size (for point vector)\n");
  printf("\t-t {b2, i2, p1, r0, r1, r2} : object type\n");
  printf("\t-u                          : unformatted\n");
  /* return */
}
