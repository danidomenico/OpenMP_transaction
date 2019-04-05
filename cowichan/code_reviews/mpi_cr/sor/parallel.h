/**
 * Parallel implementation of successive over-relaxation
 *
 * \file parallel.h
 * \author Andrew Borzenko
 * \date 03-02-09
 */

#pragma once
#ifndef SOR_PARALLEL_H
#define SOR_PARALLEL_H

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

void sor_mpi (mpi::communicator world,
              real2D*	matrix,			/* to solve */
              real1D*	vector,			/* target vector */
              real1D*	answer,			/* solution found */
              int		n,			/* size */
              real		tol);			/* tolerance on answer */

#endif /* SOR_PARALLEL_H */
