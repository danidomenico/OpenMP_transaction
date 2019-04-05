module Cowichan.Life.Sequential where
import Cowichan.Matrix

-- includes self!
neighborhood mat x y =
    let
        (rows, cols) = dim mat
        mmod x n = 
            let
                m = x `mod` n
            in
              if m < 0 then m + n else m
    in
      [ index mat (x `mmod` rows) (y `mmod` cols) | x <- [x-1, x, x+1],
                                                    y <- [y-1, y, y+1] ]

life mat numgen =
    let
        (rows, cols) = dim mat
        liveNeighborhood mat x y = length $ filter id (neighborhood mat x y)
        alive mat x y = (liveNeighborhood mat x y) >= 3
        step mat = matrix (alive mat) rows cols
    in
      (iterate step mat) !! numgen
