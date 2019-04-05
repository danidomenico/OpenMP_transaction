/**
 * \file cowichan_openmp.hpp
 * \brief Open Multi-Processing (OpenMP) implementation of Cowichan problems.
 */

#ifndef __cowichan_openmp_hpp__
#define __cowichan_openmp_hpp__

#include "../cowichan/cowichan.hpp"

/**
 * \brief Additional classes and functions specific to openmp implementation.
 */
namespace cowichan_openmp
{
}

// using a namespace to avoid (documentation) name clashes
using namespace cowichan_openmp;

#include <omp.h>

/**
 * \brief Open Multi-Processing (OpenMP) implementation.
 *
 * Tags: shared memory, data parallel, shared variables, task based,
 * compiler specific.
 */
class CowichanOpenMP : public Cowichan {
protected:

  void mandel(IntMatrix matrix);
  void randmat(IntMatrix matrix);
  void half(IntMatrix matrixIn, IntMatrix matrixOut);
  void invperc(IntMatrix matrix, BoolMatrix mask);
  void thresh(IntMatrix matrix, BoolMatrix mask);
  void life(BoolMatrix matrixIn, BoolMatrix matrixOut);
  void winnow(IntMatrix matrix, BoolMatrix mask, PointVector points);
  void norm(PointVector pointsIn, PointVector pointsOut);
  void hull(PointVector pointsIn, PointVector pointsOut);
  void outer(PointVector points, Matrix matrix, Vector vector);
  void gauss(Matrix matrix, Vector target, Vector solution);
  void sor(Matrix matrix, Vector target, Vector solution);
  void product(Matrix matrix, Vector candidate, Vector solution);
  real vecdiff(Vector actual, Vector computed);

public:

  /**
   * Cutoff value n for hull.
   */
  static const index_t HULL_CUTOFF = 20000;

};

#endif

