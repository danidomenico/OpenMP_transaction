module Cowichan.Thresh.Sequential where
import Cowichan.Matrix
import Data.Map
import Debug.Trace

thresh :: Matrix Int -> Float -> Matrix Bool
thresh mat percent =
    let
        (nrows, ncols) = dim mat
        incrMaybe (Just n) = Just (n+1)
        incrMaybe Nothing  = Just 1

        incr map x = alter incrMaybe x map
        hist =  reverse $ toAscList $ matFoldl incr empty mat
        retain = floor (percent * (fromIntegral (nrows * ncols)))

        threshold [] ret = -1 -- ack
        threshold ((val, count):hs) ret = 
            if count > ret
            then val
            else threshold hs (ret - count)
    in
      fmap (> (threshold hist retain)) mat
