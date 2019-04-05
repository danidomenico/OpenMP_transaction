/**
 * \file cowichan_serial.hpp
 * \brief Serial implementation of Cowichan problems.
 */

#ifndef __cowichan_serial_hpp__
#define __cowichan_serial_hpp__

#include "../cowichan/cowichan.hpp"

/**
 * \brief Additional classes and functions specific to serial implementation.
 */
namespace cowichan_serial
{
}

// using a namespace to avoid (documentation) name clashes
using namespace cowichan_serial;

/**
 * \brief Serial implementation.
 *
 * This implementation can serve as a reference when comparing to parallel
 * programming systems in terms of performance, code size, complexity, etc.
 */
class CowichanSerial : public Cowichan {
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

};

#endif

