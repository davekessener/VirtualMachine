grammar KiloP;

A : [aA] ;
B : [bB] ;
C : [cC] ;
D : [dD] ;
E : [eE] ;
F : [fF] ;
G : [gG] ;
H : [hH] ;
I : [iI] ;
J : [jJ] ;
K : [kK] ;
L : [lL] ;
M : [mM] ;
N : [nN] ;
O : [oO] ;
P : [pP] ;
Q : [qQ] ;
R : [rR] ;
S : [sS] ;
T : [tT] ;
U : [uU] ;
V : [vV] ;
W : [wW] ;
X : [xX] ;
Y : [yY] ;
Z : [zZ] ;

PROGRAM : P R O G R A M ;
BEGIN : B E G I N ;
END : E N D ;
IF : I F ;
ELSE : E L S E ;
THEN : T H E N ;
FI : F I ;
WHILE : W H I L E ;
DO : D O ;
DONE : D O N E ;
REPEAT : R E P E A T ;
UNTIL : U N T I L ;
VAR : V A R ;
RETURN : R E T U R N ;

WS : [ \t\n\r]+ -> skip;

ID
	: [a-zA-Z][a-zA-Z0-9_]*
	;

NUM
	: [1-9][0-9]* ('.' [0-9]*)?
	;

STR
	: '\''
	  ( ~[']
	    ('\'' WS '\'')?
	  )*
	  '\''
	;



