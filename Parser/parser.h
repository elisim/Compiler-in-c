#ifndef PARSER_H
#define PARSER_H
#include "../Token/Token.h"

#define SUCCESS 0
#define FAIL 1


extern FILE *parser_out;
extern int debug;

// void error(eTOKENS expected[]);
void error();
int match(eTOKENS token);
void parser();
void output(char* rule);
int contains_in(eTOKENS arr[], eTOKENS token, int size);
char *concatenate(size_t size, eTOKENS *array, const char *joint);


/* === Parsing Functions === */
void PROGRAM();
void VAR_DEFINITIONS();
void VAR_DEFINITIONS_TEMP();
void VAR_DEFINITION();
void TYPE();
void VARIABLES_LIST();
void VARIABLES_LIST_TEMP();
void VARIABLE();
void VARIABLE_TEMP();
void FUNC_DEFINITIONS();
void FUNC_DEFINITIONS_TEMP();
void FUNC_DEFINITION();
void RETURNED_TYPE();
void PARAM_DEFINITIONS();
void STATEMENTS();
void STATEMENTS_TEMP();
void STATEMENT();
void STATEMENT_TEMP();
void STATEMENT_TWO_TEMP();
void BLOCK();
void FUNCTION_CALL();
void PARAMETERS_LIST();
void EXPRESSION();
void EXPRESSION_TEMP();
#endif