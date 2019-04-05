/**
 * \file cowichan_openmp.cpp
 * \brief This file contains main method that drives the OpenMP implementation.
 */

#include "cowichan_openmp.hpp"

/**
 * Main method - creates a CowichanOpenMP instance and executes Cowichan::main.
 * \param argc number of command line arguments.
 * \param argv command line arguments.
 */
int main(int argc, char* argv[])
{
  Cowichan* openmp = new CowichanOpenMP ();

  //omp_set_num_threads(2);

  openmp->main(argc, argv, false, true);

  return 0;
}

