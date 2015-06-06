is_sentence(W) :-
	is_np(W,U),
	is_rc(U).

is_np([A,B|X],X) :-
	is_det(A),
	is_n(B).
is_np([A|X],X) :-
	is_pn(A).

is_rc([A|X]) :-
	is_v(A),
	is_lc(X).

is_lc(W) :-
	is_np(W,[]).
is_lc([]).

is_det(the).
is_pn(john).
is_n(man).
is_v(sings).

/*
S -> NP RC
NP -> Noun | Art Name
RC -> Verb LC
LC -> NP | e
*/

