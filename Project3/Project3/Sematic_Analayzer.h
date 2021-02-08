#pragma once
#include <stdbool.h>

extern FILE* yyin2, * yyout2;

//the way the table looks, name and type will be reaclocated each time a new variable is added to the code in the scope

//a single entry to the table consitst of the variable name and type - basicaly one raw of the table
typedef struct table_entry
{
	char* name;
	char* type;
	char* role;
	char* dimSize1;
	char* dimSize2;
	int pre_def;
	int full_def;
	struct table_entry* next;
	struct arrayValues *arrayVals;
	struct functionParameters *params;
	struct arrayDimenstions* dimenstions;
	struct arrayPointer* arrPtr;
}table_entry;

typedef struct table_ptr
{
	int number_of_variables;
	char* name[1];
	char* type[1];
	struct table_ptr* father;
	struct table_ptr* sons;
	table_entry *rows;
	
}table_ptr;

typedef struct functionParameters
{
	char* name;
	char* type;
	bool needParam;
	struct functionParameters* next;

}functionParameters;

typedef struct arrayValues
{
	char* value;
	struct arrayValues* next;
}arrayValues;

typedef struct arrayDimenstions {
	int dim;
	struct arrayDimenstions* nextDim;
}arrayDims;

typedef struct arrayPointer {
	char** values; //one dimensional array of strings
	arrayDims *dims;
	struct arrayPointer* nextPtr; //is set to NULL and update later for new dimensions
}arrayPointer;

/*typedef struct arrayDimenstions
{
	char* dim;
	struct arrayDimenstions* next;
}arrayDimenstions;*/

//when a type is set, the corresponding field will be changed to true
typedef struct elm_type
{ 
	char* elm_type;
}elm_type;
//table ptr functions
table_ptr* make_table(table_ptr* current_table);

table_ptr* pop_table(table_ptr* current_table);

//table entry functions
table_entry* insert(table_ptr* current_table, char* id_name, int semLinenum);

table_entry* lookup(table_ptr* current_table, char* id_name);

table_entry* find(table_ptr* current_table, char* id_name, int semLinenum);//check whether the id is declared 

//other functions

void set_id_type(table_entry *id_entry, elm_type id_type);

elm_type get_id_type(table_entry id_entry);

//table_entry* updateArray(table_entry* id_entry, char* num);

void newArrayData(table_entry* entry, char* num);

void updateArrayData(table_entry* entry, char* num, int index);

void updateRole(table_entry* entry, char* role);

table_entry* set_func_params(table_entry* entry, char* name);

void set_func_param_type(table_entry* entry, elm_type type);

table_entry* insert_possible_function(char* name, table_ptr* table, int semLinenum);

void checkTypes(table_ptr* table, table_entry* variable2);

void checkAssign(table_entry* entry, char* type, int semLinenum);

int int_numToInteger(char* int_num);

void checkSimilarParams(table_entry* entry, int semLinenum);

void setArrayPtrDim(table_ptr* table, table_entry* entry, char* dim);