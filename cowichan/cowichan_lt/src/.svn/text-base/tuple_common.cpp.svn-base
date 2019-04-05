/**
 * \file cowichan_lt/src/tuple_common.cpp
 * \brief Implementation of common tuple application logic.
 */

#include "tuple_common.hpp"
#include <iostream>

void TupleApplication::addInput(int name, void* data) {
	inputs[name] = data;
}

void TupleApplication::addOutput(int name, void* data) {
	outputs[name] = data;
}

int TupleApplication::start(const char* host, int portNumber, int numWorkers) {

	// connect to the tuple server.
	if (get_server_portnumber(&ctx)) {
		strcpy(ctx.peername, host);
		ctx.portnumber = portNumber;
	}

	// spawn worker processes and record their PIDs
	int workers[numWorkers];
	for (int i = 0; i < numWorkers; ++i) {		
		pid_t pid = fork();
		if (pid == 0) {
			this->work();
			exit(0);
		}
		workers[i] = pid;
	}

	// spawn input consumer
	if (fork() == 0) {
		this->consumeInput();
		exit(0);
	}

	// run the output producer.
	// we run it in this thread so that its outputs are saved.
	produceOutput();

	// kill all of the worker processes (they spin)
	// TODO send them tuple-space quit messages
	for (int i = 0; i < numWorkers; ++i) {
		kill(workers[i], SIGKILL);
	}

	// everything was successful.
	return 0;

}

