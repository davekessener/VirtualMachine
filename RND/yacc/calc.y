%{
	#include <stdlib.h>
	#include <stdio.h>

	void yyerror(const char *);
	int yylex(void);

	extern FILE *yyin, *yyout;

	int vars['z' - 'a' + 1] = {};
	int ans = 0;
%}

%token INTEGER
%token VARIABLE
%token ANS

%left '+' '-'
%left '*' '/'

%%

S : S assign '\n'	{ printf("%d\n", $2); }
  |
  ;

assign	: VARIABLE '=' expr	{ ans = vars[$1] = $3; $$ = $3; }
		| expr				{ ans = $1; $$ = $1; }
		;

expr	: INTEGER		{ $$ = $1; }
		| ANS			{ $$ = ans; }
		| VARIABLE		{ $$ = vars[$1]; }
		| expr '+' expr	{ $$ = $1 + $3; }
		| expr '-' expr { $$ = $1 - $3; }
		| expr '*' expr { $$ = $1 * $3; }
		| expr '/' expr { $$ = $1 / $3; }
		| '(' expr ')'	{ $$ = $2; }
		;

%%

void yyerror(const char *s)
{
	fprintf(stderr, "%s\n", s);
}

int main(int argc, char *argv[])
{
	if(argc > 1)
	{
		yyin = fopen(argv[1], "r");
	}

	yyparse();

	if(argc > 1)
	{
		fclose(yyin);
	}
	
	return 0;
}

