/**
 * Serial implementation of outer product matrix
 *
 * \file serial.h
 * \author Andrew Borzenko
 * \date 02-27-09
 */

#pragma once
#ifndef OUTER_SERIAL_H
#define OUTER_SERIAL_H

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

void
outer(
  pt1D*		ptVec,			/* vector of points */
  real2D*	matrix,			/* matrix to fill */
  real1D*	realVec,		/* vector to fill */
  int		n			/* size */
);

#endif /* OUTER_SERIAL_H */
