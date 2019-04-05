/**
 * \file cowichan_lt/src/outer.cpp
 * \brief LinuxTuples outer implementation.
 * \see CowichanLinuxTuples::outer
 */

#include <iostream>
#include <cstdio>
#include <cmath>
#include "outer.hpp"

const char* LTOuter::SYNCH_LOCK = "outer synch lock";
const char* LTOuter::REQUEST = "outer request";

const char* LTOuter::MAX_DISTANCE = "outer max distance";
const char* LTOuter::MATRIX_ENTRY = "outer matrix entry";

void CowichanLinuxTuples::outer(PointVector points, Matrix matrix, Vector vector) {
	std::cout << "outer" << std::endl;
	// calculate the 2D bounds of the point cloud
	LTOuter program;
	program.addInput(0, points);
	program.addOutput(0, matrix);
	program.addOutput(1, vector);
	program.start(SERVER, PORT, NUM_WORKERS);

}

//===========================================================================//

void LTOuter::consumeInput() {

	// create a tuple synch lock
	tuple *synchLock = make_tuple("s", SYNCH_LOCK);
	put_tuple(synchLock, &ctx);

	// tuple template.
	tuple *send = make_tuple("sii", REQUEST);

	// split points, based on a cluster size of the square-root of the
	// number of elements in the given vectors.
	index_t skip = (size_t) sqrt((real) OUTER_N);
	for (index_t pos = 0; pos < OUTER_N; pos += skip) {
		send->elements[1].data.i = pos;
		send->elements[2].data.i = std::min(pos + skip, OUTER_N);
		put_tuple(send, &ctx);
	}

}

void LTOuter::work() {

	// tuple templates
	tuple *synchLock = make_tuple("s", SYNCH_LOCK);
	tuple *recv = make_tuple("s??", REQUEST);
	tuple *send = make_tuple("siid", MATRIX_ENTRY);

	// grab pointers locally.
	PointVector points = (PointVector) inputs[0];

	while (1) {

		// block until we receieve a tuple.
		tuple* gotten = get_tuple(recv, &ctx);
		index_t start = gotten->elements[1].data.i;
		index_t stop = gotten->elements[2].data.i;

		// perform the actual computation for these elements (max distance)
		real dMax = -1.0, d;
		for (index_t r = start; r < stop; ++r) {

			// note that plain point distance is done by the output producer
			// because it's such a simple computation, and the program
			// is simplified significantly. also, it is just as fast as long
			// as the processor doing the input/output has a math co-processor.

			// lower-triangular portion of output matrix.
			for (index_t c = 0; c < r; c++) {

				d = Point::distance(points[r], points[c]);
				if (d > dMax) {
					dMax = d;
				}

				// craft the tuple
				send->elements[1].data.i = r; // row
				send->elements[2].data.i = c; // column
				send->elements[3].data.d = d; // value
				put_tuple(send, &ctx);

			}

		}

		// purge local memory of the tuple we received
		destroy_tuple(gotten);

		// Now, we combine the results from these points with the "world".
		// We are computing the maximum distance for inclusion into the
		// diagonal portion of the output matrix.
		// enter the critical section
		get_tuple(synchLock, &ctx);

			// combine results with master copy (max element difference)
			tuple *tmpMax = make_tuple("s?", MAX_DISTANCE);
			tuple *tupleMax = get_nb_tuple(tmpMax, &ctx);
			if (tupleMax != NULL) {
				dMax = std::max(dMax, (real) tupleMax->elements[1].data.d);
			}
			tmpMax->elements[1].data.d = dMax;
			put_tuple(tmpMax, &ctx);
			destroy_tuple(tmpMax);

		// leave the critical section
		put_tuple(synchLock, &ctx);

	}

}

void LTOuter::produceOutput() {

	// tuple templates.
	tuple *recv = make_tuple("s???", MATRIX_ENTRY);

	// grab input pointers locally.
	PointVector points = (PointVector) inputs[0];
	Point zeroPoint(0.0, 0.0);

	// grab output pointers locally.
	Matrix matrix = (Matrix) outputs[0];
	Vector vector = (Vector) outputs[1];

	// grab all of the outer computations from the workers,
	// in an unspecified order.
	int computations = (OUTER_N * (OUTER_N - 1)) / 2; // triangular portion
	while (computations > 0) {

		// grab a tuple.
		tuple *received = get_tuple(recv, &ctx);
		index_t row = received->elements[1].data.i;
		index_t col = received->elements[2].data.i;
		real d = received->elements[3].data.d;

		// put it in the upper- and lower-triangular portions of the output matrix.
		MATRIX_SQUARE_N(matrix, row, col, OUTER_N) = MATRIX_SQUARE_N(matrix, col, row, OUTER_N) = d;

		// we just received and handled one computation.
		destroy_tuple(received);
		computations--;

	}

	// Once all the computations are received, we can use the
	// maximum value computed by the works to fill in the diagonal.
	tuple *tmpMax = make_tuple("s?", MAX_DISTANCE);
	tuple *tupleMax = get_nb_tuple(tmpMax, &ctx);
	real dMax = tupleMax->elements[1].data.d;
	dMax *= OUTER_N;
	for (index_t r = 0; r < OUTER_N; r++) {
		MATRIX_SQUARE_N(matrix, r, r, OUTER_N) = dMax;
	}

	// Finally, we can fill in the output vector.
	// Rationale for doing this here in ::work()
	for (index_t r = 0; r < OUTER_N; r++) {
		vector[r] = Point::distance(points[r], zeroPoint);
	}

	// remove the tuple synch lock from tuple space
	tuple *synchLock = make_tuple("s", SYNCH_LOCK);
	get_tuple(synchLock, &ctx);

	// clean-up local tuple memory.
	destroy_tuple(synchLock);
	destroy_tuple(recv);
	destroy_tuple(tmpMax);
	destroy_tuple(tupleMax);

}
