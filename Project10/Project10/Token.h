#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE* yyin, * yyout;

int printLexer;

typedef enum eTOKENS
{
	ID_TOKEN = 1, COMMA_TOKEN, COLON_TOKEN, SEMICOLON_TOKEN, LEFT_PARENTHESES_TOKEN,
	RIGHT_PARENTHESES_TOKEN, LEFT_BRACKET_TOKEN, RIGHT_BRACKET_TOKEN, LEFT_CURLY_BRACE_TOKEN, RIGHT_CURLY_BRACE_TOKEN,
	SMALLER_TOKEN, SMALLER_EQUAL_TOKEN, EQUAL_TOKEN, BIGGER_EQUAL_TOKEN, BIGGER_TOKEN,
	NOT_EQUAL_TOKEN, ASSIGN_TOKEN, BOOLEAN_ADD_TOKEN, BOOLEAN_MULTIPLY_TOKEN, INT_NUM_TOKEN,
	FLOAT_NUM_TOKEN, INT_TOKEN, FLOAT_TOKEN, VOID_TOKEN, IF_TOKEN, RETURN_TOKEN, EOF_TOKEN

	/*
		Examples of tokens, add/change according to your needs.

		Alternative way to define tokens:
		#define TOKEN_INTEGER 1
		#define TOKEN_IF 2
		...........
	*/
}eTOKENS;

typedef struct Token
{
	eTOKENS kind;
	char* lexeme;
	int lineNumber;
}Token;

typedef struct Node
{
	Token* tokensArray;
	struct Node* prev;
	struct Node* next;
} Node;

void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine);
Token* next_token();
Token* back_token();

//void printToken();

char* tokenType(eTOKENS tok);
#endif