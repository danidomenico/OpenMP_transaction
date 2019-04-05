/*==============================================================*/
/* generic/app/elastic.c : generic elastic implementation	*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*--------------------------------------------------------------*/
/* private function prototypes					*/
/*--------------------------------------------------------------*/

static void
elastic_cityDelta(
  pt1D		cities,			/* cities to connect */
  int		n_cities,		/* number of cities */
  pt1D		net,			/* net being moved */
  pt1D		deltas,			/* deltas to add to */
  int		n_net,			/* number of points in net */
  real		K			/* annealing force */
);
static void
elastic_init(
  pt1D		cities,			/* cities to connect */
  int		n_cities,		/* number of cities */
  pt1D		net,			/* net points to create */
  int		n_net			/* number of points to create */
);
static void
elastic_move(
  pt1D		net,			/* interacting points */
  pt1D		deltas,			/* how much to move */
  int		n_net			/* number of points */
);
static void
elastic_neighDelta(
  pt1D		net,			/* interacting points */
  pt1D		deltas,			/* where to store deltas */
  int		n_net,			/* number of points */
  real		beta_K			/* current constant */
);

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

/*
 * @ elastic : do elastic net calculations
 * > none
 * + move net points
 */

void
elastic(
  pt1D		cities,			/* cities to link */
  int		n_cities,		/* number of cities */
  pt1D		net,			/* net to link with */
  int		n_net,			/* number of net points */
  int		iters,			/* total iterations */
  int		relax			/* relaxation interval */
){
  pt1D		deltas;			/* delta on each point */
  real		K;			/* annealing parameter */
  int		i;			/* iteration index */
#if GRAPHICS
  int		gfxCount;
#endif

  /* initialize point locations */
  elastic_init(cities, n_cities, net, n_net);
#if GRAPHICS
  gfx_elastic(gfxCount++, cities, n_cities, net, n_net);
#endif

  /* move net */
  K = ELASTIC_K_INIT;
  for (i=1; i<=iters; i++){
    elastic_neighDelta(net, deltas, n_net, K * ELASTIC_BETA);
    elastic_cityDelta(cities, n_cities, net, deltas, n_net, K);
    elastic_move(net, deltas, n_net);
    if ((i % relax) == 0){
      K *= ELASTIC_RATE;
#if GRAPHICS
      gfx_elastic(gfxCount++, cities, n_cities, net, n_net);
#endif
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
  pt1D		cities,			/* cities to connect */
  int		n_cities,		/* number of cities */
  pt1D		net,			/* net being moved */
  pt1D		deltas,			/* deltas to add to */
  int		n_net,			/* number of points in net */
  real		K			/* annealing force */
){
  real1D	force;			/* city-point force */
  real		total;			/* total force from city */
  real		x, y;			/* distances */
  real		dSq;			/* square of city-point distance */
  double	scaling;		/* exponential scaling */
  int		c, n;			/* loop indices */

  /* set scaling factor */
  scaling = -1/(2 * K * K);

  /* forces */
  for (c=0; c<n_cities; c++){
    /* calculate and sum force from city on each point */
    total = 0.0;
    for (n=0; n<n_net; n++){
      x = cities[c].x - net[n].x;
      y = cities[c].y - net[n].y;
      dSq = x * x + y * y;
      force[n] = exp(scaling * dSq);
      total += force[n];
    }
    /* scale and apply force */
    ASSERT(total != 0.0);
    for (n=0; n<n_net; n++){
      deltas[n].x += ELASTIC_ALPHA *
		     (force[n]/total) * (cities[c].x - net[n].x);
      deltas[n].y += ELASTIC_ALPHA *
		     (force[n]/total) * (cities[c].y - net[n].y);
    }
  }

  /* return */
}

/*
 @ elastic_init : initialize point locations
 > none
 + create initial elastic net points
 */

static void
elastic_init(
  pt1D		cities,			/* cities to connect */
  int		n_cities,		/* number of cities */
  pt1D		net,			/* net points to create */
  int		n_net			/* number of points to create */
){
  pt		ptMin, ptMax, center;	/* locations */
  double	theta;			/* initialization angle */
  int		n;			/* loop index */
  real		radius;			/* starting net radius */

  redPt1DPos(cities, n_cities, &ptMin, &ptMax);
  center.x = (ptMin.x + ptMax.x) / 2.0;
  center.y = (ptMin.y + ptMax.y) / 2.0;
  radius = (ptMax.x - ptMin.x);
  if (radius < (ptMax.y - ptMin.y)){
    radius = ptMax.y - ptMin.y;
  }
  radius = radius * ELASTIC_RADIUS / 2.0;

  theta = (2 * M_PI) / n_net;
  for (n=0; n<n_net; n++){
    net[n].x = center.x + (radius * cos(theta*n));
    net[n].y = center.y + (radius * sin(theta*n));
  }

  /* return */
}

/*
 @ elastic_move : move points by specified deltas
 > none
 + move net points
 */

static void
elastic_move(
  pt1D		net,			/* interacting points */
  pt1D		deltas,			/* how much to move */
  int		n_net			/* number of points */
){
  int		n;			/* loop index */

  for (n=0; n<n_net; n++){
    net[n].x += deltas[n].x;
    net[n].y += deltas[n].y;
  }

  /* return */
}

/*
 @ elastic_neighDelta : calculate neighbor deltas
 > none
 + set delta vector (effectively zeroes previous iter vals
 */

static void
elastic_neighDelta(
  pt1D		net,			/* interacting points */
  pt1D		deltas,			/* where to store deltas */
  int		n_net,			/* number of points */
  real		beta_K			/* current constant */
){
  int		n;			/* loop index */

  /* low point */
  deltas[0].x = beta_K *
    ((net[n_net-1].x + net[1].x) - (2 * net[0].x));
  deltas[0].y = beta_K *
    ((net[n_net-1].y + net[1].y) - (2 * net[0].y));

  /* high point */
  deltas[n_net-1].x = beta_K *
    ((net[0].x + net[n_net-2].x) - (2 * net[n_net-1].x));
  deltas[n_net-1].y = beta_K *
    ((net[0].y + net[n_net-2].y) - (2 * net[n_net-1].y));

  /* intermediate points */
  for (n=1; n<(n_net-1); n++){
    deltas[n].x = beta_K *
      ((net[n+1].x + net[n-1].x) - (2 * net[n].x));
    deltas[n].y = beta_K *
      ((net[n+1].y + net[n-1].y) - (2 * net[n].y));
  }

  /* return */
}
