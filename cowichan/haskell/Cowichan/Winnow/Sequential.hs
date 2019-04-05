module Cowichan.Winnow.Sequential where
import Cowichan.Matrix
import Data.List

winnow mat mask nelts =
    let 
        (nrows, ncols) = dim mat
        candidates = [ (index mat r c, r, c) | r <- [0..nrows-1],
                                               c <- [0..ncols-1],
                                               index mask r c ]
        candidates' = zip (sort candidates) [0..]
        takeEvery n = foldr (\(x, idx) xs -> if idx `mod` n == 0
                                             then x:xs
                                             else xs) []
    in
      map (\(w,x,y) -> (x,y))
          (takeEvery (length candidates' `quot` nelts) candidates')
