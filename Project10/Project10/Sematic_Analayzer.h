#pragma once

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
	struct arrayValues* values;
	struct functionParameters* params;
	struct arrayDimenstions* dimenstions;
}table_entry;

typedef struct table_ptr //main table
{
	int number_of_variables;// how many variables he counted
	char* name[1];
	char* type[1];
	struct table_ptr* father;//father scope
	struct table_ptr* sons;//child scope
	table_entry* rows; // data
}table_ptr;

typedef struct functionParameters
{
	char* name;
	char* type;
	int count;
	struct functionParameters* next;
}functionParameters;

typedef struct arrayValues
{
	char* value;
	struct arrayValues* next;
}arrayValues;

typedef struct arrayDimenstions
{
	char* dim;
	struct arrayDimenstions* next;
}arrayDimenstions;

//when a type is set, the corresponding field will be changed to true
typedef struct elm_type
{
	char* elm_type;
}elm_type;
//table ptr functions
table_ptr* make_table(table_ptr* current_table);

table_ptr* pop_table(table_ptr* current_table);

//table entry functions
table_entry* insert(table_ptr* current_table, char* id_name);

table_entry* lookup(table_ptr* current_table, char* id_name);//inside scope

table_entry* find(table_ptr* current_table, char* id_name);//check whether the id is declared in all of the program 

//other functions

void set_id_type(table_entry* id_entry, elm_type id_type);

elm_type get_id_type(table_entry id_entry);

table_entry* updateArray(table_entry* id_entry, char* num);

void updateArrayData(table_entry* entry, char* num);

void updateRole(table_entry* entry, char* role);

table_entry* set_func_params(table_entry* entry, char* name);

void set_func_param_type(table_entry* entry, elm_type type);

table_entry* insert_possible_function(char* name, table_ptr* table);

void cmpPreDefFullDef(table_entry* preDef, table_entry* fullDef);

void checkPredef(table_ptr* tableList, table_entry* fullDef);