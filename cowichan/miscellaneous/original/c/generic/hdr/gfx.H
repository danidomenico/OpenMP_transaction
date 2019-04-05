/*==============================================================*/
/* generic/hdr/gfx.h : generic graphics interface definitions	*/
/*==============================================================*/

#if GRAPHICS

/*--------------------------------------------------------------*/
/* interface functions (used by applications)			*/
/*--------------------------------------------------------------*/

void
gfx_close(
  void
);
void
gfx_elastic(
  int		call_count,		/* number of times called */
  pt1D		cities,			/* cities to connect */
  int		n_cities,		/* number of cities */
  pt1D		net,			/* net points */
  int		n_net			/* number of net points */
);
void
gfx_gauss(
  int		count,			/* number of times called */
  real2D	matrix,			/* matrix to solve */
  real1D	vector,			/* target vector */
  real1D	result,			/* solution */
  int		n			/* actual number of elements */
);
void
gfx_half(
  int		count,			/* number of times called */
  int2D		matrix,			/* matrix to be filled */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
);
void
gfx_invperc(
  int		count,			/* number of times called */
  int2D		matrix,			/* matrix of values */
  bool2D	mask,			/* mask to be filled */
  int		nr,			/* number of rows */
  int		nc,			/* number of columns */
  int		r,			/* row filling index */
  int		c			/* column filling index */
);
void
gfx_life(
  int		count,			/* number of times called */
  bool2D	matrix,			/* matrix to be filled */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
);
void
gfx_mandel(
  int		count,			/* number of times called */
  int2D		matrix,			/* matrix to be filled */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
);
void
gfx_norm(
  int		count,			/* number of times called */
  pt1D		points,			/* points to normalize */
  int		n			/* number of points */
);
void
gfx_open(
  app_e		app,			/* who's calling */
  gfxCtrl_e	gc			/* graphics control */
);
void
gfx_outer(
  int		count,			/* number of times called */
  pt1D		points,			/* points to convert */
  real2D	matrix,			/* matrix to fill */
  real1D	vector,			/* vector to fill */
  int		n			/* number of points */
);
void
gfx_product(
  int		count,			/* number of times called */
  real2D	matrix,			/* matrix to multiply by */
  real1D	vector,			/* vector to be multiplied */
  real1D	result,			/* vector to fill */
  int		nr,			/* row size */
  int		nc			/* column size */
);
void
gfx_randmat(
  int		count,			/* number of times called */
  int2D		matrix,			/* matrix to be filled */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
);
void
gfx_sor(
  int		count,			/* number of times called */
  real2D	matrix,			/* matrix to solve */
  real1D	vector,			/* target vector */
  real1D	result,			/* solution */
  int		n			/* actual number of elements */
);
void
gfx_thresh(
  int		count,			/* number of times called */
  int2D		matrix,			/* matrix to read */
  bool2D	mask,			/* mask to fill */
  int		nr,			/* number of rows */
  int		nc,			/* number of columns */
  int		hist[],			/* histogram */
  int		maxval			/* maximum histogram value */
);
void
gfx_vecdiff(
  int		count,			/* number of times called */
  real1D	left,			/* first vector */
  real1D	right,			/* second vector */
  int		n,			/* number of elements */
  real		m			/* maximum found */
);
void
gfx_winnow(
  int		count,			/* number of times called */
  int2D		matrix,			/* matrix */
  bool2D	mask,			/* mask of included points */
  pt1D		points,			/* vector to fill */
  int		nr,			/* number of rows */
  int		nc,			/* number of columns */
  int		n			/* number of elements */
);

/*--------------------------------------------------------------*/
/* subsidiary functions (used by viewing tool as well)		*/
/*--------------------------------------------------------------*/

void
gfx_bool2D(
  bool2D	mask,			/* to draw */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
);
void
gfx_canvasClr(
  void
);
void
gfx_dotIntBlk(
  int		col,			/* color */
  int		x,			/* x coordinate */
  int		y			/* y coordinate */
);
void
gfx_dotIntPt(
  int		col,			/* color */
  int		x,			/* x coordinate */
  int		y			/* y coordinate */
);
void
gfx_dotRealBlk(
  int		col,			/* color */
  real		x,			/* x coordinate */
  real		y			/* y coordinate */
);
void
gfx_dotRealPt(
  int		col,			/* color */
  real		x,			/* x coordinate */
  real		y			/* y coordinate */
);
void
gfx_int2D(
  int2D		matrix,			/* to draw */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
);
void
gfx_pause(
  void
);
void
gfx_pt1D_i(
  pt1D		points,			/* points */
  int		npt			/* number of points */
);
void
gfx_pt1D_r(
  pt1D		points,			/* points */
  int		npt,			/* number of points */
  int		connect			/* draw connecting lines? */
);
void
gfx_real1D(
  real1D	vec,			/* to draw */
  int		n,			/* size */
  real		lo,			/* low value */
  real		hi			/* high value */
);
void
gfx_real2D(
  real2D	mat,			/* to draw */
  int		nr,			/* row size */
  int		nc,			/* column size */
  real		lo,			/* low value */
  real		hi			/* high value */
);
void
gfx_canvasInit(
  app_e		app,			/* caller */
  int		world_xHi,		/* high world coordinate */
  int		world_yHi		/* high world coordinate */
);

#endif
