%option yylineno
%option noyywrap

%{
#include "Token/Token.h"
#include "Actions/actions.h"
#include "Parser/parser.h"
FILE *parser_out = NULL;
FILE *semantic_out = NULL;
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
  int test;
  char filename[100]={0};
  eTOKENS kind;

  for(test = 1 ; test <= 2 ; test++)
  {
    sprintf(filename,"%s%d%s","//home//ubuntu//Documents//test",test,".txt");
    yyin = fopen(filename,"r");

    sprintf(filename,"%s%d%s","//home//ubuntu//Documents//test",test,"_301606810_302677463_lex.txt");
    yyout = fopen(filename,"w");

    sprintf(filename,"%s%d%s","//home//ubuntu//Documents//test",test,"_301606810_302677463_syntactic.txt");    
    parser_out = fopen(filename, "w");

    sprintf(filename,"%s%d%s","//home//ubuntu//Documents//test",test,"_301606810_302677463_semantic.txt");    
    semantic_out = fopen(filename, "w"); 


    parser();

    fclose(yyin);
    fclose(yyout);
    fclose(parser_out);
    fclose(semantic_out);
    yyin = NULL;
    yyout = NULL;
    parser_out = NULL;
    semantic_out = NULL;
    yylineno = 1;
    clean_stored_tokes();
  }
  
  printf("DONE\n");
  return 0;
}
