extern int inCharLiteral;
extern int inStrLiteral;

int checkComment(char *l)
{
	static int coment;
	if (*l == '\a')
		coment = 0;
	if (*l == '/' || coment)
	{
		if (!coment)
		{
			if (*(l + 1) == '/')
				coment = 1;
			else if (*(l + 1) == '*')
				coment = 2;
		}
		else
		{
			switch (coment)
			{
			case 1:
				if (*(l + 1) == '\0')
					coment = 0;
				break;
			case 2:
				if (*(l - 1) == '*' && *l == '/')
					coment = 0;
				break;
			}
		}
		return 0;
	}
	return 1;
}

void checkCharLiteral(char *l)
{
	int k;
	k = 0;
	while (*(l - k - 1) == '\\') k++;
	if (inCharLiteral)
	{
		if (!(k & 1))
			inCharLiteral = 0;
	}
	else
		if (!(k & 1))
			inCharLiteral = 1;
}

void checkStrLiteral(char *l)
{
	int k;
	k = 0;
	while (*(l - k - 1) == '\\') k++;
	if (inStrLiteral)
	{
		if (!(k & 1))
			inStrLiteral = 0;
	}
	else
		if (!(k & 1))
			inStrLiteral = 1;
}

int checkSybol(char *c)
{
	if (*c >= 'a' && *c <= 'z' || *c >= 'A' && *c <= 'Z' || *c == '_' || *c >= '0' && *c <= '9')
		return 1;
	return 0;
}

int checkLetter(char *c)
{
	if (*c >= 'a' && *c <= 'z' || *c >= 'A' && *c <= 'Z' || *c == '_')
		return 1;
	return 0;
}

