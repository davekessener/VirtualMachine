%{
	#include <stdio.h>
	#include <stdlib.h>

	void yyerror(const char *s) { fprintf(stderr, "ERR: %s\n", s); }
%}

//%union {
//	double d;
//	int i;
//	void *c;
//}

%token <d> T_NUM
%token <i> T_VAR

//%type <c> statement
//%type <c> lval
//%type <c> expr
//%type <c> varlist
//%type <c> vl2
//%type <c> exprlist
//%type <c> el2
//%type <c> O1
//%type <c> O2
//%type <c> O3
//%type <c> O4
//%type <c> O5

%%

S : T_VAR { printf("a var '%c'\n", $1 + 'a'); } ;

//start		: start statement '\n'		{ eval($2); }
//			|
//			;
//
//statement	: lval '=' expr				{ $$ = assign($1, $3); }
//			| expr						{ $$ = $1; }
//			;
//
//lval		: T_VAR '(' varlist ')'		{ $$ = function($1, $3); }
//			| T_VAR				   		{ $$ = variable($1); }
//			;
//
//varlist		: T_VAR vl2			   		{ $$ = var_list($1, $2); } ;
//
//vl2			: vl2 ',' T_VAR		   		{ $$ = append($1, var_list($3, NULL)); }
//			| 					   		{ $$ = NULL; }
//			;
//
//exprlist	: expr el2					{ $$ = expr_list($1, $2); } ;
//
//el2			: el2 ',' expr				{ $$ = append($1, expr_list($3, NULL)); }
//			|							{ $$ = NULL; }
//			;
//
//expr		: O1 ;
//O1			: O1 '+' O2 		   		{ $$ = op_add($1, $3); }
//			| O1 '-' O2 		   		{ $$ = op_sub($1, $3); }
//			| O2
//			;
//
//O2			: O2 '*' O3 		   		{ $$ = op_mul($1, $3); }
//			| O2 '/' O3 		   		{ $$ = op_div($1, $3); }
//			| O3
//			;
//
//O3			: O4 '^' O3 		   		{ $$ = op_exp($1, $3); }
//			| O4
//			;
//
//O4			: '-' O5			   		{ $$ = op_neg($2); }
//			| O5
//			;
//
//O5			: '(' O1 ')'		   		{ $$ = $2; }
//			| T_NUM				   		{ $$ = number($1); }
//			| T_VAR				   		{ $$ = variable($1); }
//			| T_VAR '(' exprlist ')'	{ $$ = fncall($1, $3); }
//			;

