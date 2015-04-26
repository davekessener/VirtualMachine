%s	WORD
%s	PUNC
%s	NPUNC

digit	[0-9]
nndigit	[1-9]
natural	{nndigit}{digit}*
integer	0|-?{natural}
decimal	{integer}\.{digit}*|{integer}?\.{digit}*{nndigit}+{digit}*
number	{decimal}([eE]-?{natural})?
word	({number}|[a-zA-Z_][a-zA-Z0-9_]*)+

punc	[.,:;?!)]
npunc	[(]
%%

[ \t]+	;

<INITIAL>{word}		{ ECHO; BEGIN WORD; }
<INITIAL>{punc}		{ ECHO; BEGIN PUNC; }
<INITIAL>{npunc}	{ ECHO; BEGIN NPUNC; }

\n					{ ECHO; BEGIN INITIAL; }

<WORD>{word}		{ printf(" %s", yytext); }
<WORD>{punc}		{ ECHO; BEGIN PUNC; }
<WORD>{npunc}		{ printf(" %s", yytext); BEGIN NPUNC; }

<PUNC>{word}		{ printf(" %s", yytext); BEGIN WORD; }
<PUNC>{punc}		{ ECHO; }
<PUNC>{npunc}		{ printf(" %s", yytext); BEGIN NPUNC; }

<NPUNC>{word}		{ ECHO; BEGIN WORD; }
<NPUNC>{punc}		{ printf(" %s", yytext); BEGIN PUNC; }
<NPUNC>{npunc}		{ ECHO; }

