/**
 * Serial implementation of weighted point selection
 *
 * \file serial.h
 * \author Andrew Borzenko
 * \date 02-23-09
 */

#pragma once
#ifndef WINNOW_SERIAL_H
#define WINNOW_SERIAL_H

/*--------------------------------------------------------------*/
/* private function prototypes					*/
/*--------------------------------------------------------------*/

int
winnow_redBool2DCount(
  bool2D*	mask,			/* to reduce */
  int		nr,			/* row size */
  int		nc			/* column size */
);

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

/*
 * @ winnow : do point winnowing
 * > none
 * + create vector of points
 */

void
winnow(
  int2D*		matrix,			/* point values */
  bool2D*	mask,			/* suitable points */
  int		nr,			/* row size */
  int		nc,			/* column size */
  pt1D*		pt,			/* points to create */
  int		npt			/* number of points */
);

#endif /* WINNOW_SERIAL_H */
