module Cowichan.RandMat.RandList where

import Cowichan.RandMat.Common
import Cowichan.Matrix
import Control.Monad.State
import System.Random

randlist size seed =
    fst (runState (mapM (const $ rand) [1..size]) (mkStdGen seed))
