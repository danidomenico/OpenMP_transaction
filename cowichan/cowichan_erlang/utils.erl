-module(utils).
-export([randomDie/1, randomFloat/0, sleep/1]).


% Kills the current process randomly with the specified probability (a
% floating-point number between 0 and 1).
randomDie(Prob) when 0 =< Prob, Prob =< 1 ->
    Die = randomFloat() < Prob,
    if Die -> exit(randomDeath);
        true -> ok
    end.


% Returns a random floating-point number in the range [0, 1).
randomFloat() ->
    Hash = erlang:md5(term_to_binary({self(), now()})),
    <<N:32, _/binary>> = Hash,
    N / 4294967296.


% Sleeps for the specified number of milliseconds.
sleep(Millis) ->
    receive
    after
        Millis -> ok
    end.