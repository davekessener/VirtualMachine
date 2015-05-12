%{
	#define SPACE printf(" ")
%}

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

[ \t]+		;
\n			{ ECHO; BEGIN INITIAL; }

<INITIAL>{
{word}		{ ECHO; BEGIN WORD; }
{punc}		{ ECHO; BEGIN PUNC; }
{npunc}		{ ECHO; BEGIN NPUNC; }
}

<WORD>{
{word}		{ SPACE; ECHO; }
{punc}		{ ECHO; BEGIN PUNC; }
{npunc}		{ SPACE; ECHO; BEGIN NPUNC; }
}

<PUNC>{
{word}		{ SPACE; ECHO; BEGIN WORD; }
{punc}		{ ECHO; }
{npunc}		{ SPACE; ECHO; BEGIN NPUNC; }
}

<NPUNC>{
{word}		{ ECHO; BEGIN WORD; }
{punc}		{ SPACE; ECHO; BEGIN PUNC; }
{npunc}		{ ECHO; }
}

