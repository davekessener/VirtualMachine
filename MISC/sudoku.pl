is_unsorted_equal([],[]).
is_unsorted_equal([H|T],L) :-
	is_in_list(H,L),
	not_is_in_list(H,T),
	is_unsorted_equal(T,L).

is_in_list(X,[X|_]).
is_in_list(X,[_|T]) :-
	is_in_list(X,T).

not_is_in_list(_,[]).
not_is_in_list(X,[_|T]) :-
	not_is_in_list(X,T).


solve_sudoku(Field) :-
	evaluate_sudoku(Field, [1,2,3,4,5,6,7,8,9]).

evaluate_sudoku([A1,B1,C1,D1,E1,F1,G1,H1,I1,
				 A2,B2,C2,D2,E2,F2,G2,H2,I2,
				 A3,B3,C3,D3,E3,F3,G3,H3,I3,
				 A4,B4,C4,D4,E4,F4,G4,H4,I4,
				 A5,B5,C5,D5,E5,F5,G5,H5,I5,
				 A6,B6,C6,D6,E6,F6,G6,H6,I6,
				 A7,B7,C7,D7,E7,F7,G7,H7,I7,
				 A8,B8,C8,D8,E8,F8,G8,H8,I8,
				 A9,B9,C9,D9,E9,F9,G9,H9,I9],C) :-
	is_unsorted_equal([A1,B1,C1,D1,E1,F1,G1,H1,I1],C),
	is_unsorted_equal([A2,B2,C2,D2,E2,F2,G2,H2,I2],C),
	is_unsorted_equal([A3,B3,C3,D3,E3,F3,G3,H3,I3],C),
	is_unsorted_equal([A4,B4,C4,D4,E4,F4,G4,H4,I4],C),
	is_unsorted_equal([A5,B5,C5,D5,E5,F5,G5,H5,I5],C),
	is_unsorted_equal([A6,B6,C6,D6,E6,F6,G6,H6,I6],C),
	is_unsorted_equal([A7,B7,C7,D7,E7,F7,G7,H7,I7],C),
	is_unsorted_equal([A8,B8,C8,D8,E8,F8,G8,H8,I8],C),
	is_unsorted_equal([A9,B9,C9,D9,E9,F9,G9,H9,I9],C),
	is_unsorted_equal([A1,A2,A3,A4,A5,A6,A7,A8,A9],C),
	is_unsorted_equal([B1,B2,B3,B4,B5,B6,B7,B8,B9],C),
	is_unsorted_equal([C1,C2,C3,C4,C5,C6,C7,C8,C9],C),
	is_unsorted_equal([D1,D2,D3,D4,D5,D6,D7,D8,D9],C),
	is_unsorted_equal([E1,E2,E3,E4,E5,E6,E7,E8,E9],C),
	is_unsorted_equal([F1,F2,F3,F4,F5,F6,F7,F8,F9],C),
	is_unsorted_equal([G1,G2,G3,G4,G5,G6,G7,G8,G9],C),
	is_unsorted_equal([H1,H2,H3,H4,H5,H6,H7,H8,H9],C),
	is_unsorted_equal([I1,I2,I3,I4,I5,I6,I7,I8,I9],C),
	is_unsorted_equal([A1,B1,C1,A2,B2,C2,A3,B3,C3],C),
	is_unsorted_equal([A4,B4,C4,A5,B5,C5,A6,B6,C6],C),
	is_unsorted_equal([A7,B7,C7,A8,B8,C8,A9,B9,C9],C),
	is_unsorted_equal([D1,E1,F1,D2,E2,F2,D3,E3,F3],C),
	is_unsorted_equal([D4,E4,F4,D5,E5,F5,D6,E6,F6],C),
	is_unsorted_equal([D7,E7,F7,D8,E8,F8,D9,E9,F9],C),
	is_unsorted_equal([G1,H1,I1,G2,H2,I2,G3,H3,I3],C),
	is_unsorted_equal([G4,H4,I4,G5,H5,I5,G6,H6,I6],C),
	is_unsorted_equal([G7,H7,I7,G8,H8,I8,G9,H9,I9],C).

