module Cowichan.Gauss.Sequential where
import Cowichan.Matrix

scMul s v = map (* s) v
vSub = zipWith (-)
cancel i v1 v2 = v2 `vSub` (scMul ((v2 !! i) / (v1 !! i)) v1)

gauss :: Matrix Float -> [Float] -> [Float]
gauss mat target =
    let
        nelts = length target

        -- augment matrix with target column
        augMat = concatH mat (matrix (\_ y -> target !! y) nelts 1)

        result = augMat:(map (\i -> matrix (elim i) nelts (nelts+1))
                                 [0..nelts - 1])
        elim i x y =
            let prev = result !! i in
            if x <= i
            then index prev x y
            else (cancel i ((rows prev) !! i) ((rows prev) !! x)) !! y

{-
        elim prev i x y =
            if x <= i
            then index prev x y
            else (cancel i ((rows prev) !! i) ((rows prev) !! x)) !! y
-}
        -- back substitution
        backsub (i, row) env =
            let
                sub = zipWith (*) (drop (i + 1) row) env
                sol = (foldl (-) (last row) sub) / (row !! i)
            in
              sol:env
    in
      foldr backsub [] (zip [0..] (rows (last result)))
