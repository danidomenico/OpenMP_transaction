/**
 * Serial implementation of generic gauss
 *
 * \file serial.h
 * \author Andrew Borzenko
 * \date 02-03-09
 */

#pragma once
#ifndef GAUSS_SERIAL_H
#define GAUSS_SERIAL_H

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

/*
 * @ gauss : do Gaussian elimination
 * > none
 * + decompose matrix and fill in answer vector
 */

void
gauss(
  real2D*	matrix,			/* to solve */
  real1D*	vector,			/* target vector */
  real1D*	answer,			/* solution found */
  int		n			/* size */
);

#endif /* GAUSS_SERIAL_H */
