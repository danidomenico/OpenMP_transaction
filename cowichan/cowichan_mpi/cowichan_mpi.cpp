/**
 * \file cowichan_mpi.cpp
 * \brief This file contains main method that drives the MPI implementation.
 */

#include "cowichan_mpi.hpp"

/**
 * Main method - creates a CowichanMPI instance and executes Cowichan::main.
 * \param argc number of command line arguments.
 * \param argv command line arguments.
 */
int main(int argc, char* argv[])
{
  mpi::environment env(argc, argv);
  mpi::communicator world;

#ifdef OUTPUT_DATA
  std::cout << "I am process" << world.rank() << std::endl;
#endif

  Cowichan* mpi = new CowichanMPI (world);
  
  mpi->main(argc, argv, false, true);

  return 0;

}

CowichanMPI::CowichanMPI(const mpi::communicator& world) : world(world)
{
}


namespace cowichan_mpi
{

bool get_block(const mpi::communicator& world, index_t lo, index_t hi,
    index_t* start, index_t* end)
{
  return get_block(world, lo, hi, start, end, world.rank());
}

bool get_block(const mpi::communicator& world, index_t lo, index_t hi,
    index_t* start, index_t* end, index_t rank)
{
  index_t size = world.size();
  
  index_t nl;    // number of elements
  index_t num;   // number to do
  index_t extra; // spillage

  nl    = hi - lo;
  num   = nl / size;
  extra = nl % size;

  if ((nl <= 0) || (rank >= nl)) {
    // do nothing
    *start = 0;
    *end = -1;
  }
  else {
    // do share of work
    if (rank < extra){
      num += 1;
      *start = lo + rank * num;
    } else {
      *start = lo + (extra * (num + 1)) + ((rank - extra) * num);
    }
    *end = *start + num;
  }

  return (*end != -1);
}

}

