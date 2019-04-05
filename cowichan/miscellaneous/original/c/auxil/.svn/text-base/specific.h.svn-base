/*==============================================================*/
/* aux/specific.h : auxiliary specific definitions		*/
/*==============================================================*/

#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#if SUNOS5
# if GRAPHICS
#  include <vogle.h>
# endif
# if IEEE
#  include <sunmath.h>
# endif
#elif NUMA
#endif

/*
 * general definitions
 */

#include "type.h"
#include "generic.h"

/*
 * random number generation
 */

#define RAND_VAL lrand48
#define RAND_INIT(seed_) srand48(seed_)

/*
 * memory management
 */

#define ALLOC(ptr_, type_, num_, caller_, obj_)\
  if ((ptr_ = (type_ *)calloc(num_, sizeof(type_))) == NULL){ \
    fprintf(stderr, "\tallocation failed in %s at %d for %s in %s\n", \
  	  __FILE__, __LINE__, obj_, caller_); \
    exit(1); \
  }

#define FREE(ptr_) free(ptr_)

/* file types */

typedef enum {
  ft_i2D_b,				/* Boolean matrix */
  ft_i2D,				/* integer matrix */
  ft_p1D,				/* point vector */
  ft_r0D,				/* real scalar */
  ft_r1D,				/* real vector */
  ft_r2D,				/* real matrix */
  ft_e_sz				/* number of file types */
} ft_e;

/* i/o formats */

#define FMT_REAL_3_NL "%24.16e\t%24.16e\t%24.16e\n"

/* miscellaneous */

#define BLANKS " \t\n"
#define MAXLEN 4096
#define PRINT_FORMAT "%8d  %s\n"

/* support function prototypes */

char *
aux_ft2Str(
  ft_e		ft			/* file type to convert */
);
ft_e
aux_str2Ft(
  char	      * str			/* string to convert */
);
int
redInt2DExtrema(
  int2D		i2D,			/* matrix to reduce */
  int		nr,			/* row size */
  int		nc,			/* column size */
  int	      * iMin,			/* minimum value */
  int	      * iMax			/* maximum value */
);
void
redPt1DExtrema(
  pt1D		p1D,			/* vector to reduce */
  int		n,			/* vector size */
  pt	      * pMin,			/* minimum values */
  pt	      * pMax			/* maximum values */
);
real
redReal1DExtrema(
  real1D	r1D,			/* vector to reduce */
  int		n,			/* vector size */
  real	      * rMin,			/* minimum value */
  real	      * rMax			/* maximum value */
);
real
redReal2DExtrema(
  real2D	r2D,			/* vector to reduce */
  int		nr,			/* row size */
  int		nc,			/* column size */
  real	      * rMin,			/* minimum value */
  real	      * rMax			/* maximum value */
);
