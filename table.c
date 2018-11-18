#include <stdio.h>
#include <malloc.h>
#include "string.h"
#include "table.h"

void addRecord(TRECORD *record)
{
	TRECORD *newRecord;
	newRecord = creatRecord();
	setValueRecord(newRecord,record);
	insertRecord(newRecord);
}

TRECORD *creatRecord()
{
	TRECORD *newRecord;
	newRecord = (TRECORD *)calloc(1,sizeof(TRECORD));
	newRecord->value = (TVALUE*)calloc(1,sizeof(TVALUE));
	return newRecord;
}

void setValueRecord(TRECORD *record, TRECORD *value)
{
	strCopy(record->value->name_f,value->value->name_f);
	strCopy(record->value->name_i,value->value->name_i);
	strCopy(record->value->name_s,value->value->name_s);
	record->cntLine = value->cntLine;
}

void insertRecord(TRECORD *record)
{
	if(head == 0)
		head = record;
	else
	{
		record->left = tail;
		tail->right = record;
	}
	tail = record;
}

int searchRecord(TVALUE *value)
{
	TRECORD *tmpRecord;
	tmpRecord = head;
	while(tmpRecord->right)
	{
		if(!cmpValue(tmpRecord->value,value))
			return 0;
		tmpRecord = tmpRecord->right;
	}
	return 1;
}

int cmpValue(TVALUE *value1, TVALUE *value2)
{
	if(!strCmp(value1->name_i,value2->name_i) && !strCmp(value1->name_s,value2->name_s))
		return 1;
	return 0;
}

void deleteList(TRECORD *head)
{
	TRECORD *tmp;
	while(head)
	{
		tmp = head->right;
		free(head->value);
		free(head);
		head = tmp;
	}
}

void writeRecords(TRECORD *head)
{
	TRECORD *tmp;
	while(head)
	{
		printf("%-*s %-*s %-*s %d\n",20, head->value->name_s,20,head->value->name_i,20,head->value->name_f,head->cntLine);
		head = head->right;
	}
}
