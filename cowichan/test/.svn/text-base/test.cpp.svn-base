/**
 * \file cowichan.cpp
 * \brief Implementation for Cowichan class and common routines for Cowichan
 * programs.
 */

#include "cowichan.hpp"
#include <assert.h>
#include <fstream>

real uniform(real mean, real range) {
  return (rand() / (real)RAND_MAX) * (2.0f * range) - range + mean;
}

/*****************************************************************************/

void out_of_memory() {
  std::cout << "--- Out of memory! ---";
  exit(1);
}

void not_enough_points() {
  std::cout << "--- Not enough points! ---";
  exit(1);
}

void no_cells_alive() {
  std::cout << "--- No cells alive! ---";
  exit(1);
}

#ifdef OUTPUT_DATA
void Cowichan::print_vector(PointVector points)
{
  index_t r;

  for (r = 0; r < n; r++) {
    std::cout << "[" << points[r].x << ", " << points[r].y << "]\n";
  }
  std::cout << "\n";
}
#else
void Cowichan::print_vector(PointVector /* points */) { }
#endif

/*****************************************************************************/

INT64 get_ticks ()
{
  INT64 count;
#if defined(WIN32) || defined(WIN64)   // Windows
  if (! QueryPerformanceCounter((LARGE_INTEGER *) &count)) {
    count = GetTickCount (); // ms
  }
#else                // Linux
  tms tm;
  count = times (&tm);
#endif               // end of Windows/Linux definitions
  return count;
}

INT64 get_freq ()
{
  INT64 freq;
#if defined(WIN32) || defined(WIN64)   // Windows
  if (! QueryPerformanceFrequency((LARGE_INTEGER *) &freq)) {
    freq = 1000; // ms
  }
#else                // Linux
  freq = sysconf (_SC_CLK_TCK);
#endif               // end of Windows/Linux definitions
  return freq;
}

void print_elapsed_time (INT64 start, INT64 end)
{
  INT64 freq = get_freq ();
  std::cout.precision(5);
  std::cout << (((double) (end - start)) / ((double) freq)) << " seconds";
  std::cout.flush();
}

void timeInfo(INT64 *start, INT64 *end, std::string message) {
  *start = *end;
  *end = get_ticks();
  #ifdef TEST_TIME
    std::cout << message << ": ";
    print_elapsed_time(*start, *end);
    std::cout << std::endl;
  #endif
}

/*****************************************************************************/

const char* Cowichan::CHAIN = "chain";
const char* Cowichan::MANDEL = "mandel";
const char* Cowichan::RANDMAT = "randmat";
const char* Cowichan::HALF = "half";
const char* Cowichan::INVPERC = "invperc";
const char* Cowichan::THRESH = "thresh";
const char* Cowichan::LIFE = "life";
const char* Cowichan::WINNOW = "winnow";
const char* Cowichan::NORM = "norm";
const char* Cowichan::HULL = "hull";
const char* Cowichan::OUTER = "outer";
const char* Cowichan::GAUSS = "gauss";
const char* Cowichan::SOR = "sor";
const char* Cowichan::PRODUCT = "product";
const char* Cowichan::VECDIFF = "vecdiff";


using namespace std;

int g_argc;
char **g_argv;

void read_const(istream &in, string str) {
    const char* cur = str.c_str();
    string read("");
    int num_read = 0;
    while (*cur) {
	read += in.get();
	if (read[num_read] != *cur) {
	    cout << "parse error: expecting `" << str << "`; "
		 << "got `" << read << "`" << endl;
	    exit(-1);
	}
	cur++;
	num_read++;
    }
}

bool get_arg_value (const char *key, char *&val) {
    int i = 0;
    for (int i = 2; i < (g_argc - 1); i++) {
	if (strcmp (g_argv[i], key) == 0) {
	    val = g_argv[i + 1];
	    return true;
	}
    }
    return false;
}

char *must_get_arg_value (const char *key) {
    char *val;
    if (!get_arg_value(key, val)) {
	cout << "parameter `" << key << "` required";
	exit(-1);
    }
    return val;
}

bool get_int_arg_value (const char *key, int &val) {
    char *val_str;
    if (get_arg_value(key, val_str)) {
	val = atoi(val_str);
    } else {
	return false;
    }
}

int must_get_int_arg_value (const char *key) {
    return atoi(must_get_arg_value(key));
}

template <class T>
T *mat_parser (istream &in, int &rows, int &cols) {
	T *mat;
	in >> boolalpha;
	in >> rows;
	read_const(in, "x");
	in >> cols;
	mat = new T[rows * cols];
	for (int i = 0; i < cols*rows; i += cols) {
	    read_const(in, "\n");
	    for (int j = 0; j < cols; j++) {
		in >> mat[i + j];
	    }
	}
	return mat;
}

template <class T>
void mat_print (ostream &out, T *mat, int rows, int cols) {
    out << boolalpha;
    out << rows << 'x' << cols << endl;
    for (int i = 0; i < rows*cols; i += cols) {
	for (int j = 0; j < cols; j++) {
	    out << mat[i + j];
	    if (j < (cols - 1)) { out << " "; }
	}
	out << endl;
    }
}

template <class T>
T *vec_parser (istream &in, int &size) {
    T *vec;
    in >> size;
    vec = new T[size];
    for (int i = 0; i < size; i++) {
	read_const(in, "\n");
	in >> vec[i];
    }
    return vec;
}

template <class T>
void vec_print (ostream &out, T *vec, int size) {
    out << size << endl;
    for (int i = 0; i < size; i++) {
	out << vec[i] << endl;
    }
}

istream *get_istream_arg(const char *name) {
    char *file_name;
    if (get_arg_value(name, file_name)) {
	return new ifstream(file_name);
    } else {
	return &cin;
    }
}

ostream *get_ostream_arg(const char *name) {
    char *file_name;
    if (get_arg_value(name, file_name)) {
	return new ofstream(file_name);
    } else {
	return &cout;
    }
}

void istream_close (istream *stream) {
    if (stream != &cin) {
	//((ifstream*) stream)->close();
	//delete stream;
    }
}

void ostream_close (ostream *stream) {
    if (stream != &cout) {
	//((ofstream*) stream)->close();
	//delete stream;
    }
}

template <class T>
void get_matrix_arg(const char *name, T *&mat, int &rows, int &cols) {
    istream *in = get_istream_arg(name);
    mat = mat_parser<T>(*in, rows, cols);
    istream_close(in);
}

template <class T>
void get_matrix_arg_in(T *&mat, int &rows, int &cols) {
    get_matrix_arg<T>("--in", mat, rows, cols);
}

template <class T>
void put_matrix_arg(const char *name, T *mat, int rows, int cols) {
    ostream *out = get_ostream_arg(name);
    mat_print<T>(*out, mat, rows, cols);
    ostream_close(out);
}

template <class T>
void put_matrix_arg_out(T *mat, int rows, int cols) {
    put_matrix_arg<T>("--out", mat, rows, cols);
}

template <class T>
void get_vector_arg(const char *name, T *&vec, int &size) {
    istream *in = get_istream_arg(name);
    vec = vec_parser<T>(*in, size);
    istream_close(in);
}

template <class T>
void get_vector_arg_in(T *&vec, int &size) {
    get_vector_arg<T>("--in", vec, size);
}

template <class T>
void put_vector_arg(const char *name, T* vec, int size) {
    ostream *out = get_ostream_arg(name);
    vec_print<T>(*out, vec, size);
    ostream_close(out);
}

template <class T>
void put_vector_arg_out(T* vec, int size) {
    put_vector_arg<T>("--out", vec, size);
}

void Cowichan::main(int argc, char *argv[], bool, bool) {
    g_argc = argc;
    g_argv = argv;

    if (strcmp(argv[1], "gauss") == 0) {
	Matrix mat;
	Vector target;
	Vector solution;
	int size; /* throw out */

	get_matrix_arg<real>("--matrix", mat, nr, nc);
	get_vector_arg<real>("--target", target, size);
	gauss(mat, target, solution);
	put_vector_arg_out<real>(solution, size);

	delete [] mat;
	delete [] target;
	delete [] solution;

    } else if (strcmp(argv[1], "half") == 0) {
	IntMatrix mat;
	IntMatrix res;

	get_matrix_arg_in<INT_TYPE>(mat, nr, nc);
	res = NEW_MATRIX_RECT(INT_TYPE);
	half(mat, res);
	put_matrix_arg_out<INT_TYPE>(res, nr, nc);
	
	delete [] mat;
	delete [] res;
	
    } else if (strcmp(argv[1], "hull") == 0) {
	PointVector points;
	PointVector hull_points;
	
	get_vector_arg_in<Point>(points, n);
	hull_points = NEW_VECTOR(Point);
	hull(points, hull_points);
	put_vector_arg_out<Point>(hull_points, n);

	delete [] points;
	delete [] hull_points;
    } else if (strcmp(argv[1], "invperc") == 0) {
	IntMatrix mat;
	BoolMatrix mask;

	get_matrix_arg_in<INT_TYPE>(mat, nr, nc);
	mask = NEW_MATRIX_RECT(bool);
	invpercNFill = must_get_int_arg_value("--fill");
	invperc(mat, mask);
	put_matrix_arg_out<bool>(mask, nr, nc);

	delete [] mat;
	delete [] mask;

    } else if (strcmp(argv[1], "life") == 0) {
	BoolMatrix mat;
	BoolMatrix res;

	get_matrix_arg_in<bool>(mat, nr, nc);
	lifeIterations = must_get_int_arg_value("--numgens");
	res = NEW_MATRIX_RECT(bool);
	life(mat, res);
	put_matrix_arg_out<bool>(res, nr, nc);

	delete [] mat;
	delete [] res;

    } else if (strcmp(argv[1], "mandel") == 0) {
	IntMatrix mat;

	mandelX0 = must_get_int_arg_value("--x");
	mandelY0 = must_get_int_arg_value("--y");
	mandelDx = must_get_int_arg_value("--dx");
	mandelDy = must_get_int_arg_value("--dy");
	nr = must_get_int_arg_value("--rows");
	nc = must_get_int_arg_value("--cols");
	mandel(mat);
	put_matrix_arg_out<INT_TYPE>(mat, nr, nc);

	delete [] mat;

    } else if (strcmp(argv[1], "norm") == 0) {
	PointVector points_in;
	PointVector points_out;

	get_vector_arg_in<Point>(points_in, n);
	points_out = NEW_VECTOR(Point);
	norm(points_in, points_out);
	put_vector_arg_out<Point>(points_out, n);

	delete [] points_in;
	delete [] points_out;

    } else if (strcmp(argv[1], "outer") == 0) {
	PointVector points;
	Matrix mat;
	Vector vec;

	get_vector_arg_in<Point>(points, n);
	nr = n;
	nc = n;
	mat = NEW_MATRIX_SQUARE(real);
	vec = NEW_VECTOR(real);
	outer(points, mat, vec);
	put_matrix_arg<real>("--mout", mat, nr, nc);
	put_vector_arg<real>("--vout", vec, n);

	delete [] points;
	delete [] mat;
	delete [] vec;
	
    } else if (strcmp(argv[1], "product") == 0) {
	/* product seems to deviate from the paper! */
	Matrix matrix;
	Vector candidate;
	Vector solution;

	get_matrix_arg<real>("--matrix", matrix, nr, nc);
	get_vector_arg<real>("--candidate", candidate, n);
	solution = NEW_VECTOR(real);
	product(matrix, candidate, solution);
	put_vector_arg_out<real>(solution, n);

	delete [] matrix;
	delete [] candidate;
	delete [] solution;

    } else if (strcmp(argv[1], "randmat") == 0) {
	IntMatrix mat;

	seed = must_get_int_arg_value("--seed");
	nr = must_get_int_arg_value("--rows");
	nc = must_get_int_arg_value("--cols");
	mat = NEW_MATRIX_RECT(INT_TYPE);
	randmat(mat);
	put_matrix_arg_out<INT_TYPE>(mat, nr, nc);
	
	delete [] mat;

    } else if (strcmp(argv[1], "sor") == 0) {
	/* sor deviates from paper... tolerance should be a parameter */
	Matrix matrix;
	Vector target;
	Vector solution;

	get_matrix_arg<real>("--matrix", matrix, nr, nc);
	get_vector_arg<real>("--target", target, n);
	assert(nr == nc && nr == n);
	//tolerance = atof(must_get_arg_value("--tolerance"));
	solution = NEW_VECTOR(real);
	sor(matrix, target, solution);
	put_vector_arg_out<real>(solution, n);

	delete [] matrix;
	delete [] target;
	delete [] solution;

    } else if (strcmp(argv[1], "thresh") == 0) {
	IntMatrix mat;
	BoolMatrix mask;

	get_matrix_arg_in<INT_TYPE>(mat, nr, nc);
	threshPercent = atof(must_get_arg_value("--percent"));
	mask = NEW_MATRIX_RECT(bool);
	thresh(mat, mask);
	put_matrix_arg_out<bool>(mask, nr, nc);

	delete [] mat;
	delete [] mask;

    } else if (strcmp(argv[1], "vecdiff") == 0) {
	Vector v1;
	Vector v2;
	int m;
	ostream *out;
	real result;
	
	get_vector_arg<real>("--v1", v1, n);
	get_vector_arg<real>("--v2", v2, m);
	assert(m == n);
	out = get_ostream_arg("--out");
	*out << vecdiff(v1, v2) << endl;
	ostream_close(out);
	
	delete [] v1;
	delete [] v2;

    } else if (strcmp(argv[1], "winnow") == 0) {
	IntMatrix matrix;
	BoolMatrix mask;
	PointVector points;
	int mr, mc;

	get_matrix_arg<INT_TYPE>("--matrix", matrix, nr, nc);
	get_matrix_arg<bool>("--mask", mask, mr, mc);
	assert(mr == nr && mc == nc);
	n = must_get_int_arg_value("--nelts");
	points = NEW_VECTOR(Point);
	winnow(matrix, mask, points);
	put_vector_arg_out<Point>(points, n);
	
	delete [] matrix;
	delete [] mask;
	delete [] points;

    } else if (strcmp(argv[1], "echo") == 0) {
	IntMatrix matrix;
	get_matrix_arg_in<INT_TYPE>(matrix, nr, nc);
	put_matrix_arg_out<INT_TYPE>(matrix, nr,nc);
	delete [] matrix;
    } else {
	cout << "Comand not recognized" << endl;
    }
}
