/**
 * \file cowichan_lt/src/tuple_common.hpp
 * \brief Header file for tuple applications.
 */

/**
 * The common tuple application class allows LinuxTuples applications to be
 * easily created. The class takes care of process creation and destruction,
 * and makes sure outputs get carried onto the main process. Additionally,
 * virtual functions are provided for workers and input/output processes.
 * Things that need to be done in the tuple space should consider inheriting
 * from this class.
 */

#ifndef __TUPLE_COMMON_HPP__
#define __TUPLE_COMMON_HPP__

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/wait.h>
#include <cassert>
#include <sys/mman.h>

extern "C" {
	#include "tuple.h"
}

#include <map>

/**
 * The class which all tuple applications should inherit from.
 */
class TupleApplication {
protected:

	/**
	 * Inputs that should be available to all processes.
	 */
	std::map<int, void*> inputs;

	/**
	 * Outputs that should be available to all processes, and
	 * that should be writable by the outputProducer process.
	 */
	std::map<int, void*> outputs;

	/**
	 * Tuple-space context (LinuxTuples-specific).
	 */
	struct context ctx;

	/**
	 * The consume input process is spawned once and should distribute
	 * tasks to the worker processes.
	 */
	virtual void consumeInput()		= 0;

	/**
	 * Worker processes are created and killed after the output process
	 * has finished.
	 */
	virtual void work()				= 0;

	/**
	 * The output producer decides when the tuple application is finished;
	 * once this function returns, the tuple application is complete.
	 */
	virtual void produceOutput()	= 0;

public:

	/**
	 * Set up input pointers.
	 * \param name the "name" of the data
	 * \param data the pointer to passed to the tuple-space
	 */
	void addInput(int name, void* data);

	/**
	 * Set up output pointers.
	 * \param name the "name" of the data
	 * \param data the pointer to passed to the tuple-space
	 */
	void addOutput(int name, void* data);

	/**
 	 * Starts the tuple-space job.
	 */
	int start(const char* host, int portNumber, int numWorkers);

};

#endif

