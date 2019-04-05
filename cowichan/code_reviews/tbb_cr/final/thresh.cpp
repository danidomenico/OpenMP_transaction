#include "cowichan.hpp"

/**
 * Performs the maximum computation.
 */
class MaxReducer {
private:

	IntMatrix _image;
	int _max;

public:

	/**
	 * Initialise max with the lowest possible value.
	 */
	MaxReducer(IntMatrix image):
		_image(image), _max(MINIMUM_INT) { }

	int getMaximum() const {
		return _max;
	}

	/**
	 * Calculates the maximum value over the given range.
	 */
	void operator()(const Range2D& range) {

		IntMatrix image = _image;

		const Range& rows = range.rows();
		const Range& cols = range.cols();
		
		for (size_t y = rows.begin(); y != rows.end(); ++y) {
			for (size_t x = cols.begin(); x != cols.end(); ++x) {
				if (_max < MATRIX_RECT(image, y, x)) {
					_max = MATRIX_RECT(image, y, x);
				}
			}
		}
		
	}
	
	/**
	 * Splitting (TBB) constructor
	 */
	MaxReducer(MaxReducer& other, split):
		_image(other._image), _max(MINIMUM_INT) { }

	/**
	 * Joiner (TBB).
	 */
	void join(const MaxReducer& other) {
		_max = std::max(other._max, _max);
	}
	
};

/*
 * This class calculates the histogram of a matrix.
 */
class Histogram {
	
	IntMatrix _image;
	int* histogram;
	const int bins;
	
public:

	static int calculate(IntMatrix image, real cutoff) {
		
		// get the maximum value in the matrix (need 0-that number of bins)
		MaxReducer reducer(image);
		parallel_reduce(
			Range2D(0, Cowichan::NROWS, 0, Cowichan::NCOLS),
			reducer, auto_partitioner());
		int max = reducer.getMaximum();
		
		// compute the histogram to get a thresholding value
		Histogram hist(image, max);
		parallel_for(
			Range2D(0, Cowichan::NROWS, 0, Cowichan::NCOLS),
			hist, auto_partitioner());
		
		// return that value
		return hist.getValue(cutoff);
		
	}
	
public:

	Histogram(IntMatrix image, int maxValue): _image(image), bins(maxValue) {
		histogram = new int[maxValue + 1];
		for (int i = 0; i <= maxValue; ++i) {
			histogram[i] = 0;
		}
	}
	
	~Histogram() {
		delete[] histogram;
	}

	int getValue(real cutoff) const {
		int i;
		int retain = (Cowichan::NROWS * Cowichan::NCOLS) * cutoff;
		for (i = bins; (i >= 0) && (retain > 0); --i) {
			retain -= histogram[i];
		}
		return i;
	}
	
	/**
 	 * Histogram calculation.
 	 */
	void operator()(const Range2D& range) const {
		IntMatrix image = _image;
		int* hist = histogram;
		
		const Range& rows = range.rows();
		const Range& cols = range.cols();
		
		for (size_t y = rows.begin(); y != rows.end(); ++y) {
			for (size_t x = cols.begin(); x != cols.end(); ++x) {
				hist[MATRIX_RECT(image, y, x)]++;
			}
		}
	}
	
	/**
	 * Splitting (TBB) constructor
	 */
	Histogram(Histogram& other, split): _image(other._image), bins(other.bins) {
		histogram = new int[bins + 1];
		for (int i = 0; i <= bins; ++i) {
			histogram[i] = 0;
		}		
	}
	
	/**
	 * Joiner (TBB).
	 */
	void join(const Histogram& other) {
		// SERIAL... XXX can we speed up by parallel_for here, too?

    >>> More parallelization is probably not necessary. You don't know the
    >>> number of bins beforehand.

		for (int i = 0; i < bins; ++i) {
			histogram[i] += other.histogram[i];
		}
	}
	
};

/**
 * This class takes an Integer array to a boolean array based on a cut-off value.
 */
class Threshold {

	IntMatrix _image;
	BoolMatrix _result;
	const int retain;

public: 

	static void exec(IntMatrix image, int retain, BoolMatrix result) {
		Threshold thresh(image, retain, result);
		parallel_for(Range2D(0, Cowichan::NROWS, 0, Cowichan::NCOLS), thresh,
			auto_partitioner());
	}
	
public:

	float _sum;
	
	float getSum() const {
		return _sum;
	}
	
	void operator()(const Range2D& range) const {
		IntMatrix image = _image;
		BoolMatrix result = _result;
		
		const Range& rows = range.rows();
		const Range& cols = range.cols();
		
		for (size_t y = rows.begin(); y != rows.end(); ++y) {
			for (size_t x = cols.begin(); x != cols.end(); ++x) {
				MATRIX_RECT(result, y, x) = MATRIX_RECT(image, y, x) > retain;
			}
		}
	}
	
	Threshold(IntMatrix image, int retain, BoolMatrix result):
		_image(image), _result(result), retain(retain) { }

};

/*****************************************************************************/

void Cowichan::thresh(IntMatrix matrix, BoolMatrix* mask) {
	
	*mask = NEW_MATRIX_RECT(bool);
	
	// figure out what values to keep and which to get rid of
	int retain = Histogram::calculate(matrix, PERCENT);
	
	// perform the thresholding opearation
	Threshold::exec(matrix, retain, *mask);

}

