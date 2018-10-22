#include <stdio.h>
#include <stdlib.h>

#define MAX_F_N 31
#define MAX_I_N 25
#define MAX_T_N 22
#define LEN 1000 

int main(void)
{
	FILE *pLF;
	FILE *fin;
	FILE *graphs;
	char tstruct[] = "struct";
	char *pstruct;
	char fstruct;
	char tstatic[] = "static";
	char *pstatic;
	char fstatic;
	char tunsign[] = "unsigned";
	char *punsign;
	char funsign;
	char fname[MAX_F_N];
	char iname[MAX_I_N];
	char tname[MAX_T_N];
	char *piname;
	char *ptname;
	int com;
	int cntf;
	int fi;
	int ft;
	char line[LEN+1];
	char *l;
	int funf;

	if(!(pLF = fopen("listFile.txt","r"))) return 1;
	if(!(graphs = fopen("graphs.dot","w"))) return 1;

	printf("%-*s %-*s %-s\n",MAX_T_N,"type variable",MAX_I_N,"name variable","file of which variable");
	fprintf(graphs,"digraph A");
	fprintf(graphs,"\n{\n");

	while(fgets(fname,sizeof(fname),pLF))
	{
		sscanf(fname,"%[^\n]",fname);
		if(fin = fopen(fname,"r")) 
		{
			piname = iname;
			ptname = tname;
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
							if( piname != iname && ptname != tname && !funf)
							{
								*piname = '\0';
								*ptname = '\0';
								fprintf(graphs,"\t\"%s\"->\"%s\";\n",iname,fname);
								printf("%-*s %-*s %-s\n",MAX_T_N,tname,MAX_I_N,iname,fname);
							}
							piname = iname;
						}
						break;

					case '(':
						funf = 1;
						cntf++;
						break;

					case '[':
						if(!cntf && ptname-tname<MAX_T_N)
						{
							*ptname++ = ' ';
							*ptname++ = '[';
							*ptname++ = ']';
						}

					case '{':
						cntf++;
						break;

					case '*':
						if(!cntf && ptname-tname<MAX_T_N)
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
							if( piname != iname && ptname != tname && !funf)
							{
								*piname = '\0';
								*ptname = '\0';
								fprintf(graphs,"\t\"%s\"->\"%s\";\n",iname,fname);
								printf("%-*s %-*s %-s\n",MAX_T_N,tname,MAX_I_N,iname,fname);
							}
							funf = 0;
							piname = iname;
							ptname = tname;
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
								if(!funsign && !fstruct && !fstatic && ptname != tname)
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
								if(ft && ptname-tname<MAX_T_N)
									*ptname++ = *l;
								if(fi && piname-iname<MAX_I_N)
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
			printf("%-*s %-*s %-s\n",MAX_T_N,"Error",MAX_I_N,"file cannot be opened",fname);
	}

	fprintf(graphs,"}\n");

	fclose(pLF);
	fclose(graphs);

	system("graphviz-2.38\\bin\\dot -Tpng graphs.dot -o graphs.png");
	getchar();
	return 0;
}
