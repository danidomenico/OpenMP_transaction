/*==============================================================*/
/* bar/hdr/specific.h : barrier specific definitions		*/
/*==============================================================*/

/*
 * system header files
 */

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
# include <thread.h>
#elif NUMA
#endif

/*
 * general definitions
 */

#include "type.h"
#include "generic.h"
#include "gfx.h"

/*
 * types
 */

#if SUNOS5
typedef void  * thr_f(void *);		/* threading function */
#elif NUMA
typedef void	thr_f(void *);
#endif

/*
 * memory management
 */

#define ALLOC(ptr_, type_, num_, caller_, obj_)\
  if ((ptr_ = (type_ *)calloc(num_, sizeof(type_))) == (type_ *)NULL){ \
    fprintf(stderr, "\tallocation failed in %s at %d for %s in %s\n", \
  	  __FILE__, __LINE__, obj_, caller_); \
    exit(1); \
  }

#define FREE(ptr_) free(ptr_)

#if SUNOS5
# define THR_DEF void *
# define THR_END(args) return thr_end(args)
#elif NUMA
# define THR_DEF void
# define THR_END(args) thr_end(args)
#endif

/*
 * global variables
 */

#if NUMA
extern BARDEC(GlobalBar);
#endif
extern dataDist_e	DataDist;
extern int		ParWidth;

/* controls */

#define MASTER(t_) ((t_) == 0)

/* argument access macros */

#define TG_bool2D(a_, i_)	(bool2D *)(a_[i_])
#define TG_int(a_, i_)		(int)(a_[i_])
#define TG_int1D(a_, i_)	(int1D *)(a_[i_])
#define TG_int2D(a_, i_)	(int2D *)(a_[i_])
#define TG_pt1D(a_, i_)		(pt1D *)(a_[i_])
#define TG_real_p(a_, i_)	(real *)(a_[i_])
#define TG_real1D(a_, i_)	(real1D *)(a_[i_])
#define TG_real2D(a_, i_)	(real2D *)(a_[i_])
#define TG_void_p(a_, i_)	(a_[i_])

#define TP_any(v_, i_, x_)	v_[i_] = (void *)(x_)

/*
 * prototypes
 */

#include "proto.h"
