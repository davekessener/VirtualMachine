idstart	[a-zA-Z]
idchar	[a-zA-Z0-9_]
id		{idstart}{idchar}*
label	:({id}|\%[0-9]+)
var		\%{id}
pre		\.{id}
int		-?([1-9][0-9]*|0)
hex		0x[0-9a-fA-F]{4}
bin		0b[01]{16}

%s	COMMENT
%s	STRING

%{
	extern union { int v; char *s; } yylval;
	int readhex(const char *);
	int readbin(const char *);
%}

%%

<INITIAL>{
\.		{ return T_PRE; }
{id}	{ yylval.s = strdup(yytext); return T_ID; }
{int}	{ yylval.v = atoi(yytext); return T_NUM; }
{hex}	{ yylval.v = readhex(yytext); return T_NUM; }
{bin}	{ yylval.v = readbin(yytext); return T_NUM; }
[-+*/&|^]	|
<<|>>	{ yylval.s = strdup(yytext); return T_OP; }
{label}	{ yylval.s = strdup(yytext); return T_LBL; }
{var}	{ yylval.s = strdup(yytext); return T_VAR; }
;;		{ BEGIN COMMENT; }
,		{ return T_COMMA; }
(		{ return T_OPENP; }
)		{ return T_CLOSEP; }
\[		{ return T_MEMO; }
\]		{ return T_MEMC; }
\"		{ BEGIN STRING; }
\n		{ return T_EOL; }
[ \t]+	;
}

<STRING>{
[^\"\\]	{ yylval. = *yytext; return T_LITERAL; }
\\\"	{ yylval. = '"'; return T_LITERAL; }
\\n		{ yylval. = '\n'; return T_LITERAL; }
\\r		{ yylval. = '\r'; return T_LITERAL; }
\\t		{ yylval. = '\t'; return T_LITERAL; }
\"		{ BEGIN INITIAL; }
}

<COMMENT>{
.	;
\n	{ BEGIN INITIAL; }
}

%%

