/**
 * Parallel implementation of vector difference
 *
 * \file parallel.h
 * \author Andrew Borzenko
 * \date 02-27-09
 */

#pragma once
#ifndef VECDIFF_PARALLEL_H
#define VECDIFF_PARALLEL_H

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

void
vecdiff_mpi(mpi::communicator world,
  real1D*	left,			/* left vector */
  real1D*	right,			/* right vector */
  int		n,			/* vector lengths */
  real	      * diff			/* norm-1 difference */
);

#endif /* VECDIFF_PARALLEL_H */
