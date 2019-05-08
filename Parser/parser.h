#ifndef PARSER_H
#define PARSER_H

#include "tokens_names.h"
#define SUCCESS 0
#define FAIL 1

/* === Non-Terminals Functions === */
int PROGRAM();
int VAR_DEFINITIONS();
int STATEMENTS();
int FUNC_DEFINITIONS();
int error();

int parser();
#endif