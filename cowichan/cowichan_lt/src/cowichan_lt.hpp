/**
 * \file cowichan_lt/src/cowichan_lt.hpp
 * \brief Datatypes and common routines for Cowichan programs; LinuxTuples implementation.
 */
#ifndef __cowichan_lt_hpp__
#define __cowichan_lt_hpp__

#include "../../cowichan/cowichan.hpp"

// COWICHAN DEFINITIONS =====================================================//
// aka. "inputs" to the toys, and chaining functions.
/**
 * The LinuxTuples implementation of the Cowichan problem set.
 */
class CowichanLinuxTuples : public Cowichan {
public:

	/**
	 * The host where the LinuxTuples server can be found.
	 */
	static const char* SERVER;

	/**
	 * The port number the LinuxTuples server is running on.
	 */
	static const int PORT = 25000;

	/**
	 * The number of worker processes to spawn.
	 */
	static const int NUM_WORKERS = 2;

protected: // chaining functions

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

protected:

  /**
   * Runs the cowichan problem set, chained together.
   * \param use_randmat  true: generate a random matrix.
   *             false: use a window of the mandelbrot set.
   * \param use_thresh  true: use image thresholding for int->bool.
   *            false: use invasion percolation for int->bool.
   */
  void chain(bool use_randmat, bool use_thresh);

};

#endif

