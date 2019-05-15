%option yylineno
%option noyywrap

%{
#include "Token/Token.h"
#include "Actions/actions.h"
#include "Parser/parser.h"
FILE *parser_out = NULL;
int debug = 0;
%}

DIGIT	  [1-9]
DIGITZ    [0-9]
ID	      [a-zA-Z]+{DIGITZ}*(_([a-zA-Z]|{DIGITZ})+)*

%% 

0|{DIGIT}{DIGITZ}*	 					   return number_action(TOKEN_INT_NUMBER, yytext, yylineno);
{DIGIT}+"."{DIGITZ}+|0"."{DIGITZ}+		return number_action(TOKEN_REAL_NUMBER, yytext, yylineno);
"*"|"/"|"="								      return operator_action(TOKEN_OP, yytext, yylineno);
program|end|real|integer|void|return 	return keyword_action(TOKEN_KEYWORD, yytext, yylineno);
"["|"]"|"{"|"}"|","|";"|"("|")"			return sep_action(TOKEN_SEP, yytext, yylineno);
{ID}	       							      return id_action(TOKEN_ID, yytext, yylineno);
<<EOF>> 								         return eof_action(TOKEN_EOF, yytext, yylineno); 
"--".*|[[:space:]]+					     	;
. 										         print_error(yytext, yylineno);

%%

int main(int argc, char** argv)
{
	printf("Start running scanner\n");
   ++argv, --argc;	/* skip over program name */

   if (argc >= 1)
   {
		 yyin = fopen(argv[0], "r");
     if (argc == 2)
         debug = 1;
   }
   else
       yyin = stdin;

   parser_out = fopen("parse.txt", "w");
   yyout = fopen("scanner.txt", "w");

   parser();

   printf("DONE\n");
   fclose(yyin);
   fclose(yyout);
   fclose(parser_out);
   return 0;
}
