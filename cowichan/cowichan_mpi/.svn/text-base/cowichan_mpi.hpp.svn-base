/**
 * \file cowichan_mpi.hpp
 * \brief Boost Message Passing Interface (MPI) implementation of Cowichan
 * problems.
 */

#ifndef __cowichan_mpi_hpp__
#define __cowichan_mpi_hpp__

#include "../cowichan/cowichan.hpp"

#include <boost/mpi.hpp>
namespace mpi = boost::mpi;

/**
 * \brief Additional classes and functions specific to mpi implementation.
 */
namespace cowichan_mpi
{

/**
 * Initialise the random state.
 * \param seed seed value
 * \param width number of participants
 * \param state per-thread state vector
 * \param aPrime new multiplicative
 * \param cPrime new additive value
 */
void randStateInit(INT_TYPE seed, index_t width, IntVector state,
    INT_TYPE* aPrime, INT_TYPE* cPrime);

/**
 * Calculate a point in the mandelbrot set.
 * \param x x co-ordinate in the mandelbrot set.
 * \param y y co-ordinate in the mandelbrot set.
 * \return lesser of number of iterations to diverge and maximum iterations.
 */
INT_TYPE mandel_calc (real x, real y);

/**
 * Get a block (start, end) to work on in the range (lo, hi) for the current
 * process.
 * \param world communicator.
 * \param lo lower end of the range.
 * \param hi higher end of the range.
 * \param start first index to work on.
 * \param end last index to work on.
 * \return Whether any work is available for the current process.
 */
bool get_block(const mpi::communicator& world, index_t lo, index_t hi,
    index_t* start, index_t* end);

/**
 * Get a block (start, end) to work on in the range (lo, hi) for process rank.
 * \param world communicator.
 * \param lo lower end of the range.
 * \param hi higher end of the range.
 * \param start first index to work on.
 * \param end last index to work on.
 * \param rank process to get work for.
 * \return Whether any work is available for process rank.
 */
bool get_block(const mpi::communicator& world, index_t lo, index_t hi,
    index_t* start, index_t* end, index_t rank);

}


// using a namespace to avoid (documentation) name clashes
using namespace cowichan_mpi;


namespace boost {
namespace serialization {

/**
 * Serialization method for WeightedPoint class.
 * \param ar archive to use.
 * \param p weighted point to serialize/deserialize.
 * \param version WeightedPoint class version.
 */
template<class Archive>
void serialize(Archive& ar, WeightedPoint& p, const unsigned int version)
{
  ar & p.point.x;
  ar & p.point.y;
  ar & p.weight;
}

} // namespace serialization
} // namespace boost

BOOST_IS_MPI_DATATYPE(WeightedPoint)
BOOST_CLASS_TRACKING(WeightedPoint,track_never)


namespace boost {
namespace serialization {

/**
 * Serialization method for Point class.
 * \param ar archive to use.
 * \param p point to serialize/deserialize.
 * \param version Point class version.
 */
template<class Archive>
void serialize(Archive& ar, Point& p, const unsigned int version)
{
  ar & p.x;
  ar & p.y;
}

} // namespace serialization
} // namespace boost

BOOST_IS_MPI_DATATYPE(Point)
BOOST_CLASS_TRACKING(Point,track_never)


/**
 * \brief Boost Message Passing Interface (MPI) implementation.
 *
 * Boost MPI is a C++ interface that works with many MPI implementations. For
 * example: MPICH1, MPICH2, OpenMPI.
 *
 * Tags: distributed memory, message passing, mpi wrapper.
 */
class CowichanMPI : public Cowichan {
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

public:

  /**
   * Constructs a cowichan mpi object.
   * \param world global communicator.
   */
  CowichanMPI(const mpi::communicator& world);

  /**
   * Global communicator.
   */
  const mpi::communicator& world;

};

#endif

