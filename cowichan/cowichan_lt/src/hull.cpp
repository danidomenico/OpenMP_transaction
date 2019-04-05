/**
 * \file cowichan_lt/src/hull.cpp
 * \brief LinuxTuples hull implementation.
 * \see CowichanLinuxTuples::hull
 */

#include <iostream>
#include <cstdio>
#include <cmath>
#include "hull.hpp"

const char* LTHull::REQUEST = "hull request";
const char* LTHull::REQUEST_MINMAX = "hull request min/max";
const char* LTHull::REQUEST_CROSS = "hull request cross-product";

const char* LTHull::SYNCH_LOCK = "hull synch lock";
const char* LTHull::POINTS_DONE = "hull points reporting";

const char* LTHull::MIN_X_POINT = "hull minPoint";
const char* LTHull::MAX_X_POINT = "hull maxPoint";
const char* LTHull::MAX_CROSS = "hull max cross-product";
const char* LTHull::MAX_POINT = "hull furthest point";

const char* LTHull::HULL_POINT = "hull point";
const char* LTHull::MASKED_POINT = "hull masked point";
const char* LTHull::NUM_POINTS = "hull # of points in convex hull";

const char* LTHull::FLAG_OUTPUT = "hull flag output";
const char* LTHull::FINISHED_MINMAX = "hull finshed min/max";


void CowichanLinuxTuples::hull(PointVector pointsIn, PointVector pointsOut) {
	int order = 0;

	// while not all points are used up then run quickhull on the rest of points
	while (order < HULL_N) {

		// Run quickhull on the decided points as a tuple-space problem.
		LTHull app(HULL_N - order);
		app.addInput(0, pointsIn);
		app.addOutput(0, pointsOut + order); // offset into the array
		app.start(SERVER, PORT, NUM_WORKERS);

		// get and remove the num-points "order" tuple from tuple space
		// add it to the order so we can keep track of the number of
		// points that we have left to process.
		order += app.getNumPoints();

	}

	// connect to the tuple server again, for clean-up.
	struct context ctx;
	if (get_server_portnumber(&ctx)) {
		strcpy(ctx.peername, SERVER);
		ctx.portnumber = PORT;
	}

	// cleanup: delete all mask tuples
	tuple* maskTemplate = make_tuple("s?", LTHull::MASKED_POINT);
	while (true) {
		tuple* masked = get_nb_tuple(maskTemplate, &ctx);
		if (masked == NULL) break;
		destroy_tuple(masked);
	}
	destroy_tuple(maskTemplate);

}

//===========================================================================//

LTHull::LTHull(size_t left): numLeft(left) { }

void LTHull::consumeInput() {

	tuple* flagOutput = make_tuple("s", FLAG_OUTPUT);

	// create a tuple synch lock
	tuple *synchLock = make_tuple("s", SYNCH_LOCK);
	put_tuple(synchLock, &ctx);

	// base case
	if (numLeft == 1) {

		// search for the only point
		index_t pos;
		for (pos = 0; pos < HULL_N; ++pos) {
			if (!isMasked(pos)) break;
		}

		// emit the only point
		tuple* hullPoint = make_tuple("sii", HULL_POINT, 0);
		hullPoint->elements[2].data.i = pos;
		put_tuple(hullPoint, &ctx);

		// put the number of points (1) in tuple space
		tuple *tOrder = make_tuple("si", NUM_POINTS, 1);
		put_tuple(tOrder, &ctx);

	} else {

		// figure out the points with minimum and maximum x values
		index_t minPoint, maxPoint;
		computeMinMax(&minPoint, &maxPoint);

		// use these as initial pivots
		index_t order = 0;
		split(minPoint, maxPoint, &order);
		split(maxPoint, minPoint, &order);

		// put the number of points in tuple space
		tuple *tOrder = make_tuple("si", NUM_POINTS, order);
		put_tuple(tOrder, &ctx);

	}

	// flag the output producer
	put_tuple(flagOutput, &ctx);

}

/**
 * Compute hull on one side of the splitting line.
 * \param p1 boundary point #1.
 * \param p2 boundary point #2.
 * \param order the ordering number of the next hull point to emit.
 */
void LTHull::split(const index_t p1, const index_t p2, index_t *order) {

	std::cout << "split " << p1 << "," << p2 << std::endl;

	// compute the signed distances from the line for each point
	index_t maxPoint;
	real maxCross;
	computeCross(p1, p2, &maxPoint, &maxCross);

	// is there a point in the positive half-space?
	// if so, it has maximal distance, and we must recurse based on that point.
	if (maxPoint != NO_POINT && maxCross > 0.0) {
		// recurse on the new set with the given far point
		split(p1, maxPoint, order);
		split(maxPoint, p2, order);
		return;
	}

	// otherwise, it's not on the right side; we don't need to split anymore.
	// this is because all points are inside the hull when we use this
	// half-space. add the first point and return.

 	// emit p1 to tuple space using order index
	tuple* hullPoint = make_tuple("sii", HULL_POINT, (*order)++);
	hullPoint->elements[2].data.i = p1;
	put_tuple(hullPoint, &ctx);

}

void LTHull::computeCross(index_t p1, index_t p2, index_t *maxPoint, real* maxCross) {

	// create a "points reporting" tuple, so that we
	// know when the computation should end.
	tuple *pointsReporting = make_tuple("si", POINTS_DONE, 0);
	put_tuple(pointsReporting, &ctx);

	// tuple template.
	tuple *send = make_tuple("ssiiii", REQUEST, REQUEST_CROSS);
	send->elements[4].data.i = p1;
	send->elements[5].data.i = p2;

	// split points, based on a cluster size of the square-root of the
	// number of points given.
	index_t skip = (size_t) sqrt((real) HULL_N);
	index_t numToReport = 0;
	for (index_t pos = 0; pos < HULL_N; pos += skip) {
		++numToReport;
		send->elements[2].data.i = pos;
		send->elements[3].data.i = std::min(pos + skip, HULL_N);
		put_tuple(send, &ctx);
	}

	// wait for all point clusters to report
	pointsReporting->elements[1].data.i = numToReport;
	destroy_tuple(get_tuple(pointsReporting, &ctx));

	// grab the max-cross/max-point elements from tuple space
	tuple *tmpCross = make_tuple("s?", MAX_CROSS);
	tuple *tmpPoint = make_tuple("s?", MAX_POINT);
	tuple *tupleCross = get_tuple(tmpCross, &ctx);
	tuple *tuplePoint = get_tuple(tmpPoint, &ctx);
	*maxCross = tupleCross->elements[1].data.d;
	*maxPoint = tuplePoint->elements[1].data.i;
	destroy_tuple(tmpCross);
	destroy_tuple(tmpPoint);
	destroy_tuple(tupleCross);
	destroy_tuple(tuplePoint);
}

void LTHull::computeMinMax(index_t *minPoint, index_t *maxPoint) {

	// create a "points reporting" tuple, so that we
	// know when the computation should end.
	tuple *pointsReporting = make_tuple("si", POINTS_DONE, 0);
	put_tuple(pointsReporting, &ctx);

	// tuple template.
	tuple *send = make_tuple("ssiiii", REQUEST, REQUEST_MINMAX, 0, 0, 0, 0);

	// split points, based on a cluster size of the square-root of the
	// number of points given.
	index_t skip = (size_t) sqrt((real) HULL_N);
	index_t numToReport = 0;
	for (index_t pos = 0; pos < HULL_N; pos += skip) {
		++numToReport;
		send->elements[2].data.i = pos;
		send->elements[3].data.i = std::min(pos + skip, HULL_N);
		put_tuple(send, &ctx);
	}

	// wait for all point clusters to report
	pointsReporting->elements[1].data.i = numToReport;
	destroy_tuple(get_tuple(pointsReporting, &ctx));

	// grab the min-x/max-x points from tuple-space
	tuple *templateMax = make_tuple("s?", MAX_X_POINT);
	tuple *templateMin = make_tuple("s?", MIN_X_POINT);
	tuple *tupleMax = get_tuple(templateMax, &ctx);
	tuple *tupleMin = get_tuple(templateMin, &ctx);
	*minPoint = tupleMin->elements[1].data.i;
	*maxPoint = tupleMax->elements[1].data.i;
	destroy_tuple(templateMin);
	destroy_tuple(templateMax);
	destroy_tuple(tupleMin);
	destroy_tuple(tupleMax);

}

void LTHull::work() {

	// tuple templates
	tuple *recv = make_tuple("s?????", REQUEST);

	// TODO later we can make it so we don't have to kill workers
	while (true) {

		std::cout << "here" << std::endl;

		// block until we receive a tuple.
		tuple* gotten = get_tuple(recv, &ctx);

		// Dispatch based on type of request
		if (!strcmp(gotten->elements[1].data.s.ptr, REQUEST_MINMAX)) {
			serviceMinMaxRequest(gotten);
		} else {
			serviceCrossRequest(gotten);
		}

		// purge local memory of the tuple we received
		destroy_tuple(gotten);

	}

}

void LTHull::serviceMinMaxRequest(tuple* gotten) {

	tuple *synchLock = make_tuple("s", SYNCH_LOCK);

	// grab pointers locally.
	PointVector pointsIn = (PointVector) inputs[0];

	// extract data from tuple
	size_t start = gotten->elements[2].data.i;
	size_t stop = gotten->elements[3].data.i;

	// perform the actual computation for this row (min/max)
	bool first = true;
	index_t minPoint = -1, maxPoint = -1;
	for (size_t pos = start; pos < stop; ++pos) {

		// make sure we only look at non-masked points
		if (isMasked(pos)) continue;

		// figure out the points with minimum and maximum x values
		if (pointsIn[minPoint].x > pointsIn[pos].x || first) {
			minPoint = pos;
		}
		if (pointsIn[maxPoint].x < pointsIn[pos].x || first) {
			maxPoint = pos;
		}

		// do not trigger the "first" rules again.
		if (first) first = false;

	}

	// If first is still true, that means we did not look at any points.
	// Don't go through the rest of the function.
	if (first) return;

	// ... which means at this point, minPoint and maxPoint are guaranteed
	// to be valid unmasked indices into the input Point vector.

	// Now, we combine the results from these points with the "world".
	// enter the critical section
	get_tuple(synchLock, &ctx);

		// combine results with master copy (minPoint)
		tuple *tmpMin = make_tuple("s?", MIN_X_POINT);
		tuple *tupleMin = get_nb_tuple(tmpMin, &ctx);
		if (tupleMin != NULL) {
			index_t worldMin = tupleMin->elements[1].data.i;
			if (pointsIn[minPoint].x > pointsIn[worldMin].x) {
				minPoint = worldMin;
			}
			destroy_tuple(tupleMin);
		}
		tmpMin->elements[1].data.i = minPoint;
		tmpMin->elements[1].tag = 'i';
		put_tuple(tmpMin, &ctx);
		destroy_tuple(tmpMin);

		// combine results with master copy (maxPoint)
		tuple *tmpMax = make_tuple("s?", MAX_X_POINT);
		tuple *tupleMax = get_nb_tuple(tmpMax, &ctx);
		if (tupleMax != NULL) {
			index_t worldMax = tupleMax->elements[1].data.i;
			if (pointsIn[maxPoint].x < pointsIn[worldMax].x) {
				maxPoint = worldMax;
			}
			destroy_tuple(tupleMax);
		}
		tmpMax->elements[1].data.i = maxPoint;
		tmpMax->elements[1].tag = 'i';
		put_tuple(tmpMax, &ctx);
		destroy_tuple(tmpMax);

		// record the number of point clusters reporting
		tuple *templatePointsReporting = make_tuple("s?", POINTS_DONE);
		tuple *pointsReporting = get_tuple(templatePointsReporting, &ctx);
		pointsReporting->elements[1].data.i += 1;
		pointsReporting->elements[1].tag = 'i';
		put_tuple(pointsReporting, &ctx);

	// leave the critical section
	put_tuple(synchLock, &ctx);

}

void LTHull::serviceCrossRequest(tuple* gotten) {

	tuple *synchLock = make_tuple("s", SYNCH_LOCK);

	// grab pointers locally.
	PointVector pointsIn = (PointVector) inputs[0];

	// extract data from tuple
	size_t start = gotten->elements[2].data.i;
	size_t stop = gotten->elements[3].data.i;
	index_t p1 = gotten->elements[4].data.i;
	index_t p2 = gotten->elements[5].data.i;

	// perform the actual computation for these elements
	index_t maxPoint = NO_POINT;
	real maxCross = MINIMUM_REAL;
	for (size_t pos = start; pos < stop; ++pos) {

		// make sure we only look at non-masked points
		if (isMasked(pos)) continue;

		// compute the signed distances from the line for each point
		real currentCross = Point::cross(pointsIn[p1], pointsIn[p2], pointsIn[pos]);
		if (currentCross > maxCross || maxPoint == NO_POINT) {
			maxPoint = pos;
			maxCross = currentCross;
		}

	}

	// Now, we combine the results from these points with the "world".
	// enter the critical section
	get_tuple(synchLock, &ctx);

		bool updateWorldPoint = true;

		// don't update the world if we have nothing to update with!
		if (maxPoint == NO_POINT) {
			updateWorldPoint = false;
		}

		// figure out if we should combine our results with the world
		tuple *tmpCross = make_tuple("s?", MAX_CROSS);
		tuple *tupleCross = read_nb_tuple(tmpCross, &ctx);
		if (tupleCross != NULL) {
			real worldMax = tupleCross->elements[1].data.d;
			if (worldMax > maxCross) {
				// don't update if the world has a farther point
				updateWorldPoint = false;
			}
			destroy_tuple(tupleCross);
		}

		// combine results with master copy (world)
		if (updateWorldPoint) {
			tuple *tmpPoint = make_tuple("s?", MAX_POINT);

			// get rid of the max point tuple if it already exists
			tuple *tuplePoint = get_nb_tuple(tmpPoint, &ctx);
			if (tuplePoint != NULL) destroy_tuple(tuplePoint);

			// get rid of the max cross tuple if it already exists
			tuple *tupleCross = get_nb_tuple(tmpCross, &ctx);
			if (tupleCross != NULL) destroy_tuple(tupleCross);

			// fill in tuple information
			tmpPoint->elements[1].data.i = maxPoint;
			tmpPoint->elements[1].tag = 'i';
			tmpCross->elements[1].data.d = maxCross;
			tmpCross->elements[1].tag = 'd';

			// put in tuples and remove from local memory
			put_tuple(tmpPoint, &ctx);
			put_tuple(tmpCross, &ctx);
			destroy_tuple(tmpCross);
			destroy_tuple(tmpPoint);
		}

		// record the number of point clusters reporting
		tuple *templatePointsReporting = make_tuple("s?", POINTS_DONE);
		tuple *pointsReporting = get_tuple(templatePointsReporting, &ctx);
		pointsReporting->elements[1].data.i += 1;
		pointsReporting->elements[1].tag = 'i';
		put_tuple(pointsReporting, &ctx);

	// leave the critical section
	put_tuple(synchLock, &ctx);

}

bool LTHull::isMasked(index_t position) {

	tuple* masked = make_tuple("si", MASKED_POINT, position);
	if (read_nb_tuple(masked, &ctx) != NULL) return true;
	return false;

}

void LTHull::mask(index_t position) {

	tuple* masked = make_tuple("si", MASKED_POINT, position);
	put_tuple(masked, &ctx);

}

int LTHull::getNumPoints() {

	tuple *templateOrder = make_tuple("s?", NUM_POINTS);
	tuple *order = get_tuple(templateOrder, &ctx);

	int ret = order->elements[1].data.i;

	destroy_tuple(order);
	destroy_tuple(templateOrder);

	return ret;

}

void LTHull::produceOutput() {

	// grab pointers locally.
	PointVector pointsIn = (PointVector) inputs[0];
	PointVector pointsOut = (PointVector) outputs[0];

	// wait for output flag
	tuple *flag = make_tuple("s", FLAG_OUTPUT);
	destroy_tuple(get_tuple(flag, &ctx));

	// bring in all of the emitted points in order
	for (index_t i = 0;; ++i) {
		tuple *emittedPoint = make_tuple("si?", HULL_POINT, i);
		tuple *gottenPoint = get_nb_tuple(emittedPoint, &ctx);
		if (gottenPoint == NULL) break; // no more points!
		pointsOut[i] = pointsIn[gottenPoint->elements[2].data.i];
		mask(gottenPoint->elements[2].data.i);
	}

	// remove the tuple synch lock from tuple space
	tuple *synchLock = make_tuple("s", SYNCH_LOCK);
	get_tuple(synchLock, &ctx);

}
