/**
 * Serial implementation of invasion percolation
 *
 * \file serial.h
 * \author Andrew Borzenko
 * \date 02-28-09
 */

#pragma once
#ifndef INVPERC_SERIAL_H
#define INVPERC_SERIAL_H

/*--------------------------------------------------------------*/
/* private types						*/
/*--------------------------------------------------------------*/

typedef struct node_struct  node_t;
typedef struct node_struct* node_p;

struct node_struct {
  int		val;			/* matrix value */
  int		r, c;			/* location indices */
};

typedef std::multimap<int, node_p> mapNodesType;

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

void
invperc(
  int2D*		matrix,			/* matrix to invade */
  bool2D*	mask,			/* mask to fill */
  int		nr,			/* row size */
  int		nc,			/* column size */
  real		fraction		/* how much to fill */
);

/*--------------------------------------------------------------*/
/* private functions 					*/
/*--------------------------------------------------------------*/

void
inv_deq(mapNodesType* nodes,
  int	      * r,			/* row index */
  int	      * c			/* column index */
);
void
inv_enq(mapNodesType* nodes,
  node_p	node			/* what to enqueue */
);
node_p
inv_node(
  int		val,			/* location value */
  int		r,			/* row index */
  int		c			/* column index */
);
void
inv_enqPt(mapNodesType* nodes,
  int2D*		matrix,			/* matrix of values */
  bool2D*	mask,			/* mask to be filled */
  int		nr,			/* number of rows */
  int		nc,			/* number of columns */
  int		r,			/* point row */
  int		c			/* point column */
);

#endif /* INVPERC_SERIAL_H */
