#ifndef ACTIONS_H
#define ACTIONS_H

#include "../Token/Token.h"

extern FILE *yyout;

void print_error(char*, int);
void print_token_found(eTOKENS, char*, int);
int id_action(eTOKENS, char*, int);
int operator_action(eTOKENS, char*, int);
int number_action(eTOKENS, char*, int);
int sep_action(eTOKENS, char*, int);
int keyword_action(eTOKENS, char*, int);
int eof_action(eTOKENS, char*, int);

#endif