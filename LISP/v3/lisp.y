%{
	#include <stdint.h>
	#include "interface.h"

	void yyerror(char *);
%}

%union {
	double d;
	char *s;
	uint64_t i;
}

%token <i> T_INT
%token <d> T_FLOAT
%token <s> T_SYM
%token <s> T_STR

%type <i> expr list el el2

%%

start	:
			expr
				{ eval($1); }
		;

expr	:
			'\'' list
				{ $$ = cons(symbol("QUOTE"), $2); }
		|	list
				{ $$ = $1; }
		|	T_INT
				{ $$ = number_i($1); }
		|	T_FLOAT
				{ $$ = number_f($1); }
		|	T_SYM
				{ $$ = symbol($1); }
		|	T_STR
				{ $$ = string($1); }
		;

list	:
			'(' el ')'
				{ $$ = $2; }
		;

el		:
			expr el2
				{ $$ = cons($1, $2); }
		|
				{ $$ = 0; }
		;

el2		:
			el2 ',' expr
				{ $$ = append($1, $3); }
		|
				{ $$ = 0; }
		;
		

