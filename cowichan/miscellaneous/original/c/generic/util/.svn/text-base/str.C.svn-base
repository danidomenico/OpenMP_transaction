/*==============================================================*/
/* generic/util/str.c : generic string utilities		*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "util.h"

/*--------------------------------------------------------------*/
/* private data							*/
/*--------------------------------------------------------------*/

/* string <-> application */
static char * str_appTbl[app_e_sz] = {
  "chain",
  "elastic",
  "gauss",
  "half",
  "invperc",
  "life",
  "mandel",
  "norm",
  "outer",
  "product",
  "randmat",
  "sor",
  "thresh",
  "vecdiff",
  "winnow",
  "any"
};

/* string <-> data distribution */
static char * str_dataDistTbl[dataDist_e_sz] = {
  "block",
  "cyclic"
};

/* string <-> graphics control */
static char * str_gfxCtrlTbl[gfxCtrl_e_sz] = {
  "off",
  "on",
  "pause"
};

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

/*
 * @ str_app2Str : convert application code to string
 * > string
 */

char *
str_app2Str(
  app_e		app			/* to convert */
){
  CHECK(app < app_e_sz,
	fail("str_app2Str", "illegal application code",
	     "code", "%d", (int)app, NULL));
  return str_appTbl[(int)app];
}

/*
 * @ str_dataDist2Str : convert data distribution code to string
 * > string
 */

char *
str_dataDist2Str(
  dataDist_e	dd			/* to convert */
){
  CHECK(dd < dataDist_e_sz,
	fail("str_dataDist2Str", "illegal data distribution code",
	     "code", "%d", (int)dd, NULL));
  return str_dataDistTbl[(int)dd];
}

/*
 * @ str_gfxCtrl2Str : convert graphics control code to string
 * > string
 */

char *
str_gfxCtrl2Str(
  gfxCtrl_e	gc			/* to convert */
){
  CHECK(gc < gfxCtrl_e_sz,
	fail("str_gfxCtrl2Str", "illegal graphics control code",
	     "code", "%d", (int)gc, NULL));
  return str_gfxCtrlTbl[(int)gc];
}

/*
 * @ str_str2app : convert string to application
 * > application code
 */

app_e
str_str2app(
  char	      * str			/* to convert */
){
  int		i;			/* loop index (and result) */

  for (i=0;
       (i<app_e_sz) && (strcmp(str_appTbl[i], str) != 0);
       i++);
  CHECK(i < app_e_sz,
	fail("str_str2app", "no such application",
	     "application", "%s", str, NULL));

  return (app_e)i;
}

/*
 * @ str_str2dataDist : convert string to data distribution
 * > data distribution code
 */

dataDist_e
str_str2dataDist(
  char	      * str			/* to convert */
){
  int		i;			/* loop index (and result) */

  for (i=0;
       (i<dataDist_e_sz) && (strcmp(str_dataDistTbl[i], str) != 0);
       i++);
  CHECK(i < dataDist_e_sz,
	fail("str_str2dataDist", "no such data distribution",
	     "data distribution", "%s", str, NULL));

  return (dataDist_e)i;
}

/*
 * @ str_str2gfxCtrl : convert string to graphics control
 * > graphics control code
 */

gfxCtrl_e
str_str2gfxCtrl(
  char	      * str			/* to convert */
){
  int		i;			/* loop index (and result) */

  for (i=0;
       (i<gfxCtrl_e_sz) && (strcmp(str_gfxCtrlTbl[i], str) != 0);
       i++);
  CHECK(i < gfxCtrl_e_sz,
	fail("str_str2gfxCtrl", "no such graphics control",
	     "graphics control", "%s", str, NULL));

  return (gfxCtrl_e)i;
}
