/**
 * \file cowichan_lt/src/norm.cpp
 * \brief LinuxTuples norm implementation.
 * \see CowichanLinuxTuples::norm
 */

#include <iostream>
#include <cstdio>
#include <cmath>
#include "norm.hpp"

const char* LTBounds::SYNCH_LOCK = "norm synch lock";
const char* LTBounds::POINTS_DONE = "norm points reporting";
const char* LTBounds::MIN_POINT = "norm minPoint";
const char* LTBounds::MAX_POINT = "norm maxPoint";

const char* LTNorm::MIN_POINT = "norm minPoint";
const char* LTNorm::MAX_POINT = "norm maxPoint";


void CowichanLinuxTuples::norm(PointVector pointsIn, PointVector pointsOut) {

	// calculate the 2D bounds of the point cloud
	LTBounds bounds;
	bounds.addInput(0, pointsIn);
	bounds.start(SERVER, PORT, NUM_WORKERS);

	// transform all the points to lie in the unit square
	LTNorm norm;
	norm.addInput(0, pointsIn);
	norm.addOutput(0, pointsOut);
	norm.start(SERVER, PORT, NUM_WORKERS);

}

//===========================================================================//

void LTBounds::consumeInput() {

	// create a tuple synch lock
	tuple *synchLock = make_tuple("s", SYNCH_LOCK);
	put_tuple(synchLock, &ctx);

	// create a "points reporting" tuple, so that we
	// know when the computation should end.
	tuple *pointsReporting = make_tuple("si", POINTS_DONE, 0);
	put_tuple(pointsReporting, &ctx);

	// tuple template.
	tuple *send = make_tuple("sii", "bounds request");

	// split points, based on a cluster size of the square-root of the
	// number of points given.
	index_t skip = (size_t) sqrt((real) NORM_N);
	for (index_t pos = 0; pos < NORM_N; pos += skip) {
		send->elements[1].data.i = pos;
		send->elements[2].data.i = std::min(pos + skip, NORM_N);
		put_tuple(send, &ctx);
	}

}

void LTBounds::work() {

	// tuple templates
	tuple *synchLock = make_tuple("s", SYNCH_LOCK);
	tuple *recv = make_tuple("s??", "bounds request");

	// grab pointers locally.
	PointVector input = (PointVector) inputs[0];

	while (1) {

		// block until we receieve a tuple.
		tuple* gotten = get_tuple(recv, &ctx);
		size_t start = gotten->elements[1].data.i;
		size_t stop = gotten->elements[2].data.i;

		// perform the actual computation for this row (min/max)
		Point minPoint = input[start];
		Point maxPoint = input[start];
		for (size_t pos = start + 1; pos < stop; ++pos) {
			Point &cur = input[pos];
			minPoint.x = std::min(minPoint.x, cur.x);
			minPoint.y = std::min(minPoint.y, cur.y);
			maxPoint.x = std::max(maxPoint.x, cur.x);
			maxPoint.y = std::max(maxPoint.y, cur.y);
		}

		// purge local memory of the tuple we received
		destroy_tuple(gotten);

		// Now, we combine the results from these points with the "world".
		// enter the critical section
		get_tuple(synchLock, &ctx);

			// combine results with master copy (minPoint)
			tuple *tmpMin = make_tuple("s?", MIN_POINT);
			tuple *tupleMin = get_nb_tuple(tmpMin, &ctx);
			if (tupleMin != NULL) {
				Point* worldMin = (Point*) tupleMin->elements[1].data.s.ptr;
				minPoint.x = std::min(minPoint.x, worldMin->x);
				minPoint.y = std::min(minPoint.y, worldMin->y);
				destroy_tuple(tupleMin);
			}
			tmpMin->elements[1].data.s.len = sizeof(Point);
			tmpMin->elements[1].data.s.ptr = (char*) &minPoint;
			put_tuple(tmpMin, &ctx);
			destroy_tuple(tmpMin);

			// combine results with master copy (maxPoint)
			tuple *tmpMax = make_tuple("s?", MAX_POINT);
			tuple *tupleMax = get_nb_tuple(tmpMax, &ctx);
			if (tupleMax != NULL) {
				Point* worldMax = (Point*) tupleMax->elements[1].data.s.ptr;
				maxPoint.x = std::max(maxPoint.x, worldMax->x);
				maxPoint.y = std::max(maxPoint.y, worldMax->y);
				destroy_tuple(tupleMax);
			}
			tmpMax->elements[1].data.s.len = sizeof(Point);
			tmpMax->elements[1].data.s.ptr = (char*) &maxPoint;
			put_tuple(tmpMax, &ctx);
			destroy_tuple(tmpMax);

			// record the number of points reporting
			tuple *templatePointsReporting = make_tuple("s?", POINTS_DONE);
			tuple *pointsReporting = get_tuple(templatePointsReporting, &ctx);
			pointsReporting->elements[1].data.i += (stop - start);
			put_tuple(pointsReporting, &ctx);
			destroy_tuple(templatePointsReporting);

		// leave the critical section
		put_tuple(synchLock, &ctx);

	}

}

void LTBounds::produceOutput() {

	// wait for all rows to be done.
	tuple *allPointsReporting = make_tuple("si", POINTS_DONE, NORM_N);
	get_tuple(allPointsReporting, &ctx);

	// at this point minPoint and maxPoint tuples exist in the space

	// remove the tuple synch lock from tuple space
	tuple *synchLock = make_tuple("s", SYNCH_LOCK);
	get_tuple(synchLock, &ctx);

}

//===========================================================================//

void LTNorm::consumeInput() {

	// tuple template
	tuple *send = make_tuple("sii", "norm request");

	// split points, based on a cluster size of the square-root of the
	// number of points given.
	index_t skip = (size_t) sqrt((real) NORM_N);
	for (index_t pos = 0; pos < NORM_N; pos += skip) {
		send->elements[1].data.i = pos;
		send->elements[2].data.i = std::min(pos + skip, NORM_N);
		put_tuple(send, &ctx);
	}

	// destroy the template tuple
	destroy_tuple(send);

}

void LTNorm::work() {

	tuple *recv = make_tuple("s??", "norm request");
	tuple *send = make_tuple("siis", "norm done");

	// grab pointers locally.
	PointVector input = (PointVector) inputs[0];

	// get the min/max points from the bounds computation
	tuple *tmpMin = make_tuple("s?", MIN_POINT);
	tuple *tmpMax = make_tuple("s?", MAX_POINT);
	tuple *tupleMin = read_tuple(tmpMin, &ctx);
	tuple *tupleMax = read_tuple(tmpMax, &ctx);
	Point minPoint = *((Point*) tupleMin->elements[1].data.s.ptr);
	Point maxPoint = *((Point*) tupleMax->elements[1].data.s.ptr);

	// satisfy norm requests.
	while (1) {

		// block until we receive a tuple.
		tuple* gotten = get_tuple(recv, &ctx);
		size_t start = gotten->elements[1].data.i;
		size_t stop = gotten->elements[2].data.i;

		// copy over row co-ordinate of the computation; create
		// a buffer for the results of the computation.
		send->elements[1].data.i = start;
		send->elements[2].data.i = stop;
		Point* buffer = (Point*) malloc(sizeof(Point) * (stop - start));
		send->elements[3].data.s.len = sizeof(Point) * (stop - start);
		send->elements[3].data.s.ptr = (char*) buffer;

		// compute scaling factors
		real sclX = (real)((maxPoint.x == minPoint.x) ?
			0.0 : 1 / (maxPoint.x - minPoint.x));
		real sclY = (real)((maxPoint.y == minPoint.y) ?
			0.0 : 1 / (maxPoint.y - minPoint.y));

		// scale (perform the actual computation for this row)
		size_t outPos = 0;
		for (size_t inPos = start; inPos < stop; ++inPos) {
			buffer[outPos].x = sclX * (input[inPos].x - minPoint.x);
			buffer[outPos].y = sclY * (input[inPos].y - minPoint.y);
			outPos++;
		}

		// send off the new tuple and purge local memory of the one we got
		put_tuple(send, &ctx);
		destroy_tuple(gotten);

	}

	// TODO destroy the template tuples; must send tuples for this
//	destroy_tuple(send);
//	destroy_tuple(recv);

}

void LTNorm::produceOutput() {

	// tuple template
	tuple *recv = make_tuple("s???", "norm done");

	// grab output pointer locally.
	PointVector output = (PointVector) outputs[0];

	// grab all of the norm computations from the workers,
	// in an unspecified order.
	int computations = NORM_N;
	while (computations > 0) {

		// get the tuple and copy it into the matrix.
		tuple* received = get_tuple(recv, &ctx);
		memcpy(output + received->elements[1].data.i,
			received->elements[3].data.s.ptr,
			received->elements[3].data.s.len);

		// figure out how many computations that was.
		size_t start = received->elements[1].data.i;
		size_t stop = received->elements[2].data.i;
		computations -= (stop - start);
		destroy_tuple(received);

	}

	// destroy the template tuple
	destroy_tuple(recv);

}
