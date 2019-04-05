#include <iostream>
#include <cstdio>
extern "C" {
	#include "tuple.h"
}

using std::cout;
using std::endl;

typedef double real;

#define NROWS 10
#define NCOLS 10

const real width = 1.0;
const real height = 1.0;
const real baseX = -0.5;
const real baseY = -0.5;

int main(int argc, char** argv) {

	// connect to the tuple server.
	struct context ctx;
	if (get_server_portnumber(&ctx)) {
		if (argc < 3) {
			/* help message */
			fprintf(stderr, "Usage: %s <server> <portnumber>\n", argv[0]);
			exit(1);
		}
		strcpy(ctx.peername, argv[1]);
		ctx.portnumber = atoi(argv[2]);
	}

	real dX = width / (NCOLS - 1);
	real dY = height / (NROWS - 1);

	tuple *send = make_tuple("siidd", "mandel request");
	tuple *recv = make_tuple("s???", "mandel done");
	
	// send off a mandelbrot request for each grid index.
	for (size_t y = 0; y < NROWS; ++y) {
		for (size_t x = 0; x < NCOLS; ++x) {
			send->elements[1].data.i = x;					// x index
			send->elements[2].data.i = y;					// y index
			send->elements[3].data.d = baseX + (x * dX);	// x coordinate
			send->elements[4].data.d = baseY + (y * dY);	// y coordinate
			put_tuple(send, &ctx);
		}
	}

	// grab all of the mandelbrot computations from the workers,
	// in an unspecified order.
	int computations = NROWS * NCOLS;
	while (computations > 0) {
		tuple* received = get_tuple(recv, &ctx);
		cout << "(" << received->elements[1].data.i
			 << "," << received->elements[2].data.i << "): "
			 << received->elements[3].data.i << " iterations." << endl;
		computations--;
		destroy_tuple(received);
	}

	// destroy the template tuples
	destroy_tuple(send);
	destroy_tuple(recv);

	return 0;	
}

