-module(lifemaster).
-export([main/4]).


% Master function (spawns workers, hands out work, joins them, merges results)

main(Matrix, Numgen, Nprocs, Parent) ->
    % Get a few numbers
    Width = array2d:width(Matrix),
    Height = array2d:height(Matrix),
    Stride = (Width + Nprocs - 1) div Nprocs,  % Divide and round up. Note: Stride * Nprocs >= Width.
    
    % Spawn worker processes
    process_flag(trap_exit, true),
    WorkerPids = mySpawn(Matrix, Stride, Numgen, 0, Nprocs, array:new(Nprocs + 2)),
    LeftBlankPid = spawn_link(lifeworker, blankMain, [Height, Numgen, array:get(1, WorkerPids), leftEdge]),
    RightBlankPid = spawn_link(lifeworker, blankMain, [Height, Numgen, array:get(Nprocs, WorkerPids), rightEdge]),
    TempPids = array:set(0, LeftBlankPid, WorkerPids),
    AllPids = array:set(Nprocs + 1, RightBlankPid, TempPids),
    
    % For each non-edge process, send it the two neighbor PIDs
    [array:get(I, AllPids) ! {neighbors, array:get(I - 1, AllPids), array:get(I + 1, AllPids)}
        || I <- lists:seq(1, Nprocs)],
    
    % Collect data from Nprocs processes
    myJoin(Nprocs, Stride, array2d:new(Width, Height, -1), Parent).


mySpawn(_, _, _, Nprocs, Nprocs, Pids) ->
    Pids;
mySpawn(Matrix, Stride, Numgen, ProcIndex, Nprocs, Pids) ->
    Width = array2d:width(Matrix),
    Height = array2d:height(Matrix),
    Offset = min(ProcIndex * Stride, Width),
    Slice = array2d:getRect(Matrix, Offset, 0, min(Stride, Width - Offset), Height),
    Pid = spawn_link(lifeworker, main, [self(), ProcIndex, Slice, Numgen]),
    NewPids = array:set(ProcIndex + 1, Pid, Pids),
    mySpawn(Matrix, Stride, Numgen, ProcIndex + 1, Nprocs, NewPids).


myJoin(0, _, Matrix, Parent) ->
    Parent ! {life, Matrix};
myJoin(Nprocs, Stride, Matrix, Parent) ->
    receive
        {done, ProcIndex, Mat} ->
            Offset = min(ProcIndex * Stride, array2d:width(Matrix)),
            NewMatrix = array2d:setRect(Matrix, Offset, 0, Mat),
            myJoin(Nprocs - 1, Stride, NewMatrix, Parent);
        {'EXIT', _, normal} ->
            myJoin(Nprocs, Stride, Matrix, Parent);
        {'EXIT', _, Reason} ->
            Parent ! {life, error, Reason}
    end.
