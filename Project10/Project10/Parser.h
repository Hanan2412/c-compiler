#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Token.h"


//______________PARSING FUNCTIONS______________

//PROG -> GLOBAL_VARS FUNC_PREDEFS FUNC_FULL_DEFS
void parse_PROG();

//GLOBAL_VARS -> VAR_DEC GLOBAL_VARS_TAG
void parse_GLOBAL_VARS();

//GLOBAL_VARS_TAG -> VAR_DEC GLOBAL_VARS_TAG | EPSILON
void parse_GLOBAL_VARS_TAG();

//VAR_DEC -> TYPE id VAR_DEC_TAG
void parse_VAR_DEC();

//VAR_DEC_TAG -> ; | [ DIM_SIZES ] ;
void parse_VAR_DEC_TAG();

//TYPE-> int | float
void parse_TYPE();

//DIM_SIZES -> int_num DIM_SIZES_TAG
void parse_DIM_SIZES();

//DIM_SIZES_TAG -> , int_num DIM_SIZES_TAG | EPSILON
void parse_DIM_SIZES_TAG();

//FUNC_PREDEFS -> FUNC_PREDEFS_TAG FUNC_PROTOTYPE ;
void parse_FUNC_PREDEFS();

//FUNC_PREDEFS_TAG -> FUNC_PROTOTYPE ; FUNC_PREDEFS_TAG | EPSILON
void parse_FUNC_PREDEFS_TAG();

//FUNC_PROTOTYPE -> RETURNED_TYPE id ( PARAMS )
void parse_FUNC_PROTOTYPE();

//FUNC_FULL_DEFS -> FUNC_WITH_BODY FUNC_FULL_DEFS_TAG
void parse_FUNC_FULL_DEFS();

//FUNC_FULL_DEFS_TAG -> FUNC_WITH_BODY FUNC_FULL_DEFS_TAG | EPSILON
void parse_FUNC_FULL_DEFS_TAG();

//FUNC_WITH_BODY -> FUNC_PROTOTYPE COMP_STMT
void parse_FUNC_WITH_BODY();

//RETURNED_TYPE -> TYPE | void
void parse_RETURNED_TYPE();

//PARAMS -> PARAM_LIST | EPSILON
void parse_PARAMS();

//PARAM_LIST -> PARAM PARAM_LIST_TAG
void parse_PARAM_LIST();

//PARAM_LIST_TAG -> , PARAM PARAM_LIST_TAG | EPSILON
void parse_PARAM_LIST_TAG();

//PARAM -> TYPE id PARAM_TAG
void parse_PARAM();

//PARAM_TAG -> [ DIM_SIZES ] | EPSILON
void parse_PARAM_TAG();

//COMP_STMT -> { VAR_DEC_LIST STMT_LIST }
void parse_COMP_STMT();

//VAR_DEC_LIST -> VAR_DEC VAR_DEC_LIST_TAG
void parse_VAR_DEC_LIST();

//VAR_DEC_LIST_TAG -> VAR_DEC VAR_DEC_LIST_TAG | EPSILON
void parse_VAR_DEC_LIST_TAG();

//STMT_LIST -> STMT STMT_LIST_TAG
void parse_STMT_LIST();

//STMT_LIST_TAG -> ; STMT STMT_LIST_TAG | EPSILON
void parse_STMT_LIST_TAG();

//STMT -> id STMT_TAG | COMP_STMT | IF_STMT | RETURN_STMT
void parse_STMT();

//STMT_TAG -> ( ARGS ) | VAR_TAG = EXPR
void parse_STMT_TAG();

//IF_STMT -> if ( CONDITION ) STMT
void parse_IF_STMT();

//CALL -> id ( ARGS )
void parse_CALL();

//ARGS -> ARG_LIST | EPSILON
void parse_ARGS();

//ARG_LIST -> EXPR ARG_LIST_TAG
void parse_ARG_LIST();

//ARG_LIST_TAG -> , EXPR ARG_LIST_TAG | EPSILON
void parse_ARG_LIST_TAG();

//RETURN_STMT -> return RETURN_STMT_TAG
void parse_RETURN_STMT();

//RETURN_STMT_TAG -> EXPR | EPSILON
void parse_RETURN_STMT_TAG();

//VAR -> id VAR_TAG
void parse_VAR();

//VAR_TAG -> [EXPR_LIST] | EPSILON
void parse_VAR_TAG();

//EXPR_LIST -> EXPR EXPR_LIST_TAG
void parse_EXPR_LIST();

//EXPR_LIST_TAG -> , EXPR EXPR_LIST_TAG | EPSILON
void parse_EXPR_LIST_TAG();

//CONDITION -> EXPR rel_op EXPR
void parse_CONDITION();

//EXPR -> TERM EXPR_TAG
void parse_EXPR();

//EXPR_TAG -> + TERM EXPR_TAG | EPSILON
void parse_EXPR_TAG();

//TERM -> FACTOR TERM_TAG
void parse_TERM();

//TERM_TAG -> * FACTOR TERM_TAG | EPSILON
void parse_TERM_TAG();

//FACTOR -> id FACTOR_TAG | int_num | float_num | ( EXPR )
void parse_FACTOR();

void parse_FACTOR_TAG();

//______________EVERYTHING ELSE______________

void match(eTOKENS tok);

//void stepBack(int steps);
//void stepSkip(int steps);
void ErrorRecovery(int* arr, int arr_size, char* expected);
void lookahead();

char* names(eTOKENS tok);
#endif