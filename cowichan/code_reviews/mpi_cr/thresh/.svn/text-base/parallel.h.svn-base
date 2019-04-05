/**
 * Parallel implementation of histogram thresholding
 *
 * \file parallel.h
 * \author Andrew Borzenko
 * \date 03-02-09
 */

#pragma once
#ifndef THRESH_PARALLEL_H
#define THRESH_PARALLEL_H

template<typename T>
struct sum {
  typedef T result_type;

  T operator()(T a, T b)
  {
    return a + b;
  }
};

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

void thresh_mpi (mpi::communicator world,
                 int2D*		matrix,			/* to threshold */
                 bool2D*	mask,			/* threshold mask */
                 int		nr,			/* row size */
                 int		nc,			/* column size */
                 real		fraction);		/* how much to keep */

#endif /* THRESH_PARALLEL_H */
