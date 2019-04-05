/*==============================================================*/
/* generic/hdr/util.h : utility specific definitions		*/
/*==============================================================*/

#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if SUNOS5
# if GRAPHICS
#  include <vogle.h>
# endif
# if IEEE
#  include <sunmath.h>
# endif
#elif NUMA
#endif

/* other files */

#include "type.h"
#include "generic.h"

/* memory management */

#define ALLOC(ptr_, type_, num_, caller_, obj_)\
  if ((ptr_ = (type_ *)calloc(num_, sizeof(type_))) == (type_ *)NULL){ \
    fprintf(stderr, "\tallocation failed in %s at %d for %s in %s\n", \
  	  __FILE__, __LINE__, obj_, caller_); \
    exit(1); \
  }

#define FREE(ptr_) free(ptr_)
