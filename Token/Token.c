#include "Token.h"

/* This package describes the storage of tokens identified in the input text.
* The storage is a bi-directional list of nodes.
* Each node is an array of tokens; the size of this array is defined as TOKEN_ARRAY_SIZE.
* Such data structure supports an efficient way to manipulate tokens.

There are three functions providing an external access to the storage:
- function create_and_store_tokens ; it is called by the lexical analyzer when it identifies a legal token in the input text.
- functions next_token and back_token; they are called by parser during the syntax analysis (the second stage of compilation)
*/

int currentIndex = 0;
Node* currentNode = NULL;
int back = 0;

#define TOKEN_ARRAY_SIZE 100

/*
* This function creates a token and stores it in the storage.
*/
void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine)
{ 
	int length = strlen(lexeme)+1;
	
	// case 1: there is still no tokens in the storage.
	if (currentNode == NULL)
	{
		currentNode = (Node*)malloc(sizeof(Node));

		if(currentNode == NULL)
		{
			fprintf(yyout,"\nUnable to allocate memory! \n"); 
			exit(0);
		}
		currentNode->tokensArray = (Token*)calloc(sizeof(Token),TOKEN_ARRAY_SIZE);
		if(currentNode->tokensArray == NULL)
		{
			fprintf(yyout,"\nUnable to allocate memory! \n"); 
			exit(0);
		}
		currentNode->prev = NULL;
		currentNode->next = NULL;
	}

	// case 2: at least one token exsits in the storage.
	else
	{
		// the array (the current node) is full, need to allocate a new node
		if (currentIndex == TOKEN_ARRAY_SIZE - 1)
		{
			currentIndex = 0;
			currentNode->next = (Node*)malloc(sizeof(Node));

			if(currentNode == NULL)
			{
				fprintf(yyout,"\nUnable to allocate memory! \n"); 
				exit(0);
			}
			currentNode->next->prev = currentNode;
			currentNode = currentNode->next;
			currentNode->tokensArray = (Token*)calloc(sizeof(Token),TOKEN_ARRAY_SIZE);

			if(currentNode->tokensArray == NULL)
			{
				fprintf(yyout,"\nUnable to allocate memory! \n"); 
				exit(0);
			}
			currentNode->next = NULL;
		}

		// the array (the current node) is not full
		else
		{
			currentIndex++;
		}
	}

	currentNode->tokensArray[currentIndex].kind = kind;	
	currentNode->tokensArray[currentIndex].lineNumber = numOfLine;

	currentNode->tokensArray[currentIndex].lexeme = (char*)malloc(sizeof(char)*length);
	#ifdef _WIN32
		strcpy_s(currentNode->tokensArray[currentIndex].lexeme, length, lexeme);
	#else
		strcpy(currentNode->tokensArray[currentIndex].lexeme, lexeme);
	#endif		
}

/*
* This function returns the token in the storage that is stored immediately before the current token (if exsits).
*/
Token *back_token() 
{ 
	back++;
	if (currentIndex == 0) /* if we in the start of the array */
	{
		currentNode = currentNode->prev; /* go one back */
		currentIndex = TOKEN_ARRAY_SIZE - 1; /* start from the end */
	}
	else
	{
		currentIndex--;	
	}
	
	return &(currentNode->tokensArray[currentIndex]);
}

/*
* If the next token already exists in the storage (this happens when back_token was called before this call to next_token): 
*  this function returns the next stored token.
* Else: continues to read the input file in order to identify, create and store a new token (using yylex function);
*  returns the token that was created.
*/
Token *next_token() 
{ 
	if (back > 0)
	{	
		back--;	
		if (currentNode != NULL)
		{
			if (currentIndex == TOKEN_ARRAY_SIZE - 1 && currentNode->next != NULL) /* in the end and there is next */
			{
				currentNode = currentNode->next;
				currentIndex = 0;
			}
			else
			{
				currentIndex++;
			}
		}
		else
		{
			yylex();
		}
	}
	else
	{
		yylex();
	}
	
	return &(currentNode->tokensArray[currentIndex]);
}

Token* peek()
{
	Token *ans = next_token();
	back_token();
	return ans;
}

void clean_stored_tokes()
{
	Node* curr = currentNode;
	Node* temp = NULL;
	while (curr != NULL)
	{
		temp = curr->next;
		free_tokens(curr->tokensArray);
		free(curr);
		curr = temp;
	}
	currentNode = NULL;
	currentIndex = 0;
	int back = 0;
}

void free_tokens(Token* tokens)
{
	int i;
	for (i=0; i<TOKEN_ARRAY_SIZE; i++)
	{
		char *lexeme = tokens[i].lexeme;
		if (lexeme)
		{
			free(lexeme);
			lexeme = NULL;
		}
	}
	free(tokens);
	tokens = NULL;
}