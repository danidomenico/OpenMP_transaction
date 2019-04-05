/**
 * \mainpage Parallel Programming and Cowichan Problems
 *
 * \section intro_sec Introduction
 *
 * The Cowichan problems are implemented by various parallel programming
 * systems. Serial C++ implementation is available for comparison.
 * The Cowichan class is the base class for all C++ implementations.
 *
 * \section problems_sec The Problems
 *
 * Currently, there are 14 Cowichan problems. They are described below.
 * Cowichan class. Problems can be run separately by
 * passing the problem name as an argument on the command line. The inputs to
 * the problems are defined in cowichan_defaults.hpp. There is currently no way
 * to override the inputs without recompiling.
 *
 * \subsection mandel_sec 1. Mandelbrot Set Generation
 * This module generates the Mandelbrot Set for a specified region of the
 * complex plane.
 * \see Cowichan::mandel
 *
 * \subsection randmat_sec 2. Random Number Generation
 * This module fills a matrix with pseudo-random integers.
 * Note that, as in all problems, the output is required to be independent of
 * the number of processors used. However, the matrices generated may be
 * different in each implementation.
 * \see Cowichan::randmat
 *
 * \subsection half_sec 3. Two-Dimensional Shuffle
 * This module divides the values in a rectangular two-dimensional integer
 * matrix into two halves along one axis, shuffles them, and then repeats this
 * operation along the other axis. Values in odd-numbered locations are
 * collected at the low end of each row or column, while values in
 * even-numbered locations are moved to the high end.
 * \see Cowichan::half
 *
 * \subsection invperc_sec 4. Invasion Percolation
 * Invasion percolation models the displacement of one fluid (such as oil) by
 * another (such as water) in fractured rock. In two dimensions, this can be
 * simulated by generating an NxN grid of random numbers in the range
 * [1. . .R], and then marking the center cell of the grid as filled. In each
 * iteration, one examines the four orthogonal neighbors of all filled cells,
 * chooses the one with the lowest value (i.e. the one with the least
 * resistance to filling), and fills it in. The simulation continues until some
 * fixed percentage of cells have been filled, or until some other condition
 * (such as the presence of trapped regions) is achieved. The fractal structure
 * of the filled and unfilled regions is then examined to determine how much
 * oil could be recovered. The naive way to implement this is to repeatedly
 * scan the array; a more sophisticated, and much faster, sequential technique
 * is to maintain a priority queue of unfilled cells which are neighbors of
 * filled cells. This latter technique is similar to the list-based methods
 * used in some cellular automaton programs, and is very difficult to
 * parallelize effectively. Filling begins at the central cell of the matrix
 * (rounding down for even-sized axes).
 * \see Cowichan::invperc
 *
 * \subsection thresh_sec 5. Histogram Thresholding
 * This module performs histogram thresholding on an image. Given an integer
 * image I and a target percentage p, it constructs a binary image B such that
 * B[i,j] is set if no more than p percent of the pixels in I are brighter than
 * I[i,j]. The general idea is that an image's histogram should have 2 peaks,
 * one centered around the average foreground intensity, and one centered
 * around the average background intensity. This program attempts to set a
 * threshold between the two peaks in the histogram and select the pixels above
 * the threshold.
 * \see Cowichan::thresh
 *
 * \subsection life_sec 6. Game of Life
 * This module simulates the evolution of Conway's Game of Life, a
 * two-dimensional cellular automaton.
 * At each time step, this module must count the number of live (true)
 * neighbors of each cell, using both orthogonal and diagonal connectivity and
 * circular boundary conditions. The update rule is simple: if a cell has 3
 * live neighbors, or has 2 live neighbors and is already alive, it is alive in
 * the next generation. In any other situation, the cell becomes, or stays,
 * dead.
 * \see Cowichan::life
 *
 * \subsection winnow_sec 7. Weighted Point Selection
 * This module converts a matrix of integer values to a vector of points,
 * represented as x and y coordinates.
 * Each location where mask is true becomes a candidate point, with a weight
 * equal to the integer value in matrix at that location and x and y
 * coordinates equal to its row and column indices. These candidate points
 * are then \b sorted into increasing order by weight, and n evenly-spaced 
 * points selected to create the result vector.
 * \see Cowichan::winnow
 *
 * \subsection norm_sec 8. Point Location Normalization
 * This module normalizes point coordinates so that all points lie within
 * the unit square [0. . .1]x[0. . .1].
 * \see Cowichan::norm
 *
 * \subsection hull_sec 9. Convex Hull
 * This module takes a list of two-dimensional points and reorders them by
 * doing multiple convex hull computations. Convex hull is the boundary of the
 * minimal convex set containing a given non-empty finite set of points in the
 * plane. In other words, all points not in the convex hull are enclosed in the
 * convex hull polygon. At each step the convex hull points are taken out of
 * the input list and are put into the output list. The computation terminates
 * when there are no more points left in the input list.
 * \see Cowichan::hull
 *
 * \subsection outer_sec 10. Outer Product
 * This module turns a vector containing point positions into a dense,
 * symmetric, diagonally dominant matrix by calculating the distances between
 * each pair of points. It also constructs a real vector whose values are the
 * distance of each point from the origin.
 * \see Cowichan::outer
 *
 * \subsection gauss_sec 11. Gaussian Elimination
 * This module solves a matrix equation AX = V for a dense, symmetric,
 * diagonally dominant matrix A and an arbitrary vector non-zero V using
 * explicit reduction (matrices are required to be symmetric and diagonally
 * dominant in order to guarantee that there is a well-formed solution to the
 * equation).
 * \see Cowichan::gauss
 *
 * \subsection sor_sec 12. Successive Over-Relaxation
 * This module solves a matrix equation AX = V for a dense, symmetric,
 * diagonally dominant matrix A and an arbitrary vector non-zero V using
 * successive over-relaxation.
 * \see Cowichan::sor
 *
 * \subsection product_sec 13. Matrix-Vector Product
 * This module calculates V in AX = V where A is a matrix and X is a vector.
 * \see Cowichan::product
 *
 * \subsection vecdiff_sec 14. 1-Norm Vector Difference
 * This module calculates the 1-norm of the difference between two vectors. In
 * case the vectors are actual and computed values of some calculation the
 * result represents the magnitude of the error.
 * \see Cowichan::vecdiff
 */

/**
 * \file cowichan.hpp
 * \brief Datatypes and common routines for Cowichan programs.
 */

#ifndef __cowichan_hpp__
#define __cowichan_hpp__

#include "cowichan_defaults.hpp"

// BASIC HEADERS ============================================================//
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <climits>
#include <limits>
#include <string>
#include <cstring>
using std::numeric_limits;

// TIMING ===================================================================//

/**
 * Enables printing of test time.
 */
#define TEST_TIME

/**
 * Enables printing of sort time (in winnow).
 */
#define SORT_TIME

#if defined(WIN64) || defined(WIN32)   // Windows

#include <windows.h>
/*
 * Int types are defined in windows.h
 */

#else                // Linux

#include <sys/times.h>
#include <stdint.h>
#include <string.h>
/**
 * 64-bit unsigned int.
 */
typedef uint64_t UINT64;

/**
 * 32-bit unsigned int.
 */
typedef uint32_t UINT32;

/**
 * 64-bit signed int.
 */
typedef int64_t INT64;

/**
 * 32-bit signed int.
 */
typedef int32_t INT32;

#endif

/**
 * Get tick count.
 * \return Tick count.
 */
INT64 get_ticks ();

/**
 * Get tick frequency.
 * \return Tick frequency.
 */
INT64 get_freq ();

/**
 * Print elapsed time to std::cout.
 * \param start initial tick count.
 * \param end final tick count.
 */
void print_elapsed_time (INT64 start, INT64 end);

/**
 * Does a sort of swap-out, printing progress.
 * \param start pointer to initial tick count.
 * \param end pointeger to final tick count.
 * \param message message to print before the elapsed time.
 */
void timeInfo(INT64 *start, INT64 *end, std::string message);

// DEBUGGING FUNCTIONS ======================================================//

/**
 * \var OUTPUT_DATA
 * Enables printing of output data.
 */
//#define OUTPUT_DATA

/**
 * \var WINNOW_OUTPUT
 * Indicates that winnow weights should be printed.
 */
//#define WINNOW_OUTPUT

/**
 * Prints out of memory message and exits.
 */
void out_of_memory();

/**
 * Prints not enough points message and exits.
 */
void not_enough_points();

/**
 * Prints no cells alive message and exits.
 */
void no_cells_alive();

// BASIC TYPES ==============================================================//
#ifndef REAL_TYPE
/**
 * Use IEEE single floating-point by default.
 */
#define REAL_TYPE float
#endif

/**
 * Real type (used in matrix/vector values).
 */
typedef REAL_TYPE real;

/**
 * Integer type (used in matrix/vector values).
 */
typedef INT32 INT_TYPE;

/**
 * Integer matrix type.
 */
typedef INT_TYPE* IntMatrix;

/**
 * Boolean matrix type.
 */
typedef bool* BoolMatrix;

/**
 * Real matrix type.
 */
typedef real* RealMatrix;

/**
 * Integer vector type.
 */
typedef INT_TYPE* IntVector;

/**
 * Boolean vector type.
 */
typedef bool* BoolVector;

/**
 * Real vector type.
 */
typedef real* RealVector;

/**
 * Real matrix type (shorter name).
 */
typedef RealMatrix Matrix;

/**
 * Real vector type (shorter name).
 */
typedef RealVector Vector;

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

/**
 * Index type (signed version of size_t).
 */
typedef ptrdiff_t index_t;

// STATIC DEFINITIONS =======================================================//
/**
 * Maximum integer value in matrices/vectors.
 */
#define MAXIMUM_INT numeric_limits<INT_TYPE>::max()

/**
 * Minimum integer value in matrices/vectors.
 */
#define MINIMUM_INT numeric_limits<INT_TYPE>::min()

/**
 * Maximum real value in matrices/vectors.
 */
#define MAXIMUM_REAL numeric_limits<real>::min()

/**
 * Minimum real value in matrices/vectors.
 */
#define MINIMUM_REAL numeric_limits<real>::min()

/**
 * Real infinity value.
 */
#define INFINITY_REAL numeric_limits<real>::infinity()


void read_const(std::istream&, std::string);
// POINT TYPE ===============================================================//
/**
 * \brief Two-dimensional point.
 */
class Point {
public:

  /**
   * x coordinate.
   */
  real x;
  
  /**
   * y coordinate.
   */
  real y;

  friend std::ostream &operator<<(std::ostream &out, const Point &p) {
      out << "(" << p.x << ", " << p.y << ")";
      return out;
  }

  friend std::istream &operator>>(std::istream &in, Point &p) {
      read_const(in, "(");
      in >> p.x;
      read_const(in, ",");
      in >> p.y;
      read_const(in, ")");
      return in;
  }
  /**
   * Construct a point.
   * \param x x-coordinate.
   * \param y y-coordinate.
   */
  Point(real x, real y): x(x), y(y) { }

  /**
   * Default constructor.
   */
  Point(): x(0.0), y(0.0) { }

  /**
   * Copy constructor.
   * \param other point to copy.
   */
  Point(const Point& other): x(other.x), y(other.y) {}
  
  /**
   * Calculate euclidean distance between two points.
   * \param p1 first point.
   * \param p2 second point.
   * \return The distance between p1 and p2.
   */
  static inline real distance(const Point& p1, const Point& p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
  }

  /**
   * Compute the cross product of the vectors (l1,l2) and (l1,p).
   * \param l1 first point.
   * \param l2 second point.
   * \param p third point.
   * \return The cross product.
   */
  static inline real cross (const Point& l1, const Point& l2, const Point& p) {
    return (l1.x - p.x) * (l2.y - p.y) - (l1.y - p.y) * (l2.x - p.x);
  }
};

/**
 * Vector of points type.
 */
typedef Point* PointVector;
 
// WEIGHTED POINT TYPE (FOR WINNOW) =========================================//
/**
 * \brief Two-dimensional point with an integer weight.
 */
class WeightedPoint {
public:

  /**
   * Two-dimensional point.
   */
  Point point;

  /**
   * Weight.
   */
  INT_TYPE weight;
  
  /**
   * Construct a weighted point.
   * \param point point to copy.
   * \param weight weight.
   */
  WeightedPoint(Point point, INT_TYPE weight): point(point), weight(weight) { }

  /**
   * Default constructor.
   */
  WeightedPoint(): point(0.0, 0.0), weight(0) { }

  /**
   * Construct a weighted point.
   * \param x x-coordinate.
   * \param y y-coordinate.
   * \param weight weight.
   */
  WeightedPoint(real x, real y, INT_TYPE weight): point(x, y), weight(weight) { }  

  /**
   * Less than comparison by weight.
   * \param rhs right hand side weighted point.
   * \return Whether lhs < rhs.
   */
  inline bool operator<(const WeightedPoint& rhs) const {
    return (weight < rhs.weight);
  }

  /**
   * Less than or equal comparison by weight.
   * \param rhs right hand side weighted point.
   * \return Whether lhs <= rhs.
   */
  inline bool operator<=(const WeightedPoint& rhs) const {
    return (weight <= rhs.weight);
  }

};

/**
 * Vector of weighted points type.
 */
typedef WeightedPoint* WeightedPointVector;

// UTILITY FUNCTIONS ========================================================//

/**
 * Access a rectangular matrix at row/col from a class using member variable nc
 * as the number of columns.
 */
#define MATRIX_RECT(mtrx,row,col)  (mtrx)[(row)*this->nc + col]

/**
 * Access a rectangular matrix at row/col using nc as the number of columns.
 */
#define MATRIX_RECT_NC(mtrx,row,col,nc)  (mtrx)[(row)*(nc) + col]

/**
 * Access a square matrix at row/col from a class using member variable n
 * as the matrix size.
 */
#define MATRIX_SQUARE(mtrx,row,col)  (mtrx)[(row)*this->n + col]

/**
 * Access a square matrix at row/col using n as the matrix size.
 */
#define MATRIX_SQUARE_N(mtrx,row,col,n)  (mtrx)[(row)*(n) + col]

/**
 * Same as MATRIX_SQUARE (shorter name)
 */
#define MATRIX MATRIX_SQUARE

/**
 * Access row of a vector.
 */
#define VECTOR(vect,row) (vect)[row]

/**
 * Access a square matrix diagonal element v from a class using member variable
 * n as the matrix size.
 */
#define DIAG(mtrx,v) (mtrx)[(v)*this->n + v]

/**
 * Access a square matrix diagonal element v using n as the matrix size.
 */
#define DIAG_SZ(mtrx,v,n) (mtrx)[(v)*(n) + v]

/**
 * Allocate a new rectangular matrix from a class using member variable nr as
 * the number of rows and member variable nc as the number of columns.
 */
#define NEW_MATRIX_RECT(__type) (new __type[this->nr * this->nc])

/**
 * Allocate a new square matrix from a class using member variable n as the
 * matrix size.
 */
#define NEW_MATRIX_SQUARE(__type) (new __type[this->n * this->n])

/**
 * Allocate a new vector of size __num.
 */
#define NEW_VECTOR_SZ(__type,__num) (new __type[__num])

/**
 * Allocate a new vector from a class using member variable n as the size of
 * the vector.
 */
#define NEW_VECTOR(__type) NEW_VECTOR_SZ(__type, this->n)

/**
 * Returns a pseudorandom number ~ U[mean - range, mean + range].
 * \param mean mean.
 * \param range range.
 * \return The pseudorandom number.
 */
real uniform(real mean, real range);

// COWICHAN DEFINITIONS =====================================================//
/**
 * \brief Base class for all C++ implementations.
 *
 * The abstract class containing the inputs, definitions, and some debugging
 * methods for the Cowichan problems.
 */
class Cowichan {
protected:

  // constants

  /**
   * Name for chain.
   */
  static const char* CHAIN;

  /**
   * Name for mandel.
   */
  static const char* MANDEL;

  /**
   * Name for randmat.
   */
  static const char* RANDMAT;

  /**
   * Name for half.
   */
  static const char* HALF;

  /**
   * Name for invperc.
   */
  static const char* INVPERC;

  /**
   * Name for thresh.
   */
  static const char* THRESH;

  /**
   * Name for life.
   */
  static const char* LIFE;

  /**
   * Name for winnow.
   */
  static const char* WINNOW;

  /**
   * Name for norm.
   */
  static const char* NORM;

  /**
   * Name for hull.
   */
  static const char* HULL;

  /**
   * Name for outer.
   */
  static const char* OUTER;

  /**
   * Name for gauss.
   */
  static const char* GAUSS;

  /**
   * Name for sor.
   */
  static const char* SOR;

  /**
   * Name for product.
   */
  static const char* PRODUCT;

  /**
   * Name for vecdiff.
   */
  static const char* VECDIFF;

protected:

  /**
   * Number of rows for rectangular matrices.
   */
  index_t nr;

  /**
   * Number of columns for rectangular matrices.
   */
  index_t nc;

  /**
   * Number of rows/columns for square matrices.
   */
  index_t n;

  /**
   * Number of iterations.
   */
  index_t lifeIterations;

  /**
   * x-coordinate of the lower left corner.
   */
  real mandelX0;
  
  /**
   * y-coordinate of the lower left corner.
   */
  real mandelY0;
  
  /**
   * Extent of the region along the x axis.
   */
  real mandelDx;
  
  /**
   * Extent of the region along the y axis.
   */
  real mandelDy;

  /**
   * Thresholding percentage.
   */
  real threshPercent;

  /**
   * Number of cells to fill.
   */
  index_t invpercNFill;

  /**
   * Seed value for simple random number generator.
   */
  INT_TYPE seed;

protected: // individual problems

  /**
   * For description see \ref mandel_sec
   * \param matrix matrix to fill.
   */
  virtual void mandel(IntMatrix matrix) = 0;

  /**
   * For description see \ref randmat_sec
   * \param matrix matrix to fill.
   */
  virtual void randmat(IntMatrix matrix) = 0;

  /**
   * For description see \ref half_sec
   * \param matrixIn matrix to shuffle.
   * \param matrixOut shuffled matrix.
   */
  virtual void half(IntMatrix matrixIn, IntMatrix matrixOut) = 0;

  /**
   * For description see \ref invperc_sec
   * \param matrix filling resistance matrix.
   * \param mask filled cells.
   */
  virtual void invperc(IntMatrix matrix, BoolMatrix mask) = 0;

  /**
   * For description see \ref thresh_sec
   * \param matrix image.
   * \param mask image after thresholding.
   */
  virtual void thresh(IntMatrix matrix, BoolMatrix mask) = 0;

  /**
   * For description see \ref life_sec
   * \param matrixIn initial world.
   * \param matrixOut final world.
   */
  virtual void life(BoolMatrix matrixIn, BoolMatrix matrixOut) = 0;

  /**
   * For description see \ref winnow_sec
   * \param matrix integer matrix.
   * \param mask boolean matrix.
   * \param points evenly selected points.
   */
  virtual void winnow(IntMatrix matrix, BoolMatrix mask, PointVector points) = 0;

  /**
   * For description see \ref norm_sec
   * \param pointsIn points to normalize.
   * \param pointsOut normalized points.
   */
  virtual void norm(PointVector pointsIn, PointVector pointsOut) = 0;

  /**
   * For description see \ref hull_sec
   * \param pointsIn points to run convex hull on.
   * \param pointsOut reordered points from pointsIn.
   */
  virtual void hull(PointVector pointsIn, PointVector pointsOut) = 0;

  /**
   * For description see \ref outer_sec
   * \param points vector of points.
   * \param matrix resulting distance matrix.
   * \param vector resulting distance vector.
   */
  virtual void outer(PointVector points, Matrix matrix, Vector vector) = 0;

  /**
   * For description see \ref gauss_sec
   * \param matrix matrix A in AX = V.
   * \param target vector V in AX = V.
   * \param solution vector X in AX = V.
   */
  virtual void gauss(Matrix matrix, Vector target, Vector solution) = 0;

  /**
   * For description see \ref sor_sec
   * \param matrix matrix A in AX = V.
   * \param target vector V in AX = V.
   * \param solution vector X in AX = V.
   */
  virtual void sor(Matrix matrix, Vector target, Vector solution) = 0;

  /**
   * For description see \ref product_sec
   * \param matrix matrix A in AX = V.
   * \param candidate vector X in AX = V.
   * \param solution vector V in AX = V.
   */
  virtual void product(Matrix matrix, Vector candidate, Vector solution) = 0;

  /**
   * For description see \ref vecdiff_sec
   * \param actual first vector.
   * \param computed second vector.
   */
  virtual real vecdiff(Vector actual, Vector computed) = 0;

private:

  /**
   * Runs the cowichan problem set, chained together.
   * The order in the chain is:
   * <OL>
   * <LI>Cowichan::randmat or Cowichan::mandel</LI>
   * <LI>Cowichan::half</LI>
   * <LI>Cowichan::invperc or Cowichan::thresh</LI>
   * <LI>Cowichan::life</LI>
   * <LI>Cowichan::winnow</LI>
   * <LI>Cowichan::norm</LI>
   * <LI>Cowichan::hull</LI>
   * <LI>Cowichan::outer</LI>
   * <LI>Cowichan::gauss</LI>
   * <LI>Cowichan::sor</LI>
   * <LI>Cowichan::product (for gauss)</LI>
   * <LI>Cowichan::product (for sor)</LI>
   * <LI>Cowichan::vecdiff</LI>
   * </OL>
   * \param use_randmat in step 1 use: randmat (if true) or mandel (if false).
   * \param use_thresh in step 3 use: thresh (if true) or invperc (if false).
   */
  void chain(bool use_randmat, bool use_thresh);

public:

  /**
   * DEBUGGING FUNCTION: Print a rectangular matrix on std::cout.
   * \param matrix matrix to print.
   */
#ifdef OUTPUT_DATA
  template <typename T>
  void print_rect_matrix(T* matrix)
  {
    index_t r, c;

    for (r = 0; r < nr; r++) {
      for (c = 0; c < nc; c++) {
        std::cout << MATRIX_RECT(matrix, r, c) << "\t";
      }
      std::cout << "\n";
    }
    std::cout << "\n";
  }
#else
  template <typename T>
  void print_rect_matrix(T* /* matrix */) { }
#endif


  /**
   * DEBUGGING FUNCTION: Print a square matrix on std::cout.
   * \param matrix matrix to print.
   */
#ifdef OUTPUT_DATA
  template <typename T>
  void print_square_matrix(T* matrix)
  {
    index_t r, c;

    for (r = 0; r < n; r++) {
      for (c = 0; c < n; c++) {
        std::cout << MATRIX_SQUARE(matrix, r, c) << "\t";
      }
      std::cout << "\n";
    }
    std::cout << "\n";
  }
#else
  template <typename T>
  void print_square_matrix(T* /* matrix */) { }
#endif

  /**
   * DEBUGGING FUNCTION: Print a vector on std::cout.
   * \param vector vector to print.
   */
#ifdef OUTPUT_DATA
  template <typename T>
  void print_vector(T* vector)
  {
    index_t r;

    for (r = 0; r < n; r++) {
      std::cout << VECTOR(vector, r) << "\n";
    }
    std::cout << "\n";
  }
#else
  template <typename T>
  void print_vector(T* /* vector */) { }
#endif

  /**
   * DEBUGGING FUNCTION: Print a point vector.
   * \param points vector of points to print.
   */
  void print_vector(PointVector points);

public:
  void main(int, char **, bool, bool);

};

#endif

