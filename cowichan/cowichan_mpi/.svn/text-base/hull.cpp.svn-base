/**
 * \file cowichan_mpi/hull.cpp
 * \brief MPI convex hull implementation.
 * \see CowichanMPI::hull
 */

#include "cowichan_mpi.hpp"

namespace cowichan_mpi {

/**
 * Do work (find highest cross-product among all points and split again).
 * \param world MPI world.
 * \param points points to run hull on.
 * \param n number of points.
 * \param hullPoints list of points in convex hull.
 * \param hn number of points currently in convex hull.
 * \param p1 boundary point #1.
 * \param p2 boundary point #2.
 */
void split_mpi (mpi::communicator world, PointVector points, index_t n,
    PointVector hullPoints, index_t* hn, Point& p1, Point& p2);

/**
 * Reduction function for minimum x point.
 */
struct minimum_x_pt {

  /**
   * Get minimum x point.
   * \param a point 1.
   * \param b point 2.
   * \return Minimum x point.
   */
  Point operator()(Point a, Point b)
  {
    if (a.x < b.x) {
      return a;
    }
    return b;
  }
};

/**
 * Reduction function for maximum x point.
 */
struct maximum_x_pt {

  /**
   * Get maximum x point.
   * \param a point 1.
   * \param b point 2.
   * \return Maximum x point.
   */
  Point operator()(Point a, Point b)
  {
    if (a.x > b.x) {
      return a;
    }
    return b;
  }
};

/**
 * Cross point.
 */
struct pt_cross {
  /**
   * Point.
   */
  Point p;

  /**
   * Cross value.
   */
  real cross;

  /**
   * Serialization method for pt_cross class.
   * \param ar archive to use.
   * \param version pt_cross class version.
   */
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & p;
    ar & cross;
  }
};

/**
 * Reduction function for maximum cross point.
 */
struct maximum_cp {

  /**
   * Get maximum cross point.
   * \param a point 1.
   * \param b point 2.
   * \return Maximum cross point.
   */
  pt_cross operator()(pt_cross a, pt_cross b)
  {
    if (a.cross > b.cross) {
      return a;
    }
    return b;
  }
};

}

/*****************************************************************************/

void CowichanMPI::hull(PointVector points, PointVector hullPoints)
{
  index_t i;
  Point min_p, max_p;
  Point min_p_local, max_p_local;
  index_t lo, hi;

  min_p_local = points[0];
  max_p_local = points[0];
  index_t hn = 0;

  // figure out the points with minimum and maximum x values
  if (get_block(world, 1, n, &lo, &hi)) {
    for (i = lo; i < hi; i++) {
      if (min_p_local.x > points[i].x) {
        min_p_local = points[i];
      }
      if (max_p_local.x < points[i].x) {
        max_p_local = points[i];
      }
    }
  }
  
  // reduce to min_p and max_p
  all_reduce (world, min_p_local, min_p, minimum_x_pt());
  all_reduce (world, max_p_local, max_p, maximum_x_pt());

  // use these as initial pivots
  split_mpi (world, points, n, hullPoints, &hn, min_p, max_p);
  split_mpi (world, points, n, hullPoints, &hn, max_p, min_p);
}

/*****************************************************************************/

namespace cowichan_mpi {

void split_mpi (mpi::communicator world, PointVector points, index_t n,
    PointVector hullPoints, index_t* hn, Point& p1, Point& p2)
{
  pt_cross max_cp, max_cp_local;
  max_cp_local.cross = -std::numeric_limits<real>::infinity ();
  index_t lo, hi;
  index_t i;

  // compute the signed distances from the line for each point
  if (get_block(world, 0, n, &lo, &hi)) {
    for (i = lo; i < hi; i++) {
      real currentCross = Point::cross (p1, p2, points[i]);
      if (currentCross > max_cp_local.cross) {
        max_cp_local.p = points[i];
        max_cp_local.cross = currentCross;
      }
    }
  }

  // reduce max_cp
  all_reduce(world, max_cp_local, max_cp, maximum_cp());

  // is there a point in the positive half-space?
  // if so, it has maximal distance, and we must recurse based on that point.
  if (max_cp.cross > 0.0) {
  // recurse on the new set with the given far point
  split_mpi (world, points, n, hullPoints, hn, p1, max_cp.p);
  split_mpi (world, points, n, hullPoints, hn, max_cp.p, p2);
  return;
  } 

  // otherwise, it's not on the right side; we don't need to split anymore.
  // this is because all points are inside the hull when we use this half-space.
  // add the first point and return.
  hullPoints[(*hn)++] = p1;
}

}

