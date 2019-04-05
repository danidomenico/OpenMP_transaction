/*==============================================================*/
/* generic/util/arg.c : generic argument handling utilities	*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "util.h"

/*
 * @ arg_dataDist : get data distribution argument
 * > argument value
 */

dataDist_e
arg_dataDist(
  char	      * caller,			/* calling function */
  int		argc,			/* argument count */
  char	      * argv[],			/* argument vector */
  int		argd,			/* current argument index */
  char	      * flag			/* argument flag */
){
  CHECK(argd < argc,
	fail(caller, "overrand end of argument list",
	     "flag", "%s", flag, NULL));
  return str_str2dataDist(argv[argd]);
}

/*
 * @ arg_frac : get real argument value in 0..1
 * > argument value
 */

real
arg_frac(
  char	      * caller,			/* calling function */
  int		argc,			/* argument count */
  char	      * argv[],			/* argument vector */
  int		argd,			/* current argument index */
  char	      * flag			/* argument flag */
){
  real		result;
  result = arg_real(caller, argc, argv, argd, flag);
  CHECK((0.0 <= result) && (result <= 1.0),
	fail(caller, "illegal fraction argument (must be in 0..1)",
	     "flag", "%s", flag,
	     "value", FMT_REAL_ERR, result, NULL));
  return result;
}

/*
 * @ arg_gfxCtrl : get graphics controls
 * > graphics control value
 */

gfxCtrl_e
arg_gfxCtrl(
  char	      * caller,			/* calling function */
  int		argc,			/* argument count */
  char	      * argv[],			/* argument vector */
  int		argd,			/* current argument index */
  char	      * flag			/* argument flag */
){
  CHECK(argd < argc,
	fail(caller, "overrand end of argument list",
	     "flag", "%s", flag, NULL));
  return str_str2gfxCtrl(argv[argd]);
}

/*
 * @ arg_int : get integer argument value
 * > argument value
 */

int
arg_int(
  char	      * caller,			/* calling function */
  int		argc,			/* argument count */
  char	      * argv[],			/* argument vector */
  int		argd,			/* current argument index */
  char	      * flag			/* argument flag */
){
  int		result;
  CHECK(argd < argc,
	fail(caller, "overrand end of argument list",
	     "flag", "%s", flag, NULL));
  CHECK(sscanf(argv[argd], "%d", &result) == 1,
	fail(caller, "unable to get integer value from argument",
	     "flag", "%s", flag,
	     "argument", "%s", argv[argd], NULL));
  return result;
}

/*
 * @ arg_real : get real argument value
 * > argument value
 */

real
arg_real(
  char	      * caller,			/* calling function */
  int		argc,			/* argument count */
  char	      * argv[],			/* argument vector */
  int		argd,			/* current argument index */
  char	      * flag			/* argument flag */
){
  double	result;
  CHECK(argd < argc,
	fail(caller, "overrand end of argument list",
	     "flag", "%s", flag, NULL));
  CHECK(sscanf(argv[argd], FMT_REAL_RD, &result) == 1,
	fail(caller, "unable to get integer value from argument",
	     "flag", "%s", flag,
	     "argument", "%s", argv[argd], NULL));
  return (real)result;
}

/*
 * @ arg_str : get string argument value
 * > argument value (not copied
 */

char *
arg_str(
  char	      * caller,			/* calling function */
  int		argc,			/* argument count */
  char	      * argv[],			/* argument vector */
  int		argd,			/* current argument index */
  char	      * flag			/* argument flag */
){
  CHECK(argd < argc,
	fail(caller, "overrand end of argument list",
	     "flag", "%s", flag, NULL));
  return argv[argd];
}
