#include "cowichan.hpp"

#ifdef GRAPHICS
	#include "sdl.hpp"
#endif

/**
 * This class does the game of life, and facilitates a ping-pong memory model.
 */
class GameOfLife {

	int sumNeighbours(int x, int y) const {

		int peers = 0;

		// calculate possible neighbour positions
		bool l = (x > 0);
		bool r = (x < (Cowichan::NELTS - 1));
		bool u = (y > 0);
		bool d = (y < (Cowichan::NELTS - 1));		

		// calculate no. of neighbours
		if (l && 		MATRIX(_first, y,  x-1))	++peers;
		if (l && u &&	MATRIX(_first, y-1,x-1)) 	++peers;
		if (u && 		MATRIX(_first, y-1,x  ))	++peers;
		if (r && u &&	MATRIX(_first, y-1,x+1)) 	++peers;
		if (r && 		MATRIX(_first, y,  x+1))	++peers;
		if (r && d &&	MATRIX(_first, y+1,x+1))	++peers;
		if (d && 		MATRIX(_first, y+1,x  )) 	++peers;
		if (l && d &&	MATRIX(_first, y+1,x-1)) 	++peers;		
		
		return peers;
		
	}

public:
	
	BoolMatrix _first, _second;

	GameOfLife(BoolMatrix first, BoolMatrix second):
		_first(first), _second(second) { }

	void swap() {
		BoolMatrix temp = _first;
		_first = _second;
		_second = temp;
	}

	/**
	 * Performs the game of life operation over the given range.
	 */
	void operator()(const Range2D& range) const {
		
		BoolMatrix first = _first;
		BoolMatrix second = _second;

    >>> I wonder if there is a way to do an inplace life, ie using
    >>> one array.
		
		const Range& rows = range.rows();
		const Range& cols = range.cols();
		
		for (size_t y = rows.begin(); y != rows.end(); ++y) {
			for (size_t x = cols.begin(); x != cols.end(); ++x) {
				
				int peers = sumNeighbours(x, y);
				if (peers < 2 || peers > 3) {
					MATRIX(second, y,x) = false;	// hunger/overcrowding
				} else if (peers == 3) {
					MATRIX(second, y,x) = true;		// breeding
				} else {
					MATRIX(second, y,x) = MATRIX(first, y,x);	// nothing.
				}
				
			}
		}
		
	}
};

/*****************************************************************************/

void Cowichan::life(BoolMatrix input, BoolMatrix* output) {
	BoolMatrix other = NEW_MATRIX_SQUARE(bool);
	GameOfLife game(input, other);
	for (int i = 0; i < NUMGEN; ++i) {

		// update CA simulation
		parallel_for(
			Range2D(0, NELTS, 0, NELTS),
			game, auto_partitioner());

		// swap arrays (ping-pong approach)
		game.swap();		

	}
	*output = game._first;
	delete[] game._second;
}

