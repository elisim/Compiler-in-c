#include "parser.h"
#include "../Actions/actions.h"
#include <stdio.h>
#include <string.h>

/*
SUCCESS 0
FAIL 1
*/

Token *curr_token = NULL;

void parser()
{
	PROGRAM();
   	match(TOKEN_EOF);
}

int match(eTOKENS token)
{
	curr_token = next_token();
	if (curr_token->kind != token)
	{
		return 1;		
	}
	return 0;
}


/* PROGRAM -> program VAR_DEFINITIONS; STATEMENTS end FUNC_DEFINITIONS */
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

	printf("Rule(PROGRAM -> program VAR_DEFINITIONS; STATEMENTS end FUNC_DEFINITIONS)");
	return SUCCESS;
}

/* VAR_DEFINITIONS -> VAR_DEFINITION | VAR_DEFINITION; VAR_DEFINITIONS */
int VAR_DEFINITIONS()
{
	if (VAR_DEFINITION() == FAIL)
	{
		if(VAR_DEFINITION() == FAIL)
			return error();
	}

}

int VAR_DEFINITION();
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

int TYPE();
{
	match(TOKEN_KEYWORD_REAL);
	match(TOKEN_KEYWORD_INTEGER);
}