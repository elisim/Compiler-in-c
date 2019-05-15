#include "parser.h"
#include "../Actions/actions.h"
#include <stdio.h>
#include <string.h>

/*
TODO: 
1. error algo
2. match
3. when to call match
*/


Token *curr_token = NULL;

void parser()
{
	PROGRAM();
   	match(TOKEN_EOF);
}

/* match and go to next token */
int match(eTOKENS token)
{
	if (curr_token->kind != token)
	{
		return 1;		
	}
	else 
	{
		curr_token = next_token();
		return 0;
	}
}

/* should be variadic list which token should be next */
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
		case TOKEN_KEYWORD_INTEGER: 
			fprintf(parser_out, "Rule(TYPE -> integer)\n");
			match(TOKEN_KEYWORD_INTEGER);
			break;
		default: 
			error();
	}
}

/* VARIABLES_LIST -> VARIABLE VARIABLES_LIST_TEMP */
void VARIABLES_LIST() 
{
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
			fprintf(parser_out, "Rule(VARIABLES_LIST_TEMP -> ,VARIABLE VARIABLES_LIST_TEMP)\n");
			match(TOKEN_COMMA);
			VARIABLE();
			VARIABLES_LIST_TEMP();
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
	fprintf(parser_out, "Rule(VARIABLE -> id VARIABLE_TEMP)\n");
	cur_token = next_token();
	match(TOKEN_ID);
	VARIABLE_TEMP();
}

/* VARIABLE_TEMP -> [int_number] | ε */
void VARIABLE_TEMP()
{
	cur_token = next_token();
	switch(cur_token->kind)
	{
		case TOKEN_LEFT_BRACKET1:
			fprintf(parser_out, "Rule(VARIABLE_TEMP -> [int_number])\n");
			match(TOKEN_LEFT_BRACKET1);
			match(TOKEN_INT_NUMBER);
			match(TOKEN_RIGHT_BRACKET1);
			break;
		case TOKEN_COMMA:
		case TOKEN_SEMICOLON:
		case TOKEN_RIGHT_BRACKET3:
		case TOKEN_OP_EQUAL:
			cur_token = back_token();
			fprintf(parser_out, "Rule(VARIABLES_LIST_TEMP -> ε)\n");
			break;
		default:
			error();
	}
}

/* FUNC_DEFINITIONS -> FUNC_DEFINITION FUNC_DEFINITIONS_TEMP */
void FUNC_DEFINITIONS()
{
	fprintf(parser_out, "Rule(FUNC_DEFINITIONS -> FUNC_DEFINITION FUNC_DEFINITIONS_TEMP)\n");

	FUNC_DEFINITION();
	FUNC_DEFINITIONS_TEMP();
}

/* FUNC_DEFINITIONS_TEMP -> FUNC_DEFINITION FUNC_DEFINITIONS_TEMP | ε */
void FUNC_DEFINITIONS_TEMP()
{
	cur_token = next_token();
	switch(cur_token->kind)
	{
		case TOKEN_KEYWORD_INTEGER:
		case TOKEN_KEYWORD_REAL:
		case TOKEN_KEYWORD_VOID:
			fprintf(parser_out, "Rule(FUNC_DEFINITIONS_TEMP -> FUNC_DEFINITION FUNC_DEFINITIONS_TEMP)\n");
			cur_token = back_token();

			FUNC_DEFINITION();
			FUNC_DEFINITIONS_TEMP();
			break;
		case TOKEN_EOF:
			fprintf(parser_out, "Rule(FUNC_DEFINITIONS_TEMP -> ε)\n");
			cur_token = back_token();
			break;
		default:
			error();
	}
}

/* FUNC_DEFINITION -> RETURNED_TYPE id (PARAM_DEFINITIONS) BLOCK */
void FUNC_DEFINITION()
{
	fprintf(parser_out, "Rule(FUNC_DEFINITION -> RETURNED_TYPE id (PARAM_DEFINITIONS) BLOCK)\n");
	RETURNED_TYPE();
	match(TOKEN_ID);
	match(TOKEN_LEFT_BRACKET3);
	PARAM_DEFINITIONS();
	match(TOKEN_RIGHT_BRACKET3);
	BLOCK();
}

/* RETURNED_TYPE  -> void | TYPE */
void RETURNED_TYPE()
{
	cur_token = next_token();
	switch(cur_token->kind)
	{
		case TOKEN_KEYWORD_VOID:
			fprintf(parser_out, "Rule(RETURNED_TYPE -> void)\n");
			match(TOKEN_KEYWORD_VOID);
			break;
		case TOKEN_KEYWORD_INTEGER: 
		case TOKEN_KEYWORD_REAL:
			fprintf(parser_out, "Rule(RETURNED_TYPE -> TYPE)\n");
			cur_token = back_token();
			TYPE();
			break;
		default: 
			error();
	}
}

/* PARAM_DEFINITIONS -> ε | VAR_DEFINITIONS */
void PARAM_DEFINITIONS()
{	
	cur_token = next_token();
	switch(cur_token->kind)
	{
		case TOKEN_KEYWORD_INTEGER:
		case TOKEN_KEYWORD_REAL:
			fprintf(parser_out, "Rule(PARAM_DEFINITIONS -> VAR_DEFINITIONS)\n");
			cur_token = back_token();
			VAR_DEFINITIONS();
			break;
		case TOKEN_RIGHT_BRACKET3:
			fprintf(parser_out, "Rule(PARAM_DEFINITIONS -> ε)\n");
			cur_token = back_token();
			break;
		default:
			error();
	}
}

/* STATEMENTS -> STATEMENT; STATEMENTS_TEMP */
void STATEMENTS()
{
	fprintf(parser_out, "Rule(STATEMENTS -> STATEMENT; STATEMENTS_TEMP)\n");
	STATEMENT();
	match(TOKEN_SEMICOLON);
	STATEMENT_TEMP();
}

/* STATEMENTS_TEMP -> STATEMENTS | ε */
void STATEMENTS_TEMP()
{
	cur_token = next_token();
	switch(cur_token->kind)
	{
		case TOKEN_KEYWORD_RETURN:
		case TOKEN_ID:
		case TOKEN_LEFT_BRACKET2:
			fprintf(parser_out, "Rule(STATEMENTS_TEMP -> STATEMENTS)\n");
			cur_token = back_token();
			STATEMENTS();
			break;
		case TOKEN_KEYWORD_END:
		case TOKEN_RIGHT_BRACKET2:
			fprintf(parser_out, "Rule(STATEMENTS_TEMP -> ε)\n");
			cur_token = back_token();
			break;
		default:
			error();
	}
}

/* STATEMENT -> return STATEMENT_TEMP | BLOCK | id STATEMENT_TWO_TEMP */
void STATEMENT()
{
	cur_token = next_token();
	switch(cur_token->kind)
	{
		case TOKEN_KEYWORD_RETURN:
			fprintf(parser_out, "Rule(STATEMENT -> return STATEMENT_TEMP)\n");
			match(TOKEN_KEYWORD_RETURN);
			STATEMENT_TEMP();
			break;
		case TOKEN_LEFT_BRACKET2: 
			fprintf(parser_out, "Rule(STATEMENT -> BLOCK)\n");
			cur_token = back_token();
			BLOCK();
			break;
		case TOKEN_ID: 
			fprintf(parser_out, "Rule(STATEMENT -> id STATEMENT_TWO_TEMP)\n");
			match(TOKEN_ID);
			STATEMENT_TWO_TEMP();
			break;
		default: 
			error();
	}
}

/* STATEMENT_TEMP -> EXPRESSION | ε */
void STATEMENT_TEMP()
{
	cur_token = next_token();
	switch(cur_token->kind)
	{
		case TOKEN_INT_NUMBER:
		case TOKEN_ID:
		case TOKEN_REAL_NUMBER:
			fprintf(parser_out, "Rule(STATEMENT_TEMP -> EXPRESSION)\n");
			cur_token = back_token();
			EXPRESSION();
			break;
		case TOKEN_SEMICOLON:
			fprintf(parser_out, "Rule(STATEMENT_TEMP -> EXPRESSION)\n");
			cur_token = back_token();
			break;
		default:
			error();
	}
}

/* STATEMENT_TWO_TEMP -> (PARAMETERS_LIST) | VARIABLE_TEMP = EXPRESSION */
void STATEMENT_TWO_TEMP()
{
	cur_token = next_token();
	switch(cur_token->kind)
	{
		case TOKEN_LEFT_BRACKET3:
			fprintf(parser_out, "Rule(STATEMENT_TWO_TEMP -> (PARAMETERS_LIST))\n");
			match(TOKEN_LEFT_BRACKET3);
			PARAMETERS_LIST();
			match(TOKEN_RIGHT_BRACKET3);
			break;
		case TOKEN_OP_EQUAL:
		case TOKEN_LEFT_BRACKET1: 
			fprintf(parser_out, "Rule(STATEMENT_TWO_TEMP -> VARIABLE_TEMP = EXPRESSION)\n");
			cur_token = back_token();
			VARIABLE_TEMP();
			match(TOKEN_OP_EQUAL);
			EXPRESSION();
			break;
		default: 
			error();
	}
}

/* BLOCK -> { VAR_DEFINITIONS; STATEMENTS } */
void BLOCK()
{
	fprintf(parser_out, "Rule(BLOCK -> { VAR_DEFINITIONS; STATEMENTS)\n");
	cur_token = next_token();

	match(TOKEN_LEFT_BRACKET2);
	VAR_DEFINITIONS();
	match(TOKEN_SEMICOLON);
	STATEMENTS();
	match(TOKEN_RIGHT_BRACKET2);
}

/* FUNCTION_CALL -> id (PARAMETERS_LIST) */
void FUNCTION_CALL()
{
	fprintf(parser_out, "Rule(FUNCTION_CALL -> id (PARAMETERS_LIST)\n");
	cur_token = next_token();

	match(TOKEN_ID);
	match(TOKEN_LEFT_BRACKET3);
	PARAMETERS_LIST();
	match(TOKEN_RIGHT_BRACKET3);
}

/* PARAMETERS_LIST -> ε | VARIABLES_LIST */
void PARAMETERS_LIST()
{
	cur_token = next_token();
	switch(cur_token->kind)
	{
		case TOKEN_ID:
			fprintf(parser_out, "Rule(PARAMETERS_LIST -> VARIABLES_LIST)\n");
			cur_token = back_token();
			VARIABLES_LIST();
			break;
		case TOKEN_RIGHT_BRACKET3:
			fprintf(parser_out, "Rule(PARAMETERS_LIST -> ε)\n");
			cur_token = back_token();
			break;
		default:
			error();
	}
}

/* EXPRESSION -> int_number | real_number | id EXPRESSION_TEMP */
void EXPRESSION()
{
	cur_token = next_token();
	switch(cur_token->kind)
	{
		case TOKEN_INT_NUMBER:
			fprintf(parser_out, "Rule(EXPRESSION -> int_number)\n");
			match(TOKEN_INT_NUMBER);
			break;
		case TOKEN_REAL_NUMBER: 
			fprintf(parser_out, "Rule(EXPRESSION -> real_number)\n");
			match(TOKEN_REAL_NUMBER);
			break;
		case TOKEN_ID: 
			fprintf(parser_out, "Rule(EXPRESSION -> id EXPRESSION_TEMP)\n");
			match(TOKEN_ID);
			EXPRESSION_TEMP();
			break;
		default: 
			error();
	}
}

/* EXPRESSION_TEMP -> VARIABLE_TEMP | ar_op EXPRESSION */
void EXPRESSION_TEMP()
{
	cur_token = next_token();
	switch(cur_token->kind)
	{
		case TOKEN_LEFT_BRACKET1:
			fprintf(parser_out, "Rule(EXPRESSION_TEMP -> VARIABLE_TEMP)\n");
			cur_token = back_token();
			VARIABLE_TEMP();
			break;
		case TOKEN_OP_MUL:
			match(TOKEN_OP_MUL);
			fprintf(parser_out, "Rule(EXPRESSION_TEMP -> ar_op EXPRESSION)\n");
			EXPRESSION();
			break;
		case TOKEN_OP_DIV: 
			match(TOKEN_OP_MUL);
			fprintf(parser_out, "Rule(EXPRESSION_TEMP -> ar_op EXPRESSION)\n");
			EXPRESSION();
			break;
		default: 
			error();
	}
}