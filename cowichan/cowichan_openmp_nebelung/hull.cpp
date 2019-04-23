/**
 * \file cowichan_openmp_nebelung/norm.cpp
 * \brief OpenMP hull implementation (transactional memory).
 * \see CowichanOpenMP::hull
 */

#include "../cowichan_openmp/cowichan_openmp.hpp"

namespace cowichan_openmp
{

/**
 * Runs quickhull algorithm.
 * \param pointsIn input points.
 * \param n number of inputs points to use.
 * \param pointsOut output points.
 * \param hn number of output points generated so far.
 */
void quickhull(PointVector pointsIn, index_t n, PointVector pointsOut, index_t* hn);

/**
 * Recursive step of the quickhull algorithm - compute hull on one side of the
 * splitting line.
 * \param pointsIn input points.
 * \param n number of inputs points to use.
 * \param pointsOut output points.
 * \param hn number of output points generated so far.
 * \param p1 first point of the splitting line (p1,p2).
 * \param p2 second point of the splitting line (p1,p2).
 */
void split(PointVector pointsIn, index_t n, PointVector pointsOut, index_t* hn,
    Point* p1, Point* p2);

}

/*****************************************************************************/

/**
 * For description see \ref hull_sec
 *
 * Runs quickhull algorithm until all points have been used up from the
 * original vector. At each step the hull points are marked as used and a new
 * convex hull is computed on the rest of points.
 * The points that have been used up are in the range (n - hn, n), i.e. at
 * the end of pointsIn vector.
 * NOTE: pointsIn vector gets modified by the algorithm.
 */
void CowichanOpenMP::hull (PointVector pointsIn, PointVector pointsOut)
{
  index_t hn = 0;
  index_t previous_hn = 0;

  // while not all points are used up then run quickhull on the rest of points
  while (n != hn) {
    // exclude added points from pointsIn by swapping them with points from the
    // end of pointsIn vector in range (0, n - nused)
    index_t added_i;
#pragma omp parallel for schedule(static)
    for(added_i = previous_hn; added_i < hn; added_i++) {
      // search for the added point
      for(index_t i = 0; i < n - previous_hn; i++) {
        if ((pointsIn[i].x == pointsOut[added_i].x) && (pointsIn[i].y == pointsOut[added_i].y)) {
          Point tmp = pointsIn[i];
          pointsIn[i] = pointsIn[n - added_i - 1];
          pointsIn[n - added_i - 1] = tmp;
          break;
        }
      }
    }
    
    previous_hn = hn;
    quickhull(pointsIn, n - hn, pointsOut, &hn);
  }
}

/*****************************************************************************/

namespace cowichan_openmp
{

void quickhull(PointVector pointsIn, index_t n, PointVector pointsOut, index_t* hn) {
    // base case
    if (n == 1) {
        pointsOut[(*hn)++] = pointsIn[0];
        return;
    }

    Point* minPoint;
    Point* maxPoint;

    // checking cutoff value here prevents allocating unnecessary memory
    // for the reduction
    if(n > CowichanOpenMP::HULL_CUTOFF) {
        minPoint = &pointsIn[0];
        maxPoint = &pointsIn[0];

        #pragma omp parallel for schedule(static) transaction only(minPoint, maxPoint) exclude(pointsIn)
        for(index_t i = 1; i < n; i++) {
            if (minPoint->x > pointsIn[i].x) {
                minPoint = &pointsIn[i];
            }
            if (maxPoint->x < pointsIn[i].x) {
                maxPoint = &pointsIn[i];
            }
        }
  
    } else {
        minPoint = &pointsIn[0];
        maxPoint = &pointsIn[0];

        // figure out the points with minimum and maximum x values
        index_t i;
        for (i = 1; i < n; i++) {
            if (minPoint->x > pointsIn[i].x) {
                minPoint = &pointsIn[i];
            }
            if (maxPoint->x < pointsIn[i].x) {
                maxPoint = &pointsIn[i];
            }
        }
    }

    // use these as initial pivots
    split (pointsIn, n, pointsOut, hn, minPoint, maxPoint);
    split (pointsIn, n, pointsOut, hn, maxPoint, minPoint);
}

void split(PointVector pointsIn, index_t n, PointVector pointsOut, index_t* hn,
           Point* p1, Point* p2) {

    Point* maxPoint;
    real maxCross;

    // checking cutoff value here prevents allocating unnecessary memory
    // for the reduction
    if (n > CowichanOpenMP::HULL_CUTOFF) {
        maxPoint = &pointsIn[0];
        maxCross = Point::cross (*p1, *p2, pointsIn[0]);

        // compute the signed distances from the line for each point //Não precisa excluir currentCross, pois é variável local
        #pragma omp parallel for schedule(static) transaction only(maxPoint, maxCross)
        for (index_t i = 1; i < n; i++) {
            real currentCross = Point::cross (*p1, *p2, pointsIn[i]);
            if (currentCross > maxCross) {
                maxPoint = &pointsIn[i];
                maxCross = currentCross;
            }
        }
    } else {
        maxPoint = &pointsIn[0];
        maxCross = Point::cross (*p1, *p2, pointsIn[0]);

        // compute the signed distances from the line for each point
        for (index_t i = 1; i < n; i++) {
            real currentCross = Point::cross (*p1, *p2, pointsIn[i]);
            if (currentCross > maxCross) {
                maxPoint = &pointsIn[i];
                maxCross = currentCross;
            }
        }
    }

    // is there a point in the positive half-space?
    // if so, it has maximal distance, and we must recurse based on that point.
    if (maxCross > 0.0) {
        // recurse on the new set with the given far point
        split (pointsIn, n, pointsOut, hn, p1, maxPoint);
        split (pointsIn, n, pointsOut, hn, maxPoint, p2);
        return;
    }

    // otherwise, it's not on the right side; we don't need to split anymore.
    // this is because all points are inside the hull when we use this half-space.
    // add the first point and return.
    pointsOut[(*hn)++] = *p1;
}

}

