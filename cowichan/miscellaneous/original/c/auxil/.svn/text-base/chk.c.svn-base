/*==============================================================*/
/* aux/chk.c : auxiliary Cowichan file comparator		*/
/*==============================================================*/

#include "specific.h"

/*--------------------------------------------------------------*/
/* global data structures					*/
/*--------------------------------------------------------------*/

bool		DiffOnly = FALSE;	/* report only difference */

/*--------------------------------------------------------------*/
/* private function prototypes					*/
/*--------------------------------------------------------------*/

static void
chk_i2D(
  FILE	      * ofp,			/* output file */
  int2D		i2DLeft,		/* left matrix */
  int2D		i2DRight,		/* right matrix */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
);
static void
chk_p1D(
  FILE	      * ofp,			/* output file */
  pt1D		p1DLeft,		/* left vector */
  pt1D		p1DRight,		/* right vector */
  int		n			/* number of elements */
);
static void
chk_r0D(
  FILE	      * ofp,			/* output file */
  real		r0DLeft,		/* left value */
  real		r0DRight		/* right value */
);
static void
chk_r1D(
  FILE	      * ofp,			/* output file */
  real1D	r1DLeft,		/* left vector */
  real1D	r1DRight,		/* right vector */
  int		n			/* number of elements */
);
static void
chk_r2D(
  FILE	      * ofp,			/* output file */
  real2D	r2DLeft,		/* left matrix */
  real2D	r2DRight,		/* right matrix */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
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
  static char * context = "main(chk)";
  int		argd = 1;		/* argument index */
  char	      * infnLeft = NULL;	/* left input file name */
  char	      * infnRight = NULL;	/* right input file name */
  int2D		i2DLeft, i2DRight;	/* integer/boolean matrices */
  pt1D		p1DLeft, p1DRight;	/* point vectors */
  real1D	r1DLeft, r1DRight;	/* real vectors */
  real2D	r2DLeft, r2DRight;	/* real matrices */
  real		r0DLeft, r0DRight;	/* real scalars */
  ft_e		ft = ft_e_sz;		/* file type */
  int		nLeft, nRight;		/* vector sizes */
  int		nrLeft, nrRight;	/* row sizes */
  int		ncLeft, ncRight;	/* column sizes */

  /* user wants help */
  if (argc == 1){
    usage();
    exit(0);
  }

  /* arguments */
  while (argd < argc){
    if (argv[argd][0] != '-'){
      if (infnLeft == NULL){
	infnLeft = argv[argd++];
      } else if (infnRight == NULL){
	infnRight = argv[argd++];
      } else {
	fail(context, "bad argument",
	     "argument", "%s", argv[argd], NULL);
      }
    } else {
      switch(argv[argd][1]){
       case 'd' :			/* difference only */
	DiffOnly = TRUE;
	argd += 1;
	break;
       case 't' :			/* type */
	ft = aux_str2Ft(arg_str(context, argc, argv, argd+1, argv[argd]));
	argd += 2;
	break;
       case 'u' :			/* unformatted */
	io_init(FALSE);
	argd += 1;
	break;
       default :
	fail(context, "unknown flag", "flag", "%s", argv[argd], NULL);
	break;
      }
    }
  }

  /* check */
  CHECK(ft < ft_e_sz,
	fail(context, "file type not set", NULL));

  /* processing */
  switch(ft){
   case ft_i2D :
   case ft_i2D_b :
    io_rdInt2D(context, infnLeft, i2DLeft, &nrLeft, &ncLeft);
    io_rdInt2D(context, infnRight, i2DRight, &nrRight, &ncRight);
    CHECK((nrLeft == nrRight) && (ncLeft == ncRight),
	  fail(context, "file size mis-match",
	       "left file", "%s", infnLeft,
	       "right file", "%s", infnRight,
	       NULL));
    chk_i2D(stdout, i2DLeft, i2DRight, nrLeft, ncLeft);
    break;
   case ft_p1D :
    io_rdPt1D(context, infnLeft, p1DLeft, &nLeft);
    io_rdPt1D(context, infnRight, p1DRight, &nRight);
    CHECK(nLeft == nRight,
	  fail(context, "file size mis-match",
	       "left file", "%s", infnLeft,
	       "right file", "%s", infnRight,
	       NULL));
    chk_p1D(stdout, p1DLeft, p1DRight, nLeft);
    break;
   case ft_r0D :
    io_rdReal0D(context, infnLeft, &r0DLeft);
    io_rdReal0D(context, infnRight, &r0DRight);
    chk_r0D(stdout, r0DLeft, r0DRight);
    break;
   case ft_r1D :
    io_rdReal1D(context, infnLeft, r1DLeft, &nLeft);
    io_rdReal1D(context, infnRight, r1DRight, &nRight);
    CHECK(nLeft == nRight,
	  fail(context, "file size mis-match",
	       "left file", "%s", infnLeft,
	       "right file", "%s", infnRight,
	       NULL));
    chk_r1D(stdout, r1DLeft, r1DRight, nLeft);
    break;
   case ft_r2D :
    io_rdReal2D(context, infnLeft, r2DLeft, &nrLeft, &ncLeft);
    io_rdReal2D(context, infnRight, r2DRight, &nrRight, &ncRight);
    CHECK((nrLeft == nrRight) && (ncLeft == ncRight),
	  fail(context, "file size mis-match",
	       "left file", "%s", infnLeft,
	       "right file", "%s", infnRight,
	       NULL));
    chk_r2D(stdout, r2DLeft, r2DRight, nrLeft, ncLeft);
    break;
   default :
    ASSERT(FALSE);
    break;
  }

  return 0;
}

/*--------------------------------------------------------------*/
/* private functions						*/
/*--------------------------------------------------------------*/

/*
 * @ chk_i2D : check integer matrices
 * > none
 * + print difference
 */

static void
chk_i2D(
  FILE	      * ofp,			/* output file */
  int2D		i2DLeft,		/* left matrix */
  int2D		i2DRight,		/* right matrix */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
){
  int		r, c;			/* loop indices */
  int		vLo, vHi, vDiff;	/* minimum, maximum, and difference */
  int		d;			/* elementwise difference */

  vLo = vHi = i2DLeft[0][0];
  vDiff = 0;
  for (r=0; r<nr; r++){
    for (c=0; c<nc; c++){
      if (i2DLeft[r][c] < vLo)  vLo = i2DLeft[r][c];
      if (i2DRight[r][c] < vLo) vLo = i2DRight[r][c];
      if (i2DLeft[r][c] > vHi)  vHi = i2DLeft[r][c];
      if (i2DRight[r][c] > vHi) vHi = i2DRight[r][c];
      d = i2DLeft[r][c]-i2DRight[r][c];
      if (d < 0) d = -d;
      if (d > vDiff) vDiff = d;
    }
  }

  if (DiffOnly){
    fprintf(ofp, "%d\n", vDiff);
  } else {
    fprintf(ofp, "min\tmax\tdiff\n%d\t%d\t%d\n", vLo, vHi, vDiff);
  }

  /* return */
}

/*
 * @ chk_p1D : check point vectors
 * > none
 * + print difference
 */

static void
chk_p1D(
  FILE	      * ofp,			/* output file */
  pt1D		p1DLeft,		/* left vector */
  pt1D		p1DRight,		/* right vector */
  int		n			/* number of elements */
){
  int		i;			/* loop indices */
  real		vLo, vHi, vDiff;	/* minimum, maximum, and difference */
  real		d;			/* elementwise difference */

  vLo = vHi = p1DLeft[0].x;
  vDiff = 0;
  for (i=0; i<n; i++){
    if (p1DLeft[i].x < vLo)  vLo = p1DLeft[i].x;
    if (p1DRight[i].x < vLo) vLo = p1DRight[i].x;
    if (p1DLeft[i].x > vHi)  vHi = p1DLeft[i].x;
    if (p1DRight[i].x > vHi) vHi = p1DRight[i].x;
    if (p1DLeft[i].y < vLo)  vLo = p1DLeft[i].y;
    if (p1DRight[i].y < vLo) vLo = p1DRight[i].y;
    if (p1DLeft[i].y > vHi)  vHi = p1DLeft[i].y;
    if (p1DRight[i].y > vHi) vHi = p1DRight[i].y;
    d = ptDist(&(p1DLeft[i]), &(p1DRight[i]));
    if (d > vDiff) vDiff = d;
  }

  if (DiffOnly){
    fprintf(ofp, FMT_REAL_WR, vDiff);
  } else {
    fprintf(ofp, "min\tmax\tdiff\t");
    fprintf(ofp, FMT_REAL_3_NL, vLo, vHi, vDiff);
  }

  /* return */
}

/*
 * @ chk_r0D : check real scalars
 * > none
 * + print difference
 */

static void
chk_r0D(
  FILE	      * ofp,			/* output file */
  real		r0DLeft,		/* left vector */
  real		r0DRight		/* right vector */
){
  real		d;			/* elementwise difference */

  d = r0DLeft - r0DRight;
  if (d < 0) d = -d;

  if (DiffOnly){
    fprintf(ofp, FMT_REAL_WR, d);
  } else {
    fprintf(ofp, "diff\t");
    fprintf(ofp, FMT_REAL_WR, d);
  }

  /* return */
}

/*
 * @ chk_r1D : check real vectors
 * > none
 * + print difference
 */

static void
chk_r1D(
  FILE	      * ofp,			/* output file */
  real1D	r1DLeft,		/* left vector */
  real1D	r1DRight,		/* right vector */
  int		n			/* number of elements */
){
  int		i;			/* loop indices */
  real		vLo, vHi, vDiff;	/* minimum, maximum, and difference */
  real		d;			/* elementwise difference */

  vLo = vHi = r1DLeft[0];
  vDiff = 0;
  for (i=0; i<n; i++){
    if (r1DLeft[i] < vLo)  vLo = r1DLeft[i];
    if (r1DRight[i] < vLo) vLo = r1DRight[i];
    if (r1DLeft[i] > vHi)  vHi = r1DLeft[i];
    if (r1DRight[i] > vHi) vHi = r1DRight[i];
    d = r1DLeft[i]-r1DRight[i];
    if (d < 0) d = -d;
    if (d > vDiff) vDiff = d;
  }

  if (DiffOnly){
    fprintf(ofp, FMT_REAL_WR, vDiff);
  } else {
    fprintf(ofp, "min\tmax\tdiff\t");
    fprintf(ofp, FMT_REAL_3_NL, vLo, vHi, vDiff);
  }

  /* return */
}

/*
 * @ chk_r2D : check real matrices
 * > none
 * + print difference
 */

static void
chk_r2D(
  FILE	      * ofp,			/* output file */
  real2D	r2DLeft,		/* left matrix */
  real2D	r2DRight,		/* right matrix */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
){
  int		r, c;			/* loop indices */
  real		vLo, vHi, vDiff;	/* minimum, maximum, and difference */
  real		d;			/* elementwise difference */

  vLo = vHi = r2DLeft[0][0];
  vDiff = 0;
  for (r=0; r<nr; r++){
    for (c=0; c<nc; c++){
      if (r2DLeft[r][c] < vLo)  vLo = r2DLeft[r][c];
      if (r2DRight[r][c] < vLo) vLo = r2DRight[r][c];
      if (r2DLeft[r][c] > vHi)  vHi = r2DLeft[r][c];
      if (r2DRight[r][c] > vHi) vHi = r2DRight[r][c];
      d = r2DLeft[r][c]-r2DRight[r][c];
      if (d < 0) d = -d;
      if (d > vDiff) vDiff = d;
    }
  }

  if (DiffOnly){
    fprintf(ofp, FMT_REAL_WR, vDiff);
  } else {
    fprintf(ofp, "min\tmax\tdiff\t");
    fprintf(ofp, FMT_REAL_3_NL, vLo, vHi, vDiff);
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
  printf("chk filename filename\n");
  printf("\t-t {b2, i2, p1, r0, r1, r2} : object type\n");
  printf("\t-u                          : unformatted\n");
  /* return */
}
