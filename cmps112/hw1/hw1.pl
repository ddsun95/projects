% Program: hw1.pl
% Author: David Sun


father(al, bud).
father(al, kelly).
mother(peggy, kelly).
mother(peggy, bud).
mother(martha, peggy).

grandma(X, Y) :- mother(X, Z), mother(Z, Y).

descendants(X, Y) :- mother(X, Y); father(X, Y); grandma(X, Y).

siblings(X, Y) :- mother(P, X), mother(P, Y), X \= Y.

transition(q0,q1,a).
transition(q1,q2,b).
transition(q0,q3,a).
transition(q3,q3,a).
accepting(q2).
accepting(q3).

accepts(State, []) :- accepting(State).
accepts(State, InputList) :- InputList = [First | Rest],
transition(State, X, First), accepts(X, Rest).
