#include "parser.h"
#include "../Actions/actions.h"
#include <stdio.h>
#include <string.h>
#include "symbol_table.h"
#include "element_type.h"

Token *curr_token = NULL;
table_ptr curr_table = NULL;


void parser()
{
	curr_token = NULL;
	PROGRAM();
	assert(curr_table == NULL); // after parsing the program, curr_table should be NULL;
	curr_token = next_token();
   	match(TOKEN_EOF);
}

int match(eTOKENS token)
{
	if (curr_token->kind != token)
	{
		char expected_str[100] = "";
		strcpy(expected_str, token_kinds[token]);
		fprintf(parser_out, "Expected: token '%s' at line: %d, Actual token: '%s', lexeme: '%s'\n", 
			expected_str, curr_token->lineNumber, token_kinds[curr_token->kind], curr_token->lexeme);
		return FAIL;
	}
	else 
	{
		return SUCCESS;
	}
}

/* PROGRAM -> program VAR_DEFINITIONS; STATEMENTS end FUNC_DEFINITIONS */
void PROGRAM()
{
	int follows[1] = {TOKEN_EOF};
	int size = 1;

	output("PROGRAM -> program VAR_DEFINITIONS; STATEMENTS end FUNC_DEFINITIONS");
	curr_table = make_table(curr_table)
	curr_token = next_token();
	match(TOKEN_KEYWORD_PROGRAM);
	VAR_DEFINITIONS();
	curr_token = next_token();
	match(TOKEN_SEMICOLON);
	STATEMENTS();
	curr_token = next_token();
	match(TOKEN_KEYWORD_END);
	FUNC_DEFINITIONS();
	cur_table = pop_table(cur_table);
}


/* VAR_DEFINITIONS -> VAR_DEFINITION VAR_DEFINITIONS_TEMP */
void VAR_DEFINITIONS()
{
	eTOKENS follows[2] = {TOKEN_SEMICOLON, TOKEN_RIGHT_BRACKET3};
	eTOKENS expected[4] = {TOKEN_KEYWORD_REAL, TOKEN_KEYWORD_INTEGER, TOKEN_SEMICOLON, TOKEN_RIGHT_BRACKET3};
	curr_token = next_token();
	switch(curr_token->kind)
	{
		case TOKEN_KEYWORD_REAL:
		case TOKEN_KEYWORD_INTEGER:
			output("VAR_DEFINITIONS -> VAR_DEFINITION VAR_DEFINITIONS_TEMP");
			curr_token = back_token();
			VAR_DEFINITION(); 
			VAR_DEFINITIONS_TEMP();
			break;

		default:
			recover(follows, 2);
			error(expected, sizeof(expected)/sizeof(expected[0]));
	}
}


/* VAR_DEFINITIONS_TEMP -> ;VAR_DEFINITIONS | ε */
void VAR_DEFINITIONS_TEMP()
{
	eTOKENS follows[2] = {TOKEN_SEMICOLON, TOKEN_RIGHT_BRACKET3};
	eTOKENS expected[2] = {TOKEN_SEMICOLON, TOKEN_RIGHT_BRACKET3};
	Token* peek_token = NULL;

	curr_token = next_token();
	switch(curr_token->kind)
	{
		case TOKEN_SEMICOLON:
			peek_token = peek();
			if (peek_token->kind == TOKEN_KEYWORD_INTEGER || peek_token->kind == TOKEN_KEYWORD_REAL)
			{
				output("VAR_DEFINITIONS_TEMP -> ;VAR_DEFINITIONS");
				VAR_DEFINITIONS();
			}
			else 
			{
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
			error(expected, sizeof(expected)/sizeof(expected[0]));
	}
}

/* VAR_DEFINITION -> TYPE VARIABLES_LIST */
void VAR_DEFINITION()
{
	eTOKENS follows[2] = {TOKEN_SEMICOLON, TOKEN_RIGHT_BRACKET3};
	eTOKENS expected[4] = {TOKEN_SEMICOLON, TOKEN_RIGHT_BRACKET3, TOKEN_KEYWORD_REAL, TOKEN_KEYWORD_INTEGER};
	
	curr_token = next_token();

	switch(curr_token->kind)
	{
		case TOKEN_KEYWORD_REAL:
		case TOKEN_KEYWORD_INTEGER:
			output("VAR_DEFINITION -> TYPE VARIABLES_LIST");
			curr_token = back_token();
			elm_type var_type = TYPE();
			VARIABLES_LIST(var_type);
			break;

		default:
			error(expected, sizeof(expected)/sizeof(expected[0]));
			recover(follows, 2);
	}
}


/* TYPE -> real | integer */
elm_type TYPE()
{
	eTOKENS follows[1] = {TOKEN_ID};
	eTOKENS expected[2] = {TOKEN_KEYWORD_REAL, TOKEN_KEYWORD_INTEGER};

	curr_token = next_token();
	switch(curr_token->kind)
	{
		case TOKEN_KEYWORD_REAL:
			output("TYPE -> real");
			match(TOKEN_KEYWORD_REAL);
			return REAL;
		case TOKEN_KEYWORD_INTEGER: 
			output("TYPE -> integer");
			match(TOKEN_KEYWORD_INTEGER);
			return INTEGER;
		default: 
			error(expected, sizeof(expected)/sizeof(expected[0]));
			recover(follows, 1);
	}
}

/* VARIABLES_LIST -> VARIABLE VARIABLES_LIST_TEMP */
void VARIABLES_LIST(elm_type var_type) 
{
	eTOKENS follows[2] = {TOKEN_SEMICOLON, TOKEN_RIGHT_BRACKET3};
	eTOKENS expected[3] = {TOKEN_SEMICOLON, TOKEN_RIGHT_BRACKET3, TOKEN_ID};

	curr_token = next_token();
	if (curr_token->kind == TOKEN_ID)
	{
		output("VARIABLES_LIST -> VARIABLE VARIABLES_LIST_TEMP");	
		curr_token = back_token(); 
		VARIABLE(var_type);
		VARIABLES_LIST_TEMP(var_type);
	}
	else
	{
		error(expected, sizeof(expected)/sizeof(expected[0]));
		recover(follows, 2);	
	}
}


/* VARIABLES_LIST_TEMP -> ,VARIABLE VARIABLES_LIST_TEMP | ε */
void VARIABLES_LIST_TEMP(var_type)
{
	eTOKENS follows[2] = {TOKEN_SEMICOLON, TOKEN_RIGHT_BRACKET3};
	eTOKENS expected[3] = {TOKEN_COMMA, TOKEN_SEMICOLON, TOKEN_RIGHT_BRACKET3};

	curr_token = next_token();
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
			error(expected, sizeof(expected)/sizeof(expected[0]));
			recover(follows, 2);
	}
}

/* VARIABLE -> id VARIABLE_TEMP */
void VARIABLE()
{
	eTOKENS follows[3] = {TOKEN_COMMA, TOKEN_SEMICOLON, TOKEN_RIGHT_BRACKET3};
	eTOKENS expected[4] = {TOKEN_COMMA, TOKEN_SEMICOLON, TOKEN_RIGHT_BRACKET3,  TOKEN_ID};
	curr_token = next_token();

	if (curr_token->kind == TOKEN_ID)
	{
		output("VARIABLE -> id VARIABLE_TEMP");
		VARIABLE_TEMP();
	}
	else
	{
		error(expected, sizeof(expected)/sizeof(expected[0]));
		recover(follows, 3);	
	}
}

/* VARIABLE_TEMP -> [int_number] | ε */
void VARIABLE_TEMP()
{
	eTOKENS follows[4] = {TOKEN_SEMICOLON, TOKEN_COMMA, TOKEN_RIGHT_BRACKET3, TOKEN_OP_EQUAL};
	eTOKENS expected[5] = {TOKEN_LEFT_BRACKET1, TOKEN_COMMA, TOKEN_RIGHT_BRACKET3, TOKEN_SEMICOLON, TOKEN_OP_EQUAL};

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
			error(expected, sizeof(expected)/sizeof(expected[0]));
			recover(follows, 4);
	}
}

/* FUNC_DEFINITIONS -> FUNC_DEFINITION FUNC_DEFINITIONS_TEMP */
void FUNC_DEFINITIONS()
{
	eTOKENS follows[1] = {TOKEN_EOF};
	eTOKENS expected[4] = {TOKEN_KEYWORD_VOID,TOKEN_KEYWORD_REAL,TOKEN_KEYWORD_INTEGER, TOKEN_EOF};
	curr_token = next_token();

	switch(curr_token->kind)
	{
		case TOKEN_KEYWORD_REAL:
		case TOKEN_KEYWORD_INTEGER:
		case TOKEN_KEYWORD_VOID:
			output("FUNC_DEFINITIONS -> FUNC_DEFINITION FUNC_DEFINITIONS_TEMP");
			curr_token = back_token();
			FUNC_DEFINITION();
			FUNC_DEFINITIONS_TEMP();
			break;

		default:
			error(expected, sizeof(expected)/sizeof(expected[0]));
			recover(follows, 1);
	}
}


/* FUNC_DEFINITIONS_TEMP -> FUNC_DEFINITION FUNC_DEFINITIONS_TEMP | ε */
void FUNC_DEFINITIONS_TEMP()
{
	eTOKENS follows[1] = {TOKEN_EOF};
	eTOKENS expected[4] = {TOKEN_KEYWORD_INTEGER, TOKEN_KEYWORD_REAL, TOKEN_KEYWORD_VOID, TOKEN_EOF};

	curr_token = next_token();
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
			error(expected, sizeof(expected)/sizeof(expected[0]));
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
elm_type RETURNED_TYPE()
{
	eTOKENS follows[1] = {TOKEN_ID};
	eTOKENS expected[3] = {TOKEN_KEYWORD_VOID, TOKEN_KEYWORD_REAL, TOKEN_KEYWORD_INTEGER};

	curr_token = next_token();
	switch(curr_token->kind)
	{
		case TOKEN_KEYWORD_VOID:
			output("RETURNED_TYPE -> void");
			match(TOKEN_KEYWORD_VOID);
			return VOID_T;
		case TOKEN_KEYWORD_INTEGER: 
		case TOKEN_KEYWORD_REAL:
			curr_token = back_token();
			output("RETURNED_TYPE -> TYPE");
			return TYPE();
		default: 
			error(expected, sizeof(expected)/sizeof(expected[0]));
			recover(follows,1);
	}
}

/* PARAM_DEFINITIONS -> ε | VAR_DEFINITIONS */
void PARAM_DEFINITIONS()
{	
	eTOKENS follows[1] = {TOKEN_RIGHT_BRACKET3};
	eTOKENS expected[3] = {TOKEN_KEYWORD_INTEGER, TOKEN_KEYWORD_REAL, TOKEN_RIGHT_BRACKET3};

	curr_token = next_token();
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
			error(expected, sizeof(expected)/sizeof(expected[0]));
			recover(follows,1);
	}
}

/* STATEMENTS -> STATEMENT; STATEMENTS_TEMP */
void STATEMENTS()
{
	eTOKENS follows[2] = {TOKEN_KEYWORD_END, TOKEN_RIGHT_BRACKET2};
	eTOKENS expected[5] = {TOKEN_KEYWORD_END, TOKEN_RIGHT_BRACKET2, TOKEN_ID, TOKEN_LEFT_BRACKET2, TOKEN_KEYWORD_RETURN};
	curr_token = next_token();
	switch(curr_token->kind)
	{
		case TOKEN_ID:
		case TOKEN_LEFT_BRACKET2:
		case TOKEN_KEYWORD_RETURN:
			output("STATEMENTS -> STATEMENT; STATEMENTS_TEMP");
			curr_token = back_token();
			STATEMENT();
			curr_token = next_token();
			match(TOKEN_SEMICOLON);
			STATEMENTS_TEMP();
			break;
		default:
			error(expected, sizeof(expected)/sizeof(expected[0]));
			recover(follows,2);
	}
}


/* STATEMENTS_TEMP -> STATEMENTS | ε */
void STATEMENTS_TEMP()
{
	eTOKENS follows[2] = {TOKEN_KEYWORD_END, TOKEN_RIGHT_BRACKET2};
	eTOKENS expected[5] = {TOKEN_KEYWORD_RETURN, TOKEN_ID, TOKEN_LEFT_BRACKET2, TOKEN_KEYWORD_END, TOKEN_RIGHT_BRACKET2};
	
	curr_token = next_token();
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
			error(expected, sizeof(expected)/sizeof(expected[0]));
			recover(follows, 2);
	}
}

/* STATEMENT -> return STATEMENT_TEMP | BLOCK | id STATEMENT_TWO_TEMP */
void STATEMENT()
{
	eTOKENS follows[1] = {TOKEN_SEMICOLON};
	eTOKENS expected[3] = {TOKEN_KEYWORD_RETURN, TOKEN_LEFT_BRACKET2, TOKEN_ID};

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
			error(expected, sizeof(expected)/sizeof(expected[0]));
			recover(follows, 1);
	}
}

/* STATEMENT_TEMP -> EXPRESSION | ε */
void STATEMENT_TEMP()
{
	eTOKENS follows[1] = {TOKEN_SEMICOLON};
	eTOKENS expected[4] = {TOKEN_INT_NUMBER, TOKEN_ID, TOKEN_REAL_NUMBER, TOKEN_SEMICOLON};

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
			output("STATEMENT_TEMP -> ε");
			break;
		default:
			error(expected, sizeof(expected)/sizeof(expected[0]));
			recover(follows, 1);
	}
}

/* STATEMENT_TWO_TEMP -> (PARAMETERS_LIST) | VARIABLE_TEMP = EXPRESSION */
void STATEMENT_TWO_TEMP()
{
	eTOKENS follows[1] = {TOKEN_SEMICOLON};
	eTOKENS expected[3] = {TOKEN_LEFT_BRACKET3, TOKEN_OP_EQUAL, TOKEN_LEFT_BRACKET1};

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
			error(expected, sizeof(expected)/sizeof(expected[0]));
			recover(follows, 1);
	}
}

/* BLOCK -> { VAR_DEFINITIONS; STATEMENTS } */
void BLOCK()
{
	eTOKENS follows[4] = {TOKEN_KEYWORD_VOID, TOKEN_KEYWORD_REAL, TOKEN_KEYWORD_INTEGER, TOKEN_EOF};
	eTOKENS expected[5] = {TOKEN_KEYWORD_VOID, TOKEN_KEYWORD_REAL, TOKEN_KEYWORD_INTEGER, TOKEN_EOF, TOKEN_LEFT_BRACKET2};

	curr_token = next_token();
	switch(curr_token->kind)
	{
		case TOKEN_LEFT_BRACKET2:
			output("BLOCK -> { VAR_DEFINITIONS; STATEMENTS }");
			match(TOKEN_LEFT_BRACKET2);
			VAR_DEFINITIONS();
			curr_token = next_token();
			match(TOKEN_SEMICOLON);
			STATEMENTS();
			curr_token = next_token();
			match(TOKEN_RIGHT_BRACKET2);
			break;
		default:
			error(expected, sizeof(expected)/sizeof(expected[0]));
			recover(follows, 4);
	}
}


/* FUNCTION_CALL -> id (PARAMETERS_LIST) */
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
	eTOKENS expected[2] = {TOKEN_ID, TOKEN_RIGHT_BRACKET3};

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
			error(expected, sizeof(expected)/sizeof(expected[0]));
			recover(follows, 1);

	}
}

/* EXPRESSION -> int_number | real_number | id EXPRESSION_TEMP */
void EXPRESSION()
{
	eTOKENS follows[1] = {TOKEN_SEMICOLON};
	eTOKENS expected[3] = {TOKEN_INT_NUMBER, TOKEN_REAL_NUMBER, TOKEN_ID};

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
			error(expected, sizeof(expected)/sizeof(expected[0]));
			recover(follows, 1);
	}
}

/* EXPRESSION_TEMP -> VARIABLE_TEMP | ar_op EXPRESSION */
void EXPRESSION_TEMP()
{
	eTOKENS follows[1] = {TOKEN_SEMICOLON};
	eTOKENS expected[3] = {TOKEN_LEFT_BRACKET1, TOKEN_OP_MUL, TOKEN_OP_DIV};

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
			match(TOKEN_OP_DIV);
			output("EXPRESSION_TEMP -> ar_op EXPRESSION");
			EXPRESSION();
			break;
		default: 
			error(expected, sizeof(expected)/sizeof(expected[0]));
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


void error(eTOKENS expected[], int size)
{
	char expected_str[100] = "";
	if (size == 1)
		strcpy(expected_str, token_kinds[expected[0]]);
	else
		strcpy(expected_str, concatenate(size, expected, " or "));

	fprintf(parser_out, "Expected: one of tokens '%s' at line: %d, Actual token: '%s', lexeme: '%s'\n", 
			expected_str, curr_token->lineNumber, token_kinds[curr_token->kind], curr_token->lexeme);
}


void recover(eTOKENS follows[], int size)
{
	Token *token = back_token();
	int found = 0;
	int i; 
	while (found == 0)
	{
		token = next_token();
		if (token->kind == TOKEN_EOF)
		{
			exit(0);
		}
		else
		{
			for (i = 0; i < size; i++)
			{
				if (follows[i] == token->kind)
				{
					found = 1;
					back_token();
					break;
				}
			}
		}
	}	
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
	fprintf(parser_out, "Rule(%s)\n", rule);
}
