%option yylineno
%{
#include "Token/Token.h"
#include "Actions/actions.h"
%}

DIGIT	  [1-9]
DIGITZ    [0-9]
ID	      [a-zA-Z]+{DIGITZ}*(_([a-zA-Z]|{DIGITZ})+)*

%% 

0|{DIGIT}{DIGITZ}*	 					number_action(TOKEN_INT_NUMBER, yytext, yylineno);
{DIGIT}+"."{DIGITZ}+|0"."{DIGITZ}+		number_action(TOKEN_REAL_NUMBER, yytext, yylineno);
"*"|"/"|"="								operator_action(TOKEN_OP, yytext, yylineno);
program|end|real|integer|void|return 	keyword_action(TOKEN_KEYWORD, yytext, yylineno);
"["|"]"|"{"|"}"|","|";"|"("|")"			sep_action(TOKEN_SEP, yytext, yylineno);
{ID}	       							id_action(TOKEN_ID, yytext, yylineno);
<<EOF>> 								{eof_action(TOKEN_EOF, yytext, yylineno); return 0;}
"--"+|[[:space:]]+						;
. 										print_error(yytext, yylineno);

%%

int main(int argc, char** argv)
{
   ++argv, --argc;	/* skip over program name */

   if (argc == 1)
    	yyin = fopen(argv[0], "r");
   else
      yyin = stdin;

   yylex();
}


int yywrap()
{
	return 1;
}