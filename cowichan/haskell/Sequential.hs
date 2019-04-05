module Main where

import Cowichan.Gauss.Sequential
import Cowichan.Half.Sequential
import Cowichan.Hull.Sequential
import Cowichan.InvPerc.Sequential
import Cowichan.Life.Sequential
import Cowichan.Mandel.Sequential
import Cowichan.Norm.Sequential
import Cowichan.Outer.Sequential
import Cowichan.Product.Sequential as CP
import Cowichan.RandMat.Sequential
import Cowichan.Sor.Sequential
import Cowichan.Thresh.Sequential
import Cowichan.VecDiff.Sequential
import Cowichan.Winnow.Sequential

import Cowichan.Extra
import Cowichan.FrontEnd
import Cowichan.Matrix

import System
import Control.Monad.Reader

doGauss = do
  mat <- getMatrixArg "--matrix"
  target <- getListArg "--target"
  putListArgOut (gauss mat target)

doHalf = do
  mat <- getMatrixArgIn
  putMatrixArgOut (half (mat :: Matrix Int))

doHull = do
  points <- getListArgIn
  putListArgOut (hull points)

doInvPerc = do
  mat <- getMatrixArgIn
  fill <- mustGetArgValue "--fill"
  putMatrixArgOut (invperc mat fill)

doLife = do
  mat <- getMatrixArgIn
  numgens <- mustGetArgValue "--numgens"
  putMatrixArgOut (life mat numgens)

doMandel = do
  x <- mustGetArgValue "--x"
  y <- mustGetArgValue "--y"
  dx <- mustGetArgValue "--dx"
  dy <- mustGetArgValue "--dy"
  rows <- mustGetArgValue "--rows"
  cols <- mustGetArgValue "--cols"
  putMatrixArgOut (mandel rows cols x y dx dy)

doNorm = do
  points <- getListArgIn
  putListArgOut (norm points)

doOuter = do
  points <- getListArgIn
  let (mat, vec) = outer points
  putMatrixArg mat "--mout"
  putListArg vec "--vout"

doProduct = do
  mat <- getMatrixArg "--matrix"
  actual <- getListArg "--actual"
  candidate <- getListArg "--candidate"
  putStringArgOut (show (CP.product mat actual candidate))

doRandMat = do
  rows <- mustGetArgValue "--rows"
  cols <- mustGetArgValue "--cols"
  seed <- getSeed
  typ <- processArg "--type" id "int"

  let frange = fmap floatR
  case typ of
    "int" -> putMatrixArgOut ((randmat rows cols seed) :: Matrix Int)
    "float" -> putMatrixArgOut (frange (randmat rows cols seed))
    "bool" -> putMatrixArgOut ((randmat rows cols seed) :: Matrix Bool)

doSor = do
  mat <- getMatrixArg "--matrix"
  target <- getListArg "--target"
  tolerance <- getArgValue "--tolerance" 0.01
  putListArgOut (sor mat target tolerance)

doThresh = do
    mat <- getMatrixArgIn
    percent <- mustGetArgValue "--percent"
    putMatrixArgOut (thresh mat percent)

doVecDiff = do
  v1 <- getListArg "--v1"
  v2 <- getListArg "--v2"
  putStringArgOut (show (vecdiff v1 v2))

doWinnow = do
  mat <- getMatrixArg "--matrix"
  mask <- getMatrixArg "--mask"
  elts <- mustGetArgValue "--nelts" 
  putListArgOut (winnow (mat :: Matrix Int) mask elts)

main = do
  (cmd:args) <- getArgs
  runReaderT (case cmd of
                "gauss"   -> doGauss
                "half"    -> doHalf
                "hull"    -> doHull
                "invperc" -> doInvPerc
                "life"    -> doLife
                "mandel"  -> doMandel
                "norm"    -> doNorm
                "outer"   -> doOuter
                "product" -> doProduct
                "randmat" -> doRandMat
                "sor"     -> doSor
                "thresh"  -> doThresh
                "vecdiff" -> doVecDiff
                "winnow"  -> doWinnow
              -- extra
                "randvec" -> doRandVec
                "randsdd" -> doRandSDD
                "mateq"   -> doMatEq
                "veceq"   -> doVecEq
                _         -> lift $ putStrLn "Command not recognized")
             args
