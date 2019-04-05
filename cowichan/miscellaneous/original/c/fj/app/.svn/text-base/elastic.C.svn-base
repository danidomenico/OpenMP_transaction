/*==============================================================*/
/* fj/app/elastic.c : forkjoin elastic implementation		*/
/*==============================================================*/

#if NUMA
EXTERN_ENV
#endif
#include "specific.h"

/*--------------------------------------------------------------*/
/* private data structures					*/
/*--------------------------------------------------------------*/

static pt1D	Deltas;			/* delta on each point */
static real	K;			/* annealing parameter */
static real	Beta_K;			/* annealing parameter */
static real1D	Sums;			/* force sums */
static real2D	Forces;			/* actual forces */
static pt	PtMin, PtMax;		/* extents */
static pt	PtCenter;		/* center point */
static real	Theta, Radius;		/* for starting net */

/*--------------------------------------------------------------*/
/* private function prototypes					*/
/*--------------------------------------------------------------*/

static thr_f	elastic_thr_cityDeltaApply;
static thr_f	elastic_thr_cityDeltaCalc;
static thr_f	elastic_thr_init;
static thr_f	elastic_thr_move;
static thr_f	elastic_thr_neighDelta;

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
  void	      * args[4];
  int		i;			/* loop index */
#if GRAPHICS
  int		gfxCount;
#endif

  /* pack arguments once */
  TP_any(args, 0, cities);
  TP_any(args, 1, n_cities);
  TP_any(args, 2, net);
  TP_any(args, 3, n_net);

  /* initialize point locations */
  redPt1DPos(cities, n_cities, &PtMin, &PtMax);
  PtCenter.x = (PtMin.x + PtMax.x) / 2.0;
  PtCenter.y = (PtMin.y + PtMax.y) / 2.0;
  Radius = (PtMax.x - PtMin.x);
  if (Radius < (PtMax.y - PtMin.y)){
    Radius = PtMax.y - PtMin.y;
  }
  Radius = Radius * ELASTIC_RADIUS / 2.0;
  Theta = (2 * M_PI) / n_net;
  thr_grp(elastic_thr_init, ParWidth, args);
#if GRAPHICS
  gfx_elastic(gfxCount++, cities, n_cities, net, n_net);
#endif

  /* move net */
  K = ELASTIC_K_INIT;
  Beta_K = K * ELASTIC_BETA;
  for (i=1; i<=iters; i++){
    thr_grp(elastic_thr_neighDelta, ParWidth, args);
    thr_grp(elastic_thr_cityDeltaCalc, ParWidth, args);
    thr_grp(elastic_thr_cityDeltaApply, ParWidth, args);
    thr_grp(elastic_thr_move, ParWidth, args);
    if ((i % relax) == 0){
      K *= ELASTIC_RATE;
      Beta_K = K * ELASTIC_BETA;
#if GRAPHICS
      gfx_elastic(gfxCount++, cities, n_cities, net, n_net);
#endif
    }
  }

  /* return */
}

/*--------------------------------------------------------------*/
/* threading functions						*/
/*--------------------------------------------------------------*/

/*
 * @ elastic_thr_cityDeltaApply : apply city-point forces to points
 * > none
 * + update values in delta vector
 */

static THR_DEF
elastic_thr_cityDeltaApply(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  pt1D	      * cities;			/* cities to link */
  int		n_cities;		/* number of cities */
  pt1D	      * net;			/* net to link with */
  int		n_net;			/* number of net points */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		c, n;			/* loop indices */

  /* setup */
  cities   = TG_pt1D(argsThr, 0);
  n_cities = TG_int(argsThr, 1);
  net      = TG_pt1D(argsThr, 2);
  n_net    = TG_int(argsThr, 3);
  tid      = TA_get_id(argsThr);
  nt       = TA_get_n(argsThr);

  /* work */
  if (sch_work(nt, tid, 0, n_net, &lo, &hi, &str)){
    for (n=lo; n<hi; n+=str){
      for (c=0; c<n_cities; c++){
	ASSERT(Sums[c] != 0.0);
	Deltas[n].x += ELASTIC_ALPHA *
	  (Forces[c][n]/Sums[c]) * ((*cities)[c].x - (*net)[n].x);
	Deltas[n].y += ELASTIC_ALPHA *
	  (Forces[c][n]/Sums[c]) * ((*cities)[c].y - (*net)[n].y);
      }
    }
  }

  THR_END(argsThr);
}

/*
 * @ elastic_thr_cityDeltaCalc : add city-point forces to delta vector
 * > NULL
 * + add to delta vector
 */

static THR_DEF
elastic_thr_cityDeltaCalc(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  pt1D	      * cities;			/* cities to link */
  int		n_cities;		/* number of cities */
  pt1D	      * net;			/* net to link with */
  int		n_net;			/* number of net points */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		c, n;			/* loop indices */
  real		scaling;		/* scaling factor */
  real		xTmp, yTmp;		/* distances */
  real		dSq;			/* square of distance */

  /* setup */
  cities   = TG_pt1D(argsThr, 0);
  n_cities = TG_int(argsThr, 1);
  net      = TG_pt1D(argsThr, 2);
  n_net    = TG_int(argsThr, 3);
  tid      = TA_get_id(argsThr);
  nt       = TA_get_n(argsThr);

  /* set scaling factor */
  scaling = -1/(2 * K * K);

  /* work */
  if (sch_work(nt, tid, 0, n_cities, &lo, &hi, &str)){
    for (c=lo; c<hi; c+=str){
      Sums[c] = 0.0;
      for (n=0; n<n_net; n++){
	xTmp = (*cities)[c].x - (*net)[n].x;
	yTmp = (*cities)[c].y - (*net)[n].y;
	dSq = xTmp * xTmp + yTmp * yTmp;
	Forces[c][n] = exp(scaling * dSq);
	Sums[c] += Forces[c][n];
      }
    }
  }

  THR_END(argsThr);
}

/*
 * @ elastic_thr_init : initialize point locations
 * > none
 * + create initial elastic net points
 */

static THR_DEF
elastic_thr_init(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  pt1D	      * net;			/* net to link with */
  int		n_net;			/* number of net points */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		i;			/* loop index */

  /* setup */
  net      = TG_pt1D(argsThr, 2);
  n_net    = TG_int(argsThr, 3);
  tid      = TA_get_id(argsThr);
  nt       = TA_get_n(argsThr);

  /* work */
  if (sch_work(nt, tid, 0, n_net, &lo, &hi, &str)){
    for (i=lo; i<hi; i+=str){
      (*net)[i].x = PtCenter.x + (Radius * cos(Theta*i));
      (*net)[i].y = PtCenter.y + (Radius * sin(Theta*i));
    }
  }

  THR_END(argsThr);
}

/*
 * @ elastic_thr_ move : move points by specified deltas
 * > none
 * + move net points
 */

static THR_DEF
elastic_thr_move(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  pt1D	      * net;			/* net to link with */
  int		n_net;			/* number of net points */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		i;			/* loop index */

  /* setup */
  net      = TG_pt1D(argsThr, 2);
  n_net    = TG_int(argsThr, 3);
  tid      = TA_get_id(argsThr);
  nt       = TA_get_n(argsThr);

  /* work */
  if (sch_work(nt, tid, 0, n_net, &lo, &hi, &str)){
    for (i=lo; i<hi; i+=str){
      (*net)[i].x += Deltas[i].x;
      (*net)[i].y += Deltas[i].y;
    }
  }

  THR_END(argsThr);
}


/*
 @ elastic_thr_neighDelta : calculate neighbor deltas
 > none
 + set delta vector (effectively zeroes previous iter vals
 */

static THR_DEF
elastic_thr_neighDelta(
  void	      * argVoid
){
  void	     ** argsThr = (void **)argVoid;
  pt1D	      * net;			/* net to link with */
  int		n_net;			/* number of net points */
  int		tid, nt;		/* ID and # threads */
  int		lo, hi, str;		/* work controls */
  int		i, i_lo, i_hi;		/* loop index */

  /* setup */
  net      = TG_pt1D(argsThr, 2);
  n_net    = TG_int(argsThr, 3);
  tid      = TA_get_id(argsThr);
  nt       = TA_get_n(argsThr);

  /* work */
  if (sch_work(nt, tid, 0, n_net, &lo, &hi, &str)){
    for (i=lo; i<hi; i+=str){
      i_lo = (n_net + i - 1) % n_net;
      i_hi = (n_net + i + 1) % n_net;
      Deltas[i].x = Beta_K *
		      (((*net)[i_hi].x + (*net)[i_lo].x) - (2 * (*net)[i].x));
      Deltas[i].y = Beta_K *
		      (((*net)[i_hi].y + (*net)[i_lo].y) - (2 * (*net)[i].y));
    }
  }

  THR_END(argsThr);
}
