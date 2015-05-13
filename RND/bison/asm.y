%{
	#include <stdio.h>
	#include <stdlib.h>
%}

%token <i> T_DB 
%token <s> T_ID 
%token <s> T_VAR 
%token <i> T_NUM 
%token <s> T_LBL 
%token T_DEFMACRO 
%token T_ENDMACRO

//%type <s> we_s
//%type <i> we_n

%left '<' '>'
%left '+' '-'
%left '*' '/'
%left '&' '|' '^'

%union {
	char *s;
	int i;
}

%%

//wtf	: wtf we
//	|
//	;
//
//we	: we_s { printf("'%s'\n", $1); }
//	| we_n { printf("'%d'\n", $1); }
//	| T_DEFMACRO { printf("T_DEFMACRO\n"); }
//	| T_ENDMACRO { printf("T_ENDMACRO\n"); }
//	| '\n' { printf("\\n\n"); }
//	| ','  { printf(",\n"); }
//	| '+'  { printf("+\n"); }
//	| '-'  { printf("-\n"); }
//	| '*'  { printf("*\n"); }
//	| '/'  { printf("/\n"); }
//	| '&'  { printf("&\n"); }
//	| '|'  { printf("|\n"); }
//	| '^'  { printf("^\n"); }
//	| '>'  { printf(">\n"); }
//	| '<'  { printf("<\n"); }
//	| '['  { printf("[\n"); }
//	| ']'  { printf("]\n"); }
//	| '('  { printf("(\n"); }
//	| ')'  { printf(")\n"); }
//	;
//
//we_s	: T_ID { printf("T_ID: "); }
//		| T_VAR { printf("T_VAR: "); }
//		| T_LBL { printf("T_LBL: "); }
//		;
//
//we_n	: T_DB { printf("T_DB: "); }
//		| T_NUM { printf("T_NUM: "); }
//		;

start	: start line '\n' { printf("matched a line\n"); }
		|
		;

line	: macro
		| db
		| statement
		;

statement	: T_ID expr_list { printf("%s\n", $1); }
			| T_LBL { printf("%s\n", $1); }
			;

expr_list	: expr expr_list2
			|
			;

expr_list2	: expr_list2 ',' expr
			|
			;

expr	: T_ID			{ printf("'%s'\n", $1); }
		| T_VAR			{ printf("'%s'\n", $1); }
		| T_NUM			{ printf("'%d'\n", $1); }
		| T_LBL			{ printf("'%s'\n", $1); }
		| '-' expr %prec '^'
		| expr '+' expr
		| expr '-' expr
		| expr '*' expr
		| expr '/' expr
		| expr '&' expr
		| expr '|' expr
		| expr '^' expr
		| expr '<' expr
		| expr '>' expr
		| '[' expr ']'
		| '(' expr ')'
		;

db	: T_DB db_bod { printf(":'%c'\n", (char)$1); }
	;

db_bod	: db_bod T_LBL { printf("'%s' ", $2); }
		| db_bod T_NUM { printf("%d ", $2); }
		|
		;

macro	: T_DEFMACRO T_ID var_list '\n' macro_body T_ENDMACRO { printf("-> %s\n", $2); }
		;

var_list	: T_VAR { printf("%s\n", $1); } var_list2
			|
			;

var_list2	: var_list2 ',' T_VAR { printf("%s\n", $3); }
			|
			;

macro_body	: macro_body statement '\n'
			|
			;

