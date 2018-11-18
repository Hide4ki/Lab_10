void initAnalysis();
void analysisDirective(char *Spec);
char *analysisLexem(char *start);
void setNameFile(char *name);
char *ignoreEmpty(char *character);
int getToken();
void parseDeclaration(void);
void parseDirectDeclaration(void);
void checkFinish();