/**
 * Parallel implementation of convex hull
 *
 * \file parallel.h
 * \author Andrew Borzenko
 * \date 03-06-09
 */

#pragma once
#ifndef HULL_PARALLEL_H
#define HULL_PARALLEL_H

struct minimum_x_pt {
  pt operator()(pt a, pt b)
  {
    if (a.x < b.x) {
      return a;
    }
    return b;
  }
};

struct maximum_x_pt {
  pt operator()(pt a, pt b)
  {
    if (a.x > b.x) {
      return a;
    }
    return b;
  }
};

struct pt_cross {
  pt p;
  real cross;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & p;
    ar & cross;
  }
};

struct maximum_cp {
  pt_cross operator()(pt_cross a, pt_cross b)
  {
    if (a.cross > b.cross) {
      return a;
    }
    return b;
  }
};

/*--------------------------------------------------------------*/
/* public functions						*/
/*--------------------------------------------------------------*/

void hull_mpi (mpi::communicator world,
               pt1D* points,     // list of points
               int n,            // number of points
               pt1D* hullPoints, // list of points in convex hull
               int* hn);         // number of points in convex hull

/*--------------------------------------------------------------*/
/* private functions						*/
/*--------------------------------------------------------------*/

void split_mpi (mpi::communicator world,
                pt1D* points,     // list of points
                int n,            // number of points
                pt1D* hullPoints, // list of points in convex hull
                int* hn,          // number of points in convex hull
                pt p1,            // boundary point #1
                pt p2);           // boundary point #2

inline real cross_mpi (pt l1,    // cross product of
                       pt l2,    // (l1,l2) and (l1,p)
                       pt p);

#endif /* HULL_PARALLEL_H */
