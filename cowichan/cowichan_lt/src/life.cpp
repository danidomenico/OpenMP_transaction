/**
 * \file cowichan_lt/src/life.cpp
 * \brief LinuxTuples life implementation.
 * \see CowichanLinuxTuples::life
 */

#include <iostream>
#include <cstdio>
#include "life.hpp"

void CowichanLinuxTuples::life(BoolMatrix matrixIn, BoolMatrix matrixOut) {

	// overwrite the output matrix
	memcpy(matrixOut, matrixIn,
		sizeof(bool) * LIFE_NR * LIFE_NC);

	// operate in-place on matrixOut the number of iterations we should.
	for (size_t i = 0; i < LIFE_ITERATIONS; ++i) {
		LTLife app;
		app.addInput(0, matrixOut);
		app.addOutput(0, matrixOut);
		app.start(SERVER, PORT, NUM_WORKERS);
	}

}

/**
 * Calculate number of peers.
 */
index_t LTLife::sumNeighbours(index_t y, index_t x) {

	index_t peers = 0;

	// grab pointers locally.
	BoolMatrix first = (BoolMatrix) inputs[0];

	// calculate possible neighbour positions
	bool l = (x > 0);
	bool r = (x < (LIFE_NC - 1));
	bool u = (y > 0);
	bool d = (y < (LIFE_NR - 1));

	// calculate no. of neighbours
	if (l &&       MATRIX_RECT_NC(first, y    , x - 1, LIFE_NC)) ++peers;
	if (l && u &&  MATRIX_RECT_NC(first, y - 1, x - 1, LIFE_NC)) ++peers;
	if (u &&       MATRIX_RECT_NC(first, y - 1, x    , LIFE_NC)) ++peers;
	if (r && u &&  MATRIX_RECT_NC(first, y - 1, x + 1, LIFE_NC)) ++peers;
	if (r &&       MATRIX_RECT_NC(first, y    , x + 1, LIFE_NC)) ++peers;
	if (r && d &&  MATRIX_RECT_NC(first, y + 1, x + 1, LIFE_NC)) ++peers;
	if (d &&       MATRIX_RECT_NC(first, y + 1, x    , LIFE_NC)) ++peers;
	if (l && d &&  MATRIX_RECT_NC(first, y + 1, x - 1, LIFE_NC)) ++peers;

	return peers;

}

void LTLife::consumeInput() {

	// tuple template
	tuple *send = make_tuple("si", "life request");

	// send off a request for each grid row.
	for (size_t y = 0; y < LIFE_NR; ++y) {
		send->elements[1].data.i = y;
		put_tuple(send, &ctx);
	}
	
	// destroy the template tuple
	destroy_tuple(send);

}

void LTLife::work() {

	tuple *recv = make_tuple("s?", "life request");
	
	// grab pointers locally.
	BoolMatrix input = (BoolMatrix) inputs[0];
	
	// satisfy half requests.
	while (1) {

		// block until we receive a tuple.
		tuple* gotten = get_tuple(recv, &ctx);

		// copy over row co-ordinate of the computation; create
		// a buffer for the results of the computation.
		size_t y = gotten->elements[1].data.i;
		tuple *send = make_tuple("sis", "life done", y, "");
		BoolVector buffer = (BoolVector) NEW_VECTOR_SZ(bool, LIFE_NC);
		send->elements[2].data.s.len = sizeof(bool) * LIFE_NC;
		send->elements[2].data.s.ptr = (char*) buffer;

		// perform the actual computation for this row.
		for (int x = 0; x < LIFE_NC; ++x) {

	        index_t peers = sumNeighbours(y, x);
	        if (peers < 2 || peers > 3) {
	          buffer[x] = false; // hunger/overcrowding
	        } else if (peers == 3) {
	          buffer[x] = true; // breeding
	        } else {
	          buffer[x] = MATRIX_RECT_NC(input, y, x, LIFE_NC); // nothing
	        }

		}
	
		// send off the new tuple and purge local memory of the one we got
		put_tuple(send, &ctx);
		destroy_tuple(gotten);
		destroy_tuple(send);

	}

	// TODO destroy the template tuples; must send tuples for this
//	destroy_tuple(recv);

}

void LTLife::produceOutput() {

	// grab output pointer locally.
	BoolMatrix output = (BoolMatrix) outputs[0];

	// grab all of the life computations from the workers,
	// in an unspecified order.
	int computations = LIFE_NR;
	while (computations > 0) {

		// get the tuple and copy it into the matrix.
		tuple *recv = make_tuple("s??", "life done");
		tuple* received = get_tuple(recv, &ctx);
		memcpy(
			&MATRIX_RECT_NC(output, received->elements[1].data.i, 0, LIFE_NC),
			received->elements[2].data.s.ptr,
			received->elements[2].data.s.len);
		computations--;
		destroy_tuple(received);
		destroy_tuple(recv);

	}

}

