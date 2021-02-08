#include <stdio.h>
#include "Sematic_Analayzer.h"
#include <string.h>


table_ptr* make_table(table_ptr* current_table)
{
	table_ptr* tab;
	tab = (table_ptr*)malloc(sizeof(table_ptr));
	tab->father = current_table;
	tab->rows = NULL;
	tab->sons = NULL;
	tab->number_of_variables = 0;
	return tab;
}

table_ptr* pop_table(table_ptr* current_table)
{
	return current_table->father;
}

table_entry* insert(table_ptr* current_table, char* id_name)
{
	table_entry* etr;
	etr = lookup(current_table, id_name);
	if (etr != NULL)
	{
		printf_s("error in insert\n");
		printf_s("Duplicate declaration of %s\n", id_name);
		return NULL;
	}
	else
	{
		table_entry* currentEntry = current_table->rows;
		if (currentEntry == NULL)//first time
		{
			table_entry* tey = (table_entry*)malloc(sizeof(table_entry));
			tey->name = id_name;
			tey->next = NULL;
			tey->params = NULL;
			tey->dimenstions = NULL;
			tey->role = NULL;
			currentEntry = tey;
			current_table->rows = tey;
		}
		else {
			while (currentEntry->next != NULL)
				currentEntry = currentEntry->next;
			table_entry* tey = (table_entry*)malloc(sizeof(table_entry));
			tey->name = id_name;
			tey->next = NULL;
			tey->dimSize1 = NULL;
			tey->dimSize2 = NULL;
			tey->params = NULL;
			tey->dimenstions = NULL;
			tey->role = NULL;
			currentEntry->next = tey;
			currentEntry = currentEntry->next;
			table_entry* headEntry = current_table->rows;
		}
		current_table->number_of_variables++;
		return currentEntry;
	}
}

table_entry* insert_possible_function(char* name, table_ptr* table)
{
	table_entry* search = table->rows;
	while (search != NULL)//checks if a full defenition function allready exists
	{
		if (strcmp(search->name, name) == 0) {
			if (strcmp(search->role, "full-defenition") == 0)
			{
				return NULL;
			}

		}
		search = search->next;
	}
	table_entry* etr;
	etr = (table_entry*)malloc(sizeof(table_entry));
	etr->name = name;
	etr->next = NULL;
	etr->params = NULL;
	etr->dimenstions = NULL;
	etr->role = NULL;
	return etr;
}

table_entry* lookup(table_ptr* current_table, char* id_name)
{
	table_entry* tey = current_table->rows;
	if (current_table->sons != NULL)//checks function declaration for coresponding variables
	{
		while (tey->next != NULL)
			tey = tey->next;
		functionParameters* params = tey->params;
		while (params != NULL)
		{
			if (strcmp(params->name, id_name) == 0)
				return tey;
			params = params->next;
		}
		tey = current_table->rows;
	}
	if (tey == NULL) return NULL;
	do
	{
		if (strcmp(tey->name, id_name) == 0)
		{
			return tey;
		}
		tey = tey->next;
	} while (tey != NULL);
	return NULL;
}

table_entry* find(table_ptr* current_table, char* id_name)
{

	table_ptr* tab = current_table;
	table_entry* entry;
	while (tab != NULL)
	{
		entry = lookup(tab, id_name);
		if (entry != NULL)
			return entry;
		else
			tab = pop_table(tab);
	}
	printf_s("Error: undeclared identifire %s\n", id_name);
	return NULL;

}

void set_id_type(table_entry* id_entry, elm_type id_type)
{
	if (id_entry != NULL)
		id_entry->type = id_type.elm_type;
}

elm_type get_id_type(table_entry id_entry)
{
	elm_type et;
	et.elm_type = id_entry.type;
	return et;
}

table_entry* updateArray(table_entry* id_entry, char* num)
{
	if (id_entry != NULL) {
		if (id_entry->dimenstions == NULL)
		{
			id_entry->dimenstions = (arrayDimenstions*)malloc(sizeof(arrayDimenstions));
			id_entry->dimenstions->dim = num;
			id_entry->dimenstions->next = NULL;
			return id_entry;
		}
		else {
			arrayDimenstions* arraydim = id_entry->dimenstions;
			while (arraydim->next != NULL)
				arraydim = arraydim->next;
			arraydim->next = (arrayDimenstions*)malloc(sizeof(arrayDimenstions));
			arraydim = arraydim->next;
			arraydim->dim = num;
			arraydim->next = NULL;
			return id_entry;
		}
	}
	else return NULL;
}

void updateArrayData(table_entry* entry, char* num)
{

	entry->values->value = num;
	arrayValues* value = (arrayValues*)malloc(sizeof(arrayValues));
	value->next = NULL;
	value->value = "NULL";
	entry->values->next = value;
}

void updateRole(table_entry* entry, char* role)
{
	if (entry != NULL)
		entry->role = role;
}

table_entry* set_func_params(table_entry* entry, char* name)
{
	if (entry != NULL) {
		if (entry->params == NULL) {
			entry->params = (functionParameters*)malloc(sizeof(functionParameters));
			entry->params->count = 1;//if paramaters change from NULL to 1 that means there's at least one parameter
			entry->params->name = name;
			entry->params->next = NULL;
		}
		else
		{
			entry->params->count++;
			functionParameters* p = entry->params;
			while (p->next != NULL)
				p = p->next;
			p->next = (functionParameters*)malloc(sizeof(functionParameters));
			p = p->next;
			p->name = name;
			p->next = NULL;
			p->type = NULL;
		}
		return entry;
	}
	return entry;
}

void set_func_param_type(table_entry* entry, elm_type type)
{
	if (entry != NULL) {
		functionParameters* p = entry->params;
		while (p->next != NULL)
			p = p->next;
		p->type = type.elm_type;
	}
}

void cmpPreDefFullDef(table_entry* preDef, table_entry* fullDef) {
	functionParameters* params1 = preDef->params;
	functionParameters* params2 = fullDef->params;
	//checking amount of parameters in both tables
	if ((params1 == NULL && params2 != NULL) || (params1 != NULL && params2 == NULL) || (params1->count != params2->count))
	{
		printf("Error: amount of parameters mismatch.\n");
	}
	else
		while (params1 != NULL)//we're only checking names and types here
		{
			if (strcmp(params1->name, params2->name) != 0) {
				printf("Error: function parameters name mismatch.\n");
			}
			if (strcmp(params1->type, params2->type)) {
				printf("Error: function parameters type mismatch.\n");
			}
			params1 = params1->next;
			params2 = params2->next;
		}
}

void checkPredef(table_ptr* tableList, table_entry* fullDef) {
	table_ptr* currList = tableList;
	while (currList->rows != NULL && strcmp(currList->rows->name, fullDef->name) != 0 && strcmp(currList->rows->role, "pre-definition") != 0) {
		currList->rows = currList->rows->next;
	}
	if (currList->rows != NULL) {
		cmpPreDefFullDef(currList->rows, fullDef);
	}
}

/* @@@@@@@@@@ this is here for reference @@@@@@@@@
typedef struct table_entry
{
	char* name;
	char* type;
	char* role;
	struct table_entry* next;
	struct arrayValues *values;
	struct functionParameters *params;
	struct arrayDimenstions* dimenstions;
}table_entry;*/