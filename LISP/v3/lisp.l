%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdargs.h>
	#include <assert.h>
	#include "y.tab.h"

	#define MXT_BUFSIZE 0x1000

	char sbuf[MXT_BUFSIZE];
	size_t s_i;

	void push_s(char);
	void lex_error(const char *, ...);
%}

int		[-+]?(0|[1-9][0-9]*)
decimal	\.[0-9]+
exp		[eE][-+]?[0-9]+
float	{int}({decimal}{exp}?|{decimal}?{exp})
symbol	[a-zA-Z_:.][a-zA-Z0-9_?!:.-]*
ops		[-+*/^()']

%s STRING

%%

<INITIAL>{
[ \t]+	;

{int}		{ yylval.i = atoi(yytext); return T_INT; }
{float}		{ yylval.d = atof(yytext); return T_FLOAT; }
{symbol}	{ yylval.s = strdup(yytext); return T_SYM; }
{ops}		{ return *yytext; }
\"			{ s_i = '\0'; BEGIN STRING; }
.|\n		{ fprintf(stderr, "Ignoring '%c'\n", *yytext); }
}

<STRING>{
[^\"\\]		{ push_s(*yytext); }
\\\"		{ push_s('"'); }
\\n			{ push_s('\n'); }
\\r			{ push_s('\r'); }
\\t			{ push_s('\t'); }
\\.			{ lex_error("ERR: Invalid char '%c' in string.\n", yytext[1]); }
\"			{ push_s('\0'); yylval.s = strdup(sbuf); BEGIN INITIAL; return T_STR; }
}

%%

void push_s(char c)
{ 
	assert(s_i<MXT_BUFSIZE);
	sbuf[s_i++] = c;
}

void lex_error(const char *s, ...)
{
	va_list vl;

	va_start(vl, s);

	vfprintf(stderr, s, vl);

	va_end(vl);

	exit(1);
}

