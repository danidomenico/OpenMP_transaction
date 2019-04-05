-module(lifeworker).
-export([main/4, blankMain/4]).
-compile([export_all]).


% Blank worker node, always sending a column of zeros

blankMain(_, 0, _, _) ->
    true;
blankMain(Height, Numgen, Neighbor, Type) ->
    blankFlush(),
    Neighbor ! {Type, array2d:new(1, Height, 0)},
    blankMain(Height, Numgen - 1, Neighbor, Type).

blankFlush() ->
    receive
        {leftEdge , _} -> blankFlush();
        {rightEdge, _} -> blankFlush()
    after
        0 -> true
    end.


% Worker function

main(MasterPid, ProcIndex, Matrix, Numgen) ->
    Neighs = getNeighbors(),
    PaddedMatrix = padMatrix(Matrix),
    NewMatrix = iterate(Neighs, PaddedMatrix, Numgen),
    utils:randomDie(0.2),
    MasterPid ! {done, ProcIndex, unpadMatrix(NewMatrix)}.


padMatrix(Matrix) ->
    Width = array2d:width(Matrix),
    Height = array2d:height(Matrix),
    NewMatrix = array2d:new(Width + 2, Height + 2, 0),
    array2d:setRect(NewMatrix, 1, 1, Matrix).

unpadMatrix(Matrix) ->
    Width = array2d:width(Matrix),
    Height = array2d:height(Matrix),
    array2d:getRect(Matrix, 1, 1, Width - 2, Height - 2).


% Receives the two neighbor PIDs.
getNeighbors() ->
    receive
        {neighbors, LeftNeigh, RightNeigh} -> {LeftNeigh, RightNeigh}
    end.

% Receives the left and right edges and returns a new matrix with that data
% overwriting the old edges of the specified matrix.
receiveEdges(Matrix) ->
    receive
        {leftEdge, LeftEdge} ->
            Temp = array2d:setRect(Matrix, 0, 1, LeftEdge),
            receive {rightEdge, RightEdge} ->
                array2d:setRect(Temp, array2d:width(Matrix) - 1, 1, RightEdge)
            end
    end.


% Iterates Game of Life Numgen times on the specified padded matrix, with the
% specified neighbor PIDs.
iterate(_, Matrix, 0) ->
    Matrix;
iterate(Neighs={LeftNeigh, RightNeigh}, Matrix, Numgen) ->
    LeftNeigh ! {rightEdge, array2d:getRect(Matrix, 1, 1, 1, array2d:height(Matrix) - 2)},
    RightNeigh ! {leftEdge, array2d:getRect(Matrix, array2d:width(Matrix) - 2, 1, 1, array2d:height(Matrix) - 2)},
    TempMatrix = receiveEdges(Matrix),
    NewMatrix = compute(TempMatrix),
    iterate(Neighs, NewMatrix, Numgen - 1).


% Returns the result of computing one iteration of Life on the specified matrix,
% but with the border cells are unchanged.
compute(Matrix) ->
    array2d:mapIndex(Matrix, computeCellFunction(array2d:width(Matrix), array2d:height(Matrix))).

computeCellFunction(Width, Height) ->
    fun(Mat, X, Y) when X =:= 0; X =:= Width - 1; Y =:= 0; Y =:= Height - 1 ->  % Skip updating the border cells, to prevent indexing out of bounds
           array2d:get(Mat, X, Y);
       (Mat, X, Y) ->
           Self = array2d:get(Mat, X, Y),
           Neighs = array2d:get(Mat, X - 1, Y - 1)
                  + array2d:get(Mat, X - 1, Y + 0)
                  + array2d:get(Mat, X - 1, Y + 1)
                  + array2d:get(Mat, X + 0, Y - 1)
                  + array2d:get(Mat, X + 0, Y + 1)
                  + array2d:get(Mat, X + 1, Y - 1)
                  + array2d:get(Mat, X + 1, Y + 0)
                  + array2d:get(Mat, X + 1, Y + 1),
           lifeOrDeath(Self, Neighs)
    end.


% Computes the next state of the given cell, given the state of the current cell
% and the number of live neighbors it has. The first argument is whether the
% current cell is alive (0 or 1). The second argument is the number of live
% neighbors the cell has (0 to 8).
lifeOrDeath(0, 3) -> 1;
lifeOrDeath(0, _) -> 0;
lifeOrDeath(1, 2) -> 1;
lifeOrDeath(1, 3) -> 1;
lifeOrDeath(1, _) -> 0.
