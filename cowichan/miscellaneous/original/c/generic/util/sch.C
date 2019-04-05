/*==============================================================*/
/* generic/util/sch.c : generic scheduling utilities		*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "util.h"

/*--------------------------------------------------------------*/
/* private function prototypes					*/
/*--------------------------------------------------------------*/

static bool
sch_fail(
  int		n,			/* number of threads */
  int		i,			/* this thread's ID */
  int		base,			/* base of loop section */
  int		lim,			/* limit of loop section */
  int	      * start,			/* loop start */
  int	      * end,			/* loop end */
  int	      * stride			/* loop stride */
);

/*--------------------------------------------------------------*/
/* global data							*/
/*--------------------------------------------------------------*/

bool (*sch_work)(
  int		n,			/* number of threads */
  int		i,			/* this thread's ID */
  int		base,			/* base of loop section */
  int		lim,			/* limit of loop section */
  int	      * start,			/* loop start */
  int	      * end,			/* loop end */
  int	      * stride			/* loop stride */
) = sch_fail;

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

/*
 * @ sch_block : block scheduling function
 * > none
 * + set start/end/stride values for block work allocation
 */

bool
sch_block(
  int		n,			/* number of threads */
  int		i,			/* this thread's ID */
  int		base,			/* base of loop section */
  int		lim,			/* limit of loop section */
  int	      * start,			/* loop start */
  int	      * end,			/* loop end */
  int	      * stride			/* loop stride */
){
  int		nl;			/* number of loops */
  int		num;			/* number to do */
  int		extra;			/* spillage */

  nl    = lim - base;
  num   = nl / n;
  extra = nl % n;

  if ((nl <= 0) || (i >= nl)){		/* do nothing */
    *start = 0;
    *end = -1;
    *stride = 1;
  } else {				/* do share of work */
    if (i < extra){
      num += 1;
      *start = base + i * num;
    } else {
      *start = base + (extra * (num + 1)) + ((i - extra) * num);
    }
    *end = *start + num;
    *stride = 1;
  }

  return (*end != -1);
}

/*
 * @ sch_cyclic : cyclic scheduling function
 * > none
 * + set start/end/stride values for cyclic work allocation
 */

bool
sch_cyclic(
  int		n,			/* number of threads */
  int		i,			/* this thread's ID */
  int		base,			/* base of loop section */
  int		lim,			/* limit of loop section */
  int	      * start,			/* loop start */
  int	      * end,			/* loop end */
  int	      * stride			/* loop stride */
){
  int		nl;			/* number of loops */

  nl = lim - base;

  if ((nl <= 0) || (i >= nl)){		/* do nothing */
    *start = 0;
    *end = -1;
    *stride = 1;
  } else {				/* do share of work */
    *start  = base + i;
    *end    = lim;
    *stride = n;
  }

  return (*end != -1);
}

/*
 * @ sch_init : initialize scheduling function
 * > none
 * + set up work allocation function
 */

void
sch_init(
  dataDist_e	dd			/* data distribution */
){
  CHECK(sch_work == sch_fail,
	fail("sch_init", "scheduling already initialized", NULL));

  switch(dd){
   case dataDist_block :
    sch_work = sch_block;
    break;
   case dataDist_cyclic :
    sch_work = sch_cyclic;
    break;
   default :
    ASSERT(FALSE);
    break;
  }

  /* return */
}

/*--------------------------------------------------------------*/
/* private functions						*/
/*--------------------------------------------------------------*/

/*
 * @ sch_fail : scheduling failure (uninitialized)
 * > none
 * + cause error
 */

static bool
sch_fail(
  int		n,			/* number of threads */
  int		i,			/* this thread's ID */
  int		base,			/* base of loop section */
  int		lim,			/* limit of loop section */
  int	      * start,			/* loop start */
  int	      * end,			/* loop end */
  int	      * stride			/* loop stride */
){
  fail("sch_fail", "data/work distribution not specified", NULL);
  return FALSE;
}
