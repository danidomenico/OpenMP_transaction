/**
 * Parallel implementation of invasion percolation
 *
 * \file parallel.h
 * \author Andrew Borzenko
 * \date 02-28-09
 */

#pragma once
#ifndef INVPERC_PARALLEL_H
#define INVPERC_PARALLEL_H

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

void invperc_mpi (mpi::communicator world,
                  int2D*		matrix,			/* matrix to invade */
                  bool2D*	mask,			/* mask to fill */
                  int		nr,			/* row size */
                  int		nc,			/* column size */
                  real		fraction);		/* how much to fill */

/*--------------------------------------------------------------*/
/* private functions 					*/
/*--------------------------------------------------------------*/

void inv_deq_mpi (mpi::communicator world,
                  mapNodesType* nodes,
                  int	      * r,			/* row index */
                  int	      * c);			/* column index */

void inv_enq_mpi (mpi::communicator world,
                  mapNodesType* nodes,
                  node_p	node);			/* what to enqueue */

node_p inv_node_mpi (mpi::communicator world,
                     int		val,			/* location value */
                     int		r,			/* row index */
                     int		c);			/* column index */

void inv_enqPt_mpi (mpi::communicator world,
                    mapNodesType* nodes,
                    int2D*		matrix,			/* matrix of values */
                    bool2D*	mask,			/* mask to be filled */
                    int		nr,			/* number of rows */
                    int		nc,			/* number of columns */
                    int		r,			/* point row */
                    int		c);			/* point column */

#endif /* INVPERC_PARALLEL_H */
