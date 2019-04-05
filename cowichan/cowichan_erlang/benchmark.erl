-module(benchmark).
-compile(export_all).
-define(NTRIES, 10).

run_paces() ->
    %Small = [10, 10, -0.5, -0.5, 0.5, 0.5],
    %Medium = [100, 100, -0.5, -0.5, 0.5, 0.5],
    %Big = [1000, 1000, -0.5, -0.5, 0.5, 0.5],
    Small = [10, 10, -1.5, -1.5, 3, 3],
    Medium = [100, 100, -1.5, -1.5, 3, 3],
    Big = [1000, 1000, -1.5, -1.5, 3, 3],
    %Ginormous = [10000, 10000, -1.5, -1.5, 3, 3],
    
    io:format("--------Testing small matrices--------~n"),
    test_matrix(Small),
    io:format("--------Testing medium matrices--------~n"),
    test_matrix(Medium),
    io:format("--------Testing large matrices--------~n"),
    %test_matrix(Big),
    io:format("--------Testing Ginormous matrices--------~n").
    %test_matrix(Ginormous).
    
test_matrix(Params) ->
    io:format("Baseline (sequential implementation):~n"),
    test_avg(mandel, sequential_mandel, Params, ?NTRIES),
    io:format("Testing basic parallel implementation (1 process/row):~n"),
    test_avg(mandel, parallel_mandel, Params, ?NTRIES),
    io:format("Testing binary parallel implementation (halving list until the lists are sufficiently small and gathering in a tree-like pattern):~n"),
    test_avg(mandel, binary_mandel, Params, ?NTRIES),
    io:format("Testing Qiyu's implementation:~n"),
    test_avg(mandelfaulttol, mandel, Params, ?NTRIES),
    %io:format("Testing ridiculously parallel implementation (1 process/cell):~n"),
    %test_avg(mandel, superparallel_mandel, Params, ?NTRIES).
    

% Function blatantly stolen from http://www.trapexit.org/Measuring_Function_Execution_Time
test_avg(M, F, A, N) when N > 0 ->
    L = test_loop(M, F, A, N, []),
    Length = length(L),
    Min = lists:min(L),
    Max = lists:max(L),
    Med = lists:nth(round((Length / 2)), lists:sort(L)),
    Avg = round(lists:foldl(fun(X, Sum) -> X + Sum end, 0, L) / Length),
    io:format("Range: ~b - ~b mics~n"
	      "Median: ~b mics~n"
	      "Average: ~b mics~n",
	      [Min, Max, Med, Avg]),
    Med.

test_loop(_M, _F, _A, 0, List) ->
    List;
test_loop(M, F, A, N, List) ->
    {T, _Result} = timer:tc(M, F, A),
    test_loop(M, F, A, N - 1, [T|List]).

