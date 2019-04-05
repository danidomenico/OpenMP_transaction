/**
 * \file cowichan_lt/src/vecdiff.cpp
 * \brief LinuxTuples vecdiff implementation.
 * \see CowichanLinuxTuples::vecdiff
 */

#include <iostream>
#include <cstdio>
#include <cmath>
#include "vecdiff.hpp"

const char* LTVecdiff::SYNCH_LOCK = "vecdiff synch lock";
const char* LTVecdiff::ELEMENTS_DONE = "vecdiff elements reporting";
const char* LTVecdiff::MAX_DIFF = "vecdiff max difference";

real CowichanLinuxTuples::vecdiff(Vector actual, Vector computed) {
	std::cout << "vecdiff" << std::endl;
	real answer;

	// calculate the 2D bounds of the point cloud
	LTVecdiff program;
	program.addInput(0, actual);
	program.addInput(1, computed);
	program.addOutput(0, &answer);
	program.start(SERVER, PORT, NUM_WORKERS);

	// pass back the answer
	return answer;

}

//===========================================================================//

void LTVecdiff::consumeInput() {

	// create a tuple synch lock
	tuple *synchLock = make_tuple("s", SYNCH_LOCK);
	put_tuple(synchLock, &ctx);

	// create an "elements reporting" tuple, so that we
	// know when the computation should end.
	tuple *elsReporting = make_tuple("si", ELEMENTS_DONE, 0);
	put_tuple(elsReporting, &ctx);

	// tuple template.
	tuple *send = make_tuple("sii", "vecdiff request");

	// split points, based on a cluster size of the square-root of the
	// number of elements in the given vectors.
	index_t skip = (size_t) sqrt((real) VECDIFF_N);
	for (index_t pos = 0; pos < VECDIFF_N; pos += skip) {
		send->elements[1].data.i = pos;
		send->elements[2].data.i = std::min(pos + skip, VECDIFF_N);
		put_tuple(send, &ctx);
	}

}

void LTVecdiff::work() {

	// tuple templates
	tuple *synchLock = make_tuple("s", SYNCH_LOCK);
	tuple *recv = make_tuple("s??", "vecdiff request");

	// grab pointers locally.
	Vector actual = (Vector) inputs[0];
	Vector computed = (Vector) inputs[1];

	while (1) {

		// block until we receieve a tuple.
		tuple* gotten = get_tuple(recv, &ctx);
		size_t start = gotten->elements[1].data.i;
		size_t stop = gotten->elements[2].data.i;

		// perform the actual computation for these elements (max)
		real maximum = 0.0;
		for (size_t pos = start; pos < stop; ++pos) {
			real thisMaximum = (real) fabs(computed[pos] - actual[pos]);
			maximum = std::max(thisMaximum, maximum);
		}

		// purge local memory of the tuple we received
		destroy_tuple(gotten);

		// Now, we combine the results from these points with the "world".
		// enter the critical section
		get_tuple(synchLock, &ctx);

			// combine results with master copy (max element difference)
			tuple *tmpMax = make_tuple("s?", MAX_DIFF);
			tuple *tupleMax = get_nb_tuple(tmpMax, &ctx);
			if (tupleMax != NULL) {
				maximum = std::max(maximum, (real) tupleMax->elements[1].data.d);
			}
			tmpMax->elements[1].data.d = maximum;
			put_tuple(tmpMax, &ctx);
			destroy_tuple(tmpMax);

			// record the number of elements reporting
			tuple *templateElsReporting = make_tuple("s?", ELEMENTS_DONE);
			tuple *elsReporting = get_tuple(templateElsReporting, &ctx);
			elsReporting->elements[1].data.i += (stop - start);
			put_tuple(elsReporting, &ctx);

		// leave the critical section
		put_tuple(synchLock, &ctx);

	}

}

void LTVecdiff::produceOutput() {

	// wait for all rows to be done.
	tuple *allElsReporting = make_tuple("si", ELEMENTS_DONE, VECDIFF_N);
	get_tuple(allElsReporting, &ctx);

	// we can now record the final maximum norm
	tuple *tmpMax = make_tuple("s?", MAX_DIFF);
	tuple *tupleMax = get_tuple(tmpMax, &ctx);
	real* answer = (real*) outputs[0];
	*answer = tupleMax->elements[1].data.d;

	// remove the tuple synch lock from tuple space
	tuple *synchLock = make_tuple("s", SYNCH_LOCK);
	get_tuple(synchLock, &ctx);

}
