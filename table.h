#define LEN 1000
#define MAX_N_F 100
#define MAX_N_I 100
#define MAX_N_S 100

typedef struct value
{
	char name_f[MAX_N_F];
	char name_i[MAX_N_I];
	char name_s[MAX_N_S];
}TVALUE;

typedef struct record
{
	TVALUE *value;
	int cntLine;
	struct record *left;
	struct record *right;
}TRECORD;

TRECORD *head;
TRECORD *tail;
TRECORD *creatRecord();
void addRecord(TRECORD *record);
void setValueRecord(TRECORD *record, TRECORD *value);
void insertRecord(TRECORD *record);
int searchRecord(TVALUE *value);
int cmpValue(TVALUE *value1, TVALUE *value2);
void writeRecords(TRECORD *record);
void deleteList(TRECORD *head);
