/**
 * Serial implementation of invasion percolation
 *
 * \file serial.cpp
 * \author Andrew Borzenko
 * \date 02-28-09
 */

#include "../include/main.h"
#include "serial.h"

// public

/*
 * @ invperc : do invasion percolation
 * > none
 * + fill mask
 */

void
invperc(
  int2D*		matrix,			/* matrix to invade */
  bool2D*	mask,			/* mask to fill */
  int		nr,			/* row size */
  int		nc,			/* column size */
  real		fraction		/* how much to fill */
){
  int		r, c;			/* row and column indices */
  int		num, i;			/* filling index */

  /* initialize */
  num = (int) (fraction * nr * nc);
  mapNodesType nodes;
  inv_enq (&nodes, inv_node (matrix[nr/2][nc/2], nr/2, nc/2));

  /* fill */
  for (i = 0; i < num; i++) {
    inv_deq (&nodes, &r, &c);
    mask[r][c] = TRUE;
    inv_enqPt (&nodes, matrix, mask, nr, nc, r, c);
  }

  /* return */
}

// private

/*
 * @ inv_deq : take item out of priority queue
 * > queue after dequeueing
 * + fill row and column indices
 */

void inv_deq(mapNodesType* nodes,
  int	      * r,			/* row index */
  int	      * c			/* column index */
){
  // remove the first element in Nodes
  mapNodesType::iterator iter = nodes->begin ();

  *r = iter->second->r;
  *c = iter->second->c;
  delete iter->second;

  nodes->erase (iter);
}

/*
 * @ inv_enq : add item to priority queue
 * > queue after dequeueing
 * + enqueue item in given tree
 */

void inv_enq(mapNodesType* nodes,
  node_p	node			/* what to enqueue */
){
  // insert new element in Nodes
  nodes->insert (std::pair<int, node_p> (node->val, node));
}

/*
 * @ inv_node : allocate and fill queue node
 * > new node
 */

node_p inv_node(
  int		val,			/* location value */
  int		r,			/* row index */
  int		c			/* column index */
){
  node_p node;

  node = new node_t;
  node->val  = val;
  node->r    = r;
  node->c    = c;

  return node;
}

/*
 * @ inv_enqPt : possibly add point to priority queue
 * > new queue
 * + possibly add point to priority queue
 */

void inv_enqPt(mapNodesType* nodes,
  int2D*		matrix,			/* matrix of values */
  bool2D*	mask,			/* mask to be filled */
  int		nr,			/* number of rows */
  int		nc,			/* number of columns */
  int		r,			/* point row */
  int		c			/* point column */
){
  bool		e[8];			/* empty neighbors */
  bool		r_lo = r > 0,
		r_hi = r < (nr-1),
		c_lo = c > 0,
		c_hi = c < (nc-1);

  /*   0   */
  /*  1A2  */
  /* 3BXC4 */
  /*  5D6  */
  /*   7   */
  if (r_lo){
    e[0] = ((r > 1) && !mask[r-2][c]) || (r - 2 < 0);
    e[1] = (c_lo && !mask[r-1][c-1]) || (r - 1 < 0) || (c - 1 < 0);
    e[2] = (c_hi && !mask[r-1][c+1]) || (r - 1 < 0) || (c + 1 > nc - 1);
  }
  e[3] = ((c > 1) && !mask[r][c-2]) || (c - 2 < 0);
  e[4] = ((c < (nc-2)) && !mask[r][c+2]) || (c + 2 > nc - 1);
  if (r_hi){
    e[5] = (c_lo && !mask[r+1][c-1]) || (r + 1 > nr - 1) || (c - 1 < 0);
    e[6] = (c_hi && !mask[r+1][c+1]) || (r + 1 > nr - 1) || (c + 1 > nc - 1);
    e[7] = ((r < (nr-2)) && !mask[r+2][c]) || (r + 2 > nr - 2);
  }

  if (r_lo && (!mask[r-1][c]) && e[0] && e[1] && e[2]){	/* A */
    inv_enq (nodes, inv_node (matrix[r-1][c], r-1, c));
  }
  if (c_lo && (!mask[r][c-1]) && e[1] && e[3] && e[5]){	/* B */
    inv_enq (nodes, inv_node (matrix[r][c-1], r, c-1));
  }
  if (c_hi && (!mask[r][c+1]) && e[2] && e[4] && e[6]){	/* C */
    inv_enq (nodes, inv_node (matrix[r][c+1], r, c+1));
  }
  if (r_hi && (!mask[r+1][c]) && e[5] && e[6] && e[7]){	/* D */
    inv_enq (nodes, inv_node (matrix[r+1][c], r+1, c));
  }

}
