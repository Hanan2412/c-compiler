#include <stdio.h>
#include "Sematic_Analayzer.h"
#include "parser.h"
#include <string.h>
#include <stdbool.h>

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

table_entry* insert(table_ptr* current_table, char* id_name, int semLinenum)
{
	table_entry* etr;
	etr = lookup(current_table, id_name);
	if (etr != NULL )
	{
		if (strcmp(etr->role, "pre-definition") == 0)
			return NULL;
		fprintf( yyout2,"line %d, Error: duplicate declaration of %s\n",semLinenum, id_name);
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

table_entry* insert_possible_function(char *name,table_ptr *table, int semLinenum)
{
	table_entry *search = table->rows;
	while (search != NULL)//checks if a full defenition function allready exists
	{
		if (strcmp(search->name, name) == 0) {
			if (strcmp(search->role, "full-defenition") == 0)
			{
				fprintf(yyout2, "line %d, Error: duplicate full-definition %s\n",semLinenum, name);
				return NULL;
			}
		}
		search = search->next;
	}
	table_entry *etr;
	etr = (table_entry*)malloc(sizeof(table_entry));
	etr->name = name;
	etr->next = NULL;
	etr->params = NULL;
	etr->dimenstions = NULL;
	etr->role = NULL;
	return etr;
}

table_entry* lookup(table_ptr* current_table, char* id_name)//outside scope
{
	table_entry* tey = current_table->rows;
	if (current_table->sons != NULL)//checks function declaration for coresponding variables
	{
		while(tey->next!=NULL)
			tey = tey->next;
		functionParameters *params = tey->params;
		while (params != NULL)
		 {
			if (strcmp(params->name, id_name) == 0)
			{
				params->needParam = true;
				return tey;
			}
			params = params->next;
		} 
		tey = current_table->rows;
	}
	if (tey == NULL) return NULL;
	do
	{
		if (strcmp(tey->name,id_name)==0)
		{
			return tey;
		}
		tey = tey->next;
	} while (tey != NULL);
	return NULL;
}

table_entry* find(table_ptr* current_table, char* id_name, int semLinenum)//inside scope
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
	fprintf(yyout2, "line %d, Error: undeclared identifier %s\n",semLinenum, id_name);
	return NULL;

}

void set_id_type(table_entry* id_entry, elm_type id_type)
{
	if(id_entry!=NULL)
		id_entry->type = id_type.elm_type;
}

elm_type get_id_type(table_entry id_entry)
{
	/*elm_type et;
	et.elm_type = id_entry.type;
	return et;*/
}

/*table_entry* updateArray(table_entry *id_entry,char* num)
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
}*/

void newArrayData(table_entry* entry, char* num)
{
	
	entry->arrayVals->value = num;
	arrayValues *arrayValue = (arrayValues*)malloc(sizeof(arrayValues));
	arrayValue->next = NULL;
	//value->value = "NULL"; //we don't need to assign it here if we can assign it later anyway...
	entry->arrayVals->next = arrayValue;
}

/*void updateArrayData(table_entry* entry, char* num, int index)//declaring arr[i]=num;
{
	arrayValues* arrayVals = entry->arrayVals;
	int i=0;
	while(i <= index) {
		if (i == index && arrayVals != NULL) {
			printf("Error: cannot redeclare array %s's elements.", entry->name);
		}
		else newArrayData(entry, num);
		i++;
		arrayVals = arrayVals->next;
	}
}*/

void updateRole(table_entry* entry, char* role)
{
	if(entry!=NULL)
		entry->role = role;
}

table_entry* set_func_params(table_entry* entry,char* name)
{
	if (entry != NULL) {
		if (entry->params == NULL) {
			entry->params = (functionParameters*)malloc(sizeof(functionParameters));
			entry->params->name = name;
			entry->params->next = NULL;
			entry->params->needParam = false;
		}
		else
		{
			functionParameters* p = entry->params;
			while (p->next != NULL)
				p = p->next;
			p->next = (functionParameters*)malloc(sizeof(functionParameters));
			p = p->next;
			p->name = name;
			p->next = NULL;
			p->needParam = false;
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

void checkTypes(table_ptr *table, table_entry* variable2)
{
	table_entry* rows = table->rows;
	bool finish = false;
	while (rows != NULL && !finish)
	{
		if (strcmp(rows->name, variable2->name) == 0)
		{
			functionParameters* params = rows->params;
			functionParameters* params1 = variable2->params;
			if(params != NULL && params1!= NULL)
				while (params != NULL && params1 != NULL)
				{
					if (strcmp(params->type, params1->type) != 0)
					{
						printf_s("missmatch with parameters types between pre-defenition function and full - defenition function: %s parameters in pre: %s,parameter in full: %s\n",variable2->name,params->name,params1->name);
						finish = true;
					}
					if (strcmp(params->name, params1->name) != 0)
					{
						printf_s("missmatch with parameters names between pre-defenition function and full - defenition function: %s parameters in pre: %s,parameter in full: %s\n", variable2->name, params->name, params1->name);
						finish = true;
					}
					if ((params->next == NULL && params1->next != NULL) || params1->next == NULL && params->next != NULL)
					{
						printf_s("parameters count doent match\n");
						finish = true;
					}
					params = params->next;
					params1 = params1->next;
					if (params == NULL) finish = true;
				}
		}
		else {
			rows = rows->next;
		}
	}
}

void checkAssign(table_entry* entry,char* type, int semLinenum) {
	if (entry!=NULL && *(entry->type)=='i' && *type=='f')
	{
		fprintf(yyout2, "line %d, Error: cannot assign float to int in %s\n",semLinenum, entry->name);
	}
}

int int_numToInteger(char* int_num){
	char* tempString = int_num;
	int num = 0;
	int size = 1;
	int currNum = *(tempString)-'0';
	while (tempString != NULL && currNum >= 0 && currNum <= 9) {
		if(size>1) num *= 10;
		num += currNum;
		size++;
		tempString++;
		currNum = *(tempString)-'0';
	}
	return num;
}

void checkSimilarParams(table_entry* entry, int semLinenum) {
	functionParameters* funcParams = entry->params;
	if (funcParams != NULL)
	{
		if (funcParams->next != NULL && strcmp(funcParams->next->name, funcParams->name) == 0) {
			printf("line %d, Error: same parameter names in %s's predefinition\n",semLinenum, entry->name);
		}
	}
		
}

void setArrayPtrDim(table_ptr* table, table_entry* entry, char* dim) {
	int int_dim = int_numToInteger(dim);
	arrayPointer* arrpointer = entry->arrPtr;
	if (lookup(table,entry->name)->arrPtr == NULL) //new array
	{
		arrpointer = (arrayPointer*)malloc(sizeof(arrayPointer));
		arrpointer->dims = (arrayDims*)malloc(sizeof(arrayDims));
		arrpointer->dims->dim = int_dim;
		arrpointer->dims->nextDim = NULL;
		arrpointer->values = NULL;
		arrpointer->nextPtr = NULL;
		entry->arrPtr = arrpointer;
	}
	else {
		if (arrpointer->dims == NULL)
		{
			arrpointer->dims = (arrayDims*)malloc(sizeof(arrayDims));
			arrpointer->dims->dim = int_dim;
			arrpointer->dims->nextDim = NULL;
		}
		else {
			arrayDims* dimPtr = arrpointer->dims;
			while (dimPtr->nextDim != NULL) {
				dimPtr = dimPtr->nextDim;
			}
			dimPtr->nextDim = (arrayDims*)malloc(sizeof(arrayDims));
			dimPtr->nextDim->dim = int_dim;
			dimPtr->nextDim->nextDim = NULL;
		}
	}
	/*if(tmpEntry->arrPtr == NULL){//new array
		tmpEntry->arrPtr = (arrayPointer*)malloc(sizeof(arrayPointer) * int_dim);
		tmpEntry->arrPtr->currDim = int_dim;
		tmpEntry->arrPtr->nextPtr = NULL;
	}
	else {
		if (tmpEntry->arrPtr->nextPtr != NULL)
			tmpEntry->arrPtr = tmpEntry->arrPtr->nextPtr;
		int i;
		for (i = 0; i < tmpEntry->arrPtr->currDim; i++) {
			tmpEntry->arrPtr->nextPtr = (arrayPointer*)malloc(sizeof(arrayPointer) * int_dim);
			tmpEntry->arrPtr->nextPtr->currDim = int_dim;
			tmpEntry->arrPtr->nextPtr->nextPtr = NULL;
			tmpEntry->arrPtr->nextPtr++;
		}
	}*/
}