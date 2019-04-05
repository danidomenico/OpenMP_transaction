#include <iostream>
#include <cstdio>

using std::cout;
using std::endl;

typedef double real;

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
	tuple *recv = make_tuple("s????", "mandel request");
	tuple *send = make_tuple("siii", "mandel done");
	
	// satisfy mandelbrot requests.
	while (1) {

		// block until we receieve a tuple.
		tuple* gotten = get_tuple(recv, &ctx);

		// copy over grid co-ordinates of the computation.
		send->elements[1].data.i = gotten->elements[1].data.i;
		send->elements[2].data.i = gotten->elements[2].data.i;

		// perform the actual computation.
		send->elements[3].data.i = mandelCalc(
			gotten->elements[3].data.d, // x co-ordinate
			gotten->elements[4].data.d  // y co-ordinate
		);

		// send off the new tuple and purge local memory of the one we gotten
		put_tuple(send, &ctx);
		destroy_tuple(gotten);

	}

	// destroy the template tuples
	destroy_tuple(send);
	destroy_tuple(recv);

	return 0;	
}

