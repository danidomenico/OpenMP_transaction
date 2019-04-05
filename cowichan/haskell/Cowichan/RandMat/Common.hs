module Cowichan.RandMat.Common where

import System.Random
import Control.Monad.State
import System.IO.Unsafe
import Data.Char

rand :: (Random r) => State StdGen r
rand = do
    gen <- get
    let (n, gen') = random gen
    put gen'
    return n

nextSeed :: Int -> Int
nextSeed seed =
    fst (random (mkStdGen seed))

charToInt string =
    foldr (\x y -> 256 * y + (ord x)) 0 string

defaultSeed :: Int
defaultSeed = charToInt $ take 4
              (System.IO.Unsafe.unsafePerformIO (readFile "/dev/urandom"))

