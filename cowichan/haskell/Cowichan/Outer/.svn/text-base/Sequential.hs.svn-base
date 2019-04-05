module Cowichan.Outer.Sequential where
import Cowichan.Matrix

outer :: [(Float,Float)] -> (Matrix Float, [Float])
outer points =
    let
        nelts = length points
        dist (x1, y1) (x2, y2) = sqrt $ (x2 - x1)^2 + (y2 - y1)^2

        dMat = matrix (\r c -> dist (points !! r) (points !! c)) nelts nelts
        diag = (matFoldl max 0 dMat) * (fromIntegral nelts)
        mat = matrix (\r c -> if r == c then diag else index dMat r c)
                     nelts
                     nelts

        vec = map (\(x, y) -> sqrt $ x^2 + y^2) points
    in
      (mat, vec) 
