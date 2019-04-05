module Cowichan.Norm.Sequential where
import Cowichan.Matrix

norm :: [(Float,Float)] -> [(Float,Float)]
norm points =
    let
        (xvals, yvals) = unzip points
        minmax xs = foldl (\(mn, mx) x -> if x < mn
                                          then (x, mx)
                                          else if x > mx
                                               then (mn, x)
                                               else (mn, mx))
                          (head xs, head xs)
                          (tail xs)
        (xmin, xmax) = minmax xvals
        (ymin, ymax) = minmax yvals
        normalize (x, y) = ((x - xmin) / (xmax - xmin),
                            (y - ymin) / (ymax - ymin))
    in
      map normalize points




