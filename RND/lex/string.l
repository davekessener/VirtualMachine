%%

\"[^\"]*	{ 	if(yytext[yyleng - 1] == '\\')
				{
					yymore();
				}
				else
				{
					printf("%s\n", yytext + 1);
					input();
				}
			}
.|\n	;

