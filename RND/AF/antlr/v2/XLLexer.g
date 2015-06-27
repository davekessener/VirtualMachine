class XLLexer extends Lexer;

options {
	charVocabulary = '\0'..'\377';
	testLiterals = false;
	k = 2;
}

WS
	: ( ' '
	  | '\t'
	  | '\f'
	  | ( "\r\n"
	    | '\r'
		| '\n'
		)
		{ newline(); }
	  )
	  { $setType(Token.SKIP); }
	;

COMMENT
	: "//" (~('\n' | '\r'))*
	  { $setType(Token.SKIP); }
	;

INTLIT
	: ('0'..'9')+
	;

CHARLIT
	: '\''! . '\''!
	;

STRINGLIT
	: '"'!
	  ( '"' '"'!
	  | ~('"' | '\n' | '\r')
	  )*
	  ( '"'!
	  |
	  )
	;

IDENT
	options { testLiterals = true; }
	: ( 'a'..'z' | 'A'..'Z') ('a'..'z'|'A'..'Z'|'0'..'9')*
	;

DOT        : '.'   ;
BECOMES    : ":="  ;
COLON      : ':'   ;
SEMI       : ';'   ;
COMMA      : ','   ;
EQUALS     : '='   ;
LBRACKET   : '['   ;
RBRACKET   : ']'   ;
DOTDOT     : ".."  ;
LPAREN     : '('   ;
RPAREN     : ')'   ;
NOT_EQUALS : "/="  ;
LT         : '<'   ;
LTE        : "<="  ;
GT         : '>'   ;
GTE        : ">="  ;
PLUS       : '+'   ;
MINUS      : '-'   ;
TIMES      : '*'   ;
DIV        : '/'   ;

