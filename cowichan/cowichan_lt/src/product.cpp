/**
 * \file cowichan_lt/src/product.cpp
 * \brief LinuxTuples product implementation.
 * \see CowichanLinuxTuples::product
 */

#include <iostream>
#include <cstdio>
#include "product.hpp"

void CowichanLinuxTuples::product(Matrix matrix, Vector candidate, Vector solution) {
	std::cout << "product" << std::endl;
	LTProduct app;
	app.addInput(0, matrix);
	app.addInput(1, candidate);
	app.addOutput(0, solution);
	app.start(SERVER, PORT, NUM_WORKERS);
}

void LTProduct::consumeInput() {

	// tuple template
	tuple *send = make_tuple("si", "product request");

	// send off a request for each grid row.
	for (size_t y = 0; y < PRODUCT_N; ++y) {
		send->elements[1].data.i = y;
		put_tuple(send, &ctx);
	}
	
	// destroy the template tuple
	destroy_tuple(send);

}

void LTProduct::work() {

	tuple *recv = make_tuple("s?", "product request");
	tuple *send = make_tuple("sid", "product done");
	
	// grab pointers locally.
	Matrix matrix = (Matrix) inputs[0];
	Vector candidate = (Vector) inputs[1];
	
	// satisfy half requests.
	while (1) {

		// block until we receive a tuple.
		tuple* gotten = get_tuple(recv, &ctx);

		// copy over row co-ordinate of the computation; create
		// a buffer for the results of the computation.
		size_t r = gotten->elements[1].data.i;
		send->elements[1].data.i = r;

		// perform and store the actual computation for this row.
		real result = 0.0;
		for (int c = 0; c < PRODUCT_N; ++c) {
			result += MATRIX_SQUARE_N(matrix, r, c, PRODUCT_N) * VECTOR(candidate, c);
		}
		send->elements[2].data.d = result;
	
		// send off the new tuple and purge local memory of the one we got
		put_tuple(send, &ctx);
		destroy_tuple(gotten);

	}

	// TODO destroy the template tuples; must send tuples for this
//	destroy_tuple(send);
//	destroy_tuple(recv);

}

void LTProduct::produceOutput() {

	// tuple template
	tuple *recv = make_tuple("s??", "product done");

	// grab output pointer locally.
	Vector solution = (Vector) outputs[0];

	// grab all of the computations from the workers,
	// in an unspecified order.
	int computations = PRODUCT_N;
	while (computations > 0) {

		// get the tuple and copy it into the matrix.
		tuple* received = get_tuple(recv, &ctx);
		VECTOR(solution, received->elements[1].data.i) = received->elements[2].data.d;
		computations--;
		destroy_tuple(received);

	}

	// destroy the template tuple
	destroy_tuple(recv);

}
