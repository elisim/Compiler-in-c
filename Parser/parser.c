#include "parser.h"
#include "../Actions/actions.h"
#include <stdio.h>
#include <string.h>


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

int error()
{
	printf("ERROR\n");
	return FAIL;
}


/* PROGRAM -> program VAR_DEFINITIONS; STATEMENTS end FUNC_DEFINITIONS */
void PROGRAM()
{
	fprintf(parser_out, "Rule(PROGRAM -> program VAR_DEFINITIONS; STATEMENTS end FUNC_DEFINITIONS)\n");	
	curr_token = next_token();

	match(TOKEN_KEYWORD_PROGRAM);
	VAR_DEFINITIONS();
	match(TOKEN_SEMICOLON);
	STATEMENTS();
	match(TOKEN_KEYWORD_END);
	FUNC_DEFINITIONS();
}


/* VAR_DEFINITIONS -> VAR_DEFINITION VAR_DEFINITIONS_TEMP */
void VAR_DEFINITIONS()
{
	fprintf(parser_out, "Rule(VAR_DEFINITIONS -> VAR_DEFINITION VAR_DEFINITIONS_TEMP)\n");
	curr_token = next_token();

	VAR_DEFINITION(); 
	VAR_DEFINITIONS_TEMP();
}


/* VAR_DEFINITIONS_TEMP -> ;VAR_DEFINITIONS | ε */
void VAR_DEFINITIONS_TEMP()
{
	curr_token = next_token();
	switch(curr_token->kind)
	{
		case TOKEN_SEMICOLON:
			fprintf(parser_out, "Rule(VAR_DEFINITIONS_TEMP -> ;VAR_DEFINITIONS)\n");
			match(TOKEN_SEMICOLON);
			VAR_DEFINITIONS();
			break;
		case TOKEN_RIGHT_BRACKET3:
			fprintf(parser_out, "Rule(VAR_DEFINITIONS_TEMP -> ε)\n");
			cur_token = back_token();
			break;
		default:
			error();
	}
}

/* VAR_DEFINITION -> TYPE VARIABLES_LIST */
void VAR_DEFINITION();
{
	fprintf(parser_out, "Rule(VAR_DEFINITION -> TYPE VARIABLES_LIST)\n");
	cur_token = next_token();
	TYPE();
	VARIABLES_LIST();
}


/* TYPE -> real | integer */
void TYPE();
{
	cur_token = next_token();
	switch(cur_token->kind)
	{
		case TOKEN_KEYWORD_REAL:
			fprintf(parser_out, "Rule(TYPE -> real)\n");
			match(TOKEN_KEYWORD_REAL);
			break;
		case TOKEN_KEYWORD_integer: 
			fprintf(parser_out, "Rule(TYPE -> integer)\n");
			match(TOKEN_KEYWORD_integer);
			break;
		default: 
			error();
	}
}

/* VARIABLES_LIST -> VARIABLE VARIABLES_LIST_TEMP */
void VARIABLES_LIST() 
{
	cur_token = next_token();
	fprintf(parser_out, "Rule(VARIABLES_LIST -> VARIABLE VARIABLES_LIST_TEMP)\n");
	VARIABLE();
	VARIABLES_LIST_TEMP();
}


/* VARIABLES_LIST_TEMP -> ,VARIABLE VARIABLES_LIST_TEMP | ε */
void VARIABLES_LIST_TEMP()
{
	cur_token = next_token();
	switch(cur_token->kind)
	{
		case TOKEN_COMMA:
			match(TOKEN_COMMA);
			break;
		case TOKEN_SEMICOLON:
		case TOKEN_RIGHT_BRACKET3:
			cur_token = back_token();
			fprintf(parser_out, "Rule(VARIABLES_LIST_TEMP -> ε)\n");
			break;
		default:
			error();
	}
}

/* VARIABLE -> id VARIABLE_TEMP */
void VARIABLE()
{

}

/* VARIABLE_TEMP -> [int_number] | ε */
void VARIABLE_TEMP()
{

}

/* FUNC_DEFINITIONS -> FUNC_DEFINITION FUNC_DEFINITIONS_TEMP */
void FUNC_DEFINITIONS()
{

}

/* FUNC_DEFINITIONS_TEMP -> FUNC_DEFINITION FUNC_DEFINITIONS_TEMP| ε */
void FUNC_DEFINITIONS_TEMP()
{

}

/* FUNC_DEFINITION -> RETURNED_TYPE id (PARAM_DEFINITIONS) BLOCK  */
void FUNC_DEFINITION()
{

}

/* RETURNED_TYPE  -> void | TYPE */
void RETURNED_TYPE()
{

}

/* PARAM_DEFINITIONS -> ε | VAR_DEFINITIONS */
void PARAM_DEFINITIONS()
{

}

/* STATEMENTS -> STATEMENT; STATEMENTS_TEMP */
void STATEMENTS()
{

}

/* STATEMENT_TEMP -> EXPRESSION | ε */
void STATEMENTS_TEMP()
{

}

/* STATEMENT -> return STATEMENT_TEMP | BLOCK | id STATEMENT_TWO_TEMP */
void STATEMENT()
{

}

/* STATEMENT_TEMP -> EXPRESSION | ε */
void STATEMENT_TEMP()
{

}

/* STATEMENT_TWO_TEMP -> (PARAMETERS_LIST) | VARIABLE_TEMP = EXPRESSION */
void STATEMENT_TWO_TEMP()
{

}

/* BLOCK -> { VAR_DEFINITIONS; STATEMENTS } */
void BLOCK()
{

}

/* FUNCTION_CALL -> id (PARAMETERS_LIST) */
void FUNCTION_CALL()
{

}

/* PARAMETERS_LIST -> ε | VARIABLES_LIST */
void PARAMETERS_LIST()
{

}

/* EXPRESSION -> int_number | real_number | id EXPRESSION_TEMP */
void EXPRESSION()
{

}

/* EXPRESSION_TEMP -> VARIABLE_TEMP | ar_op EXPRESSION */
void EXPRESSION_TEMP()
{

}