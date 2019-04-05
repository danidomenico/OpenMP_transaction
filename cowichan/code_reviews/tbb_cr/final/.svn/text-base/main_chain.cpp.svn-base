#include "cowichan.hpp"

// default values for the toys.
int Cowichan::NROWS = 40;
int Cowichan::NCOLS = 40;
int Cowichan::NELTS = 40;
int Cowichan::NUMGEN = 20;
real Cowichan::x0 = -2.0;
real Cowichan::y0 = -1.0;
real Cowichan::dx = 3.0;
real Cowichan::dy = 2.0;
real Cowichan::PERCENT = 0.25;
int Cowichan::NFILL = 200;
uint Cowichan::SEED = 681304;

>>> TODO: Most inputs should be taken from command line

	/**
	 * Runs all problems in the cowichan problem set, chained together.
	 * @param numThreads	the number of threads to spawn using TBB.
	 * @param use_randmat	true: generate a random matrix.
	 * 						false: use a window of the mandelbrot set.
	 * @param use_thresh	true: use image thresholding for int->bool.
	 *						false: use invasion percolation for int->bool.
	 */
	void Cowichan::run(int numThreads, bool use_randmat, bool use_thresh) {
		IntMatrix matrix;
		Matrix realmx;
		Vector vector, x_sor, x_gauss;
		BoolMatrix bm;
		PointList* points;
		real e_gauss, e_sor;
		
		// set up for the number of threads we will use
		COWICHAN(numThreads);		
		
/* 1 */	if (use_randmat) {
			randmat(&matrix);
		} else {
			mandel(&matrix);
		}
		
/* 2 */	half(matrix, &matrix);

/* 3 */	if (use_thresh) {
			thresh(matrix, &bm);
		} else {
			invperc(matrix, &bm);
		}

/* 4 */	life(bm, &bm);
/* 5 */	winnow(matrix, bm, &points);

/* 6 */ //hull(points, &points);

/* 7 */	norm(points, &points);

		print(*points);

/* 8 */	outer(points, &realmx, &vector);

/* 9 */	gauss(realmx, vector, &x_gauss);
		sor(realmx, vector, &x_sor);
		
/* 10*/	product(realmx, vector, x_gauss, &e_gauss);
		product(realmx, vector, x_sor, &e_sor);

    >>> There are 11 steps in the chain, not 10.
		
	}
	
Point Point::minimum = Point(MINIMUM_REAL, MINIMUM_REAL);
Point Point::maximum = Point(MAXIMUM_REAL, MAXIMUM_REAL);
Point Point::origin  = Point(0.0, 0.0);	
	
/*****************************************************************************/

/**
 * Returns a pseudorandom number ~ U[mean - range, mean + range].
 */
real uniform(real mean, real range) {
	return (rand() / (real)RAND_MAX) * (2.0f * range) - range + mean;
}

/**
 * "pretty-print" a list of points.
 */
#define PRINT_BREAK 4
void print(PointList& points) {
	int b = 0;
	for (PointList::const_iterator it = points.begin(); it != points.end(); ++it) {
		if (b == 0) std::cout << std::endl << "\t";
		std::cout << "(" << it->x << "," << it->y << ")\t";
		b = (b + 1) % PRINT_BREAK;
	}
}

/**
 * show a matrix result
 */
void printAxb(Matrix matrix, Vector answer, Vector vector) {
	std::cout.precision(5);
	for (int row = 0; row < Cowichan::NELTS; ++row) {

		// print out the matrix
		std::cout << " [ ";
		for (int col = 0; col < Cowichan::NELTS; ++col) {
			std::cout << MATRIX(matrix, row,col) << "\t";
		}
		
		// print out the answer
		std::cout << "] [ " << VECTOR(answer, row) << " ]\t";

		// print out the vector
		if (row == int(Cowichan::NELTS / 2)) {
			std::cout << "= [ ";
		} else {
			std::cout << "  [ ";
		}
		std::cout << VECTOR(vector, row) << " ]" << std::endl;
		
	}
}

/*****************************************************************************/

/**
 * The entry point of the Cowichan/TBB problem set.
 */
int main(int argc, char** argv) {
	Cowichan::run(2, true, false);
	return 0;
}

