#include "parser.h"
#include <stdio.h>
#include <string.h>

int parser()
{
	printf("IN PARSER\n");
	return 0;
}

// PROGRAM -> program VAR_DEFINITIONS; STATEMENTS end FUNC_DEFINITIONS
int PROGRAM()
{
	if (strncpy(foo, keyword_program, strlen(keyword_program) != 0))
		return FAIL;
	if (VAR_DEFINITIONS() == FAIL)
		return error();
	if (strncpy(foo, semicolon, strlen(semicolon) != 0))
		return error();
	if (STATEMENTS() == FAIL)
		return error();
	if (strncpy(foo, keyword_end, strlen(keyword_end) != 0))
		return error();
	if (FUNC_DEFINITIONS() == FAIL)
		return error();

	// action
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
	// error msg
	return FAIL;
}
