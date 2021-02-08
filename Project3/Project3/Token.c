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
int maxIndex = 0;
Node* currentNode = NULL;

#define TOKEN_ARRAY_SIZE 1000

/*void printToken() {
	//fprintf(yyout, "Token of type %s ,lexme: %s ,found at line %d\n", tokenType(currentNode->tokensArray[currentIndex].kind), currentNode->tokensArray[currentIndex].lexeme, currentNode->tokensArray[currentIndex].lineNumber);
}*/

/*
* This function creates a token and stores it in the storage.
*/
void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine)
{
	int length = strlen(lexeme) + 1;

	// case 1: there is still no tokens in the storage.
	if (currentNode == NULL)
	{
		currentNode = (Node*)malloc(sizeof(Node));

		if (currentNode == NULL)
		{
			fprintf(yyout, "\nUnable to allocate memory! \n");
			exit(0);
		}
		currentNode->tokensArray = (Token*)calloc(sizeof(Token), TOKEN_ARRAY_SIZE);
		if (currentNode->tokensArray == NULL)
		{
			fprintf(yyout, "\nUnable to allocate memory! \n");
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

			if (currentNode == NULL)
			{
				fprintf(yyout, "\nUnable to allocate memory! \n");
				exit(0);
			}
			currentNode->next->prev = currentNode;
			currentNode = currentNode->next;
			currentNode->tokensArray =
				(Token*)calloc(sizeof(Token), TOKEN_ARRAY_SIZE);

			if (currentNode->tokensArray == NULL)
			{
				fprintf(yyout, "\nUnable to allocate memory! \n");
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

	currentNode->tokensArray[currentIndex].lexeme = (char*)malloc(sizeof(char) * length);
#ifdef _WIN32
	strcpy_s(currentNode->tokensArray[currentIndex].lexeme, length, lexeme);
#else
	strcpy(currentNode->tokensArray[currentIndex].lexeme, lexeme);
#endif
	if (printLexer == 1) fprintf(yyout, "Token of type %s ,lexme: %s ,found at line %d\n", tokenType(kind), lexeme, numOfLine);
}

/*
* This function returns the token in the storage that is stored immediately before the current token (if exsits).
*/
Token* back_token() {
	if (currentIndex == 0)return NULL;
	else {
		currentIndex--;
		return &currentNode->tokensArray[currentIndex];
	}
}

/*
* If the next token already exists in the storage (this happens when back_token was called before this call to next_token):
*  this function returns the next stored token.
* Else: continues to read the input file in order to identify, create and store a new token (using yylex function);
*  returns the token that was created.
*/
Token* next_token() {
	//printf_s("current_index b4 yylex %d\n", currentIndex);
	if (currentIndex >= maxIndex)
	{
		yylex();
		maxIndex = currentIndex;
	}
	else { currentIndex++; }
	//printf_s("current_index after yylex %d\n", currentIndex);
	return &currentNode->tokensArray[currentIndex];
}

char* tokenType(eTOKENS tok) {
	switch (tok) {
	case ID_TOKEN: return "ID_TOKEN";
	case COMMA_TOKEN: return "COMMA_TOKEN";
	case COLON_TOKEN: return "COLON_TOKEN";
	case SEMICOLON_TOKEN: return "SEMICOLON_TOKEN";
	case LEFT_PARENTHESES_TOKEN: return "LEFT_PARENTHESES_TOKEN";
	case RIGHT_PARENTHESES_TOKEN: return "RIGHT_PARENTHESES_TOKEN";
	case LEFT_BRACKET_TOKEN: return "LEFT_BRACKET_TOKEN";
	case RIGHT_BRACKET_TOKEN: return "RIGHT_BRACKET_TOKEN";
	case LEFT_CURLY_BRACE_TOKEN: return "LEFT_CURLY_BRACE_TOKEN";
	case RIGHT_CURLY_BRACE_TOKEN: return "RIGHT_CURLY_BRACE_TOKEN";
	case SMALLER_TOKEN: return "SMALLER_TOKEN";
	case SMALLER_EQUAL_TOKEN: return "SMALLER_EQUAL_TOKEN";
	case EQUAL_TOKEN: return "EQUAL_TOKEN";
	case BIGGER_EQUAL_TOKEN: return "BIGGER_EQUAL_TOKEN";
	case BIGGER_TOKEN: return "BIGGER_TOKEN";
	case NOT_EQUAL_TOKEN: return "NOT_EQUAL_TOKEN";
	case ASSIGN_TOKEN: return "ASSIGN_TOKEN";
	case BOOLEAN_ADD_TOKEN: return "BOOLEAN_ADD_TOKEN";
	case BOOLEAN_MULTIPLY_TOKEN: return "BOOLEAN_MULTIPLY_TOKEN";
	case INT_NUM_TOKEN: return "INT_NUM_TOKEN";
	case FLOAT_NUM_TOKEN: return "FLOAT_NUM_TOKEN";
	case INT_TOKEN: return "INT_TOKEN";
	case FLOAT_TOKEN: return "FLOAT_TOKEN";
	case VOID_TOKEN: return "VOID_TOKEN";
	case IF_TOKEN: return "IF_TOKEN";
	case RETURN_TOKEN: return "RETURN_TOKEN";
	case EOF_TOKEN: return "EOF_TOKEN";
	}
	return "INVALID_TOKEN";
}