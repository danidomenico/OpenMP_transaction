/**
 * \file cowichan_tbb.cpp
 * \brief This file contains main method that drives the TBB implementation.
 */

#include "cowichan_tbb.hpp"

/**
 * Main method - creates a CowichanTBB instance and executes Cowichan::main.
 * \param argc number of command line arguments.
 * \param argv command line arguments.
 */
int main(int argc, char* argv[])
{
  Cowichan* tbb = new CowichanTBB ();

  task_scheduler_init init(2);

  tbb->main(argc, argv, false, true);

  return 0;
}

