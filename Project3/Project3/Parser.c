
#include "Parser.h"
#include "Token.h"
#include <stdbool.h>
#include "Sematic_Analayzer.h"

table_ptr* table; 
table_entry* entry,*tmpEntry,*assignEntry;
elm_type e_type;
char* variables = "NULL";
Token * t;
bool function = false;
bool assigment = false;
char* preName = NULL;

//PROG -> GLOBAL_VARS FUNC_PREDEFS FUNC_FULL_DEFS
int PROG_follow[1] = { EOF_TOKEN };
int GLOBAL_VARS_follow[3] = { INT_TOKEN,FLOAT_TOKEN,VOID_TOKEN };
int GLOBAL_VARS_TAG_follow[3] = { INT_TOKEN,FLOAT_TOKEN,VOID_TOKEN };
int VAR_DEC_follow[7] = { INT_TOKEN,FLOAT_TOKEN,VOID_TOKEN,ID_TOKEN,LEFT_CURLY_BRACE_TOKEN,IF_TOKEN,RETURN_TOKEN };
int VAR_DEC_TAG_follow[7] = { INT_TOKEN,FLOAT_TOKEN,VOID_TOKEN,ID_TOKEN,LEFT_CURLY_BRACE_TOKEN,IF_TOKEN,RETURN_TOKEN };
int TYPE_follow[1] = { ID_TOKEN };
int DIM_SIZES_follow[1] = { RIGHT_BRACKET_TOKEN };
int DIM_SIZES_TAG_follow[1] = { RIGHT_BRACKET_TOKEN };
int FUNC_PREDEFS_follow[3] = { INT_TOKEN,FLOAT_TOKEN,VOID_TOKEN };
int FUNC_PREDEFS_TAG_follow[3] = { INT_TOKEN,FLOAT_TOKEN,VOID_TOKEN };
int FUNC_PROTOTYPE_follow[2] = { COMMA_TOKEN,LEFT_CURLY_BRACE_TOKEN };
int FUNC_FULL_DEFS_follow[1] = { EOF_TOKEN };
int FUNC_FULL_DEFS_TAG_follow[1] = { EOF_TOKEN };
int FUNC_WITH_BODY_follow[4] = { INT_TOKEN,FLOAT_TOKEN,VOID_TOKEN,EOF_TOKEN };
int RETURED_TYPE_follow[1] = { ID_TOKEN };
int PARAMS_follow[1] = { RIGHT_PARENTHESES_TOKEN };
int PARAM_LIST_follow[1] = { RIGHT_PARENTHESES_TOKEN };
int PARAM_LIST_TAG_follow[1] = { RIGHT_PARENTHESES_TOKEN };
int PARAM_follow[2] = { COMMA_TOKEN,RIGHT_PARENTHESES_TOKEN };
int PARAM_TAG_follow[2] = { COMMA_TOKEN,RIGHT_PARENTHESES_TOKEN };
int COMP_STMT_follow[6] = { INT_TOKEN,FLOAT_TOKEN, VOID_TOKEN,SEMICOLON_TOKEN,RIGHT_CURLY_BRACE_TOKEN,EOF_TOKEN };
int VAR_DEC_LIST_follow[4] = { ID_TOKEN,LEFT_CURLY_BRACE_TOKEN,IF_TOKEN,RETURN_TOKEN };
int VAR_DEC_LIST_TAG_follow[4] = { ID_TOKEN,LEFT_CURLY_BRACE_TOKEN,IF_TOKEN,RETURN_TOKEN };
int STMT_LIST_follow[1] = { RIGHT_CURLY_BRACE_TOKEN };
int STMT_LIST_TAG_follow[1] = { RIGHT_CURLY_BRACE_TOKEN };
int STMT_follow[2] = { COMMA_TOKEN,RIGHT_CURLY_BRACE_TOKEN };
int STMT_TAG_follow[2] = { SEMICOLON_TOKEN,RIGHT_CURLY_BRACE_TOKEN };
int IF_STMT_folow[2] = { COMMA_TOKEN,RIGHT_CURLY_BRACE_TOKEN };
int ARGS_follow[1] = { RIGHT_PARENTHESES_TOKEN };
int ARGS_LIST_follow[1] = { RIGHT_PARENTHESES_TOKEN };
int ARGS_LIST_TAG_follow[1] = { RIGHT_PARENTHESES_TOKEN };
int RETURN_STMT_follow[2] = { SEMICOLON_TOKEN,RIGHT_CURLY_BRACE_TOKEN };
int RETURN_STMT_TAG_follow[2] = { SEMICOLON_TOKEN,RIGHT_CURLY_BRACE_TOKEN };
int VAR_TAG_follow[14] = { ASSIGN_TOKEN,BOOLEAN_MULTIPLY_TOKEN,BOOLEAN_ADD_TOKEN,SEMICOLON_TOKEN,RIGHT_CURLY_BRACE_TOKEN,COMMA_TOKEN,RIGHT_BRACKET_TOKEN,RIGHT_PARENTHESES_TOKEN,BIGGER_EQUAL_TOKEN,BIGGER_TOKEN,EQUAL_TOKEN,SMALLER_EQUAL_TOKEN,SMALLER_TOKEN };
int EXPR_LIST_follow[1] = { RIGHT_BRACKET_TOKEN };
int EXPR_LIST_TAG_follow[1] = { RIGHT_BRACKET_TOKEN };
int CONDITION_follow[1] = { RIGHT_PARENTHESES_TOKEN };
int EXPR_follow[11] = { SEMICOLON_TOKEN,RIGHT_CURLY_BRACE_TOKEN,COMMA_TOKEN,RIGHT_PARENTHESES_TOKEN,RIGHT_BRACKET_TOKEN,BIGGER_EQUAL_TOKEN,BIGGER_TOKEN,EQUAL_TOKEN,SMALLER_EQUAL_TOKEN,SMALLER_TOKEN };
int EXPR_TAG_follow[11] = { SEMICOLON_TOKEN,RIGHT_CURLY_BRACE_TOKEN,COMMA_TOKEN,RIGHT_PARENTHESES_TOKEN,RIGHT_BRACKET_TOKEN,BIGGER_EQUAL_TOKEN,BIGGER_TOKEN,EQUAL_TOKEN,SMALLER_EQUAL_TOKEN,SMALLER_TOKEN };
int TERM_follow[12] = { BOOLEAN_ADD_TOKEN,SEMICOLON_TOKEN,RIGHT_CURLY_BRACE_TOKEN,COMMA_TOKEN,RIGHT_PARENTHESES_TOKEN,RIGHT_BRACKET_TOKEN,BIGGER_EQUAL_TOKEN,BIGGER_TOKEN,EQUAL_TOKEN,SMALLER_EQUAL_TOKEN,SMALLER_TOKEN };
int TERM_TAG_follow[12] = { BOOLEAN_ADD_TOKEN,SEMICOLON_TOKEN,RIGHT_CURLY_BRACE_TOKEN,COMMA_TOKEN,RIGHT_PARENTHESES_TOKEN,RIGHT_BRACKET_TOKEN,BIGGER_EQUAL_TOKEN,BIGGER_TOKEN,EQUAL_TOKEN,SMALLER_EQUAL_TOKEN,SMALLER_TOKEN };
int FACTOR_follow[13] = { BOOLEAN_MULTIPLY_TOKEN, BOOLEAN_ADD_TOKEN,SEMICOLON_TOKEN,RIGHT_CURLY_BRACE_TOKEN,COMMA_TOKEN,RIGHT_PARENTHESES_TOKEN,RIGHT_BRACKET_TOKEN,BIGGER_EQUAL_TOKEN,BIGGER_TOKEN,EQUAL_TOKEN,SMALLER_EQUAL_TOKEN,SMALLER_TOKEN };
int FACTOR_TAG_follow[13] = { BOOLEAN_MULTIPLY_TOKEN, BOOLEAN_ADD_TOKEN,SEMICOLON_TOKEN,RIGHT_CURLY_BRACE_TOKEN,COMMA_TOKEN,RIGHT_PARENTHESES_TOKEN,RIGHT_BRACKET_TOKEN,BIGGER_EQUAL_TOKEN,BIGGER_TOKEN,EQUAL_TOKEN,SMALLER_EQUAL_TOKEN,SMALLER_TOKEN };


void printTable()
{
    table_entry* print = table->rows;
    while (print != NULL)
    {
        printf_s("%s", print->type);
        printf_s(" %s", print->name);
        if (print->params != NULL)
        {
            printf_s(" (");
            while (print->params != NULL)
            {
                printf_s(" %s", print->params->type);
                printf_s(" %s,", print->params->name);
                print->params = print->params->next;
            }
            printf_s(" )");
        }
        if (strcmp(print->role,"array")==0)
        {
            printf_s(" [ ");
            while (print->arrPtr->dims != NULL)
            {
                printf_s(" %d, ", print->arrPtr->dims->dim);
                print->arrPtr->dims = print->arrPtr->dims->nextDim;
            }
            printf_s(" ] ");
        }
        printf_s(" %s\n", print->role);
        print = print->next;
    }
}


void parse_PROG()

{
    //back_token();

    /*int i = 0;
    t = next_token();
    while (t->kind != EOF_TOKEN)
    //while(t->lineNumber < 50)
    {
        printf("%d) token kind %d , token lexme: %s , token line number %d\n", i, t->kind,t->lexeme,t->lineNumber);
        t = next_token();
        i++;
    }
    printf("%d) token kind %d , token lexme: %s , token line number %d\n", i, t->kind, t->lexeme, t->lineNumber);
   */
   //fprintf(yyout,"RULE(PROG -> GLOBAL_VARS)\n");
    table = make_table(NULL);
    fprintf(yyout, "RULE(PROG -> GLOBAL_VARS)\n");
    parse_GLOBAL_VARS();
    
    do {
        
        //fprintf(yyout,"token in parse_PROG in do kind %d , lexme %s , line number %d\n",t->kind,t->lexeme,t->lineNumber);
        parse_FUNC_PROTOTYPE();
        if (entry != NULL) 
        {
            updateRole(entry, "pre-definition");
            checkSimilarParams(entry,t->lineNumber);
        }
        else
            fprintf(yyout2, "line %d ,Error: duplicate declaration of pre defenition function: %s\n",t->lineNumber,preName);
        t = next_token();
        //printTable();
        /* legally, the next token after FUNC_PROTOTYPE
           can be either semicolon or {
           any other token is erroneous/unexpected */
    } while (t->kind == SEMICOLON_TOKEN);  /* if FUNC_PROTOTYPE is followed by a semicolon,
                                            then at least one more FUNC_PROTOTYPE is expected */

    if (t->kind == LEFT_CURLY_BRACE_TOKEN)  /* if FUNC_PROTOTYPE is followed by {
                                                 then actually the first FUNC_WITH_BODY is detected
                                                 and hence now the body of the function is expected */
    {
       
        t = back_token();        /* because every function parse_X starts itself with a call to next_token  */
        parse_COMP_STMT();       /* body of the first FUNC_WITH_BODY */
        entry = table->rows;
        while (entry->next != NULL)//moves the latest function inorder to update its role
        {
            entry = entry->next;
        }
        updateRole(entry, "full-defenition");
        t = next_token();
        while (t->kind != EOF_TOKEN)    /* something more after the first FUNC_WITH_BODY -
                                    this can be only FUNC_FULL_DEFS */
        {
            t = back_token();     /* because every function parse_X starts itself with a call to next_token  */
            parse_FUNC_FULL_DEFS();
        }

        t = back_token();     /* because in the main of parser, match(EOF) appears after the call to parse_PROG(),
                                 and function match() itself starts with next_token  */
    }
    else {
        //fprintf(yyout,"ERROR: FUNC_PROTOTYPE can be followed only by semicolon or by {\n");   /* FUNC_PROTOTYPE can be followed only by semicolon or by {   */
        fprintf(yyout, "ERROR: FUNC_PROTOTYPE can be followed only by semicolon or by {\n");
        while (t->kind != EOF_TOKEN) t = next_token();   /* skip till the end of input */
    }
    printTable();
}  /* end of parse_PROG  */

void parse_GLOBAL_VARS() //GLOBAL_VARS -> VAR_DEC GLOBAL_VARS_TAG
{
    //fprintf(yyout,"RULE(GLOBAL_VARS -> VAR_DEC)\n");
    fprintf(yyout, "RULE(GLOBAL_VARS -> VAR_DEC)\n");
    parse_VAR_DEC();
    //fprintf(yyout,"RULE(GLOBAL_VARS -> GLOBAL_VARS_DEC)\n");
    fprintf(yyout, "RULE(GLOBAL_VARS -> GLOBAL_VARS_DEC)\n");
    parse_GLOBAL_VARS_TAG();

}/* end of parse_GLOBAL_VARS() */

void parse_GLOBAL_VARS_TAG() //GLOBAL_VARS_TAG -> VAR_DEC GLOBAL_VARS_TAG | EPSILON
{
    //fprintf(yyout,"RULE(GLOBAL_VARS_TAG -> VAR_DEC)\n");
    fprintf(yyout, "RULE(GLOBAL_VARS_TAG -> VAR_DEC)\n");
    parse_VAR_DEC();
    t = next_token();
    //fprintf(yyout,"token in parse_GLOBAL_VARS_TAG: kind: %d , lexme: %s, line number %d\n", t->kind, t->lexeme, t->lineNumber);
    switch (t->kind)
    {
    case INT_TOKEN: {
        //fprintf(yyout,"int token in parse_GLOBAL_VARS_TAG\n"); 
        back_token();
        lookahead();
        if (!function)
        {
            //fprintf(yyout,"RULE(GLOBAL_VARS_TAG -> GLOBAL_VARS_TAG)\n");
            fprintf(yyout, "RULE(GLOBAL_VARS_TAG -> GLOBAL_VARS_TAG)\n");
            parse_GLOBAL_VARS_TAG();
        }
        break; }
    case FLOAT_TOKEN: {
        //fprintf(yyout,"float token in parse_GLOBAL_VARS_TAG\n");
        back_token();
        lookahead();
        if (!function)
        {
            //fprintf(yyout,"RULE(GLOBAL_VARS_TAG -> GLOBAL_VARS_TAG)\n");
            fprintf(yyout, "RULE(GLOBAL_VARS_TAG -> GLOBAL_VARS_TAG)\n");
            parse_GLOBAL_VARS_TAG();
        }
        break; }
    case VOID_TOKEN: {
        //fprintf(yyout,"RULE(GLOBAL_VARS_TAG -> EPSILON)\n");
        fprintf(yyout, "RULE(GLOBAL_VARS_TAG -> EPSILON)\n");
        back_token();
        break;
    }
    case RIGHT_BRACKET_TOKEN: {
        //fprintf(yyout,"right bracket token in parse_GLOBAL_VARS_TAG");
        match(SEMICOLON_TOKEN); break; }
    default: {ErrorRecovery(GLOBAL_VARS_TAG_follow, 3, "{ INT_TOKEN, FLOAT_TOKEN, VOID_TOKEN, RIGHT_BRACKET_TOKEN }");
        parse_GLOBAL_VARS_TAG();
    }

    }

}

void parse_VAR_DEC() //VAR_DEC -> TYPE id VAR_DEC_TAG
{
    t = next_token();
    fprintf(yyout, "RULE(VAR_DEC -> TYPE)\n");
    parse_TYPE();
    t = next_token();
    switch (t->kind)
    {
    case ID_TOKEN: {
        entry = insert(table, t->lexeme,t->lineNumber);
       /* if (entry == NULL)
        {
            tmpEntry = insert_possible_function(t->lexeme,table);
            set_id_type(tmpEntry, e_type);
            updateRole(tmpEntry, "full-defenition");
        }*/
        set_id_type(entry,e_type);/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        updateRole(entry,"variable");
        // printf_s("RULE(id VAR_DEC -> VAR_DEC_TAG)\n");
        fprintf(yyout, "RULE(id VAR_DEC -> VAR_DEC_TAG)\n");
        parse_VAR_DEC_TAG();
        break; }
    default: ErrorRecovery(VAR_DEC_follow, 7, "{ ID_TOKEN }");
    }
}

void parse_VAR_DEC_TAG() //VAR_DEC_TAG -> ; | [ DIM_SIZES ] ;
{
    t = next_token();
    switch (t->kind) {
    case SEMICOLON_TOKEN: {fprintf(yyout, "RULE(VAR_DEC_TAG -> ;)\n");
        break; }

    case LEFT_BRACKET_TOKEN: {fprintf(yyout, "RULE(VAR_DEC_TAG -> [ DIM_SIZES ] ;)\n");
        updateRole(entry, "array");
        entry->arrPtr = NULL;
        parse_DIM_SIZES();
        match(RIGHT_BRACKET_TOKEN);
        match(SEMICOLON_TOKEN);
        break; }
    case INT_TOKEN: {fprintf(yyout, "RULE(VAR_DEC_TAG -> EPSILON)"); back_token(); break; }
    case FLOAT_TOKEN: {fprintf(yyout, "RULE(VAR_DEC_TAG -> EPSILON)"); back_token(); break; }
    case VOID_TOKEN: {fprintf(yyout, "RULE(VAR_DEC_TAG -> EPSILON)"); back_token(); break; }
    case ID_TOKEN: {fprintf(yyout, "RULE(VAR_DEC_TAG -> EPSILON)"); back_token(); break; }
    case LEFT_CURLY_BRACE_TOKEN: {fprintf(yyout, "RULE(VAR_DEC_TAG -> EPSILON)"); back_token(); break; }
    case IF_TOKEN: {fprintf(yyout, "RULE(VAR_DEC_TAG -> EPSILON)"); back_token();  break; }
    case RETURN_TOKEN: {fprintf(yyout, "RULE(VAR_DEC_TAG -> EPSILON)"); back_token(); break; }
    default: {ErrorRecovery(VAR_DEC_TAG_follow, 7, "{ SEMICOLON_TOKEN, LEFT_BRACKET_TOKEN, INT_TOKEN, FLOAT_TOKEN, VOID_TOKEN, ID_TOKEN, LEFT_CURLY_BRACE_TOKEN, IF_TOKEN, RETURN_TOKEN }"); }
    }
}

void parse_TYPE() //TYPE -> int | float
{
    switch (t->kind) {
    case INT_TOKEN: {fprintf(yyout, "RULE(TYPE -> int)\n");
        e_type.elm_type = "int";
        break; }
    case FLOAT_TOKEN: {fprintf(yyout, "RULE(TYPE -> float)\n");
        e_type.elm_type = "float";
        break; }
                    //case EOF_TOKEN:fprintf(yyout,"TYPE - EOF\n"); break;
    default:
        ErrorRecovery(TYPE_follow, 1, "{ INT_TOKEN, FLOAT_TOKEN }");
        //parse_TYPE();
    }

}

void parse_DIM_SIZES() //DIM_SIZES -> int_num DIM_SIZES_TAG
{
    t = next_token();
    switch (t->kind) {
    case INT_NUM_TOKEN: {fprintf(yyout, "RULE(int_num DIM_SIZES_TAG)\n");

        //entry = updateArray(entry, t->lexeme);
        setArrayPtrDim(table, entry, t->lexeme);
        parse_DIM_SIZES_TAG();
        break;
    }
    default: ErrorRecovery(DIM_SIZES_follow, 1, "{ INT_NUM_TOKEN }");
    }
}

void parse_DIM_SIZES_TAG()//DIM_SIZES_TAG -> , int_num DIM_SIZES_TAG | EPSILON
{
    t = next_token();
    switch (t->kind)
    {
    case COMMA_TOKEN: {fprintf(yyout, "RULE(DIM_SIZES_TAG -> , int_num DIM_SIZES_TAG)\n");
        //fprintf(yyout,"comma token in parse_DIM_SIZES_TAG");
        match(INT_NUM_TOKEN);
        setArrayPtrDim(table,entry, t->lexeme);
        parse_DIM_SIZES_TAG();
        break;
    }
                    //case INT_NUM_TOKEN: {fprintf(yyout,"integer token in parse_DIM_SIZES_TAG"); parse_DIM_SIZES_TAG(); break; }
    case RIGHT_BRACKET_TOKEN: {
        fprintf(yyout, "RULE(DIM_SIZES_TAG -> EPSILON)\n");
        back_token();
        break; }
                            //case LEFT_BRACKET_TOKEN: {printf("left braket - token kind %d\n", t->kind); }
    default:ErrorRecovery(DIM_SIZES_TAG_follow, 1, "{ COMMA_TOKEN, RIGHT_BRACKET_TOKEN }");
    }
}

void parse_FUNC_PREDEFS() //FUNC_PREDEFS -> FUNC_PREDEFS_TAG FUNC_PROTOTYPE ;
{
    do {
        t = next_token();
        parse_FUNC_PREDEFS_TAG();
    } while (t->kind == SEMICOLON_TOKEN);

    parse_FUNC_PROTOTYPE();
}

void parse_FUNC_PREDEFS_TAG() //FUNC_PREDEFS_TAG -> FUNC_PROTOTYPE ; FUNC_PREDEFS_TAG | EPSILON
{
    parse_FUNC_PROTOTYPE();
    t = next_token();
    switch (t->kind)
    {//need lookahead to check if it in first or follow
    case INT_NUM_TOKEN: {fprintf(yyout, "RULE(FUNC_PREDEF_TAG -> EPSILON)\n");
        back_token();
        break; }
    case FLOAT_NUM_TOKEN: {fprintf(yyout, "RULE(FUNC_PREDEF_TAG -> EPSILON)\n");
        back_token();
        break; }
    case VOID_TOKEN: {fprintf(yyout, "RULE(FUNC_PREDEF_TAG -> EPSILON)\n");
        back_token();
        break; }
    default: {ErrorRecovery(FUNC_PREDEFS_TAG_follow, 3, "{ INT_NUM_TOKEN, FLOAT_NUM_TOKEN, VOID_TOKEN }");
        break; }
    }


}

void parse_FUNC_PROTOTYPE() //FUNC_PROTOTYPE -> RETURNED_TYPE id ( PARAMS )
{
    // fprintf(yyout,"token in parse_FUNC_PROTOTYPE %d  lexeme: %s , line number %d\n",t->kind,t->lexeme,t->lineNumber);
    parse_RETURNED_TYPE();
    t = next_token();
    switch (t->kind) {
    case ID_TOKEN: {
        preName = t->lexeme;
        entry = insert(table,t->lexeme,t->lineNumber);
        if (entry == NULL)
        {
            tmpEntry = insert_possible_function(t->lexeme,table,t->lineNumber);
            set_id_type(tmpEntry, e_type);
            updateRole(tmpEntry, "full-defenition");
        }
        set_id_type(entry, e_type);
        fprintf(yyout, "RULE(FUNC_PROTOTYPE -> RETURNED_TYPE id ( PARAMS ) )\n");
        match(LEFT_PARENTHESES_TOKEN);
        parse_PARAMS();
        match(RIGHT_PARENTHESES_TOKEN);
        break;
    }
    case COMMA_TOKEN: {fprintf(yyout, "FUNC_PROTOTYPE -> ,\n"); break; }
    default: ErrorRecovery(FUNC_PROTOTYPE_follow, 2, "{ ID_TOKEN, COMMA_TOKEN }");
    }

}

void parse_FUNC_FULL_DEFS()
{
    fprintf(yyout, "RULE(FUNC_FULL_DEFS -> FUNC_WITH_BODY FUNC_FULL_DEFS_TAG)\n");
    parse_FUNC_WITH_BODY();
    parse_FUNC_FULL_DEFS_TAG();
}

void parse_FUNC_FULL_DEFS_TAG() //FUNC_FULL_DEFS_TAG -> FUNC_WITH_BODY FUNC_FULL_DEFS_TAG | EPSILON
{
    entry = table->rows;
    while (entry->next != NULL)
        entry = entry->next;
    updateRole(entry,"full-defenition");
    t = next_token();
    switch (t->kind)
    {
    case INT_TOKEN: {
        fprintf(yyout, "RULE(FUNC_FULL_DEFS_TAG -> FUNC_WITH_BODY FUNC_FULL_DEFS_TAG)\n");
        back_token();
        parse_FUNC_WITH_BODY();
        parse_FUNC_FULL_DEFS_TAG();
        break; }
    case FLOAT_TOKEN: {
        fprintf(yyout, "RULE(FUNC_FULL_DEFS_TAG -> FUNC_WITH_BODY FUNC_FULL_DEFS_TAG)\n");
        back_token();
        parse_FUNC_WITH_BODY();
        parse_FUNC_FULL_DEFS_TAG();
        break; }
    case VOID_TOKEN: {
        fprintf(yyout, "RULE(FUNC_FULL_DEFS_TAG -> FUNC_WITH_BODY FUNC_FULL_DEFS_TAG)\n");
        back_token();
        parse_FUNC_WITH_BODY();
        parse_FUNC_FULL_DEFS_TAG();
        break; }
    case EOF_TOKEN: {fprintf(yyout, "EOF\n"); break; }
    default: ErrorRecovery(FUNC_FULL_DEFS_TAG_follow, 1, "{ INT_TOKEN, FLOAT_TOKEN, VOID_TOKEN, EOF_TOKEN }");
        break;
    }

}

void parse_FUNC_WITH_BODY()
{
    fprintf(yyout, "RULE(FUNC_WITH_BODY -> FUNC_PROTOTYPE COMP_STMT)\n");
    parse_FUNC_PROTOTYPE();
    parse_COMP_STMT();
}

void parse_RETURNED_TYPE() //RETURNED_TYPE -> TYPE | void
{
    t = next_token();
    switch (t->kind)
    {
    case INT_TOKEN: {
        fprintf(yyout, "RULE(RETURNED_TYPE -> TYPE)\n");
        parse_TYPE();
        break; }
    case FLOAT_TOKEN: {
        fprintf(yyout, "RULE(RETURNED_TYPE -> TYPE)\n");
        parse_TYPE();
        break; }
    case VOID_TOKEN: {
        //fprintf(yyout,"RETURNED_TYPE -> void\n"); 
        break; }
    default: ErrorRecovery(RETURED_TYPE_follow, 1, "{ INT_TOKEN, FLOAT_TOKEN, VOID_TOKEN }");
        break;
    }


}//continues running until it sees an ID_TOKEN because  it belongs to PARAMS

void parse_PARAMS() {

    t = next_token();
    switch (t->kind)
    {
    case INT_TOKEN: {
        fprintf(yyout, "RULE(PARAMS -> PARAM_LIST)\n");
        parse_PARAM_LIST();
        break; }
    case FLOAT_TOKEN: {
        fprintf(yyout, "RULE(PARAMS -> PARAM_LIST)\n");
        parse_PARAM_LIST();
        break; }
    case RIGHT_PARENTHESES_TOKEN: {
        fprintf(yyout, "RULE(PARAMS -> EPSILON)");
        back_token();
        break; }
    default: {
        ErrorRecovery(PARAMS_follow, 1, "{ INT_TOKEN, FLOAT_TOKEN, RIGHT_PARENTHESES_TOKEN }");
        //parse_PARAMS();
        break;
    }
    }


}

void parse_PARAM_LIST()
{
    fprintf(yyout, "RULE(PARAM_LIST -> PARAM PARAM_LIST_TAG)\n");
    parse_PARAM();
    parse_PARAM_LIST_TAG();

}

void parse_PARAM_LIST_TAG() //PARAM_LIST_TAG -> , PARAM PARAM_LIST_TAG | EPSILON
{
    t = next_token();
    switch (t->kind)
    {
    case COMMA_TOKEN: {fprintf(yyout, "RULE(PARAM_LIST_TAG -> , PARAM PARAM_LIST_TAG)\n");
        t = next_token();
        parse_PARAM();
        parse_PARAM_LIST_TAG();
        break;
    }
    case RIGHT_PARENTHESES_TOKEN: {
        back_token();
        break; }
    default: {
        ErrorRecovery(PARAM_LIST_TAG_follow, 1, "{ COMMA_TOKEN, RIGHT_PARENTHESES_TOKEN }");
        //back_token();
        parse_PARAM_LIST_TAG();
    }
    };
}

void parse_PARAM()
{
    parse_TYPE();
    t = next_token();
    switch (t->kind)
    {
    case ID_TOKEN: {fprintf(yyout, "RULE(PARAM -> TYPE id PARAM_TAG)\n");
        if (entry == NULL) {
            tmpEntry = set_func_params(tmpEntry, t->lexeme);
            set_func_param_type(tmpEntry, e_type);
        }
        else {
            entry = set_func_params(entry, t->lexeme);
            set_func_param_type(entry, e_type);
        }
        parse_PARAM_TAG();
        break; }
    default:ErrorRecovery(PARAM_follow, 2, "{ ID_TOKEN }");
    }

}

void parse_PARAM_TAG()
{
    t = next_token();
    switch (t->kind)
    {
    case LEFT_BRACKET_TOKEN: {fprintf(yyout, "RULE(PARAM_TAG -> [ DIM_SIZES ])\n");
        parse_DIM_SIZES();
        break; }
    case COMMA_TOKEN: {fprintf(yyout, "RULE(PARAM_TAG -> EPSILON)\n"); back_token(); break; }
    case RIGHT_PARENTHESES_TOKEN: {fprintf(yyout, "RULE(PARAM_TAG -> EPSILON)\n"); back_token(); break; }
    default: {
        ErrorRecovery(PARAM_TAG_follow, 2, "{ COMMA_TOKEN, RIGHT_PARENTHESES_TOKEN }");
        t = back_token();
    }
    }

}

void parse_COMP_STMT()
{
    t = next_token();//<<============================================================================================MADE A CHANGE HERE
    fprintf(yyout, "RULE(COMP_STMT -> { VAR_DEC_LIST STMT_LIST } )\n");
    if (entry == NULL)
    {
        entry = table->rows;
        while (entry->next != NULL)
            entry = entry->next;
        entry->next = tmpEntry;
        if(tmpEntry!=NULL)
            checkTypes(table,entry->next);
    }
    match(LEFT_CURLY_BRACE_TOKEN);
    table->sons = make_table(table);
    table = table->sons;
    parse_VAR_DEC_LIST();
    parse_STMT_LIST();
    match(RIGHT_CURLY_BRACE_TOKEN);
}

void parse_VAR_DEC_LIST()
{
    fprintf(yyout, "RULE(VAR_DEC_LIST -> VAR_DEC_LIST_TAG)\n");
    parse_VAR_DEC_LIST_TAG();
}

void parse_VAR_DEC_LIST_TAG()
{
    t = next_token();
    switch (t->kind)
    {
    case INT_TOKEN: {fprintf(yyout, "RULE(VAR_DEC_LIST_TAG -> VAR_DEC VAR_DEC_LIST_TAG)\n");
        t = back_token();
        parse_VAR_DEC();
        parse_VAR_DEC_LIST_TAG();
        break; }
    case FLOAT_TOKEN: {fprintf(yyout, "RULE(VAR_DEC_LIST_TAG -> VAR_DEC VAR_DEC_LIST_TAG)\n");
        t = back_token();
        parse_VAR_DEC();
        parse_VAR_DEC_LIST_TAG();
        break; }
    case ID_TOKEN: {fprintf(yyout, "RULE(VAR_DEC_LIST_TAG -> EPSILON)\n");
        t = back_token();
        break; }
    case LEFT_CURLY_BRACE_TOKEN: {fprintf(yyout, "RULE(VAR_DEC_LIST_TAG -> EPSILON)\n");
        back_token();
        break; }
    case IF_TOKEN: {fprintf(yyout, "RULE(VAR_DEC_LIST_TAG -> EPSILON)\n");
        back_token();
        break; }
    case RETURN_TOKEN: {fprintf(yyout, "RULE(VAR_DEC_LIST_TAG -> EPSILON)\n");
        back_token();
        break; }
    default: {ErrorRecovery(VAR_DEC_LIST_TAG_follow, 4, "{ INT_TOKEN, FLOAT_TOKEN, ID_TOKEN, LEFT_CURLY_BRACE_TOKEN, IF_TOKEN, RETURN_TOKEN }"); break; }

    }
}

void parse_STMT_LIST() //STMT_LIST -> STMT STMT_LIST_TAG
{
    fprintf(yyout, "RULE(STMT_LIST -> STMT STMT_LIST_TAG)\n");
    parse_STMT();
    parse_STMT_LIST_TAG();
}

void parse_STMT_LIST_TAG() //STMT_LIST_TAG -> ; STMT STMT_LIST_TAG | EPSILON
{
    t = next_token();
    switch (t->kind)
    {
    case SEMICOLON_TOKEN: {
        fprintf(yyout, "RULE(STMT_LIST_TAG -> ; STMT STMT_LIST_TAG)\n");
        parse_STMT();
        parse_STMT_LIST_TAG();
        break; }
    case RIGHT_CURLY_BRACE_TOKEN: {
        table = pop_table(table);
        t = back_token();
        break;
    }
    default: {
        ErrorRecovery(STMT_LIST_TAG_follow, 1, "{ SEMICOLON_TOKEN, RIGHT_CURLY_BRACE_TOKEN }");
        break; }
    }
}

void parse_STMT() //STMT -> id STMT_TAG | COMP_STMT | IF_STMT | RETURN_STMT
{
    t = next_token();
    switch (t->kind)
    {
    case ID_TOKEN: {fprintf(yyout, "RULE(STMT -> id STMT_TAG)\n");
        if(entry!=NULL)
            find(table, t->lexeme,t->lineNumber);
        parse_STMT_TAG();
        break; }
    case LEFT_CURLY_BRACE_TOKEN: {fprintf(yyout, "RULE(STMT -> COMP_STMT)\n");
        back_token();
        parse_COMP_STMT();
        break; }
    case IF_TOKEN: {fprintf(yyout, "RULE(STMT -> IF_STMT)\n");
        parse_IF_STMT();
        break; }
    case RETURN_TOKEN: {fprintf(yyout, "RULE(STMT -> RETURN_STMT)\n");
        parse_RETURN_STMT();
        break; }
    case RIGHT_CURLY_BRACE_TOKEN: back_token(); break;//<<==========================================================================CHANGED THIS AS WELL
    default:ErrorRecovery(STMT_follow, 2, "{ ID_TOKEN, LEFT_CURLY_BRACE_TOKEN, IF_TOKEN, RETURN_TOKEN }");
    }
}

void parse_STMT_TAG() //STMT_TAG -> ( ARGS ) | VAR_TAG = EXPR
{
    assignEntry = lookup(table, t->lexeme);/*@ I DID A THING @*/
    t = next_token();
    {
        switch (t->kind)
        {
        case LEFT_PARENTHESES_TOKEN: {
            fprintf(yyout, "RULE(STMT_TAG -> ( ARGS ) )\n");
            parse_ARGS();
            match(RIGHT_PARENTHESES_TOKEN);
            break; }
        case ASSIGN_TOKEN: {fprintf(yyout, "RULE(STMT_TAG -> VAR_TAG = EXPR )\n");
           // assigment = true;
            
            parse_VAR_TAG();
            match(ASSIGN_TOKEN);
            parse_EXPR();
            break; }
        case LEFT_BRACKET_TOKEN: {fprintf(yyout, "RULE(STMT_TAG -> VAR_TAG = EXPR )\n");
            parse_VAR_TAG();
            match(ASSIGN_TOKEN);
            parse_EXPR();
            break; }
        default: {
            /*fprintf(yyout,"RULE(STMT_TAG -> VAR_TAG = EXPR )\n");
            parse_VAR_TAG();
            match(ASSIGN_TOKEN);
            parse_EXPR();*/
            ErrorRecovery(STMT_TAG_follow, 2, "{ LEFT_PARENTHESES_TOKEN, ASSIGN_TOKEN, LEFT_BRACKET_TOKEN }");
            break; }
        }
    }

}

void parse_IF_STMT()
{
    fprintf(yyout, "RULE(IF_STMT -> if ( CONDITION ) STMT )\n");
    match(LEFT_PARENTHESES_TOKEN);
    if (t->kind == LEFT_PARENTHESES_TOKEN)
    {
        parse_CONDITION();
        match(RIGHT_PARENTHESES_TOKEN);
        parse_STMT();
    }
    else { ErrorRecovery(IF_STMT_folow, 2, "{ LEFT_PARENTHESES_TOKEN }"); }
}

void parse_CALL()
{
    t = next_token();
    switch (t->kind)
    {
    case ID_TOKEN: {
        fprintf(yyout, "RULE(CALL -> id ( ARGS ) )\n");
        match(LEFT_PARENTHESES_TOKEN);
        parse_ARGS();
        match(RIGHT_PARENTHESES_TOKEN); }
    default: fprintf(yyout, "Expected token of type %s at line: %d, Actual token of type %s, lexeme: %s.", tokenType(ID_TOKEN), t->lineNumber, tokenType(t->kind), t->lexeme);
    }
}

void parse_ARGS()
{
    switch (t->kind)
    {
    case RIGHT_PARENTHESES_TOKEN: {fprintf(yyout, "RULE(ARGS -> EPSILON)\n"); back_token(); break; }
    case ID_TOKEN: {fprintf(yyout, "RULE(ARGS -> ID_TOKEN)\n"); break; }
    case INT_NUM_TOKEN: {fprintf(yyout, "RULE(ARGS -> INT_NUM_TOKEN)\n"); break; }
    case FLOAT_NUM_TOKEN: {fprintf(yyout, "RULE(ARGS -> FLOAT_NUM_TOKEN)\n"); break; }
    case LEFT_PARENTHESES_TOKEN: {fprintf(yyout, "RULE(ARGS -> ARG_LIST)\n");
        parse_ARG_LIST();
        break; }

    default: ErrorRecovery(ARGS_follow, 1, "{ RIGHT_PARENTHESES_TOKEN, ID_TOKEN, INT_NUM_TOKEN, FLOAT_NUM_TOKEN, LEFT_PARENTHESES_TOKEN }");
    }
}

void parse_ARG_LIST()
{
    fprintf(yyout, "RULE(ARG_LIST -> EXPR ARG_LIST_TAG)\n");
    parse_EXPR();
    parse_ARG_LIST_TAG();
}

void parse_ARG_LIST_TAG()
{
    t = next_token();
    switch (t->kind)
    {
    case COMMA_TOKEN: {fprintf(yyout, "RULE(ARG_LIST_TAG -> , EXPR ARG_LIST_TAG)\n");
        parse_EXPR();
        parse_ARG_LIST_TAG();
        break; }
    case RIGHT_PARENTHESES_TOKEN: {fprintf(yyout, "RULE(ARG_LIST_TAG -> EPSILON)\n");
        back_token();
        break; }
    default:ErrorRecovery(ARGS_LIST_TAG_follow, 1, "{ COMMA_TOKEN, RIGHT_PARENTHESES_TOKEN }");
    }
}

void parse_RETURN_STMT()
{
    switch (t->kind)
    {
    case RETURN_TOKEN: {fprintf(yyout, "RULE(RETURN_STMT -> return RETURN_STMT_TAG)\n");
        parse_RETURN_STMT_TAG();
        break; }
    default:ErrorRecovery(RETURN_STMT_follow, 2, "{ RETURN_TOKEN }");
    }
}
void parse_RETURN_STMT_TAG()    //RETURN_STMT_TAG -> EXPR | EPSILON
{
    t = next_token();
    switch (t->kind)
    {
    case ID_TOKEN: {fprintf(yyout, "RULE(RETURN_STMT_TAG -> EXPR)\n");
        back_token();
        parse_EXPR();
        break; }
    case SEMICOLON_TOKEN: {fprintf(yyout, "RULE(RETURN_STMT_TAG -> EPSILON)\n"); back_token(); break; }
    case RIGHT_CURLY_BRACE_TOKEN: {fprintf(yyout, "RULE(RETURN_STMT_TAG -> EPSILON)\n"); back_token(); break; }
    case INT_NUM_TOKEN: {
        break; }
    case FLOAT_NUM_TOKEN: {
        break; }
    case LEFT_PARENTHESES_TOKEN: {
        break; }
    default:ErrorRecovery(RETURN_STMT_TAG_follow, 2, "{ ID_TOKEN, SEMICOLON_TOKEN, RIGHT_CURLY_BRACE_TOKEN, INT_NUM_TOKEN, FLOAT_NUM_TOKEN, LEFT_PARENTHESES_TOKEN }");
    }
}

void parse_VAR() //VAR -> id VAR_TAG
{
    t = next_token();
    switch (t->kind)
    {
    case ID_TOKEN:
    {
        fprintf(yyout, "RULE(VAR -> id VAR_TAG)\n");
        parse_VAR_TAG();
        break;
    }
    default:fprintf(yyout, "Expected token of type %s at line: %d, Actual token of type %s, lexeme: %s.\n", tokenType(ID_TOKEN), t->lineNumber, tokenType(t->kind), t->lexeme);
        break;
    }
}

void parse_VAR_TAG() //VAR_TAG -> [EXPR_LIST] | EPSILON
{
    //t = next_token();
    switch (t->kind)
    {
    case LEFT_BRACKET_TOKEN: {
        fprintf(yyout, "RULE(VAR_TAG -> [ EXPR_LIST ])\n");
        parse_EXPR_LIST();
        match(RIGHT_BRACKET_TOKEN);
        break; }
    case ASSIGN_TOKEN: {fprintf(yyout, "RULE(VAR_TAG -> EPSILON)\n"); back_token(); break; }
    case BOOLEAN_MULTIPLY_TOKEN: {fprintf(yyout, "RULE(VAR_TAG -> EPSILON)\n"); back_token(); break; }
    case BOOLEAN_ADD_TOKEN: {fprintf(yyout, "RULE(VAR_TAG -> EPSILON)\n"); back_token(); break; }
    case SEMICOLON_TOKEN: {fprintf(yyout, "RULE(VAR_TAG -> EPSILON)\n"); back_token(); break; }
    case RIGHT_CURLY_BRACE_TOKEN: {fprintf(yyout, "RULE(VAR_TAG -> EPSILON)\n"); back_token(); break; }
    case COMMA_TOKEN: {fprintf(yyout, "RULE(VAR_TAG -> EPSILON)\n"); back_token(); break; }
    case RIGHT_PARENTHESES_TOKEN: {fprintf(yyout, "RULE(VAR_TAG -> EPSILON)\n"); back_token(); break; }
    case RIGHT_BRACKET_TOKEN: {fprintf(yyout, "RULE(VAR_TAG -> EPSILON)\n"); back_token(); break; }
    case BIGGER_TOKEN: case SMALLER_EQUAL_TOKEN: case BIGGER_EQUAL_TOKEN: case SMALLER_TOKEN: case EQUAL_TOKEN: case NOT_EQUAL_TOKEN: {fprintf(yyout, "RULE(VAR_TAG -> EPSILON)\n"); back_token(); break; }
    default: {ErrorRecovery(VAR_TAG_follow, 14, "{ LEFT_BRACKET_TOKEN, ASSIGN_TOKEN, BOOLEAN_MULTIPLY_TOKEN, BOOLEAN_ADD_TOKEN, SEMICOLON_TOKEN, RIGHT_CURLY_BRACE_TOKEN, COMMA_TOKEN, RIGHT_PARENTHESES_TOKEN, RIGHT_BRACKET_TOKEN, BIGGER_TOKEN, SMALLER_EQUAL_TOKEN, BIGGER_EQUAL_TOKEN, SMALLER_TOKEN, EQUAL_TOKEN, NOT_EQUAL_TOKEN }");  break; }
    }
}

void parse_EXPR_LIST() //EXPR_LIST -> EXPR EXPR_LIST_TAG
{
    fprintf(yyout, "RULE(EXPR_LIST -> EXPR EXPR_LIST_TAG)\n");
    parse_EXPR();
    parse_EXPR_LIST_TAG();
}

void parse_EXPR_LIST_TAG() //EXPR_LIST_TAG -> , EXPR EXPR_LIST_TAG | EPSILON
{
    t = next_token();
    switch (t->kind)
    {
    case COMMA_TOKEN: {fprintf(yyout, "RULE(EXPR_LIST_TAG -> , EXPR EXPR_LIST_TAG)\n");
        parse_EXPR();
        parse_EXPR_LIST_TAG();
        break; }
    case RIGHT_BRACKET_TOKEN: {fprintf(yyout, "RULE(EXPR_LIST_TAG -> EPSILON)\n");
        back_token();
        break; }
    default: ErrorRecovery(EXPR_LIST_TAG_follow, 1, "{ COMMA_TOKEN, RIGHT_BRACKET_TOKEN }");
    }
}


void parse_CONDITION() //CONDITION -> EXPR rel_op EXPR
{
    fprintf(yyout, "RULE(CONDITION -> EXPR rel_op EXPR)\n");
    parse_EXPR();
    t = next_token();
    switch (t->kind)
    {
    case BIGGER_EQUAL_TOKEN: case BIGGER_TOKEN: case EQUAL_TOKEN: case SMALLER_EQUAL_TOKEN: case SMALLER_TOKEN: case NOT_EQUAL_TOKEN: {
        break; }
    default: {ErrorRecovery(CONDITION_follow, 1, "{ BIGGER_EQUAL_TOKEN, BIGGER_TOKEN, EQUAL_TOKEN, SMALLER_EQUAL_TOKEN, SMALLER_TOKEN, NOT_EQUAL_TOKEN }"); break; }
    }
    parse_EXPR();
}

void parse_EXPR()//EXPR -> TERM EXPR_TAG
{
    fprintf(yyout, "RULE(EXPR -> TERM EXPR_TAG)\n");
    parse_TERM();
    parse_EXPR_TAG();
}

void parse_EXPR_TAG()//EXPR_TAG -> + TERM EXPR_TAG | EPSILON
{
    t = next_token();
    switch (t->kind)
    {
    case BOOLEAN_ADD_TOKEN: {fprintf(yyout, "RULE(EXPR_TAG -> + TERM EXPR_TAG)\n");
        parse_TERM();
        parse_EXPR_TAG();
        break; }
    case SEMICOLON_TOKEN: {fprintf(yyout, "RULE(EXPR_TAG -> EPSILON)\n"); back_token();  break; }
    case RIGHT_CURLY_BRACE_TOKEN: {fprintf(yyout, "RULE(EXPR_TAG -> EPSILON)\n"); back_token();  break; }
    case COMMA_TOKEN: {fprintf(yyout, "RULE(EXPR_TAG -> EPSILON)\n"); back_token();  break; }
    case RIGHT_PARENTHESES_TOKEN: {fprintf(yyout, "RULE(EXPR_TAG -> EPSILON)\n"); back_token();  break; }
    case RIGHT_BRACKET_TOKEN: {fprintf(yyout, "RULE(EXPR_TAG -> EPSILON)\n"); back_token();  break; }
    case BIGGER_TOKEN:case SMALLER_EQUAL_TOKEN: case BIGGER_EQUAL_TOKEN: case SMALLER_TOKEN: case EQUAL_TOKEN: case NOT_EQUAL_TOKEN: {fprintf(yyout, "RULE(EXPR_TAG -> EPSILON)\n"); back_token(); break; }
    default:ErrorRecovery(EXPR_TAG_follow, 11, "{ BOOLEAN_ADD_TOKEN, SEMICOLON_TOKEN, RIGHT_CURLY_BRACE_TOKEN, RIGHT_CURLY_BRACE_TOKEN, COMMA_TOKEN, RIGHT_PARENTHESES_TOKEN, RIGHT_BRACKET_TOKEN, BIGGER_TOKEN, SMALLER_EQUAL_TOKEN, BIGGER_EQUAL_TOKEN, SMALLER_TOKEN, EQUAL_TOKEN, NOT_EQUAL_TOKEN }");
    }
}

void parse_TERM()//TERM -> FACTOR TERM_TAG
{
    fprintf(yyout, "RULE(TERM -> FACTOR TERM_TAG)\n");
    parse_FACTOR();
    parse_TERM_TAG();
}

void parse_TERM_TAG()//TERM_TAG -> * FACTOR TERM_TAG | EPSILON
{
    t = next_token();
    switch (t->kind)
    {
    case BOOLEAN_MULTIPLY_TOKEN: {fprintf(yyout, "RULE(TERM_TAG -> * FACTOR TERM_TAG)\n");
        parse_FACTOR();
        parse_TERM_TAG();
        break; }
    case BOOLEAN_ADD_TOKEN: {fprintf(yyout, "RULE(TERM_TAG -> EPSILON)\n"); back_token(); break; }
    case SEMICOLON_TOKEN: {fprintf(yyout, "RULE(TERM_TAG -> EPSILON)\n"); back_token(); break; }
    case RIGHT_CURLY_BRACE_TOKEN: {fprintf(yyout, "RULE(TERM_TAG -> EPSILON)\n"); back_token(); break; }
    case COMMA_TOKEN: {fprintf(yyout, "RULE(TERM_TAG -> EPSILON)\n"); back_token(); break; }
    case RIGHT_PARENTHESES_TOKEN: {fprintf(yyout, "RULE(TERM_TAG -> EPSILON)\n"); back_token(); break; }
    case RIGHT_BRACKET_TOKEN: {fprintf(yyout, "RULE(TERM_TAG -> EPSILON)\n"); back_token(); break; }
    case BIGGER_TOKEN: case SMALLER_EQUAL_TOKEN: case BIGGER_EQUAL_TOKEN:case SMALLER_TOKEN:case EQUAL_TOKEN:case NOT_EQUAL_TOKEN: {fprintf(yyout, "RULE(TERM_TAG -> EPSILON)\n"); back_token(); break; }
    default: {ErrorRecovery(TERM_TAG_follow, 12, "{ BOOLEAN_MULTIPLY_TOKEN, BOOLEAN_ADD_TOKEN, SEMICOLON_TOKEN, RIGHT_CURLY_BRACE_TOKEN, COMMA_TOKEN, RIGHT_PARENTHESES_TOKEN, RIGHT_BRACKET_TOKEN, BIGGER_TOKEN, SMALLER_EQUAL_TOKEN, BIGGER_EQUAL_TOKEN, SMALLER_TOKEN, EQUAL_TOKEN, NOT_EQUAL_TOKEN }"); break;  }
    }
}

void parse_FACTOR()//FACTOR -> id FACTOR_TAG | int_num | float_num | ( EXPR )
{
    t = next_token();
    switch (t->kind)
    {
    case LEFT_PARENTHESES_TOKEN: {fprintf(yyout, "RULE(FACTOR -> ( EXPR ) )\n");
        parse_EXPR();
        match(RIGHT_PARENTHESES_TOKEN);
        break; }
    case FLOAT_NUM_TOKEN: {
        checkAssign(assignEntry, "float_num",t->lineNumber);
        fprintf(yyout, "RULE(FACTOR -> FLOAT_NUM)\n"); break; 
    }
    case INT_NUM_TOKEN: {
        checkAssign(assignEntry, "int_num",t->lineNumber);
        fprintf(yyout, "RULE(FACTOR -> INT_NUM)\n"); break; 
    }
    case ID_TOKEN: {fprintf(yyout, "RULE(FACTOR -> id FACTOR_TAG)\n");
        if (entry != NULL) {
            find(table, t->lexeme,t->lineNumber);
        }
         table_entry* tempType = lookup(table, t->lexeme);
         if (tempType != NULL)
             checkAssign(entry, tempType->type,t->lineNumber);
        /*if (assigment)
        {
            assigment = false;
            checkTypes(assingEntry,entry);
        }*/
        parse_FACTOR_TAG();
        break; }
    default: ErrorRecovery(FACTOR_follow, 13, "{ LEFT_PARENTHESES_TOKEN, FLOAT_NUM_TOKEN, INT_NUM_TOKEN, ID_TOKEN }"); break;
    }

}

void parse_FACTOR_TAG()//FACTOR_TAG -> ( ARGS ) | VAR_TAG
{

    t = next_token();
    switch (t->kind)
    {
    case LEFT_PARENTHESES_TOKEN: {fprintf(yyout, "RULE(FACTOR_TAG -> ( ARGS ) )\n");
        parse_ARGS();
        match(RIGHT_PARENTHESES_TOKEN);
        break; }
    case LEFT_BRACKET_TOKEN: {
        fprintf(yyout, "RULE(FACTOR_TAG -> VAR_TAG)\n");
        parse_VAR_TAG();
        break; }
    case BOOLEAN_MULTIPLY_TOKEN: {fprintf(yyout, "RULE(FACTOR_TAG -> EPSILON)\n"); back_token(); break; }
    case BOOLEAN_ADD_TOKEN: {fprintf(yyout, "RULE(FACTOR_TAG -> EPSILON)\n"); back_token(); break; }
    case SEMICOLON_TOKEN: {fprintf(yyout, "RULE(FACTOR_TAG -> EPSILON)\n"); back_token(); break; }
    case RIGHT_CURLY_BRACE_TOKEN: {fprintf(yyout, "RULE(FACTOR_TAG -> EPSILON)\n"); back_token(); break; }
    case COMMA_TOKEN: {fprintf(yyout, "RULE(FACTOR_TAG -> EPSILON)"); back_token(); break; }
    case RIGHT_PARENTHESES_TOKEN: {fprintf(yyout, "RULE(FACTOR_TAG -> EPSILON)\n"); back_token(); break; }
    case RIGHT_BRACKET_TOKEN: {fprintf(yyout, "RULE(FACTOR_TAG -> EPSILON)"); back_token(); break; }
    case BIGGER_TOKEN: case SMALLER_EQUAL_TOKEN: case BIGGER_EQUAL_TOKEN:case SMALLER_TOKEN:case EQUAL_TOKEN:case NOT_EQUAL_TOKEN: {fprintf(yyout, "RULE(FACTOR_TAG -> EPSILON)\n"); back_token(); break; }
    default: {ErrorRecovery(FACTOR_TAG_follow, 13, "{ LEFT_PARENTHESES_TOKEN, LEFT_BRACKET_TOKEN, BOOLEAN_MULTIPLY_TOKEN, BOOLEAN_ADD_TOKEN, SEMICOLON_TOKEN, RIGHT_CURLY_BRACE_TOKEN, COMMA_TOKEN, RIGHT_PARENTHESES_TOKEN, RIGHT_BRACKET_TOKEN, BIGGER_TOKEN, SMALLER_EQUAL_TOKEN, BIGGER_EQUAL_TOKEN, SMALLER_TOKEN, EQUAL_TOKEN, NOT_EQUAL_TOKEN }"); }
    }
}



void match(eTOKENS tok) {
    t = next_token();
    if (t->kind != tok) {

        //fprintf(yyout,"Expected token of type %s at line %d , Actual token of type %s , lexme %s\n",names(tok),t->lineNumber,names(t->kind),t->lexeme);
        t = back_token();

    }


}

void lookahead()
{
    int i = 0;
    while (i < 3)//look forward 3 tokens
    {
        t = next_token();
        i++;
    }
    if (t->kind == LEFT_PARENTHESES_TOKEN)
    {
        function = true;
    }
    while (i >= 1)//go back to starting point
    {
        t = back_token();
        --i;
    }
}

void ErrorRecovery(int* arr, int arr_size, char* expected)
{
    //fprintf(yyout, "Error at line %d . actual token of type %s , lexme: %s\n", t->lineNumber, names(t->kind), t->lexeme);
    fprintf(yyout, "Expected token of type %s at line: %d, Actual token of type %s, lexeme: %s.\n", expected, t->lineNumber, tokenType(t->kind), t->lexeme);
    int i = 0;
    while (i == 0)
        //for (i = 0;i<arr_size;i++)
    {
        t = next_token();
        for (int j = 0; j < arr_size; j++)
        {
            if (t->kind == EOF_TOKEN)
            {
                i = arr_size;
                break;
            }
            //fprintf(yyout,"arr[j] %d\n", arr[j]);
            if (t->kind == arr[j])
            {
                i = arr_size;
                break;
            }
        }
        /* fprintf(yyout,"this is arr[i] %d\n", arr[i]);
         if (t == EOF_TOKEN)break;
         if (t->kind == arr[i])
             break;*/
    }
    // t = back_token();
    back_token();
}