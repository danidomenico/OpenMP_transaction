/**
 * \file cowichan_lt/src/cowichan_lt.cpp
 * \brief Entry point for the cowichan linuxtuples application.
 */

#include "cowichan_lt.hpp"

/**
 * Entry point of the program.
 */
int main(int argc, char* argv[]) {
	Cowichan* linuxTuples = new CowichanLinuxTuples();
	linuxTuples->main(argc, argv, true, true);
	return 0;
}

/**
 * The host where the LinuxTuples server can be found.
 */
const char* CowichanLinuxTuples::SERVER = "localhost";
