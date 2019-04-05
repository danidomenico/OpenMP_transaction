/**
 * Serial implementation of halving shuffle
 *
 * \file serial.h
 * \author Andrew Borzenko
 * \date 02-27-09
 */

#pragma once
#ifndef HALF_SERIAL_H
#define HALF_SERIAL_H

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

void
half(
  int2D*		matrix,			/* to shuffle */
  int		nr,			/* row size */
  int		nc			/* column size */
);

#endif /* HALF_SERIAL_H */
