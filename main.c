#include <stdio.h>
#include <stdlib.h>

#define MAX_F_N 31
#define MAX_I_N 25
#define MAX_T_N 22
#define DLEN 100
#define LEN 1000 

int strcmp(char *s1, char *s2)
{
	for(;*s1 == *s2; s1++, s2++)
		if(*s1 == '\0')
			return 0;
	return s1-s2;
}

int main(void)
{
	FILE *pLF;
	FILE *fin;
	FILE *graphs;
	const char tstruct[] = "struct";
	const char *pstruct;
	char fstruct;
	const char tstatic[] = "static";
	const char *pstatic;
	char fstatic;
	const char tunsign[] = "unsigned";
	const char *punsign;
	char funsign;
	char searchName[MAX_I_N];
	struct Trecord
	{
		char fname[MAX_F_N];
		char iname[MAX_I_N];
		char tname[MAX_T_N];
	}*record;
	char *piname;
	char *ptname;
	int com;
	int cntf;
	int fi;
	int ft;
	char line[LEN+1];
	char *l;
	int i;
	int funf;
	int iterator;
	iterator = 0;
	record = (struct Trecord*)calloc(DLEN,sizeof(struct Trecord));
	if(!(pLF = fopen("listFile.txt","r"))) return 1;

	printf("%-*s %-*s %-s\n",MAX_T_N,"type variable",MAX_I_N,"name variable","file of which variable");
	
	while(fgets((record+iterator)->fname,sizeof((record+iterator)->fname),pLF))
	{
		sscanf((record+iterator)->fname,"%[^\n]",(record+iterator)->fname);
		if(fin = fopen((record+iterator)->fname,"r")) 
		{
			piname = (record+iterator)->iname;
			ptname = (record+iterator)->tname;
			pstruct = tstruct; 
			pstatic = tstatic; 
			punsign = tunsign; 

			cntf = 0;

			fi = 0;
			com = 0;
			funf = 0;
			ft = 1;
			fstruct = 1;
			fstatic = 1; 
			funsign = 1; 

			while(l = fgets(line,sizeof(line),fin))
			{
				while(*l)
				{
					if(*l == '/')
					{
						if(*(l+1)=='/')
							goto next;
						if(!com) 
							com = 1;
						else 
							if(*(l-1)=='*')
								com = 0;
					}

					switch(*l)
					{

					case ',':
						if(!cntf)
						{
							if( piname != (record+iterator)->iname && ptname != (record+iterator)->tname && !funf)
							{
								*piname = '\0';
								*ptname = '\0';
								printf("%-*s %-*s %-s\n",MAX_T_N,(record+iterator)->tname,MAX_I_N,(record+iterator)->iname,(record+iterator)->fname);
							}
							iterator++;
							if(DLEN - iterator%DLEN==DLEN)
								record = (struct Trecord*)realloc(record,iterator+DLEN);
							*(record+iterator) = *(record+iterator-1);
							piname = (record+iterator)->iname;
						}
						break;

					case '(':
						funf = 1;
						cntf++;
						break;

					case '[':
						if(!cntf && ptname-(record+iterator)->tname<MAX_T_N)
						{
							*ptname++ = ' ';
							*ptname++ = '[';
							*ptname++ = ']';
						}

					case '{':
						cntf++;
						break;

					case '*':
						if(!cntf &&	ptname-(record+iterator)->tname<MAX_T_N)
							*ptname++ = '*';
						break;

					case '}':
					case ')':
					case ']':
						cntf--;
						break;

					case '\\':
					case '#':
						goto next;
						break;

					case '=':
					case ';':
						if(!cntf)
						{
							if( piname != (record+iterator)->iname && ptname != (record+iterator)->tname && !funf)
							{
								*piname = '\0';
								*ptname = '\0';
								printf("%-*s %-*s %-s\n",MAX_T_N,(record+iterator)->tname,MAX_I_N,(record+iterator)->iname,(record+iterator)->fname);
							}
							funf = 0;
							iterator++;
							if(DLEN - iterator%DLEN==DLEN)
								record = (struct Trecord*)realloc(record,iterator+DLEN);
							*(record+iterator) = *(record+iterator-1); 
							piname = (record+iterator)->iname;
							ptname = (record+iterator)->tname;
							pstruct = tstruct; 
							pstatic = tstatic; 
							punsign = tunsign; 
							fi = 0;
							ft = 1;
							fstruct  = 1;
							fstatic  = 1;
							funsign  = 1;
						}
						break;

					default:
						if(!cntf && !com)
						{
							if(*l == ' ' || *l == '\t' || *l == '\n' )
							{
								if(!funsign && !fstruct && !fstatic && ptname != (record+iterator)->tname)
								{
									ft = 0;
									fi = 1;
								}
								if(pstruct!=tstruct && fstruct )
								{
									*ptname++ = ' ';
									fstruct = 0;
								}
								if(punsign!=tunsign && funsign )
								{
									*ptname++ = ' ';
									funsign = 0;
								}
								if(pstatic!=tstatic && fstatic )
								{
									*ptname++ = ' ';
									fstatic = 0;
								}
							}
							else 
							{
								if(*pstruct!=0 && *l!=*pstruct++)
									fstruct = 0;
								if(*punsign!=0 && *l!=*punsign++)
									funsign = 0;
								if(*pstatic!=0 && *l!=*pstatic++)
									fstatic = 0;
								if(ft && ptname-(record+iterator)->tname<MAX_T_N)
									*ptname++ = *l;
								if(fi && piname-(record+iterator)->iname<MAX_I_N)
									*piname++ = *l;
							}
						}
					}
					l++;
				}
				next:;
			}
			fclose(fin);
		}
		else
			printf("%-*s %-*s %-s\n",MAX_T_N,"Error",MAX_I_N,"file cannot be opened",(record+iterator)->fname);
	}
	fclose(pLF);
	
	if(!(graphs = fopen("graphs.dot","w"))) 
		return 1;
	printf("Enter indificator, for bilding graph:");
	scanf("%s",searchName);
	fprintf(graphs,"digraph A");
	fprintf(graphs,"\n{\n");
	for(i = 0;i<iterator;i++)
		if(strcmp(searchName,(record+i)->iname)==0)
			fprintf(graphs,"\t\"%s\"->\"%s\";\n",(record+i)->iname,(record+i)->fname);
	fprintf(graphs,"}\n");
	fprintf(graphs,"\t\"%s\"->\"%s\";\n",(record+iterator)->iname,(record+iterator)->fname);
	
	fclose(graphs);

	system("graphviz-2.38\\bin\\dot -Tpng graphs.dot -o graphs.png");
	getchar();
	return 0;
}
