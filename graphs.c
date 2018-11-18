#include <stdio.h>
#include <stdlib.h>

#include "table.h"
#include "string.h"

void graphs(TRECORD *head, char *indeficator)
{
	FILE *graphs;
	if(!(graphs = fopen("graphs.dot","w"))) 
		return;
	fprintf(graphs,"digraph A");
	fprintf(graphs,"\n{\n");
	while(head)
	{
		if(!strCmp(head->value->name_i,indeficator))
			fprintf(graphs,"\t\"%s\"->\"%s\";\n",head->value->name_i,head->value->name_f);
		head = head->right;
	}
	fprintf(graphs,"}\n");
	fclose(graphs);
	system("graphviz-2.38\\bin\\dot -Tpng graphs.dot -o graphs.png");
}