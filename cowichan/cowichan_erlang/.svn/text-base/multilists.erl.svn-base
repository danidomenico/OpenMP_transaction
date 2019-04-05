-module(multilists).
-define(MAX_LOAD, 1000).
-compile(export_all).

% Multi-d arrays implemented using nested lists (indexing is from 1 right now, but should probably be changed)

% Array initialized to zeroes. Takes a list corresponding to the size of
% dimensions. Zeroes([100, 10, 10]) ~= new int[100][10][10]
zeroes([Dim|[]]) -> lists:duplicate(Dim, 0);
zeroes([Dim|Rem]) -> lists:duplicate(Dim, zeroes(Rem)).

% get([5, 10], arr) ~= arr[5][10]
get([Coord|[]], Arr) -> lists:nth(Coord, Arr);
get([Coord|Rest], Arr) -> get(Rest, lists:nth(Coord, Arr)).

% set([5, 10], arr, 17) ~= arr[5][10] = 17
set([Coord|[]], Arr, Value) -> lists:sublist(Arr, Coord-1) ++ [Value] ++ lists:sublist(Arr, Coord+1, length(Arr));
set([Coord|Rest], Arr, Value) -> lists:sublist(Arr, Coord-1) ++ [set(Rest, get([Coord], Arr), Value)] ++ lists:sublist(Arr, Coord+1, length(Arr)).

list_to_str([]) -> "~n";
list_to_str([X|Rem]) when is_integer(X) -> integer_to_list(X) ++ " " ++ list_to_str(Rem);
list_to_str([X|Rem]) when is_float(X) -> io_lib:format("~.3f", [X]) ++ " " ++ list_to_str(Rem);
list_to_str([{X,Y}|Rem]) when is_integer(X) -> "(" ++ integer_to_list(X) ++ ", " ++ integer_to_list(Y) ++ ")" ++ list_to_str(Rem);
list_to_str([{X,Y}|Rem]) when is_float(X) -> "(" ++ io_lib:format("~.3f", [X]) ++ ", " ++ io_lib:format("~.3f", [Y]) ++ ")" ++ list_to_str(Rem);
list_to_str([X|Rem]) -> list_to_str(X) ++ list_to_str(Rem).

print(Arr) -> io:format(list_to_str(Arr)).

pmap_binary(Fun, Deeplist) -> Parent = self(),
    Arr = lists:append(Deeplist),
    Halfway = length(Arr) div 2,
    Firsthalf = lists:sublist(Arr, Halfway),
    Secondhalf = lists:sublist(Arr, Halfway+1, Halfway+1),
    Firstkid = spawn(multilists, pmap_binary_f, [Parent, Fun, Firsthalf]),
    Secondkid = spawn(multilists, pmap_binary_f, [Parent, Fun, Secondhalf]),
    binary_gather([Firstkid, Secondkid]).
    
pmap_binary_f(Parent, Fun, Arr) when length(Arr) < ?MAX_LOAD -> Parent ! {self(), lists:map(Fun, Arr)};
pmap_binary_f(Parent, Fun, Arr) ->
    Halfway = length(Arr) div 2,
    Firsthalf = lists:sublist(Arr, Halfway),
    Secondhalf = lists:sublist(Arr, Halfway+1, Halfway+1),
    Firstkid = spawn(multilists, pmap_binary_f, [self(), Fun, Firsthalf]),
    Secondkid = spawn(multilists, pmap_binary_f, [self(), Fun, Secondhalf]),
    Parent ! {self(), binary_gather([Firstkid, Secondkid])}.
    
binary_gather([A, B]) ->
    receive
        {A, Ret} -> receive {B, Ret2} -> Ret ++ Ret2 end;
        {B, Ret2} -> receive {A, Ret} -> Ret ++ Ret2 end
    end.

% parallel 2-d map
pmap(Fun, Arr) -> Parent = self(),
    Pids = lists:map(fun(Ele) -> spawn(fun() -> pmap_f(Parent, Fun, Ele) end) end, Arr),
    pmap_gather(Pids).
    
% parallel 2-d map using monitors for fault-tolerance
pmap_monitor(Fun, Arr) -> Parent = self(),
    io:format("I, ~.w, am doling out the work.~n", [self()]),
    {Pids, _Refs} = lists:unzip(lists:map(fun(Ele) -> spawn_monitor(fun() -> pmap_faulty_f(Parent, Fun, Ele) end) end, Arr)),
    pmap_monitor_gather(Fun, lists:zip(Pids, Arr)).
    
% parallel 2-d map with one process per cell
pmap_one(Fun, Arr) -> Parent = self(),
    Pids = lists:map(fun(Ele) -> spawn(fun() -> pmap_f_one(Parent, Fun, Ele) end) end, Arr),
    pmap_gather(Pids).
    

pmap_f_one(Parent, Fun, Element) when is_list(Element) -> Subparent = self(),
    Grandpids = lists:map(fun(Ele) -> spawn(fun() -> pmap_f_one(Subparent, Fun, Ele) end) end, Element), % Please forgive me for this pun
    Parent ! {self(), pmap_gather(Grandpids)};

pmap_f_one(Parent, Fun, Element) -> Parent ! {self(), Fun(Element)}.
    
pmap_f(Parent, Fun, Element) -> Parent ! {self(), lists:map(Fun, Element)}.

% Map fun to element, and with some probability, succumb to a plague of locusts
% o'er the land. Otherwise, send the results to the parent.
pmap_faulty_f(Parent, Fun, Element) -> 
    Res = {self(), lists:map(Fun, Element)},
    random:seed(now()),
    [random:uniform() || _ <- lists:seq(1, 30)],  % Burn some data
    Rnd = random:uniform(),
    if Rnd < 0.2 ->
        io:format("Process ~.w died of locusts.~n", [self()]),
        exit(locusts);
    true ->
        io:format("Process ~.w done, sending results to parent ~.w.~n", [self(), Parent]),
        Parent ! Res
    end.

pmap_gather([H|T]) ->
    receive
        {H, Ret} -> [Ret|pmap_gather(T)]
    end;
pmap_gather([]) -> [].

pmap_monitor_gather(Fun, Refs=[{Pid, Arr}|T]) ->
    receive
        {Pid, Ret} ->
            io:format("Received results from process ~.w~n", [Pid]),
            [Ret|pmap_monitor_gather(Fun, T)];
        {'DOWN', _MonitorRef, process, Deadpid, locusts} ->
            io:format("I, ~.w, am reviving processes.~n", [self()]),
            {_, Deadarr} = lists:keyfind(Deadpid, 1, Refs),
            Me = self(),
            {Newpid, _Newref} = spawn_monitor(fun() -> pmap_faulty_f(Me, Fun, Deadarr) end),
            io:format("Restarting process ~.w. Reborn as process ~.w~n", [Deadpid, Newpid]),
            pmap_monitor_gather(Fun, [{Newpid, Deadarr}| lists:keydelete(Deadpid, 1, Refs)])
    after 100000 ->
        {Newpid, _Newref} = spawn_monitor(fun() -> pmap_f(self(), Fun, Arr) end),
        pmap_monitor_gather(Fun, [{Newpid, Arr}|T])
    end;
    
pmap_monitor_gather(_, []) -> [].

% parallel 2-d map with process restarting
persistent_pmap(Fun, Arr) -> Parent = self(),
    Pids = lists:map(fun(Ele) -> spawn(fun() -> pmap_f(Parent, Fun, Ele) end) end, Arr),
    persistent_gather(lists:zip(Pids, Arr), Fun).
    
persistent_gather([], _) -> [];
persistent_gather([{Pid, Elem}|T], Fun) ->
    receive
        {Pid, Ret} -> [Ret|persistent_gather(T, Fun)]
    after 2000 -> 
        spawn(fun() -> pmap_f(self(), Fun, Elem) end),
        persistent_gather([{Pid, Elem}|T], Fun)
    end.
    
