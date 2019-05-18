#include "parser.h"
#include "../Actions/actions.h"
#include <stdio.h>
#include <string.h>

/*
TODO: 
1. match: when error, recover
2. rule FUNCTION CALL
3. VARIABLE_DEFITINIOS_TEMP first and follow not disjoint
4. error msg
5. files to output
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
		return FAIL;
	}
	else 
	{
		return SUCCESS;
	}
}

// void error()
// {
// 	eTOKENS expected[] = {TOKEN_RIGHT_BRACKET3};
// 	int size = sizeof(expected)/sizeof(expected[0]);
// 	char *expected_str = concatenate(size, expected, " or ");
// 	fprintf(parser_out, "Expected token of type '%s' at line: %d, Actual token of type '%s', lexeme: '%s'", 
// 			expected_str, curr_token->lineNumber, token_kinds[curr_token->kind], curr_token->lexeme);
// }

void error()
{
	if (debug == 1)
		printf("error\n");	
	else
		fprintf(parser_out, "error\n");
}

void recover(eTOKENS follows[], int size)
{
	if (debug == 1)
		printf("recover\n");	
	else
		fprintf(parser_out, "recover\n");	// do {
	// 	curr_token = next_token();
	// } while(contains_in(follows, curr_token->kind, size) == FAIL);
	
}

int contains_in(eTOKENS arr[], eTOKENS token, int size)
{
	int i;
	for(i=0; i<size; i++)
	{
		if (arr[i] == token)
			return SUCCESS;
	}
	return FAIL;
}

void output(char* rule)
{
	if (debug == 1)
		printf("Rule(%s)\n", rule);
	else
		fprintf(parser_out, "Rule(%s)\n", rule);
}

/* PROGRAM -> program VAR_DEFINITIONS; STATEMENTS end FUNC_DEFINITIONS */
void PROGRAM()
{
	int follows[1] = {TOKEN_EOF};
	int size = 1;

	output("PROGRAM -> program VAR_DEFINITIONS; STATEMENTS end FUNC_DEFINITIONS");
	curr_token = next_token();
	match(TOKEN_KEYWORD_PROGRAM);
	VAR_DEFINITIONS();
	curr_token = next_token();
	match(TOKEN_SEMICOLON);
	STATEMENTS();
	curr_token = next_token();
	match(TOKEN_KEYWORD_END);
	FUNC_DEFINITIONS();
}


/* VAR_DEFINITIONS -> VAR_DEFINITION VAR_DEFINITIONS_TEMP */
void VAR_DEFINITIONS()
{
	output("VAR_DEFINITIONS -> VAR_DEFINITION VAR_DEFINITIONS_TEMP");

	VAR_DEFINITION(); 
	VAR_DEFINITIONS_TEMP();
}


/* VAR_DEFINITIONS_TEMP -> ;VAR_DEFINITIONS | ε */
void VAR_DEFINITIONS_TEMP()
{
	// output("======= IN VAR DEFS TEMP =========");
	eTOKENS follows[2] = {TOKEN_SEMICOLON, TOKEN_RIGHT_BRACKET3};
	curr_token = next_token();
	Token* peek_token = NULL;

	switch(curr_token->kind)
	{
		case TOKEN_SEMICOLON:
			// output("=============== peek ===========");
			peek_token = peek();
			if (peek_token->kind == TOKEN_KEYWORD_INTEGER || peek_token->kind == TOKEN_KEYWORD_REAL)
			{
				output("VAR_DEFINITIONS_TEMP -> ;VAR_DEFINITIONS");
				VAR_DEFINITIONS();
			}
			else 
			{
				// output("=============== else ===========");
				curr_token = back_token();
				output("VAR_DEFINITIONS_TEMP -> ε");
			}
			break;
		case TOKEN_RIGHT_BRACKET3:
			curr_token = back_token();
			output("VAR_DEFINITIONS_TEMP -> ε");
			break;
		default:
			recover(follows, 2);
			error();
	}
}

/* VAR_DEFINITION -> TYPE VARIABLES_LIST */
void VAR_DEFINITION()
{
	output("VAR_DEFINITION -> TYPE VARIABLES_LIST");

	TYPE();
	VARIABLES_LIST();
}


/* TYPE -> real | integer */
void TYPE()
{
	eTOKENS follows[1] = {TOKEN_ID};
	curr_token = next_token();
	switch(curr_token->kind)
	{
		case TOKEN_KEYWORD_REAL:
			output("TYPE -> real");
			match(TOKEN_KEYWORD_REAL);
			break;
		case TOKEN_KEYWORD_INTEGER: 
			output("TYPE -> integer");
			match(TOKEN_KEYWORD_INTEGER);
			break;
		default: 
			error();
			recover(follows, 1);
	}
}

/* VARIABLES_LIST -> VARIABLE VARIABLES_LIST_TEMP */
void VARIABLES_LIST() 
{
	output("VARIABLES_LIST -> VARIABLE VARIABLES_LIST_TEMP");
	VARIABLE();
	VARIABLES_LIST_TEMP();
}


/* VARIABLES_LIST_TEMP -> ,VARIABLE VARIABLES_LIST_TEMP | ε */
void VARIABLES_LIST_TEMP()
{
	curr_token = next_token();
	eTOKENS follows[2] = {TOKEN_SEMICOLON, TOKEN_RIGHT_BRACKET3};

	switch(curr_token->kind)
	{
		case TOKEN_COMMA:	
			output("VARIABLES_LIST_TEMP -> ,VARIABLE VARIABLES_LIST_TEMP");
			match(TOKEN_COMMA);
			VARIABLE();
			VARIABLES_LIST_TEMP();
			break;
		case TOKEN_SEMICOLON:
		case TOKEN_RIGHT_BRACKET3:
			curr_token = back_token();
			output("VARIABLES_LIST_TEMP -> ε");
			break;
		default:
			error();
			recover(follows, 2);
	}
}

/* VARIABLE -> id VARIABLE_TEMP */
void VARIABLE()
{
	eTOKENS follows[3] = {TOKEN_COMMA, TOKEN_SEMICOLON, TOKEN_RIGHT_BRACKET3};
	curr_token = next_token();
	output("VARIABLE -> id VARIABLE_TEMP");

	match(TOKEN_ID);
	VARIABLE_TEMP();
}

/* VARIABLE_TEMP -> [int_number] | ε */
void VARIABLE_TEMP()
{
	eTOKENS follows[4] = {TOKEN_SEMICOLON, TOKEN_COMMA, TOKEN_RIGHT_BRACKET3, TOKEN_OP_EQUAL};
	curr_token = next_token();
	switch(curr_token->kind)
	{
		case TOKEN_LEFT_BRACKET1:
			output("VARIABLE_TEMP -> [int_number]");
			match(TOKEN_LEFT_BRACKET1);
			curr_token = next_token();
			match(TOKEN_INT_NUMBER);
			curr_token = next_token();
			match(TOKEN_RIGHT_BRACKET1);
			break;
		case TOKEN_COMMA:
		case TOKEN_SEMICOLON:
		case TOKEN_RIGHT_BRACKET3:
		case TOKEN_OP_EQUAL:
			curr_token = back_token();
			output("VARIABLE_TEMP -> ε");
			break;
		default:
			error();
			recover(follows, 4);
	}
}

/* FUNC_DEFINITIONS -> FUNC_DEFINITION FUNC_DEFINITIONS_TEMP */
void FUNC_DEFINITIONS()
{
	output("FUNC_DEFINITIONS -> FUNC_DEFINITION FUNC_DEFINITIONS_TEMP");

	FUNC_DEFINITION();
	FUNC_DEFINITIONS_TEMP();
}

/* FUNC_DEFINITIONS_TEMP -> FUNC_DEFINITION FUNC_DEFINITIONS_TEMP | ε */
void FUNC_DEFINITIONS_TEMP()
{
	curr_token = next_token();
	eTOKENS follows[1] = {TOKEN_EOF};
	switch(curr_token->kind)
	{
		case TOKEN_KEYWORD_INTEGER:
		case TOKEN_KEYWORD_REAL:
		case TOKEN_KEYWORD_VOID:
			curr_token = back_token();
			output("FUNC_DEFINITIONS_TEMP -> FUNC_DEFINITION FUNC_DEFINITIONS_TEMP");

			FUNC_DEFINITION();
			FUNC_DEFINITIONS_TEMP();
			break;
		case TOKEN_EOF:
			curr_token = back_token();
			output("FUNC_DEFINITIONS_TEMP -> ε");
			break;
		default:
			error();
			recover(follows,1);
	}
}

/* FUNC_DEFINITION -> RETURNED_TYPE id (PARAM_DEFINITIONS) BLOCK */
void FUNC_DEFINITION()
{
	eTOKENS follows[4] = {TOKEN_EOF, TOKEN_KEYWORD_VOID, TOKEN_KEYWORD_REAL, TOKEN_KEYWORD_INTEGER};
	int size = 4;

	output("FUNC_DEFINITION -> RETURNED_TYPE id (PARAM_DEFINITIONS) BLOCK");
	RETURNED_TYPE();
	curr_token = next_token();
	match(TOKEN_ID);
	curr_token = next_token();
	match(TOKEN_LEFT_BRACKET3);
	PARAM_DEFINITIONS();
	curr_token = next_token();
	match(TOKEN_RIGHT_BRACKET3);
	BLOCK();
}

/* RETURNED_TYPE  -> void | TYPE */
void RETURNED_TYPE()
{
	curr_token = next_token();
	eTOKENS follows[1] = {TOKEN_ID};
	switch(curr_token->kind)
	{
		case TOKEN_KEYWORD_VOID:
			output("RETURNED_TYPE -> void");
			match(TOKEN_KEYWORD_VOID);
			break;
		case TOKEN_KEYWORD_INTEGER: 
		case TOKEN_KEYWORD_REAL:
			curr_token = back_token();
			output("RETURNED_TYPE -> TYPE");
			TYPE();
			break;
		default: 
			error();
			recover(follows,1);
	}
}

/* PARAM_DEFINITIONS -> ε | VAR_DEFINITIONS */
void PARAM_DEFINITIONS()
{	
	curr_token = next_token();
	eTOKENS follows[1] = {TOKEN_RIGHT_BRACKET3};
	switch(curr_token->kind)
	{
		case TOKEN_KEYWORD_INTEGER:
		case TOKEN_KEYWORD_REAL:
			curr_token = back_token();
			output("PARAM_DEFINITIONS -> VAR_DEFINITIONS");
			VAR_DEFINITIONS();
			break;
		case TOKEN_RIGHT_BRACKET3:
			curr_token = back_token();
			output("PARAM_DEFINITIONS -> ε");
			break;
		default:
			error();
			recover(follows,1);
	}
}

/* STATEMENTS -> STATEMENT; STATEMENTS_TEMP */
void STATEMENTS()
{
	eTOKENS follows[2] = {TOKEN_KEYWORD_END, TOKEN_RIGHT_BRACKET2};
	output("STATEMENTS -> STATEMENT; STATEMENTS_TEMP");
	STATEMENT();
	curr_token = next_token();
	match(TOKEN_SEMICOLON);
	STATEMENTS_TEMP();
}

/* STATEMENTS_TEMP -> STATEMENTS | ε */
void STATEMENTS_TEMP()
{
	curr_token = next_token();
	eTOKENS follows[2] = {TOKEN_KEYWORD_END, TOKEN_RIGHT_BRACKET2};
	switch(curr_token->kind)
	{
		case TOKEN_KEYWORD_RETURN:
		case TOKEN_ID:
		case TOKEN_LEFT_BRACKET2:
			curr_token = back_token();
			output("STATEMENTS_TEMP -> STATEMENTS");
			STATEMENTS();
			break;
		case TOKEN_KEYWORD_END:
		case TOKEN_RIGHT_BRACKET2:
			curr_token = back_token();
			output("STATEMENTS_TEMP -> ε");
			break;
		default:
			error();
			recover(follows, 2);
	}
}

/* STATEMENT -> return STATEMENT_TEMP | BLOCK | id STATEMENT_TWO_TEMP */
void STATEMENT()
{
	eTOKENS follows[1] = {TOKEN_SEMICOLON};
	curr_token = next_token();
	if (curr_token->kind == TOKEN_SEMICOLON)
		curr_token = next_token();
	switch(curr_token->kind)
	{
		case TOKEN_KEYWORD_RETURN:
			output("STATEMENT -> return STATEMENT_TEMP");
			match(TOKEN_KEYWORD_RETURN);
			STATEMENT_TEMP();
			break;
		case TOKEN_LEFT_BRACKET2: 
			curr_token = back_token();
			output("STATEMENT -> BLOCK");
			BLOCK();
			break;
		case TOKEN_ID: 
			output("STATEMENT -> id STATEMENT_TWO_TEMP");
			match(TOKEN_ID);
			STATEMENT_TWO_TEMP();

			break;
		default: 
			error();
			recover(follows, 1);
	}
}

/* STATEMENT_TEMP -> EXPRESSION | ε */
void STATEMENT_TEMP()
{
	eTOKENS follows[1] = {TOKEN_SEMICOLON};
	curr_token = next_token();
	switch(curr_token->kind)
	{
		case TOKEN_INT_NUMBER:
		case TOKEN_ID:
		case TOKEN_REAL_NUMBER:
			curr_token = back_token();
			output("STATEMENT_TEMP -> EXPRESSION");
			EXPRESSION();
			break;
		case TOKEN_SEMICOLON:
			curr_token = back_token();
			output("STATEMENT_TEMP -> EXPRESSION");
			break;
		default:
			error();
			recover(follows, 1);
	}
}

/* STATEMENT_TWO_TEMP -> (PARAMETERS_LIST) | VARIABLE_TEMP = EXPRESSION */
void STATEMENT_TWO_TEMP()
{
	eTOKENS follows[1] = {TOKEN_SEMICOLON};
	curr_token = next_token();
	switch(curr_token->kind)
	{
		case TOKEN_LEFT_BRACKET3:
			output("STATEMENT_TWO_TEMP -> (PARAMETERS_LIST)");
			match(TOKEN_LEFT_BRACKET3);
			PARAMETERS_LIST();
			curr_token = next_token();
			match(TOKEN_RIGHT_BRACKET3);
			break;
		case TOKEN_OP_EQUAL:
		case TOKEN_LEFT_BRACKET1: 
			curr_token = back_token();
			output("STATEMENT_TWO_TEMP -> VARIABLE_TEMP = EXPRESSION");
			VARIABLE_TEMP();
			curr_token = next_token();
			match(TOKEN_OP_EQUAL);
			EXPRESSION();
			break;
		default: 
			error();
			recover(follows, 1);
	}
}

/* BLOCK -> { VAR_DEFINITIONS; STATEMENTS } */
void BLOCK()
{
	eTOKENS follows[4] = {TOKEN_KEYWORD_VOID, TOKEN_KEYWORD_REAL, TOKEN_KEYWORD_INTEGER, TOKEN_EOF};
	curr_token = next_token();
	output("BLOCK -> { VAR_DEFINITIONS; STATEMENTS");

	match(TOKEN_LEFT_BRACKET2);
	VAR_DEFINITIONS();
	curr_token = next_token();
	match(TOKEN_SEMICOLON);
	STATEMENTS();
	curr_token = next_token();
	match(TOKEN_RIGHT_BRACKET2);
}

/* FUNCTION_CALL -> id (PARAMETERS_LIST) */
// ========================================================= TODO
void FUNCTION_CALL()
{
	eTOKENS follows[4] = {TOKEN_KEYWORD_VOID, TOKEN_KEYWORD_REAL, TOKEN_KEYWORD_INTEGER, TOKEN_EOF};
	curr_token = next_token();
	output("FUNCTION_CALL -> id (PARAMETERS_LIST");

	match(TOKEN_ID);
	curr_token = next_token();
	match(TOKEN_LEFT_BRACKET3);
	PARAMETERS_LIST();
	curr_token = next_token();
	match(TOKEN_RIGHT_BRACKET3);
}

/* PARAMETERS_LIST -> ε | VARIABLES_LIST */
void PARAMETERS_LIST()
{
	eTOKENS follows[1] = {TOKEN_RIGHT_BRACKET3};
	curr_token = next_token();
	switch(curr_token->kind)
	{
		case TOKEN_ID:
			curr_token = back_token();
			output("PARAMETERS_LIST -> VARIABLES_LIST");
			VARIABLES_LIST();
			break;
		case TOKEN_RIGHT_BRACKET3:
			curr_token = back_token();
			output("PARAMETERS_LIST -> ε");
			break;
		default:
			error();
			recover(follows, 1);

	}
}

/* EXPRESSION -> int_number | real_number | id EXPRESSION_TEMP */
void EXPRESSION()
{
	eTOKENS follows[1] = {TOKEN_SEMICOLON};
	curr_token = next_token();
	switch(curr_token->kind)
	{
		case TOKEN_INT_NUMBER:
			output("EXPRESSION -> int_number");
			match(TOKEN_INT_NUMBER);
			break;
		case TOKEN_REAL_NUMBER: 
			output("EXPRESSION -> real_number");
			match(TOKEN_REAL_NUMBER);
			break;
		case TOKEN_ID: 
			output("EXPRESSION -> id EXPRESSION_TEMP");
			match(TOKEN_ID);
			EXPRESSION_TEMP();
			break;
		default: 
			error();
			recover(follows, 1);
	}
}

/* EXPRESSION_TEMP -> VARIABLE_TEMP | ar_op EXPRESSION */
void EXPRESSION_TEMP()
{
	eTOKENS follows[1] = {TOKEN_SEMICOLON};
	curr_token = next_token();
	switch(curr_token->kind)
	{
		case TOKEN_LEFT_BRACKET1:
			curr_token = back_token();
			output("EXPRESSION_TEMP -> VARIABLE_TEMP");
			VARIABLE_TEMP();
			break;
		case TOKEN_OP_MUL:
			match(TOKEN_OP_MUL);
			output("EXPRESSION_TEMP -> ar_op EXPRESSION");
			EXPRESSION();
			break;
		case TOKEN_OP_DIV: 
			match(TOKEN_OP_MUL);
			output("EXPRESSION_TEMP -> ar_op EXPRESSION");
			EXPRESSION();
			break;
		default: 
			error();
			recover(follows, 1);
	}
}


char *concatenate(size_t size, eTOKENS *array, const char *joint){
    size_t jlen, lens[size];
    size_t i, total_size = (size-1) * (jlen=strlen(joint)) + 1;
    char *result, *p;


    for(i=0;i<size;++i){
        total_size += (lens[i]=strlen(token_kinds[array[i]]));
    }
    p = result = malloc(total_size);
    for(i=0;i<size;++i){
        memcpy(p, token_kinds[array[i]], lens[i]);
        p += lens[i];
        if(i<size-1){
            memcpy(p, joint, jlen);
            p += jlen;
        }
    }
    *p = '\0';
    return result;
}