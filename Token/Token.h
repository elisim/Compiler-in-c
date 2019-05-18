#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin, *yyout;
extern int yylex();
extern int debug;

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


typedef enum eTOKENS
{
	/* === IDs === */
	TOKEN_ID, // names of variables and functions 

	/* === Operatios === */
	TOKEN_OP,
	TOKEN_OP_EQUAL, // assignment (=) 
	TOKEN_OP_MUL, // multiplication (*)
	TOKEN_OP_DIV, // division (/)

	/* === Numbers === */
	TOKEN_NUMBER,
	TOKEN_INT_NUMBER, // unsigned integer number (e.g. 2019)
	TOKEN_REAL_NUMBER, // unsigned real number in the fixed point format (e.g. 2.78)

	/* === Keywords === */
	TOKEN_KEYWORD,
	TOKEN_KEYWORD_PROGRAM, // program
	TOKEN_KEYWORD_END, // end
	TOKEN_KEYWORD_REAL, // real
	TOKEN_KEYWORD_INTEGER, // integer
	TOKEN_KEYWORD_VOID, // void
	TOKEN_KEYWORD_RETURN, // return

	/* === Separation signs === */
	TOKEN_SEP,
	TOKEN_COMMA, // ,
	TOKEN_SEMICOLON, // ;
	TOKEN_LEFT_BRACKET1, // [
	TOKEN_RIGHT_BRACKET1, // ]
	TOKEN_LEFT_BRACKET2, // {
	TOKEN_RIGHT_BRACKET2, // }
	TOKEN_LEFT_BRACKET3, // (
	TOKEN_RIGHT_BRACKET3, // )

	TOKEN_EOF
} eTOKENS;

typedef struct Token
{
	eTOKENS kind;
	char *lexeme;
	int lineNumber;
}Token;

typedef struct Node
{
	Token *tokensArray;
	struct Node *prev;
	struct Node *next;
} Node;

void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine);
Token *next_token();
Token *back_token();
Token *peek	();

#endif