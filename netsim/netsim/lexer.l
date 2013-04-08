%{
#include <stdio.h>
#include "parser.hpp"
%}

%%

[0-9]+			{yylval.val = atoi(yytext); return number;}
0x[0-9a-fA-F]+		{(void) sscanf(yytext, "%x", &yylval.val); return number;}
[A-Za-z][A-Za-z_0-9]*	{strcpy (yylval.string, yytext); return identifier;}
\"[^\"]*\"		{strcpy(yylval.string, yytext+1); yylval.string[yyleng-2] = 0; return quotedstr;}
[(),]			return yytext[0];
[ \t\n]			;
"#"[^\n]*		;
.			{ECHO; yyerror ("unexpected character");}

%%

int yywrap ()
{
  return 1;
}

int yyerror(char* s)
{
  fprintf(stderr, "%s\n", s);
}
