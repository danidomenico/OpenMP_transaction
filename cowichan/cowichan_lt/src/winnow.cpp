/**
 * \file cowichan_lt/src/winnow.cpp
 * \brief LinuxTuples winnow implementation.
 * \see CowichanLinuxTuples::winnow
 */

#include <iostream>
#include <cstdio>
#include <cmath>
#include "winnow.hpp"

const char* LTWinnow::SYNCH_LOCK = "winnow synchronization lock";
const char* LTWinnow::REQUEST = "winnow request";
const char* LTWinnow::WEIGHTED_POINT = "winnow weighted point";
const char* LTWinnow::COUNT = "winnow number of points";
const char* LTWinnow::ROWS_DONE = "winnow rows reporting";

void CowichanLinuxTuples::winnow(IntMatrix matrix, BoolMatrix mask, PointVector points) {

	bool notEnoughPoints = false;

	// calculate the 2D bounds of the point cloud
	LTWinnow program;
	program.addInput(0, matrix);
	program.addInput(1, mask);
	program.addOutput(0, points);
	program.addOutput(1, &notEnoughPoints);
	program.start(SERVER, PORT, NUM_WORKERS);

	// error condition set by LTWinnow::produceOutput
	if (notEnoughPoints) {
		not_enough_points();
	}

}

//===========================================================================//

void LTWinnow::consumeInput() {

	// create a tuple synch lock
	tuple *synchLock = make_tuple("s", SYNCH_LOCK);
	put_tuple(synchLock, &ctx);
	destroy_tuple(synchLock);

	// tuple template.
	tuple *send = make_tuple("si", REQUEST, 0);

	// keep track of the number of rows reporting
	tuple *rowsReporting = make_tuple("si", ROWS_DONE, 0);
	put_tuple(rowsReporting, &ctx);
	destroy_tuple(rowsReporting);

	// allow workers to grab work by-the-row
	for (index_t row = 0; row < WINNOW_NR; ++row) {
		send->elements[1].data.i = row;
		put_tuple(send, &ctx);
	}
	destroy_tuple(send);

}

void LTWinnow::work() {

	// tuple templates
	tuple *synchLock = make_tuple("s", SYNCH_LOCK);
	tuple *recv = make_tuple("s?", REQUEST);
	tuple *send = make_tuple("ssi", WEIGHTED_POINT, "", 0);
	tuple *templateRowsReporting = make_tuple("s?", ROWS_DONE);

	// grab pointers locally.
	IntMatrix matrix = (IntMatrix) inputs[0];
	BoolMatrix mask = (BoolMatrix) inputs[1];

	while (1) {

		// block until we receive a tuple.
		tuple* gotten = get_tuple(recv, &ctx);
		index_t r = gotten->elements[1].data.i;

		// perform the actual computation for these elements;
		// check mask and add points to tuple space/count
		index_t sum = 0;
		for (index_t c = 0; c < WINNOW_NC; ++c) {
			if (MATRIX_RECT_NC(mask, r, c, WINNOW_NC)) {

				++sum;

				// put a point, with weight, into tuple space
				Point pt((real)c, (real)r);
				send->elements[1].data.s.len = sizeof(WeightedPoint);
				send->elements[1].data.s.ptr = (char*) &pt;
				send->elements[2].data.i = MATRIX_RECT_NC(matrix, r, c, WINNOW_NC); // weight
				put_tuple(send, &ctx);

			}
		}

		// purge local memory of the tuple we received/generated
		destroy_tuple(gotten);

		// Now, we combine the results from these points with the "world".
		// We are computing the number of points that are not masked off.
		get_tuple(synchLock, &ctx);

			// combine results with master copy (max element difference)
			tuple *tmpCount = make_tuple("s?", COUNT);
			tuple *tupleCount = get_nb_tuple(tmpCount, &ctx);
			if (tupleCount != NULL) {
				sum += tupleCount->elements[1].data.i;
				destroy_tuple(tupleCount);
			}
			tmpCount->elements[1].data.i = sum;
			tmpCount->elements[1].tag = 'i';
			put_tuple(tmpCount, &ctx);
			destroy_tuple(tmpCount);

			// record the number of rows reporting
			tuple *rowsReporting = get_tuple(templateRowsReporting, &ctx);
			rowsReporting->elements[1].data.i += 1;
			put_tuple(rowsReporting, &ctx);
			destroy_tuple(rowsReporting);

		// leave the critical section
		put_tuple(synchLock, &ctx);

	}

}

Point LTWinnow::nextWeightedPoint(INT_TYPE* order) {

	// tuple templates.
	tuple *recv = make_tuple("s?i", WEIGHTED_POINT);

	// Grab the next weighted point from tuple space.
	tuple* gotten = NULL;
	while (true) {
		recv->elements[2].data.i = *order;
		gotten = get_nb_tuple(recv, &ctx);
		if (gotten) {
			break; // we can exit the function soon.
		} else {
			++(*order); // go onto next potential weight
		}
	}

	// strip data out of the received tuple and destroy it.
	Point pt = *((Point*) gotten->elements[1].data.s.ptr);
	destroy_tuple(gotten);
	destroy_tuple(recv);

	// return the point.
	return pt;

}

void LTWinnow::produceOutput() {

	// wait for all rows to be computed.
	tuple *rowsReporting = make_tuple("si", ROWS_DONE, WINNOW_NR);
	destroy_tuple(get_tuple(rowsReporting, &ctx));
	destroy_tuple(rowsReporting);

	// grab output pointers locally.
	PointVector points = (PointVector) outputs[0];

	// grab the count of weighted points generated.
	tuple *tmpCount = make_tuple("s?", COUNT);
	tuple *tupleCount = get_tuple(tmpCount, &ctx);
	index_t count = tupleCount->elements[1].data.i;
	if (count < WINNOW_N) {

		std::cout << count << std::endl;

		// error condition; flag caller.
		bool* notEnoughPoints = (bool*) outputs[1];
		*notEnoughPoints = true;
		return;

	}
	destroy_tuple(tmpCount);
	destroy_tuple(tupleCount);

	// selection stride.
	index_t stride = count / WINNOW_N;

	// loop over all generated points.
	Point current;
	INT_TYPE order = MINIMUM_INT;
	index_t pos = 0;
	while (pos < count) {

		// skip over as many points as we need to
		for (index_t i = 0; i < stride; ++i) {
			current = nextWeightedPoint(&order);
		}

		// put one in the list
		points[pos++] = current;

	}

	// remove the tuple synch lock from tuple space
	tuple *synchLock = make_tuple("s", SYNCH_LOCK);
	destroy_tuple(get_tuple(synchLock, &ctx));
	destroy_tuple(synchLock);

}
