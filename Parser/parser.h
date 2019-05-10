#ifndef PARSER_H
#define PARSER_H
#include "../Token/Token.h"

#define SUCCESS 0
#define FAIL 1

#include "tokens_names.h"

int error();

int match(eTOKENS token);

/* === Non-Terminals Functions === */
int PROGRAM();
int VAR_DEFINITIONS();
int VAR_DEFINITION();
int TYPE();
int VARIABLES_LIST();
int VARIABLES_LIST_TEMP();
int VARIABLE();
int FUNC_DEFINITIONS();
int FUNC_DEFINITIONS_TEMP();
int FUNC_DEFINITION();
int RETURNED_TYPE();
int PARAM_DEFINITIONS();
int STATEMENTS();
int STATEMENT();
int BLOCK();
int FUNCTION_CALL();
int PARAMETERS_LIST();
int EXPRESSION();
#endif