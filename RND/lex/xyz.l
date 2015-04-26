%%

SIE	{ printf("Y"); REJECT; }
ER	{ printf("X"); }
ES	{ printf("Z"); REJECT; }
.|\n	;

%%

int main(int argc, char *argv[])
{
	yylex();

	printf("\n");

	return 0;
}

