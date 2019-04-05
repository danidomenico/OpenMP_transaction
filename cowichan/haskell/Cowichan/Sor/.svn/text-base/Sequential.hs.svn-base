module Cowichan.Sor.Sequential where
import Cowichan.Matrix
import Cowichan.Product.Sequential as CP

import Data.List

omega = 0.5

sor mat target tolerance =
    let
        nelts = length target
        diag = (map (\i -> index mat i i) [0..nelts-1])
        g xs xs' (x,i) =
            xs' ++ 
            [(1 - omega)*x
            + (omega / (diag !! i)) * ((target !! i)
                                   - (sum (map (\j -> xs!!j * (index mat i j))
                                               [i+1..nelts-1]))
                                   - (sum (map (\j -> xs'!!j * (index mat i j))
                                               [0..i-1])))]

        f x = foldl (g x) [] (zip x [0..])
        converge candidate =
            (CP.product mat target candidate) < tolerance
    in
      case find converge (iterate f (map (const 0) [1..nelts])) of
        Just soln -> soln
