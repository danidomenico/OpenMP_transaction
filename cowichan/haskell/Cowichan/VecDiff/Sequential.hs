module Cowichan.VecDiff.Sequential where

vecdiff :: [Float] -> [Float] -> Float
vecdiff v1 v2 = maximum (map abs (zipWith (-) v1 v2))
