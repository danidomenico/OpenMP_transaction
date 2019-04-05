module Cowichan.InvPerc.Sequential where
import Cowichan.Matrix
import qualified Data.Map as Map
import qualified Data.Foldable as Foldable

type PriorityQueue p e = Map.Map p [e]
empty :: PriorityQueue p e
empty = Map.empty

enqueue :: (Ord p) => PriorityQueue p e -> p -> e -> PriorityQueue p e
enqueue q p e =
    Map.alter (\x -> Just $ case x of Just es -> e:es; Nothing -> [e]) p q

mayEnqueue :: (Ord p, Eq e) => PriorityQueue p e -> p -> e -> PriorityQueue p e
mayEnqueue q p e =
    Map.alter (\x -> Just $ case x of Just es -> if e `elem` es
                                                 then es
                                                 else e:es
                                      Nothing -> [e]) p q

dequeue :: (Ord p) => PriorityQueue p e -> (e, PriorityQueue p e)
dequeue q =
    let (p, e:es) = Map.findMin q in
    case es of
      [] -> (e, Map.deleteMin q)
      _  -> (e, Map.updateMin (\x -> Just es) q)


neighbors nrows ncols x y =
    filter (\(x,y) -> 0 <= x && x < nrows && 0 <= y && y < ncols)
               [(x-1, y), (x+1, y), (x, y-1), (x, y+1)]

invperc :: Matrix Float -> Int -> Matrix Bool
invperc mat nfill =
    let
        (nrows, ncols) = dim mat
        mayEnq mask q x y =
            let p = index mat x y in
            if index mask x y
            then q
            else mayEnqueue q p (x,y)
        invperc' mask queue 0 = mask
        invperc' mask queue n =
            let ((x,y), dq) = dequeue queue
                queue' = foldl (\q (x,y) -> mayEnq mask q x y)
                         dq
                         (neighbors nrows ncols x y)
            in
              invperc' (update mask x y True) queue' (n - 1)

        -- center coordinates
        (cx, cy) = (nrows `quot` 2, ncols `quot` 2)
        queue = enqueue empty (index mat cx cy) (cx, cy)
    in
      invperc' (matrix (const . const False) nrows ncols) queue nfill
