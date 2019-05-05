#include <stdio.h>
#include <string.h>
#include "../Token/Token.h"
#include "../Actions/actions.h"


void print_error(char* character, int line_number)
{
	printf("The character '%s' at line: %d does not begin any legal token in the language.\n", character, line_number);
}

void print_token_found(eTOKENS token_kind, char* lexeme, int line_number)
{
	const char *token_name = token_kinds[token_kind];
	printf("Token of kind '%s' was found at line: %d, lexeme: '%s'.\n", token_name, line_number, lexeme);
}

void id_action(eTOKENS token_kind, char* lexeme, int line_number)
{
	create_and_store_token(token_kind, lexeme, line_number);
	print_token_found(token_kind, lexeme, line_number);
}

void number_action(eTOKENS token_kind, char* lexeme, int line_number)
{
	create_and_store_token(token_kind, lexeme, line_number);
	print_token_found(token_kind, lexeme, line_number);
}

void eof_action(eTOKENS token_kind, char* lexeme, int line_number)
{
	create_and_store_token(token_kind, lexeme, line_number);
	print_token_found(token_kind, lexeme, line_number);
}

void operator_action(eTOKENS token_kind, char* lexeme, int line_number)
{
	eTOKENS kind = token_kind;

	if (strcmp(lexeme, "*") == 0)
		kind = TOKEN_OP_MUL;
	else if (strcmp(lexeme, "/") == 0)
		kind = TOKEN_OP_DIV;
	else if (strcmp(lexeme, "=") == 0)
		kind = TOKEN_OP_EQUAL;
	else
		fprintf(stderr, "ERROR: Invalid lexeme %s\n",lexeme);

	create_and_store_token(kind, lexeme, line_number);
	print_token_found(kind, lexeme, line_number);
}

void sep_action(eTOKENS token_kind, char* lexeme, int line_number)
{
	eTOKENS kind = token_kind;

	if (strcmp(lexeme, ",") == 0)
		kind = TOKEN_COMMA;
	else if (strcmp(lexeme, ";") == 0)
		kind = TOKEN_SEMICOLON;
	else if (strcmp(lexeme, "[") == 0)
		kind = TOKEN_LEFT_BRACKET1;
	else if (strcmp(lexeme, "]") == 0)
		kind = TOKEN_RIGHT_BRACKET1;
	else if (strcmp(lexeme, "{") == 0)
		kind = TOKEN_LEFT_BRACKET2;
	else if (strcmp(lexeme, "}") == 0)
		kind = TOKEN_RIGHT_BRACKET2;
	else if (strcmp(lexeme, "(") == 0)
		kind = TOKEN_LEFT_BRACKET3;
	else if (strcmp(lexeme, ")") == 0)
		kind = TOKEN_RIGHT_BRACKET3;
	else
		fprintf(stderr, "ERROR: Invalid lexeme %s\n",lexeme);

	create_and_store_token(kind, lexeme, line_number);
	print_token_found(kind, lexeme, line_number);
}

void keyword_action(eTOKENS token_kind, char* lexeme, int line_number)
{
	eTOKENS kind = token_kind;

	if (strcmp(lexeme, "program") == 0)	
		kind = TOKEN_KEYWORD_PROGRAM;
	else if (strcmp(lexeme, "end") == 0)
		kind = TOKEN_KEYWORD_END;
	else if (strcmp(lexeme, "real") == 0)
		kind = TOKEN_KEYWORD_REAL;
	else if (strcmp(lexeme, "integer") == 0)
		kind = TOKEN_KEYWORD_INTEGER;
	else if (strcmp(lexeme, "void") == 0)
		kind = TOKEN_KEYWORD_VOID;
	else if (strcmp(lexeme, "return") == 0)
		kind = TOKEN_KEYWORD_RETURN;
	else
		fprintf(stderr, "ERROR: Invalid lexeme %s\n",lexeme);

	create_and_store_token(kind, lexeme, line_number);
	print_token_found(kind, lexeme, line_number);
}