/**
 * Serial implementation of random matrix generation
 *
 * \file serial.h
 * \author Andrew Borzenko
 * \date 01-26-09
 */

#pragma once
#ifndef RANDMAT_SERIAL_H
#define RANDMAT_SERIAL_H

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

/*
 * @ randmat : do random matrix generation
 * > none
 * + fill matrix
 */

void
randmat(
  int2D*		matrix,			/* to fill */
  int		nr,			/* row size */
  int		nc,			/* column size */
  int		limit,			/* value limit */
  int		seed			/* RNG seed */
);

#endif /* RANDMAT_SERIAL_H */
