#include "parser.h"
#include "../Actions/actions.h"
#include <stdio.h>
#include <string.h>

/*
SUCCESS 0
FAIL 1
*/

Token *current_token = NULL;

int match(eTOKENS token)
{
	current_token = next_token();
	printf("kind = %s\n", token_kinds[current_token->kind]);
   	printf("lexeme = %s\n", current_token->lexeme);
   	printf("line = %d\n",  current_token->lineNumber);
    printf("OK?\n");
    getchar();

	if (current_token->kind != token)
	{
		return 1;		
	}
	return 0;
}


// PROGRAM -> program VAR_DEFINITIONS; STATEMENTS end FUNC_DEFINITIONS
int PROGRAM()
{
	if (match(TOKEN_KEYWORD_PROGRAM) != 0)
		return FAIL;
	if (VAR_DEFINITIONS() == FAIL)
		return error();
	if (match(TOKEN_SEMICOLON) != 0)
		return error();
	if (STATEMENTS() == FAIL)
		return error();
	if (match(TOKEN_KEYWORD_END) != 0)
		return error();
	if (FUNC_DEFINITIONS() == FAIL)
		return error();
	if (match(TOKEN_EOF) != 0)
		return error();

	printf("Rule(PROGRAM -> program VAR_DEFINITIONS; STATEMENTS end FUNC_DEFINITIONS)");
	return SUCCESS;
}

int VAR_DEFINITIONS()
{
	return SUCCESS;
}

int STATEMENTS()
{
	return SUCCESS;
}

int FUNC_DEFINITIONS()
{
	return SUCCESS;
}

int error()
{
	printf("ERROR\n");
	return FAIL;
}
