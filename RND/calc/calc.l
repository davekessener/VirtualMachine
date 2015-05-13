%{
	#include <stdlib.h>
	#include "y.tab.h"
%}

number	(0|[1-9][0-9]*)(\.[0-9]+)?([eE][+-]?[1-9][0-9]*)?
ops		[-+*/^()\[\],$=]
var		[a-z]

%%

{number}	{ yylval = atof(yytext); return T_NUM; }
{ops}		{ return *yytext; }
{var}		{ yylval = *yytext - 'a'; return T_VAR; }
\n			{ return '\n'; }
[ \t]+		;
.			{ fprintf(stderr, "ERR: Unrecognized character: '%c'!\n", *yytext); exit(1); }

