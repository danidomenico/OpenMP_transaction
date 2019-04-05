module Cowichan.Matrix (Matrix,
                        matrix, matrixM,
                        index, dim, rows, submatrix,
                        matFoldl,
                        update, concatV, concatH) where
import Data.List

newtype Matrix a = Matrix [[a]]
    deriving Eq

instance Functor Matrix where
    fmap f (Matrix m) = Matrix (map (map f) m)

instance (Show a) => Show (Matrix a) where
    show (Matrix mat) =
        let 
            mat' = (map (map show) mat)
            rows = map (concat . (intersperse ", ")) mat'            
        in
          "[" ++ (concat (intersperse ";\n " rows)) ++ "]"

-- initialize a matrix
matrix :: (Int -> Int -> a) -> Int -> Int -> Matrix a
matrix f rows cols = 
    Matrix (map (\x -> (map (f x) [0..cols-1])) [0..rows-1])

-- initialize a matrix with a monadic function
matrixM :: (Monad m) => (Int -> Int -> m a) -> Int -> Int -> m (Matrix a)
matrixM f rows cols =
    (mapM (\x -> (mapM (f x) [0..cols-1])) [0..rows-1]) >>= (return . Matrix)

index :: Matrix a -> Int -> Int -> a
index (Matrix m) i j = (m !! i) !! j

matFoldl :: (a -> b -> a) -> a -> Matrix b -> a
matFoldl f z (Matrix m) = 
    foldl (foldl f) z m

rows (Matrix m) = m

{-
safeIndex (Matrix m) x y =
    if (0 <= x  && x < (length m)) && (0 <= y && y < (length (head m)))
    then Just ((m !! x) !! y)
    else Nothing
-}

-- horizontal matrix concatentation
concatH (Matrix m1) (Matrix m2) =
    if (length m1) == (length m2)
    then Matrix (zipWith (++) m1 m2)
    else error "concatH"

-- vertical matrix concatenation
concatV (Matrix m1) (Matrix m2) =
    if (length $ head m1) == (length $ head m2)
    then Matrix (m1 ++ m2)
    else error "concatV"

sublist start extent xs = take extent (drop start xs)

submatrix :: Matrix a -> Int -> Int -> Int -> Int -> Matrix a
submatrix (Matrix m) x y dx dy =
    Matrix (map (sublist y dy) (sublist x dx m))

dim (Matrix m) =
    (length m, length (head m))

alterList [] _ _ = error "alterList on empty list"
alterList (x:xs) 0 f = (f x):xs
alterList (x:xs) n f = x:(alterList xs (n - 1) f)

-- change an entry of a matrix
update :: Matrix a -> Int -> Int -> a -> Matrix a
update (Matrix m) x y e =
    Matrix (alterList m x (\row -> alterList row y (const e)))
