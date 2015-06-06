is_sentence(S) :-
	ps(S,[]).

s(X,Y) :-
	ps(X,U),
	sk(U,Y).

sk(X,X).
sk([K|X],Y) :-
	is_conjunction(K),
	ps(X,U),
	sk(U,Y).

ps(X,Y) :-
	np(X,U),
	vp(U,V),
	opp(V,Y).
ps(X,Y) :-
	opp(X,U),
	np(U,V),
	vp(V,Y).

np([D|X],Y) :-
	is_artikel(D),
	adj(X,U),
	U = [N|Y],
	is_noun(N).
np([E|X],X) :-
	is_name(E).

vp([V|X],Y) :-
	is_verb(V),
	onp(X,Y).

pp([P|X],Y) :-
	is_preposition(P),
	np(X,Y).

onp(X,X).
onp(X,Y) :-
	np(X,Y).

opp(X,X).
opp(X,Y) :-
	pp(X,Y).

adj(X,X).
adj([A|X],Y) :-
	is_adjective(A),
	adj2(X,Y).

adj2(X,X).
adj2(X,Y) :-
	adj3(X,Y).
adj2([A|X],Y) :-
	is_adjective(A),
	adj3(X,Y).

adj3([and,A|X],X) :-
	is_adjective(A).
adj3([A|X],Y) :-
	is_adjective(A),
	adj3(X,Y).

is_artikel(the).
is_artikel(a).
is_noun(apple).
is_noun(man).
is_noun(tree).
is_name(john).
is_verb(eats).
is_verb(likes).
is_preposition(under).
is_adjective(ugly).
is_adjective(big).
is_adjective(green).
is_conjunction(and).

