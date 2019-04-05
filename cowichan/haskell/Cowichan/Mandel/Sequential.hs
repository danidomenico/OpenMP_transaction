module Cowichan.Mandel.Sequential where

import Cowichan.Matrix
import Cowichan.Mandel.Common

mandel nrows ncols x0 y0 dx dy =
    let
        step (x, y) = (x^2 + y^2 + y0, 2*x*y + x0)
        diverge (x, y) = (x^2 + y^2) >= divergeConst
        iter x y = takeWhile (not . diverge) (take limit (iterate step (x, y)))
        xstep = dx / (fromIntegral nrows)
        ystep = dy / (fromIntegral ncols)
    in
      matrix (\x y -> length (iter (x0 + (fromIntegral x)*xstep)
                                   (y0 + (fromIntegral y)*ystep)))
             nrows
             ncols
