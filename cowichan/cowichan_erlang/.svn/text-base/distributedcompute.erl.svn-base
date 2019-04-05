-module(distributedcompute).
-export([distributedCompute/4]).


distributedCompute(Workunits, CompFun, BlankResult, Reducer) ->
    Indices = lists:seq(1, length(Workunits)),
    IndexAndWu = lists:zip(Indices, Workunits),
    Master = self(),
    [spawn(fun() -> leaser(CompFun, Index, Wu, Master) end) || {Index, Wu} <- IndexAndWu],
    masterReduce(BlankResult, Reducer, Indices).

masterReduce(Result, _, []) ->
    Result;
masterReduce(Result, Reducer, PendingIndices) ->
    receive
        {done, Index, Answer} ->
            IsElem = lists:member(Index, PendingIndices),
            if IsElem ->
                    masterReduce(Reducer(Result, Answer), Reducer, lists:delete(Index, PendingIndices));
                true ->
                    masterReduce(Result, Reducer, PendingIndices)
            end
    end.


leaser(CompFun, Index, Workunit, Master) ->
    Leaser = self(),
    Worker = spawn(fun() -> worker(CompFun, Index, Workunit, Master, Leaser) end),
    leaserLoop(CompFun, Index, Workunit, Master, 100, Worker).

leaserLoop(CompFun, Index, Workunit, Master, Duration, Worker) ->
    io:format("Lease WU ~.10B to ~w for ~.10B~n", [Index, Worker, Duration]),
    Worker ! {lease, Duration},
    receive
        done ->
            ok;
        renew ->
            leaserLoop(CompFun, Index, Workunit, Master, Duration*2, Worker)
    after
        Duration ->
            leaser(CompFun, Index, Workunit, Master)
    end.


worker(CompFun, Index, Workunit, Master, Leaser) ->
    Parent = self(),
    spawn_link(fun() -> workerSlave(CompFun, Index, Workunit, Master, Parent) end),
    workerLoop(Leaser).

workerLoop(Leaser) ->
    receive
        done ->
            Leaser ! done;
        {lease, Duration} ->
            erlang:send_after(round(Duration * 0.8), Leaser, renew),
            workerLoop(Leaser)
    end.


workerSlave(CompFun, Index, Workunit, Master, Parent) ->
    sleep(round(randomFloat() * 1000)),
    die(0.5),
    Master ! {done, Index, CompFun(Workunit)},
    Parent ! done.


die(Prob) ->
    Die = randomFloat() < Prob,
    if Die -> io:format("Die~n"), exit(suicide);
        true -> ok
    end.

randomFloat() ->
    Hash = erlang:md5(term_to_binary({self(), now()})),
    <<N:32, _/binary>> = Hash,
    N / 4294967296.

sleep(Millis) ->
    io:format("Sleep ~.10B~n", [Millis]),
    receive
    after
        Millis -> ok
    end.