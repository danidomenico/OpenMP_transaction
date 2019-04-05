/*==============================================================*/
/* generic/util/gfx.c : generic graphics interface utilities	*/
/*==============================================================*/

#if GRAPHICS

#if NUMA
EXTERN_ENV
#endif
#include "util.h"
#include "gfx.h"			/* to ensure consistency */

/*--------------------------------------------------------------*/
/* local definitions						*/
/*--------------------------------------------------------------*/

#define GFX_NUM_COL		128	/* number of colors */
#define GFX_COL_MAX  (GFX_NUM_COL-1)	/* maximum color */
#define GFX_COL_BACK		0	/* black */
#define GFX_COL_DRAW	GFX_COL_MAX	/* white */
#define GFX_COL_FRAME	GFX_COL_MAX	/* white frame */
#define GFX_DEVICE		"X11"
#define GFX_DOT_INT_MIN		2
#define GFX_DOT_INT_SCL		0.8
#define GFX_DOT_REAL_MIN	25
#define GFX_DOT_REAL_SCL	0.025
#define GFX_POLY_FILL		TRUE
#define GFX_SIZE_CH_X		1500
#define GFX_SIZE_CH_Y		300
#define GFX_SIZE		500

#define GFX_MAP_WIDTH		3
#define GFX_MAP_HEIGHT		3

/*--------------------------------------------------------------*/
/* private state						*/
/*--------------------------------------------------------------*/

static bool	Chained = FALSE;	/* chained version? */
static int	CanvasExtent = 0;	/* current size of canvas */
static gfxCtrl_e GfxCtrl = gfxCtrl_off;	/* graphics control */
static int	PixelExtent = 0;	/* window extent */
static real	DotIntSize = 0.0;	/* box size for integer points */
static real	DotRealSize = 0.0;	/* box size for integer points */
static bool	PolyFill = FALSE;	/* polygon filling? */
struct {
  int		used;			/* contains useful information? */
  int		dX, dY;			/* low corner indices */
} Map[app_e_sz] = {
  {FALSE, 0, 0},			/* app_chain */
  {TRUE,  0, 2},			/* app_elastic */
  {TRUE,  1, 2},			/* app_gauss */
  {TRUE,  2, 2},			/* app_half */
  {TRUE,  0, 1},			/* app_invperc */
  {TRUE,  1, 1},			/* app_life */
  {TRUE,  2, 1},			/* app_mandel */
  {FALSE, 0, 0},			/* app_norm */
  {FALSE, 0, 0},			/* app_outer */
  {FALSE, 0, 0},			/* app_product */
  {TRUE,  0, 0},			/* app_randmat */
  {FALSE, 0, 0},			/* app_sor */
  {TRUE,  1, 0},			/* app_thresh */
  {FALSE, 0, 0},			/* app_vecdiff */
  {TRUE,  2, 0},			/* app_winnow */
  {FALSE, 0, 0}				/* app_any (error) */
};
static void (*gfx_dotInt)(
  int		col,			/* color */
  int		x,			/* x coordinate */
  int		y			/* y coordinate */
) = NULL;
static void (*gfx_dotReal)(
  int		col,			/* color */
  real		x,			/* x coordinate */
  real		y			/* y coordinate */
) = NULL;

/*--------------------------------------------------------------*/
/* interface functions						*/
/*--------------------------------------------------------------*/

/*
 * @ gfx_close : close graphics
 * > none
 * + destroy window, etc. if graphics enabled
 */

void
gfx_close(
  void
){
  if (GfxCtrl != gfxCtrl_off){
    vexit();
  }
  /* return */
}

/*
 * @ gfx_elastic : drawing for elastic net
 * > none
 * + draw if graphics enabled
 */

void
gfx_elastic(
  int		call_count,		/* number of times called */
  pt1D		cities,			/* cities to connect */
  int		n_cities,		/* number of cities */
  pt1D		net,			/* net points */
  int		n_net			/* number of net points */
){
  if (GfxCtrl == gfxCtrl_off){
    return;
  } else if (call_count == 0){
    gfx_canvasInit(app_elastic, 1, 1);
  }

  gfx_canvasClr();
  gfx_pt1D_r(cities, n_cities, FALSE);
  gfx_pt1D_r(net, n_net, TRUE);

  gfx_pause();

  /* return */
}

/*
 * @ gfx_gauss : drawing for Gaussian elimination
 * > none
 * + draw if graphics enabled
 */

void
gfx_gauss(
  int		count,			/* number of times called */
  real2D	matrix,			/* matrix to solve */
  real1D	vector,			/* target vector */
  real1D	result,			/* solution */
  int		n			/* actual number of elements */
){
  float		corner;			/* location of new corner point */

  if (GfxCtrl == gfxCtrl_off){
    return;
  } else if (count == 0){
    gfx_canvasInit(app_gauss, 1, 1);
  }

  corner = (float)count / (float)n;
  color(count % GFX_NUM_COL);
  rect(corner, 0.0, 1.0, 1.0-corner);

  gfx_pause();

  /* return */
}

/*
 * @ gfx_half : drawing for shuffle
 * > none
 * + draw if graphics enabled
 */

void
gfx_half(
  int		count,			/* number of times called */
  int2D		matrix,			/* matrix to be filled */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
){
  if (GfxCtrl == gfxCtrl_off){
    return;
  } else if (count == 0){
    gfx_canvasInit(app_half, nr, nc);
  }

  gfx_int2D(matrix, nr, nc);

  gfx_pause();

  /* return */
}

/*
 * @ gfx_invperc : drawing for invasion percolation
 * > none
 * + draw if graphics enabled
 */

void
gfx_invperc(
  int		count,			/* number of times called */
  int2D		matrix,			/* matrix of values */
  bool2D	mask,			/* mask to be filled */
  int		nr,			/* number of rows */
  int		nc,			/* number of columns */
  int		r,			/* row filling index */
  int		c			/* column filling index */
){
  if (GfxCtrl == gfxCtrl_off){
    return;
  } else if (count == 0){
    gfx_canvasInit(app_invperc, nr, nc);
  }

  gfx_dotInt(GFX_COL_DRAW, r, c);

  gfx_pause();

  /* return */
}

/*
 * @ gfx_life : drawing for Game of Life
 * > none
 * + draw if graphics enabled
 */

void
gfx_life(
  int		count,			/* number of times called */
  bool2D	world,			/* world to draw */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
){
  if (GfxCtrl == gfxCtrl_off){
    return;
  } else if (count == 0){
    gfx_canvasInit(app_life, nr, nc);
  }

  gfx_canvasClr();
  gfx_bool2D(world, nr, nc);

  gfx_pause();

  /* return */
}

/*
 * @ gfx_mandel : drawing for Mandelbrot Set
 * > none
 * + draw if graphics enabled
 */

void
gfx_mandel(
  int		count,			/* number of times called */
  int2D		matrix,			/* matrix to be filled */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
){
  if (GfxCtrl == gfxCtrl_off){
    return;
  } else if (count == 0){
    gfx_canvasInit(app_mandel, nr, nc);
  }

  gfx_int2D(matrix, nr, nc);

  gfx_pause();

  /* return */
}

/*
 * @ gfx_norm : drawing for point normalization
 * > none
 * + draw if graphics enabled
 */

void
gfx_norm(
  int		count,			/* number of times called */
  pt1D		points,			/* points to normalize */
  int		n			/* number of points */
){
  /* return */
}

/*
 * @ gfx_open : open up graphics
 * > none
 * + create window etc. if graphics enabled
 */

void
gfx_open(
  app_e		app,			/* who's calling */
  gfxCtrl_e	gc			/* graphics control */
){
  int		i;			/* loop index (color map) */

  GfxCtrl = gc;

  if (gc != gfxCtrl_off){
    /* flag as chained if appropriate */
    Chained = (app == app_chain);
    
    /* set size preferences */
    if (Chained){
      prefsize(GFX_SIZE_CH_X, GFX_SIZE_CH_Y);
      PixelExtent = GFX_SIZE_CH_Y;
    } else {
      prefsize(GFX_SIZE, GFX_SIZE);
      PixelExtent = GFX_SIZE;
    }

    /* initialize device */
    vinit(GFX_DEVICE);

    /* initialize color map */
    for (i=0; i<GFX_NUM_COL; i++){
      mapcolor(i, 2*i, 2*i, 2*i);
    }

    /* clear */
    color(GFX_COL_BACK);
    clear();
    color(GFX_COL_DRAW);
  }
  
  /* return */
}

/*
 * @ gfx_outer : drawing for outer product
 * > none
 * + draw if graphics enabled
 */

void
gfx_outer(
  int		count,			/* number of times called */
  pt1D		points,			/* points to convert */
  real2D	matrix,			/* matrix to fill */
  real1D	vector,			/* vector to fill */
  int		n			/* number of points */
){
  /* return */
}

/*
 * @ gfx_product : drawing for matrix-vector product
 * > none
 * + draw if graphics enabled
 */

void
gfx_product(
  int		count,			/* number of times called */
  real2D	matrix,			/* matrix to multiply by */
  real1D	vector,			/* vector to be multiplied */
  real1D	result,			/* vector to fill */
  int		nr,			/* row size */
  int		nc			/* column size */
){
  /* return */
}

/*
 * @ gfx_randmat : drawing for random matrix generation
 * > none
 * + draw if graphics enabled
 */

void
gfx_randmat(
  int		count,			/* number of times called */
  int2D		matrix,			/* matrix to be filled */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
){
  if (GfxCtrl == gfxCtrl_off){
    return;
  } else if (count == 0){
    gfx_canvasInit(app_randmat, nr, nc);
  }

  gfx_int2D(matrix, nr, nc);

  gfx_pause();

  /* return */
}

/*
 * @ gfx_sor : drawing for successive over-relaxation
 * > none
 * + draw if graphics enabled
 */

void
gfx_sor(
  int		count,			/* number of times called */
  real2D	matrix,			/* matrix to solve */
  real1D	vector,			/* target vector */
  real1D	result,			/* solution */
  int		n			/* actual number of elements */
){
  /* return */
}

/*
 * @ gfx_thresh : drawing for image thresholding
 * > none
 * + draw if graphics enabled
 */

void
gfx_thresh(
  int		count,			/* number of times called */
  int2D		matrix,			/* matrix to draw */
  bool2D	mask,			/* mask to draw */
  int		nr,			/* number of rows */
  int		nc,			/* number of columns */
  int		hist[],			/* histogram */
  int		maxval			/* maximum histogram value */
){
  static char * context = "gfx_thresh";

  if (GfxCtrl == gfxCtrl_off){
    return;
  } else if (count == 0){
    gfx_canvasInit(app_thresh, nr, nc);
  }

  switch(count){
    case 0 :
      gfx_int2D(matrix, nr, nc);
      break;
    case 1 :
      gfx_bool2D(mask, nr, nc);
      break;
    default :
      fail(context, "bad count", (char *)NULL);
      break;
  }

  gfx_pause();

  /* return */
}

/*
 * @ gfx_vecdiff : drawing for vector difference
 * > none
 * + draw if graphics enabled
 */

void
gfx_vecdiff(
  int		count,			/* number of times called */
  real1D	left,			/* first vector */
  real1D	right,			/* second vector */
  int		n,			/* number of elements */
  real		m			/* maximum found */
){
  /* return */
}

/*
 * @ gfx_winnow : drawing for point winnowing
 * > none
 * + draw if graphics enabled
 */

void
gfx_winnow(
  int		count,			/* number of times called */
  int2D		matrix,			/* matrix */
  bool2D	mask,			/* mask of included points */
  pt1D		points,			/* vector to fill */
  int		nr,			/* number of rows */
  int		nc,			/* number of columns */
  int		n			/* number of elements */
){
  static char * context = "gfx_winnow";

  if (GfxCtrl == gfxCtrl_off){
    return;
  } else if (count == 0){
    gfx_canvasInit(app_winnow, nr, nc);
  }

  switch(count){
    case 0 :
      gfx_int2D(matrix, nr, nc);
      break;
    case 1 :
      gfx_pt1D_i(points, n);
      break;
    default :
      fail(context, "bad count", (char *)NULL);
      break;
  }

  gfx_pause();

  /* return */
}

/*--------------------------------------------------------------*/
/* subsidiary functions (used by viewing tool as well)		*/
/*--------------------------------------------------------------*/

/*
 * @ gfx_canvasClr() : safely clear drawing canvas
 * > none
 * + clear drawing canvas
 */

void
gfx_canvasClr(
  void
){
  int		prevPolyFill;		/* previous filling state */

  prevPolyFill = PolyFill;
  color(GFX_COL_BACK);
  polyfill(TRUE);
  rect(0.0, 0.0, (float)CanvasExtent, (float)CanvasExtent);
  polyfill(prevPolyFill);

  /* return */
}

/*
 * @ gfx_bool2D : draw "integer" matrix of Booleans
 * > none
 * + draw matrix
 */

void
gfx_bool2D(
  bool2D	mask,			/* to draw */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
){
  int		r, c;			/* indices */

  for (c=0; c<nc; c++){
    for (r=nr-1; r>=0; r--){
      if (mask[r][c]){
	gfx_dotInt(GFX_COL_DRAW, c, r);
      }
    }
  }

  /* return */
}

/*
 * @ gfx_dotIntBlk : draw a dot as a block
 * > none
 * + draw single dot as block
 */

void
gfx_dotIntBlk(
  int		col,			/* color */
  int		x,			/* x coordinate */
  int		y			/* y coordinate */
){
  color(col);
  rect((float)x, (float)y, (float)x+DotIntSize, (float)y+DotIntSize);
  /* return */
}

/*
 * @ gfx_dotIntPt : draw a dot as a point
 * > none
 * + draw single dot as point
 */

void
gfx_dotIntPt(
  int		col,			/* color */
  int		x,			/* x coordinate */
  int		y			/* y coordinate */
){
  color(col);
  point2((float)x, (float)y);
  /* return */
}

/*
 * @ gfx_dotRealBlk : draw a dot as a block
 * > none
 * + draw single dot as block
 */

void
gfx_dotRealBlk(
  int		col,			/* color */
  real		x,			/* x coordinate */
  real		y			/* y coordinate */
){
  color(col);
  rect((float)x, (float)y, (float)x+DotRealSize, (float)y+DotRealSize);
  /* return */
}

/*
 * @ gfx_dotRealPt : draw a dot as a point
 * > none
 * + draw single dot as point
 */

void
gfx_dotRealPt(
  int		col,			/* color */
  real		x,			/* x coordinate */
  real		y			/* y coordinate */
){
  color(col);
  point2((float)x, (float)y);
  /* return */
}

/*
 * @ gfx_int2D : draw integer matrix
 * > none
 * + draw matrix
 */

void
gfx_int2D(
  int2D		matrix,			/* to draw */
  int		nr,			/* number of rows */
  int		nc			/* number of columns */
){
  int		r, c;			/* indices */

  for (c=0; c<nc; c++){
    for (r=nr-1; r>=0; r--){
      gfx_dotInt(matrix[r][c], c, r);
    }
  }

  /* return */
}

/*
 * @ gfx_pause : possibly pause so that user can see output
 * > none
 * + possibly pause
 */

void
gfx_pause(
  void
){
  vflush();
  if (GfxCtrl == gfxCtrl_pause){
    (void)getkey();
  }
  /* return */
}

/*
 * @ gfx_pt1D_i : draw vector of points
 * > none
 * + draw points in vector
 */

void
gfx_pt1D_i(
  pt1D		points,			/* points */
  int		npt			/* number of points */
){
  int		i;			/* loop index */
  real		radius;			/* drawing radius */
  int		prevPolyFill;		/* previous filling state */

  prevPolyFill = PolyFill;
  polyfill(FALSE);
  radius = DotIntSize / 2;
  color(GFX_COL_DRAW);
  for (i=0; i<npt; i++){
    circle(points[i].x, points[i].y, radius);
  }
  polyfill(prevPolyFill);

  /* return */
}

/*
 * @ gfx_pt1D_r : draw vector of points
 * > none
 * + draw points in vector
 */

void
gfx_pt1D_r(
  pt1D		points,			/* points */
  int		npt,			/* number of points */
  int		connect			/* draw connecting lines? */
){
  int		i;			/* loop index */
  real		radius;			/* drawing radius */

  if (connect){
    radius = DotRealSize / 2;
    color(GFX_COL_DRAW);
    for (i=0; i<npt; i++){
      circle(points[i].x, points[i].y, radius);
    }
    move2(points[0].x, points[0].y);
    for (i=1; i<npt; i++){
      draw2(points[i].x, points[i].y);
    }
    draw2(points[0].x, points[0].y);
  } else {
    for (i=0; i<npt; i++){
      gfx_dotReal(GFX_COL_DRAW, points[i].x, points[i].y);
    }
  }

  /* return */
}

/*
 * @ gfx_real1D : draw real vector (as stripes)
 * > none
 * + draw real vector
 */

void
gfx_real1D(
  real1D	vec,			/* to draw */
  int		n,			/* size */
  real		lo,			/* low value */
  real		hi			/* high value */
){
  int		i;			/* loop index */
  float		width;			/* width of stripes */
  real		scale;			/* scaling factor */

  width = 1/(float)n;
  scale = GFX_NUM_COL * ((hi == lo) ? 0 : 1/(hi - lo));
  for (i=0; i<n; i++){
    color((int)((vec[i] - lo) * scale));
    rect(i*width, 0.0, (i+1)*width, 1.0);
  }

  /* return */
}

/*
 * @ gfx_real2D : draw real matrix
 * > none
 * + draw real matrix
 */

void
gfx_real2D(
  real2D	mat,			/* to draw */
  int		nr,			/* row size */
  int		nc,			/* column size */
  real		lo,			/* low value */
  real		hi			/* high value */
){
  int		r, c;			/* loop indices */
  float		width, height;		/* box sizes */
  real		scale;			/* scaling factor */

  width  = 1/(float)nr;
  height = 1/(float)nc;
  scale = GFX_NUM_COL * ((hi == lo) ? 0 : 1/(hi - lo));
  for (r=0; r<nr; r++){
    for (c=0; c<nc; c++){
      color((int)((mat[r][c] - lo) * scale));
      rect(r*width, c*height, (r+1)*width, (c+1)*height);
    }
  }

  /* return */
}

/*
 * @ gfx_canvasInit : set up canvas for drawing
 * > none
 * + set parameters for drawing
 */

void
gfx_canvasInit(
  app_e		app,			/* caller */
  int		world_xHi,		/* high world coordinate */
  int		world_yHi		/* high world coordinate */
){
  float		view_xLo, view_yLo;	/* low corner of view */
  float		view_xHi, view_yHi;	/* high corner of view */

  /* set controls */
  if (!Chained){
    view_xLo = -1.0;	view_xHi =  1.0;
    view_yLo = -1.0;	view_yHi =  1.0;
  } else {
    ASSERT(Map[app].used);
    view_xLo = ((float)Map[app].dX)       / (float)GFX_MAP_WIDTH;
    view_xHi = ((float)(Map[app].dX + 1)) / (float)GFX_MAP_WIDTH;
    view_yLo = ((float)Map[app].dY)       / (float)GFX_MAP_HEIGHT;
    view_yHi = ((float)(Map[app].dY + 1)) / (float)GFX_MAP_HEIGHT;
  }

  /* set new viewport */
  viewport(view_xLo, view_xHi, view_yLo, view_yHi);

  /* draw frame */
  color(GFX_COL_FRAME);
  move2(-1.0, -1.0);
  draw2( 1.0, -1.0);
  draw2( 1.0,  1.0);
  draw2(-1.0,  1.0);
  draw2(-1.0, -1.0);

  /* set world coordinates */
  ASSERT(world_xHi > 0);
  ASSERT(world_yHi > 0);
  CanvasExtent = world_xHi > world_yHi ? world_xHi : world_yHi;
  ortho2(0.0, (float)CanvasExtent, 0.0, (float)CanvasExtent);

  /* select integer dot-drawing routine */
  if (PixelExtent < (CanvasExtent * GFX_DOT_INT_MIN)){
    gfx_dotInt = gfx_dotIntPt;
  } else {
    gfx_dotInt = gfx_dotIntBlk;
    DotIntSize = GFX_DOT_INT_SCL;
  }

  /* select real dot-drawing routine */
  if (PixelExtent < GFX_DOT_REAL_MIN){
    gfx_dotReal = gfx_dotRealPt;
  } else {
    gfx_dotReal = gfx_dotRealBlk;
    DotRealSize = GFX_DOT_REAL_SCL;
  }

  /* turn off polygon filling (default) */
  PolyFill = GFX_POLY_FILL;
  polyfill(PolyFill);

  /* return */
}

#endif
