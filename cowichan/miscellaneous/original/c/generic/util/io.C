/*==============================================================*/
/* generic/util/io.c : generic I/O utilities			*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "util.h"

/*--------------------------------------------------------------*/
/* private utility functions					*/
/*--------------------------------------------------------------*/

/*
 * @ io_checkLen : check length during read
 * > none
 */

static void
io_checkLen(
  char	      * context,
  char	      * filename,
  int		len
){
  CHECK((0 < len) && (len <= MAXEXT),
	fail(context, "illegal length read from file",
	     "filename", "%s", filename,
	     "length read", "%d", len,
	     "maximum length allowed", "%d", MAXEXT, NULL));
  /* return */
}

/*
 * @ io_rdClose : close input file (if not stdin)
 * > none
 */

static void
io_rdClose(
  FILE	      * file			/* to close */
){
  ASSERT(file != NULL);
  if (file != stdin) fclose(file);
  /* return */
}

/*
 * @ io_rdOpen : open input file, or connect to stdin
 * > file pointer
 */

static FILE *
io_rdOpen(
  char	      * caller,			/* top-level calling function */
  char	      * io_func,		/* I/O calling function */
  char	      * filename		/* file to open */
){
  FILE	      * fp;			/* file pointer */

  if (filename == NULL){
    fp = stdin;
  } else if ((fp = fopen(filename, "r")) == NULL){
    fail(io_func, "unable to open input file",
	 "caller", "%s", caller,
	 "file", "%s", filename, NULL);
  }

  return fp;
}

/*
 * @ io_wrClose : close output file (if not stdout)
 * > none
 */

static void
io_wrClose(
  FILE	      * file			/* to close */
){
  ASSERT(file != NULL);
  if (file != stdout) fclose(file);
  /* return */
}

/*
 * @ io_wrOpen : open output file, or connect to stdout
 * > file pointer
 */

static FILE *
io_wrOpen(
  char	      * caller,			/* top-level calling function */
  char	      * io_func,		/* I/O calling function */
  char	      * filename		/* file to open */
){
  FILE	      * fp;			/* file pointer */

  if (filename == NULL){
    fp = stdout;
  } else if ((fp = fopen(filename, "w")) == NULL){
    fail(io_func, "unable to open output file",
	 "caller", "%s", caller,
	 "file", "%s", filename, NULL);
  }

  return fp;
}

/*--------------------------------------------------------------*/
/* private formatted input functions				*/
/*--------------------------------------------------------------*/

/*
 * @ io_rdBool2DFmt : read boolean matrix
 * > none
 */

static void
io_rdBool2DFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  bool2D	mask,			/* mask to read */
  int	      * rptr,			/* row size pointer */
  int	      * cptr			/* column size pointer */
){
  static char * context = "io_rdBool2DFmt";
  FILE	      * fp;			/* input file */
  int		r, c, t;		/* indices and temp */

  fp = io_rdOpen(caller, context, filename);

  CHECK(fscanf(fp, "%d", rptr) == 1,
	fail(context, "premature end of file", NULL));
  CHECK(fscanf(fp, "%d", cptr) == 1,
	fail(context, "premature end of file", NULL));
  io_checkLen(context, filename, *rptr);
  io_checkLen(context, filename, *cptr);

  for (r=0; r<*rptr; r++) {
    for (c=0; c<*cptr; c++) {
      CHECK(fscanf(fp, "%d", &t) == 1,
	    fail(context, "premature end of file", NULL));
      mask[r][c] = t ? TRUE : FALSE;
    }
  }

  io_rdClose(fp);

  /* return */
}

/*
 * @ io_rdInt2DFmt : read integer matrix
 * > none
 */

static void
io_rdInt2DFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  int2D		matrix,			/* matrix to read */
  int	      * rptr,			/* row size pointer */
  int	      * cptr			/* column size pointer */
){
  static char * context = "io_rdInt2DFmt";
  FILE	      * fp;			/* input file */
  int		r, c;			/* indices */

  fp = io_rdOpen(caller, context, filename);

  CHECK(fscanf(fp, "%d", rptr) == 1,
	fail(context, "premature end of file", NULL));
  CHECK(fscanf(fp, "%d", cptr) == 1,
	fail(context, "premature end of file", NULL));
  io_checkLen(context, filename, *rptr);
  io_checkLen(context, filename, *cptr);

  for (r=0; r<*rptr; r++) {
    for (c=0; c<*cptr; c++) {
      CHECK(fscanf(fp, "%d", &matrix[r][c]) == 1,
	    fail(context, "premature end of file", NULL));
    }
  }

  io_rdClose(fp);

  /* return */
}

/*
 * @ io_rdPt1DFmt : read point vector
 * > none
 */

static void
io_rdPt1DFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  pt1D		vector,			/* matrix to read */
  int	      * nptr			/* size pointer */
){
  static char * context = "io_rdPt1DFmt";
  FILE	      * fp;			/* input file */
  int		i;			/* index */

  fp = io_rdOpen(caller, context, filename);

  CHECK(fscanf(fp, "%d", nptr) == 1,
	fail(context, "premature end of file", NULL));
  io_checkLen(context, filename, *nptr);

  for (i=0; i<*nptr; i++) {
    CHECK(fscanf(fp, FMT_PT_RD, &(vector[i].x), &(vector[i].y), &(vector[i].w)) == 3,
	  fail(context, "premature end of file", NULL));
  }

  io_rdClose(fp);

  /* return */
}

/*
 * @ io_rdReal0DFmt : read real scalar
 * > none
 */

static void
io_rdReal0DFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  real	      * val			/* value read */
){
  static char * context = "io_rdReal0DFmt";
  FILE	      * fp;			/* input file */

  fp = io_rdOpen(caller, context, filename);
  CHECK(fscanf(fp, FMT_REAL_RD, val) == 1,
	fail(context, "premature end of file", NULL));
  io_rdClose(fp);

  /* return */
}

/*
 * @ io_rdReal1DFmt : read real vector
 * > none
 */

static void
io_rdReal1DFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  real1D	vector,			/* matrix to read */
  int	      * nptr			/* size pointer */
){
  static char * context = "io_rdReal1DFmt";
  FILE	      * fp;			/* input file */
  int		i;			/* index */

  fp = io_rdOpen(caller, context, filename);

  CHECK(fscanf(fp, "%d", nptr) == 1,
	fail(context, "premature end of file", NULL));
  io_checkLen(context, filename, *nptr);

  for (i=0; i<*nptr; i++) {
    CHECK(fscanf(fp, FMT_REAL_RD, &vector[i]) == 1,
	  fail(context, "premature end of file", NULL));
  }

  io_rdClose(fp);

  /* return */
}

/*
 * @ io_rdReal2DFmt : read real matrix
 * > none
 */

static void
io_rdReal2DFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  real2D	matrix,			/* matrix to read */
  int	      * rptr,			/* row size pointer */
  int	      * cptr			/* column size pointer */
){
  static char * context = "io_rdReal2DFmt";
  FILE	      * fp;			/* input file */
  int		r, c;			/* indices */

  fp = io_rdOpen(caller, context, filename);

  CHECK(fscanf(fp, "%d", rptr) == 1,
	fail(context, "premature end of file", NULL));
  CHECK(fscanf(fp, "%d", cptr) == 1,
	fail(context, "premature end of file", NULL));
  io_checkLen(context, filename, *rptr);
  io_checkLen(context, filename, *cptr);

  for (r=0; r<*rptr; r++) {
    for (c=0; c<*cptr; c++) {
      CHECK(fscanf(fp, FMT_REAL_RD, &matrix[r][c]) == 1,
	    fail(context, "premature end of file", NULL));
    }
  }

  io_rdClose(fp);

  /* return */
}

/*--------------------------------------------------------------*/
/* private unformatted input functions				*/
/*--------------------------------------------------------------*/

/*
 * @ io_rdBool2DUnFmt : read boolean matrix
 * > none
 */

static void
io_rdBool2DUnFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  bool2D	mask,			/* mask to read */
  int	      * rptr,			/* row size pointer */
  int	      * cptr			/* column size pointer */
){
  static char * context = "io_rdBool2DUnFmt";
  FILE	      * fp;			/* input file */
  int		r;			/* index */

  fp = io_rdOpen(caller, context, filename);
  CHECK(fread((char *)rptr, sizeof(int), 1, fp) == 1,
	fail(context, "premature end of file", NULL));
  CHECK(fread((char *)cptr, sizeof(int), 1, fp) == 1,
	fail(context, "premature end of file", NULL));
  io_checkLen(context, filename, *rptr);
  io_checkLen(context, filename, *cptr);
  for (r=0; r<*rptr; r++){
    CHECK(fread((char *)mask[r], sizeof(bool), *cptr, fp) == *cptr,
	  fail(context, "premature end of file", NULL));
  }
  io_rdClose(fp);

  /* return */
}

/*
 * @ io_rdInt2DUnFmt : read integer matrix
 * > none
 */

static void
io_rdInt2DUnFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  int2D		matrix,			/* matrix to read */
  int	      * rptr,			/* row size pointer */
  int	      * cptr			/* column size pointer */
){
  static char * context = "io_rdInt2DUnFmt";
  FILE	      * fp;			/* input file */
  int		r;			/* index */

  fp = io_rdOpen(caller, context, filename);
  CHECK(fread((char *)rptr, sizeof(int), 1, fp) == 1,
	fail(context, "premature end of file", NULL));
  CHECK(fread((char *)cptr, sizeof(int), 1, fp) == 1,
	fail(context, "premature end of file", NULL));
  io_checkLen(context, filename, *rptr);
  io_checkLen(context, filename, *cptr);
  for (r=0; r<*rptr; r++){
    CHECK(fread((char *)matrix[r], sizeof(int), *cptr, fp) == *cptr,
	  fail(context, "premature end of file", NULL));
  }
  io_rdClose(fp);

  /* return */
}

/*
 * @ io_rdPt1DUnFmt : read point vector
 * > none
 */

static void
io_rdPt1DUnFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  pt1D		vector,			/* matrix to read */
  int	      * nptr			/* size pointer */
){
  static char * context = "io_rdPt1DUnFmt";
  FILE	      * fp;			/* input file */

  fp = io_rdOpen(caller, context, filename);
  CHECK(fread((char *)nptr, sizeof(int), 1, fp) == 1,
	fail(context, "premature end of file", NULL));
  io_checkLen(context, filename, *nptr);
  CHECK(fread((char *)vector, sizeof(pt), *nptr, fp) == *nptr,
	fail(context, "premature end of file", NULL));
  io_rdClose(fp);

  /* return */
}

/*
 * @ io_rdReal0DUnFmt : read real scalar
 * > none
 */

static void
io_rdReal0DUnFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  real	      * val			/* value read */
){
  static char * context = "io_rdReal0DUnFmt";
  FILE	      * fp;			/* input file */

  fp = io_rdOpen(caller, context, filename);
  CHECK(fread((char *)val, sizeof(real), 1, fp) == 1,
	fail(context, "premature end of file", NULL));
  io_rdClose(fp);

  /* return */
}

/*
 * @ io_rdReal1DUnFmt : read real vector
 * > none
 */

static void
io_rdReal1DUnFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  real1D	vector,			/* matrix to read */
  int	      * nptr			/* size pointer */
){
  static char * context = "io_rdReal1DUnFmt";
  FILE	      * fp;			/* input file */

  fp = io_rdOpen(caller, context, filename);
  CHECK(fread((char *)nptr, sizeof(int), 1, fp) == 1,
	fail(context, "premature end of file", NULL));
  io_checkLen(context, filename, *nptr);
  CHECK(fread((char *)vector, sizeof(real), *nptr, fp) == *nptr,
	fail(context, "premature end of file", NULL));
  io_rdClose(fp);

  /* return */
}

/*
 * @ io_rdReal2DUnFmt : read real matrix
 * > none
 */

static void
io_rdReal2DUnFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  real2D	matrix,			/* matrix to read */
  int	      * rptr,			/* row size pointer */
  int	      * cptr			/* column size pointer */
){
  static char * context = "io_rdReal2DUnFmt";
  FILE	      * fp;			/* input file */
  int		r;			/* index */

  fp = io_rdOpen(caller, context, filename);
  CHECK(fread((char *)rptr, sizeof(int), 1, fp) == 1,
	fail(context, "premature end of file", NULL));
  CHECK(fread((char *)cptr, sizeof(int), 1, fp) == 1,
	fail(context, "premature end of file", NULL));
  io_checkLen(context, filename, *rptr);
  io_checkLen(context, filename, *cptr);
  for (r=0; r<*rptr; r++){
    CHECK(fread((char *)matrix[r], sizeof(real), *cptr, fp) == *cptr,
	  fail(context, "premature end of file", NULL));
  }
  io_rdClose(fp);

  /* return */
}

/*--------------------------------------------------------------*/
/* private formatted output functions				*/
/*--------------------------------------------------------------*/

/*
 * @ io_wrBool2DFmt : write boolean matrix
 * > none
 */

static void
io_wrBool2DFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of output file */
  bool2D	mask,			/* mask to write */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
){
  static char * context = "io_wrBool2DFmt";
  FILE	      * fp;			/* output file */
  int		r, c;			/* indices */

  fp = io_wrOpen(caller, context, filename);

  fprintf(fp, "%d %d\n", nr, nc);
  for (r=0; r<nr; r++){
    for (c=0; c<nc; c++){
      fprintf(fp, "%d\n", mask[r][c]);
    }
  }

  io_wrClose(fp);

  /* return */
}

/*
 * @ io_wrInt2DFmt : write integer matrix
 * > none
 */

static void
io_wrInt2DFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of output file */
  int2D		matrix,			/* matrix to write */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
){
  static char * context = "io_wrInt2DFmt";
  FILE	      * fp;			/* output file */
  int		r, c;			/* indices */

  fp = io_wrOpen(caller, context, filename);

  fprintf(fp, "%d %d\n", nr, nc);
  for (r=0; r<nr; r++){
    for (c=0; c<nc; c++){
      fprintf(fp, "%d\n", matrix[r][c]);
    }
  }

  io_wrClose(fp);

  /* return */
}

/*
 * @ io_wrPt1DFmt : write point vector
 * > none
 */

static void
io_wrPt1DFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of output file */
  pt1D		vector,			/* vector to write */
  int		n			/* size */
){
  static char * context = "io_wrPt1DFmt";
  FILE	      * fp;			/* output file */
  int		i;			/* index */

  fp = io_wrOpen(caller, context, filename);

  fprintf(fp, "%d\n", n);
  for (i=0; i<n; i++){
    fprintf(fp, FMT_PT_WR, vector[i].x, vector[i].y, vector[i].w);
  }

  io_wrClose(fp);

  /* return */
}

/*
 * @ io_wrReal0DFmt : write real scalar
 * > none
 */

static void
io_wrReal0DFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of output file */
  real		val			/* value to write */
){
  static char * context = "io_wrReal0DFmt";
  FILE	      * fp;			/* output file */

  fp = io_wrOpen(caller, context, filename);
  fprintf(fp, FMT_REAL_WR, val);
  io_wrClose(fp);

  /* return */
}

/*
 * @ io_wrReal1DFmt : write real vector
 * > none
 */

static void
io_wrReal1DFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of output file */
  real1D	vector,			/* vector to write */
  int		n			/* size */
){
  static char * context = "io_wrReal1DFmt";
  FILE	      * fp;			/* output file */
  int		i;			/* index */

  fp = io_wrOpen(caller, context, filename);

  fprintf(fp, "%d\n", n);
  for (i=0; i<n; i++){
    fprintf(fp, FMT_REAL_WR, vector[i]);
  }

  io_wrClose(fp);

  /* return */
}

/*
 * @ io_wrReal2DFmt : write real matrix
 * > none
 */

static void
io_wrReal2DFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of output file */
  real2D	matrix,			/* matrix to write */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
){
  static char * context = "io_wrReal2DFmt";
  FILE	      * fp;			/* output file */
  int		r, c;			/* indices */

  fp = io_wrOpen(caller, context, filename);

  fprintf(fp, "%d %d\n", nr, nc);
  for (r=0; r<nr; r++){
    for (c=0; c<nc; c++){
      fprintf(fp, FMT_REAL_WR, matrix[r][c]);
    }
  }

  io_wrClose(fp);

  /* return */
}

/*--------------------------------------------------------------*/
/* private unformatted output functions				*/
/*--------------------------------------------------------------*/

/*
 * @ io_wrBool2DUnFmt : write boolean matrix
 * > none
 */

static void
io_wrBool2DUnFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of output file */
  bool2D	mask,			/* matrix to write */
  int		nr,			/* row size */
  int		nc			/* column size */
){
  static char * context = "io_wrBool2DUnFmt";
  FILE	      * fp;			/* output file */
  int		r;			/* index */

  fp = io_wrOpen(caller, context, filename);
  fwrite((char *)&nr, sizeof(int), 1, fp);
  fwrite((char *)&nc, sizeof(int), 1, fp);
  for (r=0; r<nr; r++){
    fwrite((char *)mask[r], sizeof(bool), nc, fp);
  }
  io_wrClose(fp);

  /* return */
}

/*
 * @ io_wrInt2DUnFmt : write integer matrix
 * > none
 */

static void
io_wrInt2DUnFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of output file */
  int2D		matrix,			/* matrix to write */
  int		nr,			/* row size */
  int		nc			/* column size */
){
  static char * context = "io_wrInt2DUnFmt";
  FILE	      * fp;			/* output file */
  int		r;			/* index */

  fp = io_wrOpen(caller, context, filename);
  fwrite((char *)&nr, sizeof(int), 1, fp);
  fwrite((char *)&nc, sizeof(int), 1, fp);
  for (r=0; r<nr; r++){
    fwrite((char *)matrix[r], sizeof(int), nc, fp);
  }
  io_wrClose(fp);

  /* return */
}

/*
 * @ io_wrPt1DUnFmt : write point vector
 * > none
 */

static void
io_wrPt1DUnFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of output file */
  pt1D		vector,			/* matrix to write */
  int		n			/* size */
){
  static char * context = "io_wrPt1DUnFmt";
  FILE	      * fp;			/* output file */

  fp = io_wrOpen(caller, context, filename);
  fwrite((char *)&n, sizeof(int), 1, fp);
  fwrite((char *)vector, sizeof(pt), n, fp);
  io_wrClose(fp);

  /* return */
}

/*
 * @ io_wrReal0DUnFmt : write real scalar
 * > none
 */

static void
io_wrReal0DUnFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of output file */
  real		val			/* value to write */
){
  static char * context = "io_wrReal0DUnFmt";
  FILE	      * fp;			/* output file */

  fp = io_wrOpen(caller, context, filename);
  fwrite((char *)&val, sizeof(real), 1, fp);
  io_wrClose(fp);

  /* return */
}

/*
 * @ io_wrReal1DUnFmt : write real vector
 * > none
 */

static void
io_wrReal1DUnFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of output file */
  real1D	vector,			/* matrix to write */
  int		n			/* size */
){
  static char * context = "io_wrReal1DUnFmt";
  FILE	      * fp;			/* output file */

  fp = io_wrOpen(caller, context, filename);
  fwrite((char *)&n, sizeof(int), 1, fp);
  fwrite((char *)vector, sizeof(real), n, fp);
  io_wrClose(fp);

  /* return */
}

/*
 * @ io_wrReal2DUnFmt : write real matrix
 * > none
 */

static void
io_wrReal2DUnFmt(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of output file */
  real2D	matrix,			/* matrix to write */
  int		nr,			/* row size */
  int		nc			/* column size */
){
  static char * context = "io_wrReal2DUnFmt";
  FILE	      * fp;			/* output file */
  int		r;			/* index */

  fp = io_wrOpen(caller, context, filename);
  fwrite((char *)&nr, sizeof(int), 1, fp);
  fwrite((char *)&nc, sizeof(int), 1, fp);
  for (r=0; r<nr; r++){
    fwrite((char *)matrix[r], sizeof(real), nc, fp);
  }
  io_wrClose(fp);

  /* return */
}

/*--------------------------------------------------------------*/
/* externally-visible function pointers				*/
/*--------------------------------------------------------------*/

void (*io_rdBool2D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  bool2D	mask,			/* mask to read */
  int	      * rptr,			/* row size pointer */
  int	      * cptr			/* column size pointer */
) = io_rdBool2DFmt;

void (*io_rdInt2D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  int2D		matrix,			/* matrix to read */
  int	      * rptr,			/* row size pointer */
  int	      * cptr			/* column size pointer */
) = io_rdInt2DFmt;

void (*io_rdPt1D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  pt1D		vector,			/* matrix to read */
  int	      * nptr			/* size pointer */
) = io_rdPt1DFmt;

void (*io_rdReal0D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  real	      * val			/* value read */
) = io_rdReal0DFmt;

void (*io_rdReal1D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  real1D	vector,			/* matrix to read */
  int	      * nptr			/* size pointer */
) = io_rdReal1DFmt;

void (*io_rdReal2D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of input file */
  real2D	matrix,			/* matrix to read */
  int	      * rptr,			/* row size pointer */
  int	      * cptr			/* column size pointer */
) = io_rdReal2DFmt;

void (*io_wrBool2D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of output file */
  bool2D	mask,			/* mask to write */
  int		nr,			/* row size */
  int		nc			/* column size */
) = io_wrBool2DFmt;

void (*io_wrInt2D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of output file */
  int2D		matrix,			/* matrix to write */
  int		nr,			/* row size */
  int		nc			/* column size */
) = io_wrInt2DFmt;

void (*io_wrPt1D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of output file */
  pt1D		vector,			/* matrix to write */
  int		n			/* size */
) = io_wrPt1DFmt;

void (*io_wrReal0D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of output file */
  real		val			/* value to write */
) = io_wrReal0DFmt;

void (*io_wrReal1D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of output file */
  real1D	vector,			/* matrix to write */
  int		n			/* size */
) = io_wrReal1DFmt;

void (*io_wrReal2D)(
  char	      * caller,			/* calling function */
  char	      * filename,		/* name of output file */
  real2D	matrix,			/* matrix to write */
  int		nr,			/* row size */
  int		nc			/* column size */
) = io_wrReal2DFmt;

/*--------------------------------------------------------------*/
/* externally-visible functions					*/
/*--------------------------------------------------------------*/

/*
 * @ io_init : initialize I/O
 * > none
 */

void
io_init(
  int		formatted		/* use formatted I/O? */
){
  if (formatted){
    /* read */
    io_rdBool2D		= io_rdBool2DFmt;
    io_rdInt2D		= io_rdInt2DFmt;
    io_rdPt1D		= io_rdPt1DFmt;
    io_rdReal0D		= io_rdReal0DFmt;
    io_rdReal1D		= io_rdReal1DFmt;
    io_rdReal2D		= io_rdReal2DFmt;
    /* write */
    io_wrBool2D		= io_wrBool2DFmt;
    io_wrInt2D		= io_wrInt2DFmt;
    io_wrPt1D		= io_wrPt1DFmt;
    io_wrReal0D		= io_wrReal0DFmt;
    io_wrReal1D		= io_wrReal1DFmt;
    io_wrReal2D		= io_wrReal2DFmt;
  } else {
    /* read */
    io_rdBool2D		= io_rdBool2DUnFmt;
    io_rdInt2D		= io_rdInt2DUnFmt;
    io_rdPt1D		= io_rdPt1DUnFmt;
    io_rdReal0D		= io_rdReal0DUnFmt;
    io_rdReal1D		= io_rdReal1DUnFmt;
    io_rdReal2D		= io_rdReal2DUnFmt;
    /* write */
    io_wrBool2D		= io_wrBool2DUnFmt;
    io_wrInt2D		= io_wrInt2DUnFmt;
    io_wrPt1D		= io_wrPt1DUnFmt;
    io_wrReal0D		= io_wrReal0DUnFmt;
    io_wrReal1D		= io_wrReal1DUnFmt;
    io_wrReal2D		= io_wrReal2DUnFmt;
  }
  /* return */
}
