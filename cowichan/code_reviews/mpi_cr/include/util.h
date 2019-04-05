/**
 * Utility functions
 *
 * \file util.h
 * \author Andrew Borzenko
 * \date 01-26-09
 */

void
fail(
  char	      * caller,			/* calling function */
  char	      * descrip,		/* error description */
  ...					/* other things to print */
);

void print_matrix (bool2D* matrix, int nr, int nc);
void print_matrix (int2D* matrix, int nr, int nc);
void print_matrix (real2D* matrix, int nr, int nc);
void print_matrix (bool1DX* matrix, int nr, int nc);
void print_matrix (int1DX* matrix, int nr, int nc);
void print_matrix (real1DX* matrix, int nr, int nc);
void print_vector (bool1D* vector, int nr);
void print_vector (int1D* vector, int nr);
void print_vector (real1D* vector, int nr);
void print_vector (pt1D* vector, int nr);
void print_points (pt1D* vector, int nr, int limit);

bool get_block_rows_mpi (mpi::communicator world, int lo, int hi,
                         int* start, int* end);
bool get_block_rows_mpi (mpi::communicator world, int lo, int hi,
                         int* start, int* end, int rank);
int get_block_rank_mpi (mpi::communicator world, int lo, int hi,
                        int row);
bool get_cyclic_rows_mpi (mpi::communicator world, int lo, int hi,
                         int* start, int* end, int* stride);
int get_cyclic_rank_mpi (mpi::communicator world, int lo, int hi,
                        int element);

void randStateInit (unsigned int seed,       /* RNG seed */
                    int width,      /* number of participants */
                    unsigned int	* state,    /* per-thread state vector */
                    unsigned int	* aPrime,   /* new multiplicative */
                    unsigned int	* cPrime);  /* new additive value */

void redPt1DPos (pt1D*		vec,			/* vector of points */
                 int		n,			/* number of points */
                 pt	      * ptMin,			/* minimum location */
                 pt	      * ptMax);			/* maximum location */

void ptSort(pt	      * ptVec,			/* points to sort */
            int		len);			/* length of vectors */

int ptCmp(pt	      * left,			/* left point */
          pt	      * right);			/* right point */

void intSort(int  * vec,			/* to sort */
             int		len			/* length */
);

real ptDist(pt	      * left,			/* left point */
            pt	      * right);			/* right point */

real ptMag(pt	      * p);			/* point */

INT64 get_ticks (); // tick count
INT64 get_freq (); // tick frequency

void print_elapsed_time (INT64 start, INT64 end); // print elapsed time
