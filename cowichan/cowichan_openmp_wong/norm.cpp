/**
 * \file cowichan_openmp_wong/norm.cpp
 * \brief OpenMP norm implementation (transactional memory).
 * \see CowichanOpenMP::norm
 */

#include "../cowichan_openmp/cowichan_openmp.hpp"

namespace cowichan_openmp
{

/**
 * Find min/max x/y coordinates.
 * \param points point vector.
 * \param n number of points.
 * \param minPoint min x/y values.
 * \param maxPoint max x/y values.
 */
void findMinMax(PointVector points, index_t n, Point* minPoint, Point* maxPoint);

}

/*****************************************************************************/

void CowichanOpenMP::norm (PointVector pointsIn, PointVector pointsOut)
{
  Point minPoint, maxPoint;
  real sclX, sclY; // scaling factors
  index_t i;

  // compute scaling factors
  findMinMax(pointsIn, n, &minPoint, &maxPoint);

  sclX = (real)((maxPoint.x == minPoint.x) ?
      0.0 : 1.0 / (maxPoint.x - minPoint.x));
  sclY = (real)((maxPoint.y == minPoint.y) ?
      0.0 : 1.0 / (maxPoint.y - minPoint.y));

  // scale
#pragma omp parallel for schedule(static)
  for (i = 0; i < n; i++) {
    pointsOut[i].x = sclX * (pointsIn[i].x - minPoint.x);
    pointsOut[i].y = sclY * (pointsIn[i].y - minPoint.y);
  }

}

/*****************************************************************************/

namespace cowichan_openmp {

void findMinMax(PointVector points, index_t n, Point* minPoint, Point* maxPoint) {

    minPoint->x = points[0].x;
    minPoint->y = points[0].y;
    maxPoint->x = points[0].x;
    maxPoint->y = points[0].y;
    
    #pragma omp parallel for schedule(static) transaction
    for(index_t i = 1; i < n; i++) {
        if(points[i].x < minPoint->x) {
            minPoint->x = points[i].x;
        }
        if(points[i].y < minPoint->y) {
            minPoint->y = points[i].y;
        }
        if(points[i].x > maxPoint->x) {
            maxPoint->x = points[i].x;
        }
        if (points[i].y > maxPoint->y) {
            maxPoint->y = points[i].y;
        }
    }

}

} //END namespace cowichan_openmp
