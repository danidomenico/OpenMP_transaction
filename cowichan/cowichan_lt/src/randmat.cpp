/**
 * \file cowichan_lt/src/randmat.cpp
 * \brief LinuxTuples randmat implementation.
 * \see CowichanLinuxTuples::randmat
 */

#include <iostream>
#include <cstdio>
#include "randmat.hpp"

const char* LTRandmat::REQUEST = "randmat request";

void CowichanLinuxTuples::randmat(IntMatrix matrix) {
	LTRandmat app;
	app.addOutput(0, matrix);
	app.start(SERVER, PORT, NUM_WORKERS);
}

inline INT_TYPE LTRandmat::next(INT_TYPE& current) const {
	return (RANDMAT_A * current + RANDMAT_C) % RAND_M;
}

void LTRandmat::setup() {

    IntVector state = NEW_VECTOR_SZ(INT_TYPE, RANDMAT_NR);

    // generate first column values
    VECTOR(state, 0) = RAND_SEED % RAND_M;
    for (index_t row = 1; row < RANDMAT_NR; ++row) {
      VECTOR(state, row) = next(VECTOR(state, row - 1));
    }

    // generate the A and C values for the next(k) method.
    aPrime = RANDMAT_A;
    cPrime = 1;
    for (index_t i = 1; i < RANDMAT_NR; ++i) {
      cPrime = (cPrime + aPrime) % RAND_M;
      aPrime = (aPrime * RANDMAT_A) % RAND_M;
    }
    cPrime = (cPrime * RANDMAT_C) % RAND_M;

    // emit the state vector to the tuple space.
    tuple *init = make_tuple("ss", "randmat state", "");
    init->elements[1].data.s.len = sizeof(INT_TYPE) * RANDMAT_NR;
    init->elements[1].data.s.ptr = (char*) state;
    put_tuple(init, &ctx);
    destroy_tuple(init);

}

void LTRandmat::consumeInput() {

	// create the first, seed column
	setup();

	// tuple template
	tuple *send = make_tuple("si", REQUEST, 0);

	// send off a request for each grid row.
	for (size_t y = 0; y < RANDMAT_NR; ++y) {
		send->elements[1].data.i = y;
		put_tuple(send, &ctx);
	}
	
	// destroy the template tuple
	destroy_tuple(send);

}

void LTRandmat::work() {

	tuple *recv = make_tuple("s?", REQUEST);
	tuple *tmpInit = make_tuple("s?", "randmat state");
	tuple *init = NULL;
	IntVector initVector = NULL;
	
	// satisfy randmat requests.
	while (1) {

		// block until we receive a tuple.
		tuple* gotten = get_tuple(recv, &ctx);

		// grab a copy of the initializer, tuple if we haven't already
		if (!init) {
			init = read_tuple(tmpInit, &ctx);
			initVector = (IntVector) init->elements[1].data.s.ptr;
		}

		// copy over row co-ordinate of the computation; create
		// a buffer for the results of the computation.
		size_t row = gotten->elements[1].data.i;
		tuple *send = make_tuple("sis", "randmat done", row, "");
		IntVector buffer = (IntVector) NEW_VECTOR_SZ(INT_TYPE, RANDMAT_NC);
		send->elements[2].data.s.len = sizeof(INT_TYPE) * RANDMAT_NC;
		send->elements[2].data.s.ptr = (char*) buffer;

		// perform the actual computation for this row.
		buffer[0] = VECTOR(initVector, row);
		for (int x = 1; x < RANDMAT_NC; ++x) {
			buffer[x] = (aPrime * buffer[x-1] + cPrime) % RAND_M;
		}
	
		// send off the new tuple and purge local memory of the one we got
		put_tuple(send, &ctx);
		destroy_tuple(gotten);
		destroy_tuple(send);
		delete[] buffer;

	}

	// TODO destroy the template tuples; must send tuples for this
//	destroy_tuple(recv);

}

void LTRandmat::produceOutput() {

	// tuple template
	tuple *recv = make_tuple("s??", "randmat done");

	// grab output pointer locally.
	IntMatrix output = (IntMatrix) outputs[0];

	// grab all of the mandelbrot computations from the workers,
	// in an unspecified order.
	int computations = RANDMAT_NR;
	while (computations > 0) {

		// get the tuple and copy it into the matrix.
		tuple* received = get_tuple(recv, &ctx);
		memcpy(
			&MATRIX_RECT_NC(output, received->elements[1].data.i, 0, RANDMAT_NC),
			received->elements[2].data.s.ptr,
			received->elements[2].data.s.len);
		computations--;
		destroy_tuple(received);

	}

	// get rid of randmat state from tuple space
	tuple *tmpInit = make_tuple("s?", "randmat state");
	destroy_tuple(get_tuple(tmpInit, &ctx));

	// destroy the template tuple
	destroy_tuple(recv);

}

