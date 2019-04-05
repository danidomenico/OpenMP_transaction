/**
 * \file cowichan_tbb/life.cpp
 * \brief TBB life implementation.
 * \see CowichanTBB::life
 */

#include "cowichan_tbb.hpp"

namespace cowichan_tbb
{

/**
 * \brief Ping-pong solution to game of life.
 *
 * This class does the game of life, and facilitates a ping-pong memory model.
 */
class GameOfLife {

private:

  /**
   * First matrix (read from).
   */
  BoolMatrix _first;
  
  /**
   * Second matrix (write to).
   */
  BoolMatrix _second;

  /**
   * Number of rows in matrices.
   */
  index_t nr;
  
  /**
   * Number of column in matrices.
   */
  index_t nc;

  /**
   * Number of alive cells.
   */
  index_t aliveCount;

private:

  /**
   * Calculate number of peers.
   * \param x x-coordinate.
   * \param y y-coordinate.
   * \return Number of peers.
   */
  index_t sumNeighbours(index_t x, index_t y) const {

    index_t peers = 0;

    // calculate possible neighbour positions
    bool l = (x > 0);
    bool r = (x < (nc - 1));
    bool u = (y > 0);
    bool d = (y < (nr - 1));    

    // calculate no. of neighbours
    if (l &&       MATRIX_RECT(_first, y    , x - 1)) ++peers;
    if (l && u &&  MATRIX_RECT(_first, y - 1, x - 1)) ++peers;
    if (u &&       MATRIX_RECT(_first, y - 1, x    )) ++peers;
    if (r && u &&  MATRIX_RECT(_first, y - 1, x + 1)) ++peers;
    if (r &&       MATRIX_RECT(_first, y    , x + 1)) ++peers;
    if (r && d &&  MATRIX_RECT(_first, y + 1, x + 1)) ++peers;
    if (d &&       MATRIX_RECT(_first, y + 1, x    )) ++peers;
    if (l && d &&  MATRIX_RECT(_first, y + 1, x - 1)) ++peers;
    
    return peers;
    
  }

public:

  /**
   * Check if there are alive cells.
   * \return whether alive.
   */
  bool isAlive()
  {
    return aliveCount > 0;
  }
  
  /**
   * Construct a game of life object.
   * \param first matrix to read from initially.
   * \param second matrix to write to initially.
   * \param nr number of rows in matrices.
   * \param nc number of columns in matrices.
   */
  GameOfLife(BoolMatrix first, BoolMatrix second, index_t nr, index_t nc):
    _first(first), _second(second), nr(nr), nc(nc), aliveCount(0) { }

  /**
   * Swap the matrices.
   */
  void swap() {
    BoolMatrix temp = _first;
    _first = _second;
    _second = temp;
  }

  /**
   * Performs the game of life operation over the given range.
   * \param range row/column range.
   */
  void operator()(const Range2D& range) {
    
    BoolMatrix first = _first;
    BoolMatrix second = _second;
    
    const Range& rows = range.rows();
    const Range& cols = range.cols();
    
    for (index_t y = rows.begin(); y != rows.end(); ++y) {
      for (index_t x = cols.begin(); x != cols.end(); ++x) {
        
        index_t peers = sumNeighbours(x, y);
        if (peers < 2 || peers > 3) {
          MATRIX_RECT(second, y, x) = false; // hunger/overcrowding
        } else if (peers == 3) {
          MATRIX_RECT(second, y, x) = true; // breeding
        } else {
          MATRIX_RECT(second, y, x) = MATRIX_RECT(first, y, x); // nothing
        }
        
        if (MATRIX_RECT(second, y, x)) {
          aliveCount++;
        }
      }
    }
    
  }

  /**
   * Splitting (TBB) constructor.
   * \param other object to split.
   */
  GameOfLife(GameOfLife& other, split) : _first(other._first),
      _second(other._second), nr(other.nr), nc(other.nc), aliveCount(0) { }

  /**
   * Joiner (TBB).
   * \param other object to join.
   */
  void join(const GameOfLife& other) {
    aliveCount += other.aliveCount;
  }

};

}

/*****************************************************************************/

void CowichanTBB::life(BoolMatrix input, BoolMatrix output) {
  GameOfLife game(input, output, nr, nc);

  for (index_t i = 0; i < LIFE_ITERATIONS; ++i) {

    // update CA simulation
    parallel_reduce(Range2D(0, nr, 0, nc), game, auto_partitioner());

    // check if there are alive cells
    if (!game.isAlive()) {
      no_cells_alive();
    }

    // swap arrays (ping-pong approach)
    game.swap();

  }

  // final result is in input - copy to output
  if (LIFE_ITERATIONS % 2 == 0) {
    for (index_t r = 0; r < nr; r++) {
      for (index_t c = 0; c < nc; c++) {
        MATRIX_RECT(output, r, c) = MATRIX_RECT(input, r, c);
      }
    }
  }
}

