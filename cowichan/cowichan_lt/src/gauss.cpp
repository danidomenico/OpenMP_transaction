/**
 * \file cowichan_lt/src/gauss.cpp
 * \brief LinuxTuples gauss implementation.
 * \see CowichanLinuxTuples::gauss
 */

#include <iostream>
#include <cstdio>
#include <cmath>
#include "gauss.hpp"

const char* LTForward::ROWS_DONE = "forward rows reporting";
const char* LTForward::FORWARD_DONE = "forward done";
const char* LTForward::REQUEST = "forward request";

const char* LTForward::TARGET = "gauss target";
const char* LTForward::MATRIX_ROW = "gauss matrix row";

void CowichanLinuxTuples::gauss(Matrix matrix, Vector target, Vector solution) {

	std::cout << "gauss" << std::endl;

	// forward-elimination.
	// forward puts the target and matrix into tuple space, and returns them.
	LTForward forward;
	forward.addInput(0, matrix);
	forward.addInput(1, target);
	forward.addOutput(0, matrix);
	forward.addOutput(1, target);
	forward.start(SERVER, PORT, NUM_WORKERS);

	// backward-substitution and solution creation (from serial implementation).
	for (index_t k = (n - 1); k >= 0; k--) {
		solution[k] = target[k] / MATRIX_SQUARE(matrix, k, k);
		for (index_t i = k - 1; i >= 0; i--) {
			target[i] = target[i] - (MATRIX_SQUARE(matrix, i, k) * solution[k]);
		}
	}

}

//===========================================================================//

void LTForward::consumeInput() {

	// grab pointers locally.
	Matrix matrix = (Matrix) inputs[0];
	Vector target = (Vector) inputs[1];

	// tuple templates.
	tuple *send = make_tuple("sii", REQUEST);
	tuple *row = make_tuple("sis", MATRIX_ROW);
	tuple *targetTuple = make_tuple("ss", TARGET);

	// put the matrix (row-by-row) and the target vector into the tuple space.
	for (size_t r = 0; r < GAUSS_N; ++r) {
		row->elements[1].data.i = r;
		row->elements[2].data.s.len = sizeof(matrix) / GAUSS_N;
		row->elements[2].data.s.ptr = (char*) &MATRIX_RECT_NC(matrix, r, 0, GAUSS_N);
		put_tuple(row, &ctx);
	}
	targetTuple->elements[1].data.s.len = sizeof(target);
	targetTuple->elements[1].data.s.ptr = (char*) target;
	put_tuple(targetTuple, &ctx);
	destroy_tuple(targetTuple);

	// one column at-a-time.
	tuple *grabRow = make_tuple("si?", MATRIX_ROW);
	tuple *grabTarget = make_tuple("s?", TARGET);
	for (index_t c = 0; c < GAUSS_N; ++c) {

		// create a "rows reporting" tuple, so that we
		// know when the computation has ended (workers are done)
		tuple *rowsReporting = make_tuple("si", ROWS_DONE, 0);
		put_tuple(rowsReporting, &ctx);

	    // get row with maximum column i
	    index_t maxRow = c;
	    for (index_t r = c + 1; r < GAUSS_N; ++r) {

	    	// grab the two rows (r and maxRow)
	    	grabRow->elements[1].data.i = r;
	    	tuple *rRowTuple = get_tuple(grabRow, &ctx);
	    	grabRow->elements[1].data.i = maxRow;
	    	tuple *maxRowTuple = get_tuple(grabRow, &ctx);
	    	Vector rRowVector = (Vector) rRowTuple->elements[2].data.s.ptr;
	    	Vector maxRowVector = (Vector) maxRowTuple->elements[2].data.s.ptr;

	    	// make sure we have the maximum
			if (fabs(rRowVector[c]) > fabs(maxRowVector[c])) {
				maxRow = r;
			}

			// put the two rows back (r and max)
			put_tuple(rRowTuple, &ctx);
			put_tuple(maxRowTuple, &ctx);
			destroy_tuple(rRowTuple);
			destroy_tuple(maxRowTuple);

		}

		// swap max row with row c to put largest value on diagonal
		for (index_t r = c; r < GAUSS_N; ++r) {

	    	// grab the two rows (c and maxRow)
	    	grabRow->elements[1].data.i = c;
	    	tuple *cRowTuple = get_tuple(grabRow, &ctx);
	    	grabRow->elements[1].data.i = maxRow;
	    	tuple *maxRowTuple = get_tuple(grabRow, &ctx);

	    	// swap the row vectors inside the tuples
			char* tmpPointer = cRowTuple->elements[2].data.s.ptr;
	    	cRowTuple->elements[2].data.s.ptr = maxRowTuple->elements[2].data.s.ptr;
			maxRowTuple->elements[2].data.s.ptr = tmpPointer;

			// put the two rows back (c and maxRow)
			put_tuple(cRowTuple, &ctx);
			put_tuple(maxRowTuple, &ctx);
			destroy_tuple(cRowTuple);
			destroy_tuple(maxRowTuple);

		}

		// grab target, swap, put it back (c and maxRow)
		targetTuple = get_tuple(grabTarget, &ctx);
		target = (Vector) targetTuple->elements[1].data.s.ptr;
		real temp = target[c];
		target[c] = target[maxRow];
		target[maxRow] = temp;
		put_tuple(targetTuple, &ctx);
		destroy_tuple(targetTuple);

		// create a forward request for each row under the diagonal
		for (index_t r = c + 1; r < GAUSS_N; ++r) {
			send->elements[1].data.i = r; // row
			send->elements[2].data.i = c; // column
			put_tuple(send, &ctx);
		}

		// wait for the workers to finish this column
		size_t rowsToBeDone = GAUSS_N - (c + 1);
		rowsReporting->elements[1].data.i = rowsToBeDone;
		destroy_tuple(get_tuple(rowsReporting, &ctx));

	}

	// Forward pass is finished; notify output producer.
	tuple *forwardDone = make_tuple("s", FORWARD_DONE);
	get_tuple(forwardDone, &ctx);
	destroy_tuple(forwardDone);

	// Destroy all other tuple references in memory.
	destroy_tuple(grabRow);
	destroy_tuple(grabTarget);

}

void LTForward::work() {

	// tuple templates
	tuple *recv = make_tuple("s??", REQUEST);
	tuple *grabRow = make_tuple("si?", MATRIX_ROW);
	tuple *grabTarget = make_tuple("s?", TARGET);
	tuple *templateRowsReporting = make_tuple("s?", ROWS_DONE);

	while (1) {

		// block until we receive a tuple.
		tuple* gotten = get_tuple(recv, &ctx);
		index_t i = gotten->elements[1].data.i;
		index_t j = gotten->elements[2].data.i;

    	// grab the two rows (i and j)
    	grabRow->elements[1].data.i = i;
    	tuple *iRowTuple = get_tuple(grabRow, &ctx);
    	grabRow->elements[1].data.i = j;
    	tuple *jRowTuple = get_tuple(grabRow, &ctx);
    	Vector iRowVector = (Vector) iRowTuple->elements[2].data.s.ptr;
    	Vector jRowVector = (Vector) jRowTuple->elements[2].data.s.ptr;

		// actual computation.
    	real column_i = iRowVector[i];
		real factor = -(jRowVector[i] / column_i);
		for (index_t k = GAUSS_N - 1; k >= i; k--) {
			jRowVector[k] += iRowVector[k] * factor;
		}

		// put the two rows back (i and j)
		put_tuple(iRowTuple, &ctx);
		put_tuple(jRowTuple, &ctx);
		destroy_tuple(iRowTuple);
		destroy_tuple(jRowTuple);

		// update the target vector.
		tuple *targetTuple = get_tuple(grabTarget, &ctx);
		Vector target = (Vector) targetTuple->elements[1].data.s.ptr;
		target[j] += target[i] * factor;
		put_tuple(targetTuple, &ctx);
		destroy_tuple(targetTuple);

		// purge local memory of the tuple we received
		destroy_tuple(gotten);

		// increment the number of rows reporting by one.
		tuple *rowsReporting = get_tuple(templateRowsReporting, &ctx);
		rowsReporting->elements[1].data.i++;
		put_tuple(rowsReporting, &ctx);

	}

}

void LTForward::produceOutput() {

	// grab output pointers.
	Matrix matrix = (Matrix) outputs[0];
	Vector target = (Vector) outputs[1];

	// tuple templates.
	tuple *grabRow = make_tuple("si?", MATRIX_ROW);
	tuple *grabTarget = make_tuple("s?", TARGET);

	// wait for columns to be done.
	tuple *forwardDone = make_tuple("s", FORWARD_DONE);
	get_tuple(forwardDone, &ctx);

	// get the matrix (row-by-row)...
	for (size_t r = 0; r < GAUSS_N; ++r) {
		grabRow->elements[1].data.i = r;
		tuple* grabbedRow = get_tuple(grabRow, &ctx);
		memcpy(
			&MATRIX_SQUARE_N(matrix, grabbedRow->elements[1].data.i, 0, GAUSS_N),
			grabbedRow->elements[2].data.s.ptr,
			grabbedRow->elements[2].data.s.len);
		destroy_tuple(grabbedRow);
	}

	// ... and the target vector from tuple space.
	tuple* grabbedTarget = get_tuple(grabTarget, &ctx);
	memcpy(target, grabbedTarget->elements[1].data.s.ptr, grabbedTarget->elements[1].data.s.len);
	destroy_tuple(grabbedTarget);

	// we're done.

}
