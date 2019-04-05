-module(mandelfaulttol).
-export([mandel/6, mandel/7, mandelPoint/1, print/1]).

-define(ITER_LIMIT, 150).
-define(NPROCS, 10).


% Public functions

mandel(Nrows, Ncols, X0, Y0, Dx, Dy) ->
    mandel(Nrows, Ncols, X0, Y0, Dx, Dy, ?NPROCS).

mandel(Nrows, Ncols, X0, Y0, Dx, Dy, Nprocs) ->
    mandelMaster({Nrows, Ncols, X0, Y0, Dx, Dy}, Nprocs).

print(Matrix) -> print(Matrix, 0).
print({_,_,H,_}, H) -> true;
print(M, Y) -> printRow(M, 0, Y), io:fwrite("\n"), print(M, Y+1).
printRow({_,W,_,_}, W, _) -> true;
printRow(M, X, Y) -> io:fwrite(charify(array2d:get(M, X, Y))), printRow(M, X+1, Y).
charify(150) -> "O";
charify(-1) -> "X";
charify(_) -> ".".


% Private functions

% Master functions

mandelMaster(Params={Nrows, Ncols, _, _, _, _}, Nprocs) ->
    spawnAll(Params, 0, Nprocs, []),
    PendingOffsets = lists:seq(0, Nprocs - 1),
    myJoin(Params, Nprocs, PendingOffsets, array2d:new(Ncols, Nrows, -1)).


spawnAll(_, Nprocs, Nprocs, Pids) ->
    lists:reverse(Pids);
spawnAll(Params, Index, Nprocs, Pids) ->
    Pid = spawnOne(Params, Index, Nprocs),
    spawnAll(Params, Index + 1, Nprocs, [Pid | Pids]).

spawnOne(Params, Index, Nprocs) ->
    Master = self(),
    spawn(fun() -> mandelWorker(Master, Params, Nprocs, Index, Index, []) end).


myJoin(_, _, [], Data) ->
    Data;
myJoin(Params={Nrows, Ncols, _, _, _, _}, Nprocs, PendingOffsets, Data) ->
    receive
        {done, Stride, Offset, RecvData} ->
            io:format("Received ~.10B~n", [Offset]),
            NewData = addData(Data, Nrows, Ncols, RecvData, Stride, Offset),
            NewPending = lists:delete(Offset, PendingOffsets),
            myJoin(Params, Nprocs, NewPending, NewData)
    after
        1000 ->
            io:format("Timed out, respawning ~.10B~n", [lists:nth(1, PendingOffsets)]),
            spawnOne(Params, lists:nth(1, PendingOffsets), Nprocs),
            myJoin(Params, Nprocs, PendingOffsets, Data)
    end.


addData(Data, _, _, [], _, _) ->
    Data;
addData(Data, Nrows, Ncols, [D|Ds], Stride, Offset) ->
    addData(array2d:set(Data, Offset rem Ncols, Offset div Ncols, D), Nrows, Ncols, Ds, Stride, Offset + Stride).


% Worker functions

mandelWorker(Master, {Nrows, Ncols, _, _, _, _}, Stride, Offset, Index, Data)
        when Index >= Nrows * Ncols ->
    random:seed(now()),
    [random:uniform() || _ <- lists:seq(1, 30)],  % Burn some data
    Rnd = random:uniform(),
    if Rnd < 0.2  ->
        false;
    true ->
        Master ! {done, Stride, Offset, lists:reverse(Data)}
    end;

mandelWorker(Master, Params={Nrows, Ncols, X0, Y0, Dx, Dy}, Stride, Offset, Index, Data) ->
    NewData = [mandelPoint(getPoint(Nrows, Ncols, X0, Y0, Dx, Dy, Index)) | Data],
    mandelWorker(Master, Params, Stride, Offset, Index + Stride, NewData).


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
