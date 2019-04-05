/**
 * \file cowichan_lt/src/thresh.cpp
 * \brief LinuxTuples thresh implementation.
 * \see CowichanLinuxTuples::thresh
 */

#include <iostream>
#include <cstdio>
#include <map>
#include "thresh.hpp"

const char* LTFrequency::SYNCH_LOCK = "thresh synch lock";
const char* LTFrequency::ROWS_DONE = "thresh rows reporting";
const char* LTFrequency::REQUEST = "freq request";
const char* LTFrequency::POINT = "thresh point";

const char* LTThresh::REQUEST = "thresh request";
const char* LTThresh::DONE = "thresh done";
const char* LTThresh::POINT = "thresh point";

void CowichanLinuxTuples::thresh(IntMatrix matrix, BoolMatrix outMatrix) {

	// calculate the frequency breaking-point
	LTFrequency freq;
	freq.addInput(0, matrix);
	freq.start(SERVER, PORT, NUM_WORKERS);

	// calculate
	LTThresh thresh;
	thresh.addInput(0, matrix);
	thresh.addOutput(0, outMatrix);
	thresh.start(SERVER, PORT, NUM_WORKERS);

}

//===========================================================================//

void LTFrequency::consumeInput() {

	// create a tuple synch lock
	tuple *synchLock = make_tuple("s", SYNCH_LOCK);
	put_tuple(synchLock, &ctx);
	destroy_tuple(synchLock);

	// create a "rows reporting" tuple, so that we
	// know when the computation should end.
	tuple *rowsReporting = make_tuple("si", ROWS_DONE, 0);
	put_tuple(rowsReporting, &ctx);
	destroy_tuple(rowsReporting);

	// communicate all of the rows
	for (size_t y = 0; y < THRESH_NR; ++y) {
		tuple *send = make_tuple("si", REQUEST, y);
		put_tuple(send, &ctx);
		destroy_tuple(send);
	}

}

void LTFrequency::work() {

	IntMatrix matrix = (IntMatrix) inputs[0];

	// tuple templates
	tuple *synchLock = make_tuple("s", SYNCH_LOCK);
	tuple *recv = make_tuple("s?", REQUEST);
	tuple *templateRowsReporting = make_tuple("s?", ROWS_DONE);

	while (1) {

		// block until we receieve a tuple.
		tuple* gotten = get_tuple(recv, &ctx);
		size_t y = gotten->elements[1].data.i;

		// perform the actual computation for this row (counting)
		std::map<INT_TYPE, size_t> freq;
		for (index_t x = 0; x < THRESH_NC; ++x) {
			freq[MATRIX_RECT_NC(matrix, y,x, THRESH_NC)] += 1;
		}

		// purge local memory of the tuple we received
		destroy_tuple(gotten);

		// Now, we combine the results from this row with every other row.
		// enter the critical section
		get_tuple(synchLock, &ctx);

			// combine results with master copy
			std::map<INT_TYPE, size_t>::const_iterator it;
			for (it = freq.begin(); it != freq.end(); ++it) {
				tuple *templateHist = make_tuple("si?", "thresh hist", it->first);
				tuple *hist = get_nb_tuple(templateHist, &ctx);
				if (hist == NULL) {
					// use only this row's frequency
					templateHist->elements[2].data.i = it->second;
				} else {
					// combine this row's frequency and existing frequency
					templateHist->elements[2].data.i = it->second + hist->elements[2].data.i;
					destroy_tuple(hist);
				}
				put_tuple(templateHist, &ctx);
				destroy_tuple(templateHist);
			}

			// record the number of rows reporting
			tuple *rowsReporting = get_tuple(templateRowsReporting, &ctx);
			rowsReporting->elements[1].data.i += 1;
			put_tuple(rowsReporting, &ctx);
			destroy_tuple(rowsReporting);

		// leave the critical section
		put_tuple(synchLock, &ctx);

	}

}

void LTFrequency::produceOutput() {

	// wait for all rows to be done.
	tuple *allRowsReporting = make_tuple("si", ROWS_DONE, THRESH_NR);
	destroy_tuple(get_tuple(allRowsReporting, &ctx));
	destroy_tuple(allRowsReporting);

	// figure out when we have to stop.
	index_t retain = (index_t)(THRESH_PERCENT * THRESH_NC * THRESH_NR);

	// go through all tuple values until we have reached the threshold point
	for (size_t n = 0; n < MAXIMUM_INT; ++n) {
		tuple *templateHist = make_tuple("sii", "thresh hist", n);
		tuple *hist = get_nb_tuple(templateHist, &ctx);
		if (hist != NULL) {
			retain -= hist->elements[2].data.i;
		}
		destroy_tuple(hist);
		destroy_tuple(templateHist);
		if (retain <= 0) {
			retain = n;
			break;
		}
	}

	// get rid of all remaining "thresh hist" tuples
	tuple *templateHist = make_tuple("s??", "thresh hist");
	while (true) {
		tuple *hist = get_nb_tuple(templateHist, &ctx);
		if (!hist) break;
		destroy_tuple(hist);

	}
	destroy_tuple(templateHist);

	// retain now holds the threshold point.
	// communicate this into the tuple space
	tuple *threshPoint = make_tuple("si", POINT, retain);
	put_tuple(threshPoint, &ctx);
	destroy_tuple(threshPoint);

	// remove the tuple synch lock from tuple space
	tuple *synchLock = make_tuple("s", SYNCH_LOCK);
	destroy_tuple(get_tuple(synchLock, &ctx));
	destroy_tuple(synchLock);

}

//===========================================================================//

void LTThresh::consumeInput() {

	// send off a request for each grid row.
	for (size_t y = 0; y < THRESH_NR; ++y) {
		tuple *send = make_tuple("si", REQUEST, y);
		put_tuple(send, &ctx);
		destroy_tuple(send);
	}

}

void LTThresh::work() {

	tuple *recv = make_tuple("s?", REQUEST);

	// grab pointers locally.
	IntMatrix input = (IntMatrix) inputs[0];

	// get the threshold point from the frequency calculation
	tuple *templateThreshPoint = make_tuple("s?", POINT);
	tuple *threshPoint = read_tuple(templateThreshPoint, &ctx);
	size_t threshold = threshPoint->elements[1].data.i;

	// satisfy thresh requests.
	while (1) {

		// block until we receive a tuple.
		tuple* gotten = get_tuple(recv, &ctx);

		// copy over row co-ordinate of the computation; create
		// a buffer for the results of the computation.
		index_t y = gotten->elements[1].data.i;
		tuple *send = make_tuple("sis", DONE, y, "");
		BoolVector buffer = (BoolVector) NEW_VECTOR_SZ(bool, THRESH_NC);
		send->elements[2].data.s.len = sizeof(bool) * THRESH_NC;
		send->elements[2].data.s.ptr = (char*) buffer;

		// perform the actual computation for this row.
		for (index_t x = 0; x < THRESH_NC; ++x) {
			buffer[x] = (MATRIX_RECT_NC(input, y,x, THRESH_NC) > threshold);
		}

		// send off the new tuple and purge local memory of the one we got
		put_tuple(send, &ctx);
		destroy_tuple(gotten);
		delete[] buffer;

	}

	// TODO destroy the template tuples; must send tuples for this
//	destroy_tuple(send);
//	destroy_tuple(recv);

}

void LTThresh::produceOutput() {

	// grab output pointer locally.
	BoolMatrix output = (BoolMatrix) outputs[0];

	// grab all of the threshold computations from the workers,
	// in an unspecified order.
	int computations = THRESH_NR;
	while (computations > 0) {

		// get the tuple and copy it into the matrix.
		tuple *recv = make_tuple("s??", DONE);
		tuple* received = get_tuple(recv, &ctx);
		BoolVector bv = (BoolVector) received->elements[2].data.s.ptr;
		for (index_t x = 0; x < THRESH_NC; ++x) {
			MATRIX_RECT_NC(output, received->elements[1].data.i, x, THRESH_NC) = bv[x];
		}
		destroy_tuple(recv);

		// one more computation is complete.
		computations--;

	}

}
