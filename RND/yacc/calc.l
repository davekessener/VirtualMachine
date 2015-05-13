%{
	#include <stdlib.h>
	#include "calc.tab.h"

	void yyerror(const char *);
%}

%%

[0-9]+		{ yylval = atoi(yytext); return INTEGER; }
[a-z]		{ yylval = *yytext - 'a'; return VARIABLE; }
\$			{ return ANS; }
[-+*/()=\n]	{ return *yytext; }
[ \t]+		;
.			{ yyerror("Invalid character."); }

%%

int yywrap(void) { return 1; }

