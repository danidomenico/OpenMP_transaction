/**
 * Serial implementation of matrix-vector product
 *
 * \file serial.h
 * \author Andrew Borzenko
 * \date 02-02-09
 */

#pragma once
#ifndef PRODUCT_SERIAL_H
#define PRODUCT_SERIAL_H

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

/*
 * @ product : do matrix-vector product
 * > none
 * + fill result vector
 */

void
product(
  real2D*	matrix,			/* to multiply by */
  real1D*	vector,			/* to be multiplied */
  real1D*	result,			/* result of multiply */
  int		nr,			/* row size */
  int		nc			/* column size */
);

#endif /* PRODUCT_SERIAL_H */
