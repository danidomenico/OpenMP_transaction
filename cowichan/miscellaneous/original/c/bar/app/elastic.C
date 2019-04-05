/*==============================================================*/
/* bar/app/elastic.c : barrier elastic implementation		*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*--------------------------------------------------------------*/
/* shared data structures					*/
/*--------------------------------------------------------------*/

static pt1D	Deltas;			/* delta on each point */
static real2D	Forces;			/* actual forces */
static real	K;			/* annealing parameter */
static real1D	Sums;			/* force sums */

/*--------------------------------------------------------------*/
/* private function prototypes					*/
/*--------------------------------------------------------------*/

static void
elastic_cityDelta(
  int		tid,			/* own ID */
  pt1D		cities,			/* cities to connect */
  int		n_cities,		/* number of cities */
  pt1D		net,			/* net being moved */
  int		n_net,			/* number of points in net */
  real		K			/* annealing force */
);
static void
elastic_init(
  int		tid,			/* own ID */
  pt1D		cities,			/* cities to connect */
  int		n_cities,		/* number of cities */
  pt1D		net,			/* net points to create */
  int		n_net			/* number of points to create */
);
static void
elastic_move(
  int		tid,			/* own ID */
  pt1D		net,			/* interacting points */
  int		n_net			/* number of points */
);
static void
elastic_neighDelta(
  int		tid,			/* own ID */
  pt1D		net,			/* interacting points */
  int		n_net,			/* number of points */
  real		beta_K			/* current constant */
);

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

/*
 * @ elastic : body of elastic net calculations
 * > none
 * + do calculations
 */

void
elastic(
  int		tid,			/* own ID */
  pt1D		cities,			/* cities to link */
  int		n_cities,		/* number of cities */
  pt1D		net,			/* net to link with */
  int		n_net,			/* number of net points */
  int		iters,			/* total iterations */
  int		relax			/* relaxation interval */
){
  int		i;			/* loop index */
#if GRAPHICS
  int		gfxCount;
#endif

  /* initialize point locations */
  elastic_init(tid, cities, n_cities, net, n_net);
#if GRAPHICS
  if (MASTER(tid)){
    gfx_elastic(gfxCount++, cities, n_cities, net, n_net);
  }
  thr_bar(tid);
#endif

  /* move net */
  K = ELASTIC_K_INIT;
  for (i=1; i<=iters; i++){
    elastic_neighDelta(tid, net, n_net, K * ELASTIC_BETA);
    elastic_cityDelta(tid, cities, n_cities, net, n_net, K);
    elastic_move(tid, net, n_net);
    if ((i % relax) == 0){
      if (MASTER(tid)){
	K *= ELASTIC_RATE;
#if GRAPHICS
	gfx_elastic(gfxCount++, cities, n_cities, net, n_net);
#endif
      }
      thr_bar(tid);
    }
  }

  /* return */
}

/*--------------------------------------------------------------*/
/* private functions						*/
/*--------------------------------------------------------------*/

/*
 @ elastic_cityDelta : add deltas for point-city interaction
 > none
 + update values in delta vector
 */

static void
elastic_cityDelta(
  int		tid,			/* own ID */
  pt1D		cities,			/* cities to connect */
  int		n_cities,		/* number of cities */
  pt1D		net,			/* net being moved */
  int		n_net,			/* number of points in net */
  real		K			/* annealing force */
){
  real		x, y;			/* distances */
  real		dSq;			/* square of city-point distance */
  double	scaling;		/* exponential scaling */
  int		c, n;			/* loop indices */
  int		lo, hi, str;		/* work controls */

  /* set scaling factor */
  scaling = -1/(2 * K * K);

  /* calculate and sum force from city on each point */
  if (sch_work(ParWidth, tid, 0, n_cities, &lo, &hi, &str)){
    for (c=lo; c<hi; c+=str){
      Sums[c] = 0.0;
      for (n=0; n<n_net; n++){
	x = cities[c].x - net[n].x;
	y = cities[c].y - net[n].y;
	dSq = x * x + y * y;
	Forces[c][n] = exp(scaling * dSq);
	Sums[c] += Forces[c][n];
      }
    }
  }
  thr_bar(tid);

  /* scale and apply force */
  if (sch_work(ParWidth, tid, 0, n_net, &lo, &hi, &str)){
    for (n=lo; n<hi; n+=str){
      for (c=0; c<n_cities; c++){
	ASSERT(Sums[c] != 0.0);
	Deltas[n].x += ELASTIC_ALPHA *
	  (Forces[c][n]/Sums[c]) * (cities[c].x - net[n].x);
	Deltas[n].y += ELASTIC_ALPHA *
	  (Forces[c][n]/Sums[c]) * (cities[c].y - net[n].y);
      }
    }
  }
  thr_bar(tid);

  /* return */
}

/*
 @ elastic_init : initialize point locations
 > none
 + create initial elastic net points
 */

static void
elastic_init(
  int		tid,			/* own ID */
  pt1D		cities,			/* cities to connect */
  int		n_cities,		/* number of cities */
  pt1D		net,			/* net points to create */
  int		n_net			/* number of points to create */
){
  pt		ptMin, ptMax, center;	/* locations */
  double	theta;			/* initialization angle */
  int		n;			/* loop index */
  real		radius;			/* starting net radius */
  int		lo, hi, str;		/* work controls */

  redPt1DPos(tid, cities, n_cities, &ptMin, &ptMax);
  center.x = (ptMin.x + ptMax.x) / 2.0;
  center.y = (ptMin.y + ptMax.y) / 2.0;
  radius = (ptMax.x - ptMin.x);
  if (radius < (ptMax.y - ptMin.y)){
    radius = ptMax.y - ptMin.y;
  }
  radius = radius * ELASTIC_RADIUS / 2.0;
  theta = (2 * M_PI) / n_net;

  if (sch_work(ParWidth, tid, 0, n_net, &lo, &hi, &str)){
    for (n=lo; n<hi; n+=str){
      net[n].x = center.x + (radius * cos(theta*n));
      net[n].y = center.y + (radius * sin(theta*n));
    }
  }
  thr_bar(tid);

  /* return */
}

/*
 @ elastic_move : move points by specified deltas
 > none
 + move net points
 */

static void
elastic_move(
  int		tid,			/* own ID */
  pt1D		net,			/* interacting points */
  int		n_net			/* number of points */
){
  int		n;			/* loop index */
  int		lo, hi, str;		/* work controls */

  if (sch_work(ParWidth, tid, 0, n_net, &lo, &hi, &str)){
    for (n=lo; n<hi; n+=str){
      net[n].x += Deltas[n].x;
      net[n].y += Deltas[n].y;
    }
  }
  thr_bar(tid);

  /* return */
}

/*
 @ elastic_neighDelta : calculate neighbor deltas
 > none
 + set delta vector (effectively zeroes previous iter vals
 */

static void
elastic_neighDelta(
  int		tid,			/* own ID */
  pt1D		net,			/* interacting points */
  int		n_net,			/* number of points */
  real		beta_K			/* current constant */
){
  int		n, n_lo, n_hi;		/* loop indices */
  int		lo, hi, str;		/* work controls */

  if (sch_work(ParWidth, tid, 0, n_net, &lo, &hi, &str)){
    for (n=lo; n<hi; n+=str){
      n_lo = (n_net + n - 1) % n_net;
      n_hi = (n_net + n + 1) % n_net;
      Deltas[n].x = beta_K *
	((net[n_hi].x + net[n_lo].x) - (2 * net[n].x));
      Deltas[n].y = beta_K *
	((net[n_hi].y + net[n_lo].y) - (2 * net[n].y));
    }
  }
  thr_bar(tid);

  /* return */
}
