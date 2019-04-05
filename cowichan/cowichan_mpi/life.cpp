/**
 * \file cowichan_mpi/life.cpp
 * \brief MPI Conway's Game of Life implementation.
 * \see CowichanMPI::life
 */

#include "cowichan_mpi.hpp"
namespace cowichan_mpi {

  /**
   * Calculate number of peers.
   * \param first world matrix.
   * \param r row.
   * \param c column.
   * \param nr number of rows in the matrix.
   * \param nc number of columns in the matrix.
   * \return The number of peers.
   */
  index_t sumNeighbours(BoolMatrix first, index_t r, index_t c, index_t nr,
    index_t nc);
}

void CowichanMPI::life(BoolMatrix matrixIn, BoolMatrix matrixOut)
{
  int i;                   // iteration index
  index_t r;               // row index
  int alive;               // number alive
  index_t lo, hi;          // work controls
  index_t rlo, rhi;        // for broadcast
  bool work;               // useful work to do?
  int is_alive = 1;        // some cells still alive?
  BoolMatrix m_tmp;        // tmp pointer

  // work
  work = get_block (world, 0, nr, &lo, &hi);
  for (i = 0; (i < lifeIterations) && (is_alive > 0); i++) {
  
    // reset alive neighbour count
    alive = 0;

    // count neighbours and fill new matrix
    if (work) {
      for (r = lo; r < hi; r++) {
        for (index_t c = 0; c < nc; ++c) {
          index_t count = sumNeighbours(matrixIn, r, c, nr, nc);
          if (count == 3 || ((count == 2) && MATRIX_RECT(matrixIn, r, c))) {
          MATRIX_RECT(matrixOut, r, c) = true;
          ++alive;
        } else {
          MATRIX_RECT(matrixOut, r, c) = false;
        }
        }
      }
    }
    
    // broadcast matrix
    for (r = 0; r < world.size (); r++) {
      if (get_block (world, 0, nr, &rlo, &rhi, r)) {
        broadcast (world, &MATRIX_RECT(matrixOut, rlo, 0),
            (int)((rhi - rlo) * nc), (int)r);
      }
    }
    
    // is_alive is maximum of local alive's
    all_reduce (world, alive, is_alive, mpi::maximum<int>());

  // swap matrices (ping-pong)
    m_tmp = matrixIn;
    matrixIn = matrixOut;
    matrixOut = m_tmp;
    
  }
}

/*****************************************************************************/

namespace cowichan_mpi {

  index_t sumNeighbours(BoolMatrix first, index_t r, index_t c, index_t nr,
    index_t nc) {

    index_t peers = 0;

    // calculate possible neighbour positions
    bool ll = (c > 0);
    bool rr = (c < (nc - 1));
    bool uu = (r > 0);
    bool dd = (r < (nr - 1));

    // calculate no. of neighbours
    if (ll &&       MATRIX_RECT_NC(first, r    , c - 1, nc)) ++peers;
    if (ll && uu && MATRIX_RECT_NC(first, r - 1, c - 1, nc)) ++peers;
    if (uu &&       MATRIX_RECT_NC(first, r - 1, c    , nc)) ++peers;
    if (rr && uu && MATRIX_RECT_NC(first, r - 1, c + 1, nc)) ++peers;
    if (rr &&       MATRIX_RECT_NC(first, r    , c + 1, nc)) ++peers;
    if (rr && dd && MATRIX_RECT_NC(first, r + 1, c + 1, nc)) ++peers;
    if (dd &&       MATRIX_RECT_NC(first, r + 1, c    , nc)) ++peers;
    if (ll && dd && MATRIX_RECT_NC(first, r + 1, c - 1, nc)) ++peers;

    return peers;

  }

}
