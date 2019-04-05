% Conway's Game of Life in Erlang, parallel.

-module(lifeparallel).
-export([life/4, life/5]).
-compile(export_all).

-define(Nprocs, 4).
-define(NumgenPerSpawn, 5).


% Top-level public function

% Returns the result of iterating Conway's Game of Life Numgen times on Matrix.
% Matrix is an Array2D value. Nrows is the height of Matrix, and Ncols is the
% width of Matrix.
life(Matrix, Nrows, Ncols, Numgen)
        when Nrows >=0, Ncols >=0, Numgen >= 0 ->
    life(Matrix, Nrows, Ncols, Numgen, ?Nprocs).


% Computes the Game of Life with Nprocs processes.
life(Matrix, _, _, 0, _) ->
    Matrix;

life(Matrix, Nrows, Ncols, Numgen, Nprocs)
        when Nrows >=0, Ncols >=0, Numgen >= 0 ->
    Gen = min(Numgen, ?NumgenPerSpawn),
    Self = self(),
    spawn(fun() -> lifemaster:main(Matrix, Gen, Nprocs, Self) end),
    receive
        {life, error, _} ->
            io:format("recover~n"),
            life(Matrix, Nrows, Ncols, Numgen, Nprocs);
        {life, NewMatrix} ->
            life(NewMatrix, Nrows, Ncols, Numgen - Gen, Nprocs)
    end.


% Scrap functions

glider() -> setcells(array2d:new(8,6,0), [{1,0},{2,1},{0,2},{1,2},{2,2}]).

setcells(Matrix, []) -> Matrix;
setcells(Matrix, [{X,Y}|T]) -> setcells(array2d:set(Matrix, X, Y, 1), T).

print(Matrix) -> print(Matrix, 0).
print({_,_,H,_}, H) -> true;
print(M, Y) -> printRow(M, 0, Y), io:fwrite("\n"), print(M, Y+1).
printRow({_,W,_,_}, W, _) -> true;
printRow(M, X, Y) -> io:fwrite(charify(array2d:get(M, X, Y))), printRow(M, X+1, Y).

charify(0) -> ".";
charify(1) -> "O";
charify(-1) -> "X".
