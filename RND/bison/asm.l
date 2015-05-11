%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <assert.h>
	#include "y.tab.h"

	typedef const char *cstr_t;
	cstr_t ids[0x100];
	cstr_t equs[0x100];
	size_t i, line = 1;
	cstr_t id;

	FILE *infs[0x100];
	cstr_t incs[0x100];
	size_t inls[0x100];
	cstr_t inns[0x100];
	size_t f_i, i_i, l_i, n_i;
	cstr_t cfn;

	void insert(cstr_t, cstr_t);
	cstr_t get(cstr_t);

	cstr_t substitute(cstr_t);
	int evalnum(cstr_t);

	void loadfile(cstr_t);
	void doabort(void);

	char *lowerdup(cstr_t);

	void yyerror(cstr_t s) { }

	#define MXT_TOLOWER(c) (((c)>='A'&&(c)<='Z')?((c)+'a'-'A'):(c))
%}

a [aA]
b [bB]
c [cC]
d [dD]
e [eE]
f [fF]
g [gG]
h [hH]
i [iI]
j [jJ]
k [kK]
l [lL]
m [mM]
n [nN]
o [oO]
p [pP]
q [qQ]
r [rR]
s [sS]
t [tT]
u [uU]
v [vV]
w [wW]
x [xX]
y [yY]
z [zZ]

id			[a-zA-Z_][a-zA-Z0-9_]*
equ			\.{e}{q}{u}
inc			\.{i}{n}{c}
db			\.{d}[bBwWdDqQ]
defmacro	\.{d}{e}{f}{m}{a}{c}{r}{o}
endmacro	\.{e}{n}{d}{m}{a}{c}{r}{o}

op			[-+*/&|^()\[\],]|<<|>>
cur			\$
comment		;.*
label		:{id}

int		0|[1-9][0-9]*
oct		0[0-7]+
hex		0[xX][0-9a-fA-F]+
bin		0[bB][01]+
ch		\'\\?.\'
num		{ch}|{int}|{oct}|{hex}|{bin}

var		%{id}
mlbl	::[a-zA-Z0-9_]+

%s CONTENT

%s EQU
%s EQU_REST
%s EQU_END

%s MACRO
%s MACRO_BODY

%s INC
%s DB
%s EXPR
%s STR

%%

<INITIAL>{
[ \t\n]+	;
.			{ yyless(0); BEGIN CONTENT; }
.			{ fprintf(stderr, "WFT\n"); doabort(); }
}

<CONTENT>{
{inc}		{ BEGIN INC; }
{db}		{ BEGIN DB; yylval.i = MXT_TOLOWER(yytext[2]); return T_DB; }
{defmacro}	{ BEGIN MACRO; return T_DEFMACRO; }
{equ}		{ BEGIN EQU; }
{label}/[ \t]*{comment}?\n	{ yylval.s = lowerdup(yytext); return T_LBL; }
{id}		{ cstr_t t = substitute(yytext); if(t) { yylval.s = lowerdup(t); BEGIN EXPR; return T_ID; } }
{comment}	;
[ \t]+		;
.			{ fprintf(stderr, "ERR: %s: invalid character %c @%d!\n", cfn, *yytext, line); doabort(); }
\n			{ BEGIN INITIAL; ++line; return '\n'; }
}

<MACRO>{
{id}		{ yylval.s = lowerdup(yytext); return T_ID; }
,			{ return ','; }
{var}		{ yylval.s = lowerdup(yytext); return T_VAR; }
[ \t]+		;
\n			{ BEGIN MACRO_BODY; ++line; return '\n'; }
}

<MACRO_BODY>{
{id}			{ cstr_t t = substitute(yytext); if(t) { yylval.s = lowerdup(t); return T_ID; } }
{var}			{ yylval.s = lowerdup(yytext); return T_VAR; }
{op}			{ return *yytext; }
{num}			{ yylval.i = evalnum(yytext); return T_NUM; }
{mlbl}|{label}	{ yylval.s = lowerdup(yytext); return T_LBL; }
{cur}			{ return *yytext; }
{comment}		;
[ \t]+			;
\n[ \t\n]*		{ line += yyleng; return '\n'; }
{endmacro}		{ BEGIN CONTENT; return T_ENDMACRO; }
}

<EXPR>{
{id}		{ cstr_t t = substitute(yytext); if(t) { yylval.s = lowerdup(t); return T_ID; } }
{op}		{ return *yytext; }
{num}		{ yylval.i = evalnum(yytext); return T_NUM; }
{label}		{ yylval.s = lowerdup(yytext); return T_LBL; }
{cur}		{ return *yytext; }
{comment}	;
[ \t]+		;
\n			{ BEGIN INITIAL; ++line; return '\n'; }
}

<INC>{
\"[^\"]+\"	{ yytext[yyleng - 1] = '\0'; loadfile(yytext + 1); BEGIN INITIAL; }
[ \t]+		;
.|\n		{ fprintf(stderr, "ERR: %s: expected quoted filename @%d\n", cfn, line); doabort(); }
}

<DB>{
{num}		{ yylval.i = evalnum(yytext); return T_NUM; }
{label}		{ yylval.s = lowerdup(yytext); return T_LBL; }
\"			{ BEGIN STR; }
{cur}		{ return *yytext; }
[, \t]+		;
\n			{ BEGIN INITIAL; ++line; return '\n'; }
.			{ fprintf(stderr, "ERR: %s: invalid character '%c' @%d\n", cfn, *yytext, line); doabort(); }
}

<STR>{
[^\n\\\"]	{ yylval.i = *yytext; return T_NUM; }
\\.			{ yylval.i = evalnum(yytext); return T_NUM; }
\"			{ BEGIN DB; }
\n			{ fprintf(stderr, "ERR: %s: unexpected eol @%d!\n", cfn, line); doabort(); }
}

<EQU>{
{id}	{ id = lowerdup(yytext); BEGIN EQU_REST; }
[ \t]+	;
\n		{ fprintf(stderr, "ERR: %s: incomplete .equ @%d\n", cfn, line); doabort(); }
}

<EQU_REST>{
[^ \t\n](.*[^ \t\n])?	{ insert(id, lowerdup(yytext)); BEGIN EQU_END; }
[ \t]+	 			;
\n		{ fprintf(stderr, "ERR: %s: incomplete .equ @%d\n", cfn, line); doabort(); }
}

<EQU_END>{
[^ \t\n]+	{ fprintf(stderr, "ERR: %s: Invalid text after '.equ': '%s' @%d\n", cfn, yytext, line); doabort(); }
[ \t]+	;
\n		{ BEGIN INITIAL; ++line; }
}

%%

int yywrap(void)
{
	if(f_i)
	{
		yypop_buffer_state();
		free((void *)cfn);
		fclose(infs[--f_i]);
		line = inls[--l_i];
		cfn  = inns[--n_i];
		return 0;
	}
	else return 1;
}

int main(int argc, char *argv[])
{
	assert(argc>1);
	yyin = fopen(argv[1], "r");
	cfn = strdup(argv[1]);
	yyparse();
	free((void *)cfn);
	fclose(yyin);
	return 0;
}

void loadfile(cstr_t fn)
{
	size_t j;
	for(j = 0 ; j < i_i ; ++j)
	{
		if(strcmp(incs[j], fn) == 0)
		{
			fprintf(stderr, "Warning: Cyclic dependency of '%s'!\n", fn);
		}
	}

	FILE *f = fopen(fn, "r");

	if(!f)
	{
		fprintf(stderr, "ERR: Couldn't open file '%s'!\n", fn);
		doabort();
	}

	incs[i_i++] = strdup(fn);

	infs[f_i++] = f;
	inls[l_i++] = line;
	inns[n_i++] = cfn;
	yypush_buffer_state(yy_create_buffer(f, YY_BUF_SIZE));
	line = 1;
	cfn = strdup(fn);
}

void insert(cstr_t id, cstr_t equ)
{
	cstr_t s = get(id);

	if(s)
	{
		fprintf(stderr, "ERR: Redifinition of '%s': '%s' -> '%s'!\n", id, s, equ);
		doabort();
	}

	ids[i] = id;
	equs[i] = equ;
	++i;
}

char *lowerdup(cstr_t s)
{
	size_t l = strlen(s);
	char *r = malloc(++l);

	while(l--)
	{
		r[l] = MXT_TOLOWER(s[l]);
	}

	return r;
}

void doabort(void)
{
	size_t j;
	for(j = 0 ; j < f_i ; ++j)
	{
		fclose(infs[j]);
	}
	exit(1);
}

int evalnum(cstr_t s)
{
	int v = 0, b = 10;

	if(*s == '0')
	{
		if(!*++s) return 0;

		switch(*s++)
		{
			case 'x':
			case 'X':
				b = 16;
				break;
			case 'b':
			case 'B':
				b = 2;
				break;
			default:
				b = 8;
				break;
		}
	}
	else if(*s == '\'')
	{
		if(*++s == '\\')
		{
			switch(*++s)
			{
				case '0': return '\0';
				case 't': return '\t';
				case 'n': return '\n';
				case 'r': return '\r';
				case '\\': return '\\';
				case '\'': return '\'';
				default: return *s;
			}
		}
		else return *s;
	}

	do
	{
		v = b * v + (*s >= '0' && *s <= '9' ? *s - '0' : MXT_TOLOWER(*s) - 'a');
	} while(*++s);

	return v;
}

cstr_t get(cstr_t id)
{
	size_t j;
	for(j = 0 ; j < i ; ++j)
	{
		if(strcmp(ids[j], id) == 0) return equs[j];
	}

	return NULL;
}

cstr_t substitute(cstr_t id)
{
	cstr_t s = get(id);

	if(!s)
	{
		return id;
	}
	else
	{
		size_t p = strlen(s);
		while(p)
		{
			unput(s[--p]);
		}

		return NULL;
	}
}


