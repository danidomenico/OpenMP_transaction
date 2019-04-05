{-# OPTIONS -XNoMonomorphismRestriction #-}
module Cowichan.Extra where

import Cowichan.RandMat.Sequential
import Cowichan.RandMat.RandList
import Cowichan.RandMat.Common
import System.Random
import Control.Monad.Reader
import Cowichan.FrontEnd
import Cowichan.Matrix


instance (Random a, Random b) => Random (a, b) where
    random gen = let (a, gen') = random gen in
                 let (b, gen'') = random gen' in
                 ((a,b), gen'')
    randomR ((la, lb), (ha, hb)) gen =
        let (a, gen') = randomR (la, ha) gen in
        let (b, gen'') = randomR (lb, hb) gen' in
        ((a,b), gen'')

floatR :: Float -> Float
floatR f = (f - 0.5) * 2^8

-- extra
doRandVec = do
  size <- mustGetArgValue "--size"
  seed <- getSeed
  typ <- processArg "--type" id "float"
  let frange = map floatR
  let prange = map (\(x,y) -> (floatR x, floatR y))
  case typ of
    "int" -> putListArgOut ((randlist size seed) :: [Int])
    "float" -> putListArgOut (frange (randlist size seed))
    "points" -> putListArgOut (prange (randlist size seed))

doRandSDD = do
  size <- mustGetArgValue "--size"
  seed <- getSeed
  let mat = (randmat size size seed) :: Matrix Float
  let sym = matrix (\x y -> if x < y then index mat x y else index mat y x)
                   size
                   size
  let seed2 = nextSeed seed
  let negate = map (\x -> if x then 1 else -1) (randlist size seed2)
  let seed3 = nextSeed seed2
  let aug = zipWith (*) (map (1+) (randlist size seed3)) negate
  let diag = zipWith (*) (map (sum . map abs) (rows sym)) aug
  let sdd = matrix (\x y -> if x == y
                            then diag !! x
                            else index sym x y)
                   size
                   size
  putMatrixArgOut sdd

floatEq :: Float -> Float -> Float -> Bool
floatEq tolerance f1 f2 =
    abs (f1 - f2) < tolerance

pointEq tolerance (x1, y1) (x2, y2) =
    floatEq tolerance x1 x2
    && floatEq tolerance y1 y2
      
getMatrices = do
  m1 <- getMatrixArg "--m1"
  m2 <- getMatrixArg "--m2" 
  return (m1, m2)

doMatEqBool = do
  (m1, m2) <- getMatrices
  return $ m1 == (m2 :: Matrix Bool)

doMatEqInt = do
  (m1, m2) <- getMatrices
  return $ m1 == (m2 :: Matrix Int)

doMatEqFloat = do
  (m1, m2) <- getMatrices
  tolerance <- getArgValue "--tolerance" 0.01
  let (nr,nc) = dim m1
  return $ dim m1 == dim m2
           && (all (\(x,y) -> floatEq tolerance (index m1 x y) (index m2 x y))
                   [ (x,y) | x <- [0..nr-1], y <- [0..nc-1] ])

doMatEq = do
  typ <- processArg "--type" id "float"
  (case typ of
     "int" -> doMatEqInt
     "bool" -> doMatEqBool
     "float" -> doMatEqFloat)
  >>= (putStringArgOut . show)

getVectors = do
  v1 <- getListArg "--v1"
  v2 <- getListArg "--v2" 
  return (v1, v2)

doVecEqInt = do
  (v1,v2) <- getVectors
  return $ v1 == (v2 :: [Int])

doVecEqFloat = do
  (v1,v2) <- getVectors
  tolerance <- getArgValue "--tolerance" 0.01
  return $ length v1 == length v2
           && any id (zipWith (floatEq tolerance) v1 v2)

doVecEqPoint = do
  (v1,v2) <- getVectors
  tolerance <- getArgValue "--tolerance" 0.01
  return $ length v1 == length v2
           && any id (zipWith (pointEq tolerance) v1 v2)

doVecEq = do
  typ <- processArg "--type" id "float"
  (case typ of
     "int" -> doVecEqInt
     "point" -> doVecEqPoint
     "float" -> doVecEqFloat)
  >>= (putStringArgOut . show)
