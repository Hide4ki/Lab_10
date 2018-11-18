char strCmp(char *s1,char *s2)
{
	for(;*s1==*s2;s1++,s2++)
		if(*s1=='\0')
			return 0;
	return *s1-*s2;
}

void strCopy(char *s1, char *s2)
{
	while(*s1++ = *s2++)
		;
}

void strCat(char *s1, char *s2)
{
	for(;*s1;s1++)
		;
	strCopy(s1,s2);
}

int strLen(char *s)
{
	int i;
	i = 1;
	while(*s++)
		i++;
	return i;
}