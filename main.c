#include <stdio.h>
#include <stdlib.h>

#define MAX_F_N 31
#define MAX_I_N 30
#define MAX_T_N 20
#define LEN 1000

int main(void)
{
	FILE *pLF;//pointer list file
	FILE *fin; 
	char fname[MAX_F_N];
	char *iname;
	char *tname;
	char tmp[LEN];
	char *c;
	int cntf;
	int flag;
	if(!(pLF = fopen("listFile.txt","r"))) return 1;
	printf("%-*s %-*s %-s\n",MAX_T_N,"type variable",MAX_I_N,"name variable","file of which variable");
	while(fgets(fname,sizeof(fname),pLF))
	{
		sscanf(fname,"%[^\n]",fname);
		if(fin = fopen(fname,"r")) 
		{
			cntf = 0;
			while(fgets(tmp,sizeof(tmp),fin))
			{
				c = tmp;
				iname = 0;
				tname = 0;
				while(*c)
				{ 
					switch(*c)
					{
					case '{':
						cntf++;
						break;
					case '}':
						cntf--;
						break;
					default:
						if(!cntf)
						{
							if(*c != ';')
							{
								if(*c == ' ' || *c == '\t')
								{
									flag = 0;
								}
								else
								{
									tname = (tname)?tname:c;
									if(!flag)
									{
										iname = c;
										flag = 1;
									}
								}
							}
							else
							{
								if(iname && tname)
								{
									*(iname-1) = '\0';
									*c = '\0';
									printf("%-*s %-*s %-s\n",MAX_T_N,tname,MAX_I_N,iname,fname);
									iname = 0;
									tname = 0;
								}
							}
						}
					}
					c++;
				}
			}
			fclose(fin);
		}
		else
			printf("%-*s %-*s %-s\n",MAX_T_N,"Error",MAX_I_N,"file cannot be opened",fname);
	}
	getchar();
	return 0;
}