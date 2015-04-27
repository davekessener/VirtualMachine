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

<INITIAL>{
{word}		{ ECHO; BEGIN WORD; }
{punc}		{ ECHO; BEGIN PUNC; }
{npunc}	{ ECHO; BEGIN NPUNC; }
}

\n					{ ECHO; BEGIN INITIAL; }

<WORD>{
{word}		{ printf(" %s", yytext); }
{punc}		{ ECHO; BEGIN PUNC; }
{npunc}		{ printf(" %s", yytext); BEGIN NPUNC; }
}

<PUNC>{
{word}		{ printf(" %s", yytext); BEGIN WORD; }
{punc}		{ ECHO; }
{npunc}		{ printf(" %s", yytext); BEGIN NPUNC; }
}

<NPUNC>{
{word}		{ ECHO; BEGIN WORD; }
{punc}		{ printf(" %s", yytext); BEGIN PUNC; }
{npunc}		{ ECHO; }
}

