/**
 * \file cowichan_lt/src/half.cpp
 * \brief LinuxTuples half implementation.
 * \see CowichanLinuxTuples::half
 */


#include <iostream>
#include <cstdio>
#include "half.hpp"

const char* LTHalf::REQUEST = "half request";
const char* LTHalf::DONE = "half done";

void CowichanLinuxTuples::half(IntMatrix matrixIn, IntMatrix matrixOut) {
	LTHalf app;
	app.addInput(0, matrixIn);
	app.addOutput(0, matrixOut);
	app.start(SERVER, PORT, NUM_WORKERS);
}

void LTHalf::consumeInput() {

	// send off a request for each grid row.
	for (size_t y = 0; y < HALF_NR; ++y) {
		tuple *send = make_tuple("si", REQUEST, y);
		put_tuple(send, &ctx);
		destroy_tuple(send);
	}
	
}

void LTHalf::work() {

	tuple *recv = make_tuple("s?", REQUEST);
	
	// grab pointers locally.
	IntMatrix input = (IntMatrix) inputs[0];
	
	// satisfy half requests.
	while (1) {

		// block until we receive a tuple.
		tuple* gotten = get_tuple(recv, &ctx);

		// copy over row co-ordinate of the computation; create
		// a buffer for the results of the computation.
		size_t y = gotten->elements[1].data.i;
		tuple *send = make_tuple("sis", DONE, y, "");
		IntVector buffer = NEW_VECTOR_SZ(INT_TYPE, HALF_NC);
		send->elements[2].data.s.len = sizeof(INT_TYPE) * HALF_NC;
		send->elements[2].data.s.ptr = (char*) buffer;

		// perform the actual computation for this row.
		#define FLIP(item) (item % 2 == 0 ? (item + 1) : (item - 1))
		for (size_t x = 0; x < HALF_NC; ++x) {
			buffer[x] = MATRIX_RECT_NC(input, FLIP(y),FLIP(x), HALF_NC);
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

void LTHalf::produceOutput() {

	// tuple template
	tuple *recv = make_tuple("s??", DONE);

	// grab output pointer locally.
	IntMatrix output = (IntMatrix) outputs[0];

	// grab all of the mandelbrot computations from the workers,
	// in an unspecified order.
	int computations = HALF_NR;
	while (computations > 0) {

		// get the tuple and copy it into the matrix.
		tuple* received = get_tuple(recv, &ctx);
		memcpy(
			&MATRIX_RECT_NC(output, received->elements[1].data.i, 0, HALF_NC),
			received->elements[2].data.s.ptr,
			received->elements[2].data.s.len);
		computations--;
		destroy_tuple(received);

	}

	// destroy the template tuple
	destroy_tuple(recv);

}

