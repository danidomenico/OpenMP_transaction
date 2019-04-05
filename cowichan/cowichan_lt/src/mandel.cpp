/**
 * \file cowichan_lt/src/mandel.cpp
 * \brief LinuxTuples mandelbrot set implementation.
 * \see CowichanLinuxTuples::mandel
 */

#include <iostream>
#include <cstdio>
#include "mandel.hpp"

void CowichanLinuxTuples::mandel(IntMatrix matrix) {
	LTMandel mandelApp;
	mandelApp.addOutput(0, matrix);
	mandelApp.start(SERVER, PORT, NUM_WORKERS);
}

void LTMandel::consumeInput() {

	// send off a mandelbrot request for each grid row.
	for (size_t y = 0; y < MANDEL_NR; ++y) {
		tuple *send = make_tuple("si", "mandel request");
		send->elements[1].data.i = y;
		put_tuple(send, &ctx);
		destroy_tuple(send);
	}
	
}

int LTMandel::mandelCalc(real x, real y) {

	real r = 0.0, i = 0.0;
	real rs = 0.0, is = 0.0;
	int numIterations = 0;		
	do {

		// calculate the complex value according to the mandelbrot set specs.
		i = (2.0 * r * i) + x;
		r = (rs - is) + y;
	
		// calculate squared complex value
		rs = r * r;
		is = i * i;			
	
		// "step" the simulation for this co-ordinate.
		++numIterations;			
	
	} while ((numIterations < MANDEL_MAX_ITER) && ((rs + is) < MANDEL_INFINITY));

	// we are interested if the series converges or diverges. Return the
	// number of iterations before such an event (divergence).
	return numIterations;

}

void LTMandel::work() {

	tuple *recv = make_tuple("s?", "mandel request");
	
	// 2D delta between calculated points
	real dX = MANDEL_DX / (MANDEL_NC - 1);
	real dY = MANDEL_DY / (MANDEL_NR - 1);

	// satisfy mandelbrot requests.
	while (1) {

		// block until we receive a tuple.
		tuple* gotten = get_tuple(recv, &ctx);

		// copy over row co-ordinate of the computation; create
		// a buffer for the results of the computation.
		tuple *send = make_tuple("sis", "mandel done", gotten->elements[1].data.i, "");
		int* buffer = (int*) malloc(sizeof(int) * MANDEL_NC);
		send->elements[2].data.s.len = sizeof(int) * MANDEL_NC;
		send->elements[2].data.s.ptr = (char*) buffer;

		// perform the actual computation for this row.
		double rY = MANDEL_Y0 + dY * send->elements[1].data.i;
		double rX = MANDEL_X0;
		for (int x = 0; x < MANDEL_NC; ++x, rX += dX) {
			buffer[x] = mandelCalc(rX, rY);
		}
	
		// send off the new tuple and purge local memory of the one we gotten
		put_tuple(send, &ctx);
		destroy_tuple(gotten);
		destroy_tuple(send);

	}

	// TODO destroy the template tuples; must send tuples for this
//	destroy_tuple(send);
//	destroy_tuple(recv);

}

void LTMandel::produceOutput() {

	// tuple template
	tuple *recv = make_tuple("s??", "mandel done");

	// grab output pointer locally.
	IntMatrix output = (IntMatrix) outputs[0];

	// grab all of the mandelbrot computations from the workers,
	// in an unspecified order.
	int computations = MANDEL_NR;
	while (computations > 0) {

		// get the tuple and copy it into the matrix.
		tuple* received = get_tuple(recv, &ctx);
		memcpy(
			&MATRIX_RECT_NC(output, received->elements[1].data.i, 0, MANDEL_NC),
			received->elements[2].data.s.ptr,
			received->elements[2].data.s.len);
		computations--;
		destroy_tuple(received);

	}

	// destroy the template tuple
	destroy_tuple(recv);

}

