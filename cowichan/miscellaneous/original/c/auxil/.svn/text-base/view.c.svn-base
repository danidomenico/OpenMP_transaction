/*==============================================================*/
/* aux/view.c : auxiliary Cowichan file viewer			*/
/*==============================================================*/

#include "specific.h"
#include "gfx.h"

/*--------------------------------------------------------------*/
/* private function prototypes					*/
/*--------------------------------------------------------------*/

#if GRAPHICS
static void
view_i2D(
  int2D		i2D,			/* matrix to write */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
);
static void
view_p1D(
  pt1D		p1D,			/* vector to write */
  int		n			/* number of elements */
);
static void
view_r1D(
  real1D	r1D,			/* vector to write */
  int		n			/* number of elements */
);
static void
view_r2D(
  real2D	r2D,			/* matrix to write */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
);
static ft_e
suffix2Ft(
  char	      * fn			/* file name */
);
#endif
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
  static char * context = "main(view)";
  int		argd = 1;		/* argument index */
  char	      * infn = NULL;		/* input file name */
  int2D		i2D;			/* integer/boolean matrix */
  pt1D		p1D;			/* point vector */
  real1D	r1D;			/* real vector */
  real2D	r2D;			/* real matrix */
  ft_e		ft = ft_e_sz;		/* file type */
  int		n, nr = 0, nc = 0;	/* sizes */

  /* user wants help */
  if (argc == 1){
    usage();
    exit(0);
  }

#if GRAPHICS
  /* arguments */
  while (argd < argc){
    if (argv[argd][0] != '-'){
      if (infn == NULL){
	infn = argv[argd];
	argd += 1;
      } else {
	fail(context, "bad argument", "argument", "%s", argv[argd], NULL);
      }
    } else {
      switch(argv[argd][1]){
       case 'i' :			/* input file */
	infn = arg_str(context, argc, argv, argd+1, argv[argd]);
	argd += 2;
	break;
       case 't' :			/* type */
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
  }

  /* check */
  if (ft == ft_e_sz){
    ft = suffix2Ft(infn);
  }
  CHECK(ft != ft_e_sz,
	fail(context, "file type not inferred or set", NULL));

  /* processing */
  switch(ft){
   case ft_i2D :
   case ft_i2D_b :
    io_rdInt2D(context, infn, i2D, &nr, &nc);
    view_i2D(i2D, nr, nc);
    break;
   case ft_p1D :
    io_rdPt1D(context, infn, p1D, &n);
    view_p1D(p1D, n);
    break;
   case ft_r1D :
    io_rdReal1D(context, infn, r1D, &n);
    view_r1D(r1D, n);
    break;
   case ft_r2D :
    io_rdReal2D(context, infn, r2D, &nr, &nc);
    view_r2D(r2D, nr, nc);
    break;
   default :
    ASSERT(FALSE);
    break;
  }
#endif

  return 0;
}

/*--------------------------------------------------------------*/
/* private functions						*/
/*--------------------------------------------------------------*/

#if GRAPHICS

/*
 * @ view_i2D : view integer matrix
 * > none
 * + display matrix
 */

static void
view_i2D(
  int2D		i2D,			/* matrix to write */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
){
  gfx_open(app_any, gfxCtrl_pause);
  gfx_canvasInit(app_any, nr, nc);
  gfx_int2D(i2D, nr, nc);
  gfx_pause();
  gfx_close();

  /* return */
}

/*
 * @ view_p1D : view point vector (using gfx_elastic)
 * > none
 * + display points
 */

static void
view_p1D(
  pt1D		p1D,			/* vector to write */
  int		n			/* number of elements */
){
  gfx_open(app_any, gfxCtrl_pause);
  gfx_canvasInit(app_any, 1, 1);
  gfx_pt1D_r(p1D, n, FALSE);
  gfx_pause();
  gfx_close();

/* return */
}

/*
 * @ view_r1D : view real vector
 * > none
 * + display vector
 */

static void
view_r1D(
  real1D	r1D,			/* vector to write */
  int		n			/* number of elements */
){
  real		lo, hi;			/* extrema */

  (void)redReal1DExt(r1D, n, &lo, &hi);
  gfx_open(app_any, gfxCtrl_pause);
  gfx_canvasInit(app_any, 1, 1);
  gfx_real1D(r1D, n, lo, hi);
  gfx_pause();
  gfx_close();

  /* return */
}

/*
 * @ view_r2D : view real matrix
 * > none
 * + display matrix
 */

static void
view_r2D(
  real2D	r2D,			/* matrix to write */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
){
  real		lo, hi;			/* extrema */

  (void)redReal2DExt(r2D, nr, nc, &lo, &hi);
  gfx_open(app_any, gfxCtrl_pause);
  gfx_canvasInit(app_any, 1, 1);
  gfx_real2D(r2D, nr, nc, lo, hi);
  gfx_pause();
  gfx_close();

  /* return */
}

/*
 * @ suffix2Ft : infer file type from suffix
 * > file type (or ft_e_sz to indicate not inferrable)
 */

static ft_e
suffix2Ft(
  char	      * fn			/* file name */
){
  char	      * ptr;
  ft_e		ft = ft_e_sz;

  ASSERT(fn != NULL);
  ptr = strrchr(fn, '.');
  if (ptr != NULL){
    ptr += 1;
    if (strcmp(ptr, "i2") == 0){
      ft = ft_i2D;
    } else if (strcmp(ptr, "p1") == 0){
      ft = ft_p1D;
    } else if (strcmp(ptr, "r1") == 0){
      ft = ft_r1D;
    } else if (strcmp(ptr, "r2") == 0){
      ft = ft_r2D;
    }
  }

  return ft;
}

#endif

/*
 * @ usage : print usage
 * > none
 */

static void
usage(
  void
){
  printf("view\n");
  printf("\t-i filename                 : input file name\n");
  printf("\t-t {b2, i2, p1, r0, r1, r2} : object type\n");
  printf("\t-u                          : unformatted\n");
  /* return */
}
