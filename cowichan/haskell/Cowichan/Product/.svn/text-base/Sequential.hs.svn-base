module Cowichan.Product.Sequential where
import Cowichan.Matrix
import Cowichan.VecDiff.Sequential

dot v1 v2 = sum $ zipWith (*) v1 v2

vprod vec mat = map (dot vec) (rows mat)

product mat actual candidate =
    vecdiff actual (vprod candidate mat)
