/**
 * Serial implementation of convex hull
 *
 * \file serial.h
 * \author Andrew Borzenko
 * \date 03-06-09
 */

#pragma once
#ifndef HULL_SERIAL_H
#define HULL_SERIAL_H

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

void hull (pt1D* points,     // list of points
           int n,            // number of points
           pt1D* hullPoints, // list of points in convex hull
           int* hn);         // number of points in convex hull

/*--------------------------------------------------------------*/
/* private functions						*/
/*--------------------------------------------------------------*/

void split (pt1D* points,    // list of points
            int n,            // number of points
            pt1D* hullPoints, // list of points in convex hull
            int* hn,          // number of points in convex hull
            pt p1,            // boundary point #1
            pt p2);           // boundary point #2

inline real cross (pt l1,    // cross product of
                   pt l2,    // (l1,l2) and (l1,p)
                   pt p);

#endif /* HULL_SERIAL_H */
