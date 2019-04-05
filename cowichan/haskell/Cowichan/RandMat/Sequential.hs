module Cowichan.RandMat.Sequential where

import Cowichan.RandMat.Common
import Cowichan.Matrix
import Control.Monad.State
import System.Random

randmat nrows ncols seed =
    fst (runState (matrixM (\x y -> rand) nrows ncols) (mkStdGen seed))

