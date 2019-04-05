-module(distribmandel).
-export([mandel/6, mandel/7, print/1]).

-define(ITER_LIMIT, 150).
-define(NPROCS, 10).


% Public functions

mandel(Nrows, Ncols, X0, Y0, Dx, Dy) ->
    mandel(Nrows, Ncols, X0, Y0, Dx, Dy, ?NPROCS).

mandel(Nrows, Ncols, X0, Y0, Dx, Dy, Nprocs) ->
    Workunits = [{Nrows, Ncols, X0, Y0, Dx, Dy, Nprocs, Offset} || Offset <- lists:seq(0, Nprocs - 1)],
    CompFunc = fun(A) -> compute(A) end,
    Reducer = fun(A, B) -> reduce(A, B) end,
    distributedcompute:distributedCompute(Workunits, CompFunc, array2d:new(Ncols, Nrows, -1), Reducer).


print(Matrix) -> print(Matrix, 0).
print({_,_,H,_}, H) -> true;
print(M, Y) -> printRow(M, 0, Y), io:fwrite("\n"), print(M, Y+1).
printRow({_,W,_,_}, W, _) -> true;
printRow(M, X, Y) -> io:fwrite(charify(array2d:get(M, X, Y))), printRow(M, X+1, Y).
charify(150) -> "O";
charify(-1) -> "X";
charify(_) -> ".".


% Private functions

reduce(Result, {_, _, _, _, []}) ->
    Result;
reduce(Result, {Nrows, Ncols, Stride, Offset, [D|Ds]}) ->
    reduce(array2d:set(Result, Offset rem Ncols, Offset div Ncols, D), {Nrows, Ncols, Stride, Offset + Stride, Ds}).


compute(Params={Nrows, Ncols, _, _, _, _, Stride, Offset}) ->
    {Nrows, Ncols, Stride, Offset, compute(Params, Offset, [])}.

compute({Nrows, Ncols, _, _, _, _, _, _}, Index, Data)
        when Index >= Nrows * Ncols ->
    lists:reverse(Data);

compute(Params={Nrows, Ncols, X0, Y0, Dx, Dy, Stride, _}, Index, Data) ->
    NewData = [mandelPoint(getPoint(Nrows, Ncols, X0, Y0, Dx, Dy, Index)) | Data],
    compute(Params, Index + Stride, NewData).


getPoint(Nrows, Ncols, X0, Y0, Dx, Dy, Index)
        when 0 =< Index, Index < Nrows * Ncols ->
    X = Index rem Ncols,
    Y = Index div Ncols,
    {X0 + Dx * (X + 0.5) / Ncols, Y0 + Dy * (Y + 0.5) / Nrows}.


% Given a point in the complex plane, return the number of iterations it takes
% to diverge, or the iteration limit, whichever is smaller.
mandelPoint({X, Y}) -> mandelPoint(X, Y, 0, 0, 0).

mandelPoint(_X0, _Y0, _X, _Y, ?ITER_LIMIT) -> ?ITER_LIMIT;
mandelPoint(_X0, _Y0, X, Y, Count) when (X*X + Y*Y) >= 4.0 -> Count;
mandelPoint(X0, Y0, X, Y, Count) ->
    NewX = X*X - Y*Y + X0,
    NewY = 2*X*Y + Y0,
    mandelPoint(X0, Y0, NewX, NewY, Count + 1).
