#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"
#include "table.h"
#include "string.h"
#include "check.h"
#include "analysis.h"

int flagDir;
static int finish = 0;
static int cntVBloc = 0;
static TRECORD record;
TVALUE value;
static int typeDef = 0;
static int fun = 0;
int inStrLiteral = 0;
int inCharLiteral = 0;
int tokenType;
char token[LEN];
char name[LEN];
char *iteratorLexem;
int NAME = 0;
int PARENS = 1;
int BRACKETS = 2;

void initAnalysis()
{
	checkComment("\a");
	record.value = &value;
	flagDir = 0;
}

void analysisLine(char *l)
{
	if (!flagDir)
		record.cntLine++;
	while (*l)
	{
		if (checkComment(l))
		{
			switch (*l)
			{
			case '\'':
				if (!inStrLiteral)
					checkCharLiteral(l);
				break;
			case '\"':
				if (!inCharLiteral)
					checkStrLiteral(l);
				break;
			case '{':
				if (!inStrLiteral && !inCharLiteral)
					cntVBloc++;
				break;
			case '}':
				if (!inStrLiteral && !inCharLiteral)
					cntVBloc--;
				break;
			case '#':
				if (!inStrLiteral && !inCharLiteral)
				{
					flagDir++;
					analysisDirective(l);
					flagDir--;
				}
				return;
				break;
			case '\\':
				if (!inStrLiteral && !inCharLiteral)
					return;
				break;
			case ',':
				if (!inStrLiteral && !inCharLiteral && !cntVBloc && !fun)
				{
					if (finish)
						addRecord(&record);
					finish = 0;
					fun = 0;
				}
				break;
			case ';':
				if (!inStrLiteral && !inCharLiteral && !cntVBloc && !fun)
				{
					if (typeDef)
					{
						addSpec(&rootSTD, record.value->name_i, record.value->name_s);
						typeDef = 0;
						finish = 0;
						fun = 0;
						record.value->name_s[0] = 0;
						break;
					}
					if (finish)
						addRecord(&record);
					record.value->name_s[0] = 0;
				}
				if (fun)
					record.value->name_s[0] = 0;
				finish = 0;
				fun = 0;
				break;
			default:
				if (!inStrLiteral && !inCharLiteral && !cntVBloc && !finish && !fun)
					l = analysisLexem(l);
				break;
			}
		}
		l++;
	}
}

char *analysisLexem(char *start)
{
	TNODE *tmp;
	char *localIterator;
	iteratorLexem = start;
	while(getToken() == NAME)
	{
		if(searchNode(rootSTD, token) != pleaf)
		{
			if(!strCmp(token,"typedef"))
			{
				typeDef = 1;
				return --iteratorLexem;
			}
			if(strCmp(token,"struct")==0 || strCmp(token,"union")==0)
			{
				strCat(token, " ");
				strCat(record.value->name_s, token);
 				getToken();
				if(tokenType!='{')
				{
					strCat(token, " ");
					strCat(record.value->name_s, token);
				} 
				else
					cntVBloc++;
				return --iteratorLexem;
			}
			strCat(token, " ");
			strCat(record.value->name_s, token);
		}
		else if((tmp = searchNode(rootDEF, token)) != pleaf)
		{
			localIterator = iteratorLexem;
			analysisLine(tmp->value->altvalue);
			iteratorLexem = localIterator;
		}
		else 
		{
			checkFinish();
			return --iteratorLexem;
		}
	}
	parseDeclaration();
	return --iteratorLexem;
}

void setNameFile(char *name)
{
	strCopy(value.name_f,name);
	record.cntLine = 0;
}

void analysisDirective(char *directive)
{
	const char def[] = "#define";
	const char und[] = "#undef";
	int flagDef = 1;
	int flagUnd = 1;
	int iterator = 0;
	char macros[LEN];
	char *p;
	p = macros;
	while(*directive!=' ' && *directive!='\t')
	{
		if(*directive != def[iterator])
			flagDef = 0;
		if(*directive != und[iterator])
			flagUnd = 0;
		iterator++;                                                 
		directive++;
	}
	directive = ignoreEmpty(directive);

	for(*p++ = *directive++; checkSybol(directive);)
			*p++ = *directive++;
	*p = '\0';

	if(flagDef)
		addSpec(&rootDEF,macros,directive);
	if(flagUnd)
		deleteSpec(&rootDEF, searchNode(rootDEF,macros));
} 

char *ignoreEmpty(char *character)
{
	while(*character == ' '|| *character== '\t')
		character++;
	return character;
}

int getToken()
{
	char *pToken;
	pToken = token;
	iteratorLexem = ignoreEmpty(iteratorLexem);
	if(*iteratorLexem == '\0')
	{
		tokenType = -1;
		return tokenType;
	}
	if(*iteratorLexem == '(')
	{
		tokenType = '(';
		return tokenType;
	} 
	else if(*iteratorLexem == '[')
	{
		while(*iteratorLexem++ != ']')
			;
		tokenType = BRACKETS;
		return tokenType;
	} 
	else if(checkLetter(iteratorLexem))
	{
		for(*pToken++ = *iteratorLexem++; checkSybol(iteratorLexem);)
			*pToken++ = *iteratorLexem++;
		*pToken = '\0';
		tokenType = NAME;
		return tokenType;
	} 
	else
	{
		tokenType = *iteratorLexem++;
		return tokenType;
	}
}

void parseDeclaration(void)
{
	while(tokenType == '*')
	{
		getToken();
		strCat(record.value->name_s, "*");
	}
	parseDirectDeclaration();
}

void parseDirectDeclaration(void)
{
	if(tokenType == NAME)
		checkFinish();
	while(tokenType == BRACKETS)
	{
		getToken();
		strCat(record.value->name_s, "[]");
	}
	if(tokenType == ';'||tokenType == ',')
		*iteratorLexem--;
}

void checkFinish()
{
	strCopy(record.value->name_i, token);
	finish = 1;
	getToken();
	if (tokenType == '(')
		fun = 1;
	parseDirectDeclaration();
}