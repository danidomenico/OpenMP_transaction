module Cowichan.Half.Sequential where
import Cowichan.Matrix

half mat =
    let
        (nrows, ncols) = dim mat
        halfCols = (ncols+1) `quot` 2
        halfRows = (nrows+1) `quot` 2
        shuffRows m r c = if r < halfRows
                          then index m (2*r) c
                          else index m (2*(r - halfRows) + 1) c
        shuffCols m r c = if c < halfCols
                          then index m r (2*c)
                          else index m r (2*(c - halfCols) + 1)

    in
      matrix (shuffRows (matrix (shuffCols mat) nrows ncols)) nrows ncols
