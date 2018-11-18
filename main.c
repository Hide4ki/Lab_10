#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"
#include "table.h"

void init();
void initAnalysis();
void analysisLine(char *l);
void setNameFile(char *name);
void graphs(TRECORD *head, char *indeficator);

int main(void)
{
	FILE *listFile;
	FILE *fileNow;
	char *l;
	char fileName[MAX_N_F];
	char line[LEN];

	init();
	initAnalysis();

	if(!(listFile = fopen("listFile.txt","r")))
		return 1;

	while(fgets(fileName,sizeof(fileName),listFile))
	{
		sscanf(fileName,"%[^\n]",fileName);
		if(fileNow = fopen(fileName,"r"))
		{
			setNameFile(fileName);
			while(l = fgets(line,sizeof(line),fileNow))
				analysisLine(l);
		}
		else
			printf("%s \"%s\"\n","Error this file cannot be opened ->",fileName);
	}

	printf("%-*s %-*s %-*s %-s\n",20, "specificators type",20,"indificator",20,"file name","number line");
	writeRecords(head);
	printf("Enter select indificator:");
	scanf("%s",line);
	graphs(head,line);
	deleteList(head);
	
	if(rootSTD != pleaf)
		deleteTree(rootSTD);
	if(rootDEF != pleaf)
		deleteTree(rootDEF);
	
	free(pleaf->value);
	free(pleaf);
	
	system("pause");
	return 0;
}