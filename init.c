#include <stdio.h>
#include <malloc.h>
#include "rbtree.h"
#include "string.h"
#include "table.h"

void init()
{
	head = 0;
	tail = 0;
	pleaf = createNode();	
	rootDEF = pleaf;
	rootSTD = pleaf;
	addSpec(&rootSTD,"auto", "");
	addSpec(&rootSTD,"char", "");
	addSpec(&rootSTD,"const", "");
	addSpec(&rootSTD,"double", "");
	addSpec(&rootSTD,"extern", "");
	addSpec(&rootSTD,"float", "");
	addSpec(&rootSTD,"FILE", "");
	addSpec(&rootSTD,"int", "");
	addSpec(&rootSTD,"long", "");
	addSpec(&rootSTD,"register", "");
	addSpec(&rootSTD,"short", "");
	addSpec(&rootSTD,"signed", "");
	addSpec(&rootSTD,"static", "");
	addSpec(&rootSTD,"struct", "");
	addSpec(&rootSTD,"typedef", "");
	addSpec(&rootSTD,"union", "");
	addSpec(&rootSTD,"unsigned", "");
	addSpec(&rootSTD,"void", "");
	addSpec(&rootSTD,"volatile", "");
}
