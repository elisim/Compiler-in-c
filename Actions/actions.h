#ifndef ACTIONS_H
#define ACTIONS_H

#include "../Token/Token.h"

static const char* token_kinds[] = 
{
	"IDENTIFIER", 

	"OPERATOR",
	"OP_EQUAL",
	"OP_MUL",
	"OP_DIV",

	"NUMBER",
	"INT_NUMBER",
	"REAL_NUMBER",

	"KEYWORD",
	"KEYWORD_PROGRAM", 
	"KEYWORD_END", 
	"KEYWORD_REAL",
	"KEYWORD_INTEGER",
	"KEYWORD_VOID", 
	"KEYWORD_RETURN",

	"SEP",
	"COMMA", 
	"SEMICOLON", 
	"LEFT_BRACKET1", 
	"RIGHT_BRACKET1", 
	"LEFT_BRACKET2",
	"RIGHT_BRACKET2", 
	"LEFT_BRACKET3",
	"RIGHT_BRACKET3", 

	"TOKEN_EOF"
};

void print_error(char*, int);
void print_token_found(eTOKENS, char*, int);
void id_action(eTOKENS, char*, int);
void operator_action(eTOKENS, char*, int);
void number_action(eTOKENS, char*, int);
void sep_action(eTOKENS, char*, int);
void keyword_action(eTOKENS, char*, int);
void eof_action(eTOKENS, char*, int);

#endif