% An Array2D is a two-dimensional rectangular array of values. The width and
% height of an Array2D can be any non-negative integer. In an Array2D, The x
% coordinate array has the range [0, width), and the y coordinate has the range
% [0, height).

-module(array2d).
-export([new/3, width/1, height/1, get/3, set/4, getRect/5, setRect/4, mapIndex/2]).


% Returns a new Array2D with the specified width and height, with all cells set
% to the specified value. The width and height must each be a non-negative
% integer.
new(Width, Height, InitValue)
        when Width >= 0, Height >= 0 ->
    {array2d, Width, Height,
        array:new(Width * Height, {default, InitValue})}.

% Returns the width of the specified Array2D, which is a non-negative integer.
width({array2d, Width, _, _}) ->
    Width.

% Returns the height of the specified Array2D, which is a non-negative integer.
height({array2d, _, Height, _}) ->
    Height.

% Returns the value of the specified cell in the specified Array2D.
get({array2d, Width, Height, Array}, X, Y)
        when 0 =< X, X < Width, 0 =< Y, Y < Height ->
    array:get(Y * Width + X, Array).

% Returns a new Array2D resulting from taking the specified Array2D and changing
% the specified cell's value to the specified value.
set({array2d, Width, Height, Array}, X, Y, Value)
        when 0 =< X, X < Width, 0 =< Y, Y < Height ->
    {array2d, Width, Height, array:set(Y * Width + X, Value, Array)}.


% Returns a new Array2D containing the specified region. The region is specified
% by the starting point (X, Y) (0 <= X <= Width and 0 <= Y <= Height) and the
% width and height of the region to copy (0 <= W <= Width - X and
% 0 <= H <= Height - Y).
getRect(Array2D={_, Width, Height, _}, X, Y, W, H)
        when 0 =< X, X + W =< Width, 0 =< Y, Y + H =< Height, W >= 0, H >= 0 ->
    copyRect(Array2D, X, Y, new(W, H, 0), 0, 0, W, H).

% Returns a new Array2D resulting from taking the specified Array2D and setting
% the specified region with the values of the specified array. The region's
% starting point is given by (X, Y), and its width and height is implied by the
% width and height of the FromArray2D argument.
setRect(ToArray2D={_, Width, Height, _}, X, Y, FromArray2D={_, W, H, _})
        when 0 =< X, X + W =< Width, 0 =< Y, Y + H =< Height, W >= 0, H >= 0 ->
    copyRect(FromArray2D, 0, 0, ToArray2D, X, Y, W, H).


% Returns a new Array2D whose contents result from mapping Fun(Array, X, Y) over
% all indices. In other words, Result[0, 0] = Fun(Array, 0, 0), ...,
% Result[Width - 1, Height - 1] = Fun(Array, Width - 1, Height - 1).
% So this is like list:map(), but instead of Fun receiving an array element, it
% receives the array and an index.
mapIndex(Array, Fun) ->
    Width = width(Array),
    Height = height(Array),
    mapIndex(Array, new(Width, Height, 0), Width, Height, Fun, 0).

mapIndex(_, NewArray, _, Height, _, Height) ->
    NewArray;
mapIndex(OldArray, NewArray, Width, Height, Fun, Y) ->
    Temp = mapIndexRow(OldArray, NewArray, Width, Height, Fun, 0, Y),
    mapIndex(OldArray, Temp, Width, Height, Fun, Y + 1).

mapIndexRow(_, NewArray, Width, _, _, Width, _) ->
    NewArray;
mapIndexRow(OldArray, NewArray, Width, Height, Fun, X, Y) ->
    Temp = set(NewArray, X, Y, Fun(OldArray, X, Y)),
    mapIndexRow(OldArray, Temp, Width, Height, Fun, X + 1, Y).


% The actual implementation of setRect().
copyRect(_, _, _, ToArray2D, _, _, _, 0) ->
    ToArray2D;
copyRect(FromArray2D, FromX, FromY, ToArray2D, ToX, ToY, W, H) ->
    Temp = copyRectRow(FromArray2D, FromX, FromY, ToArray2D, ToX, ToY, W),
    copyRect(FromArray2D, FromX, FromY + 1, Temp, ToX, ToY + 1, W, H - 1).

copyRectRow(_, _, _, ToArray2D, _, _, 0) ->
    ToArray2D;
copyRectRow(FromArray2D, FromX, FromY, ToArray2D, ToX, ToY, W) ->
    Temp = set(ToArray2D, ToX, ToY, get(FromArray2D, FromX, FromY)),
    copyRectRow(FromArray2D, FromX + 1, FromY, Temp, ToX + 1, ToY, W - 1).
