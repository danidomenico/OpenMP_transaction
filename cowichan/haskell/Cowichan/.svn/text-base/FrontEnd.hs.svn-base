{-# OPTIONS -XNoMonomorphismRestriction #-}
module Cowichan.FrontEnd where

import Cowichan.Matrix
import Data.List
import Control.Monad.Reader
import Cowichan.RandMat.Common
import Data.Char
import Debug.Trace


capitalize (x:xs) = (toUpper x):(map toLower xs)

-- hack to get around uppercase bool problem
class Serialize a where
    serialize :: a -> String
    deserialize :: String -> a

instance Serialize Int where
    serialize = show
    deserialize = read

instance Serialize Float where
    serialize = show
    deserialize = read

instance (Serialize a, Serialize b) => Serialize (a,b) where
    serialize (a,b) = "(" ++ (serialize a) ++ "," ++ (serialize b) ++ ")"
    deserialize str =
        if head str == '(' && last str == ')'
        then let (as, bs) = splitAtElem ',' (tail (take (length str - 1) str))
             in (deserialize as, deserialize (tail bs))
        else error "parse"

instance Serialize Bool where
    serialize = map toLower . show
    deserialize = read . capitalize

splitAtElem e = break ((==) e)

matrixString mat =
    let
        (nrows, ncols) = dim mat
        dimString = (show nrows) ++ "x" ++ (show ncols)
        rowString = unwords . map serialize
    in
      unlines $ dimString:(map rowString (rows mat))


parseMatrix str =
    let 
        (dim:mrows) = lines str
        (rstr,cstr) = splitAtElem 'x' dim
        (rows,cols) = (read rstr, read (tail cstr))
        mat = map words mrows
    in
      matrix (\x y -> deserialize ((mat !! x) !! y)) rows cols

parseList str =
    let
        (dim:rows) = lines str
    in
      take (read dim) (map deserialize rows)

listString list = unlines ((show (length list)):(map serialize list))

getMatrix file = readFile file >>= return . parseMatrix
    
putMatrix mat file = writeFile file (matrixString mat)

getList file = readFile file >>= return . parseList

putList list file = writeFile file (listString list)

processArgM :: (Monad m) =>
               String -> (String -> m a) -> m a -> ReaderT [String] m a
processArgM name act other = do
  args <- ask
  case dropWhile ((/=) name) args of
    (a:v:_) -> lift $ act v
    _       -> lift $ other

processArg name act other = processArgM name (return . act) (return other)

getMatrixArg name =
    processArgM name getMatrix (getContents >>= return . parseMatrix)


getMatrixArgIn = getMatrixArg "--in"

getListArg name =
    processArgM name getList (getContents >>= return . parseList)

getListArgIn = getListArg "--in"

getArgValue name other =
    processArg name read other

mustGetArgValue name =
    getArgValue name (error ("must define: " ++ name))

putMatrixArg mat name =
    processArgM name (putMatrix mat) (putStrLn (matrixString mat))

putListArg list name =
    processArgM name (putList list) (putStrLn (listString list))

putMatrixArgOut mat = putMatrixArg mat "--out"
putListArgOut mat = putListArg mat "--out"

putStringArgOut str =
  processArgM "--out" (flip writeFile str) (putStrLn str)

getSeed = getArgValue "--seed" defaultSeed    
