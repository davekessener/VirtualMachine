grammar MiniP;

INT_T : 'integer' ;
REAL_T : 'real' ;
STR_T : 'string' ;

PLUS : '+' ;
MINUS : '-';
MUL : '*' ;
DIV : '/' ;
EQ : '==' ;
NE : '!=' | '<>' ;
GT : '>' ;
GE : '>=' ;
LT : '<' ;
LE : '<=' ;
AND : '&&' ;
OR : '||' ;

COMMENT
	: '/*' ~[*]* [*]+ (~[*/] ~[*]* [*]+)* '/'
	-> skip
	;

WS
	: [ \t\f\r\n]+
	-> skip
	;

ID
	: [a-zA-Z] [a-zA-Z0-9_]*
	;

INT
	: [1-9] [0-9]*
	| '0'
	;

REAL
	: INT '.' [0-9]+
	;

STR
	: '\'' ~[']* '\''
	;

program
	: 'program' decl* 'begin' statement* 'end'
	;

decl
	: datatype=(INT_T | REAL_T | STR_T) ID (',' ID)* ';'
	;

statement
	: ( assignment
	  | ifStatement
	  | whileStatement
	  | repeatStatement
	  | ioStatement
	  )
	  ';'
	;

assignment
	: ID ':=' expr
	;

ifStatement
	: 'if' expr 'then' statements ('else' statements)? 'fi'
	;

statements
	: statement+
	;

whileStatement
	: 'while' expr 'do' statement+ 'od'
	;

repeatStatement
	: 'repeat' statement+ 'until' expr
	;

ioStatement
	: 'read' '(' ID ')' #readStatement
	| 'println' '(' expr ')' #printlnStatement
	;

expr
	: MINUS expr #unaryExpr
	| expr op=(MUL|DIV) expr #mulExpr
	| expr op=(PLUS|MINUS) expr #addExpr
	| expr op=(LT|LE|GT|GE) expr #relExpr
	| expr op=(EQ|NE) expr #eqExpr
	| expr op=AND expr #andExpr
	| expr op=OR expr #orExpr
	| '(' expr ')' #parExpr
	| STR #strExpr
	| val=(INT|REAL) #numExpr
	| ID #idExpr
	;

//  expr : orExpr (OR orExpr)* ;
//  orExpr : andExpr (AND andExpr)* ;
//  andExpr : eqExpr (op=(EQ|NE) eqExpr)* ;
//  eqExpr : relExpr (op=(LT|LE|GT|GE) relExpr)* ;
//  relExpr : addExpr (op=(PLUS|MINUS) addExpr)* ;
//  addExpr : mulExpr (op=(MUL|DIV) mulExpr)* ;
//  mulExpr : MINUS? unaryExpr ;
//  unaryExpr
//  	: '(' expr ')'
//  	| STR
//  	| val=(INT|REAL)
//  	| ID
//  	;

