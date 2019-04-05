module Cowichan.Hull.Sequential where
import Cowichan.Matrix

cross (x1, y1) (x2, y2) (px, py) =
    (x1 - px) * (y2 - py) - (y1 - py) * (x2 - px)

maximize f (p:ps) = foldl (\(y, v) x -> if (f x) > v
                                        then (x, f x)
                                        else (y, v))
                    (p, f p)
                    ps

split [] p1 p2 = [p1]
split points p1 p2 =
    let
        (maxPoint, maxCross) = maximize (cross p1 p2) points
    in
      if maxCross > 0
      then (split points p1 maxPoint) ++ (split points maxPoint p2)
      else [p1]

hull :: [(Float, Float)] -> [(Float, Float)]
hull points = 
    let
        (max,_) = maximize fst points
        (min,_) = maximize (negate . fst) points
    in
      case points of
        []  -> []
        [p] -> [p]
        _   -> (split points min max) ++ (split points max min) 
