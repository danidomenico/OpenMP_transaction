/**
 * \file cowichan_lt/src/sor.cpp
 * \brief LinuxTuples successive over-relaxation implementation.
 * \see CowichanLinuxTuples::sor
 */

#include <iostream>
#include <cstdio>
#include <cmath>
#include "sor.hpp"

const char* LTSor::SYNCH_LOCK = "sor synch lock";
const char* LTSor::ROWS_DONE = "sor rows reporting";
const char* LTSor::SOLUTION_VECTOR = "sor solution row";
const char* LTSor::SOLUTION_SUM = "sor inner sum";
const char* LTSor::SOR_FLAG = "sor input consumed";

void CowichanLinuxTuples::sor(Matrix matrix, Vector target, Vector solution) {
	std::cout << "sor" << std::endl;
	// calculate the 2D bounds of the point cloud
	LTSor app;
	app.addInput(0, matrix);
	app.addInput(0, target);
	app.addOutput(0, solution);
	app.start(SERVER, PORT, NUM_WORKERS);

}

//===========================================================================//

void LTSor::consumeInput() {

	// create a tuple synch lock
	tuple *synchLock = make_tuple("s", SYNCH_LOCK);
	put_tuple(synchLock, &ctx);

	// Initialise. note that as these can run in processes, the
	// solution vector must live in tuple space to be communicated
	// between the workers. Setting the solution variable is LOCAL ONLY
	Matrix matrix = (Matrix) inputs[0];
	Vector target = (Vector) inputs[1];
	Vector solution = (Vector) outputs[0];
	for (index_t r = 0; r < SOR_N; ++r) {
		VECTOR(solution, r) = 1.0;
	}

	// put a tuple for the solution vector into tuple space
	tuple *solutionTuple = make_tuple("ss", SOLUTION_VECTOR);
	solutionTuple->elements[1].data.s.len = sizeof(solution);
	solutionTuple->elements[1].data.s.ptr = (char*) solution;

	// loop until we get the desired tolerance
	real maxDiff = (real)(2 * SOR_TOLERANCE);
	for (index_t t = 0; (t < SOR_MAX_ITERS) && (maxDiff >= SOR_TOLERANCE); t++) {

		maxDiff = 0.0;
		for (index_t r = 0; r < SOR_N; ++r) {

			// compute sum
			real sum = solutionSum(r);

			// calculate new solution
			real oldSolution = VECTOR(solution, r);
			VECTOR(solution, r) = (real)(
				(1.0 - SOR_OMEGA) * oldSolution +
				SOR_OMEGA * (VECTOR(target, r) - sum) / MATRIX_SQUARE_N(matrix, r, r, SOR_N)
			);

			// refresh the solution vector in tuple-space
			tuple *solutionBlank = make_tuple("s?", SOLUTION_VECTOR);
			get_tuple(solutionBlank, &ctx);
			put_tuple(solutionTuple, &ctx);
			destroy_tuple(solutionBlank);

			// compute difference
			real diff = (real) fabs((double)(oldSolution - VECTOR(solution, r)));
			if (diff > maxDiff) {
				maxDiff = diff;
			}
		}
	}

	// flag the output producer.
	put_tuple(make_tuple("s", SOR_FLAG), &ctx);

}

real LTSor::solutionSum(index_t row) {

	// tuple template.
	tuple *send = make_tuple("sii", "sor request");

	// create an "rows reporting" tuple, so that we
	// know when the computation should end.
	tuple *rowsReporting = make_tuple("si", ROWS_DONE, 0);
	put_tuple(rowsReporting, &ctx);

	// create a sum tuple
	tuple *sumTuple = make_tuple("sd", SOLUTION_SUM, 0.0);
	put_tuple(sumTuple, &ctx);

	// split points, based on a cluster size of the square-root of the
	// number of elements in the solution vector.
	index_t skip = (size_t) sqrt((real) SOR_N);
	for (index_t pos = 0; pos < SOR_N; pos += skip) {
		send->elements[1].data.i = pos;
		send->elements[2].data.i = std::min(pos + skip, SOR_N);
		put_tuple(send, &ctx);
	}

	// wait for all the rows to be consumed
	rowsReporting->elements[1].data.i = SOR_N;
	get_tuple(rowsReporting, &ctx);

	// get the sum of the tuple-space op
	destroy_tuple(sumTuple);
	sumTuple = make_tuple("s?", SOLUTION_SUM);
	get_tuple(sumTuple, &ctx);

	// return the result
	return sumTuple->elements[1].data.d;

}

void LTSor::work() {

	// tuple templates
	tuple *synchLock = make_tuple("s", SYNCH_LOCK);
	tuple *recv = make_tuple("s??", "sor request");

	// grab pointers locally.
	Matrix matrix = (Matrix) inputs[0];

	while (1) {

		// block until we receieve a tuple.
		tuple* gotten = get_tuple(recv, &ctx);
		index_t row = gotten->elements[1].data.i;
		index_t start = gotten->elements[2].data.i;
		index_t stop = gotten->elements[3].data.i;

		// grab the solution tuple
		tuple *templateSolutionTuple = make_tuple("s?", SOLUTION_VECTOR);
		tuple *solutionTuple = read_tuple(templateSolutionTuple, &ctx);
		destroy_tuple(templateSolutionTuple);
		Vector solution = (Vector) solutionTuple->elements[1].data.s.ptr;

		// sum part of the matrix row with the corresponding elements
		// in the solution vector.
		real sum = 0.0;
		for (index_t col = start; col < stop; ++col) {
			if (col != row) {
				sum += MATRIX_SQUARE_N(matrix, row, col, SOR_N) * VECTOR(solution, col);
			}
		}

		// purge local memory of the tuple we received
		destroy_tuple(gotten);

		// Now, we combine the results from these elements with the "world".
		// enter the critical section
		get_tuple(synchLock, &ctx);

			// combine results with master copy (sum)
			tuple *tmpSum = make_tuple("s?", SOLUTION_SUM);
			tuple *tupleSum = get_tuple(tmpSum, &ctx);
			tmpSum->elements[1].data.d = sum + tupleSum->elements[1].data.d;
			put_tuple(tmpSum, &ctx);
			destroy_tuple(tmpSum);
			destroy_tuple(tupleSum);

			// record the number of rows reporting
			tuple *templateRowsReporting = make_tuple("s?", ROWS_DONE);
			tuple *rowsReporting = get_tuple(templateRowsReporting, &ctx);
			rowsReporting->elements[1].data.i += (stop - start);
			put_tuple(rowsReporting, &ctx);
			destroy_tuple(templateRowsReporting);
			destroy_tuple(rowsReporting);

		// leave the critical section
		put_tuple(synchLock, &ctx);

	}

}

void LTSor::produceOutput() {

	// wait for the input producer to flag us.
	tuple* tmpFlag = make_tuple("s", SOR_FLAG);
	tuple* flag = get_tuple(tmpFlag, &ctx);

	// grab the solution tuple and copy it to the output
	tuple *templateSolutionTuple = make_tuple("s?", SOLUTION_VECTOR);
	tuple *solutionTuple = read_tuple(templateSolutionTuple, &ctx);
	memcpy(outputs[0],
		solutionTuple->elements[1].data.s.ptr,
		solutionTuple->elements[1].data.s.len);

	// remove the tuple synch lock from tuple space
	tuple *synchLock = make_tuple("s", SYNCH_LOCK);
	get_tuple(synchLock, &ctx);

	// clean-up
	destroy_tuple(tmpFlag);
	destroy_tuple(flag);
	destroy_tuple(templateSolutionTuple);
	destroy_tuple(solutionTuple);
	destroy_tuple(synchLock);

}
