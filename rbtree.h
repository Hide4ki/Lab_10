#define BLACK 0
#define RED 1

struct node;

typedef struct spec
{
	char *value;
	char *altvalue;
} TSPEC;

typedef struct node
{
	struct node *left;
	struct node *right;
	struct node *parent;
	int color;
	TSPEC *value;
}TNODE;

TNODE *pleaf;
TNODE *rootDEF;
TNODE *rootSTD;

void addSpec(TNODE **root, char *value, char *altvalue);
TNODE *createNode();
void setNodeValue(TNODE *node, TSPEC *value);
void insertNode(TNODE **root,TNODE *node);
void restoreRBTree(TNODE **root,TNODE *node);
void leftRotate(TNODE **root, TNODE *node);
void rightRotate(TNODE **root, TNODE *node);
TNODE *searchNode(TNODE *root, char *value);
void deleteTree(TNODE *root);
TNODE *deleteSpec(TNODE **root, TNODE *node);
TNODE *tree_successor(TNODE *node);
TNODE *tree_min(TNODE *node);
TNODE *tree_predecessor(TNODE *node);
TNODE *tree_max(TNODE *node);
void RB_deleteFixUp(TNODE *root, TNODE *node);