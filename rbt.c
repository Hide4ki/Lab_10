#include <malloc.h>
#include "string.h"
#include "rbtree.h"

void addSpec(TNODE **root, char *value, char *altvalue)
{
	TNODE *node;
	TSPEC spec;
	spec.altvalue = altvalue;
	spec.value = value;
	node = createNode();
	setNodeValue(node, &spec);
	insertNode(root, node);
}

TNODE *createNode()
{
	TNODE *newNode;
	newNode = (TNODE*)calloc(1,sizeof(TNODE));
	newNode->value = (TSPEC*)calloc(1,sizeof(TSPEC));
	newNode->left = pleaf;
	newNode->right = pleaf;
	return newNode;
}

void setNodeValue(TNODE *node, TSPEC *value)
{
	node->value->altvalue = (char*)calloc(strLen(value->altvalue),sizeof(char));
	node->value->value = (char*)calloc(strLen(value->value),sizeof(char));
	strCopy(node->value->altvalue,value->altvalue);
	strCopy(node->value->value,value->value);
}

void insertNode(TNODE **root,TNODE *node)
{
	TNODE *iterator;
	TNODE *tmpNode;
	tmpNode = pleaf;
	iterator = *root;
	while(iterator!=pleaf)
	{
		tmpNode = iterator;
		if(strCmp(node->value->value,iterator->value->value)<0)
			iterator = iterator->left;
		else
			iterator = iterator->right;
	}
	node->parent = tmpNode;
	if(tmpNode==pleaf)
		*root = node;
	else if(strCmp(node->value->value,tmpNode->value->value)<0)
		tmpNode->left = node;
	else
		tmpNode->right = node;
}

void restoreRBTree(TNODE **root, TNODE *node)
{
	TNODE *tmpNode;
	node->color = RED;
	while(node!=*root && node->parent->color==RED)
	{
		if(node->parent == node->parent->parent->left)
		{
			tmpNode = node->parent->parent->right;
			if(tmpNode->color == RED)
			{
				node->parent->color = BLACK;
				tmpNode->color = BLACK;
				node->parent->parent->color = RED;
				node = node->parent->parent;
			}
			else 
			{
				if(node == node->parent->right)
				{
					node = node->parent;
					leftRotate(root,node);
				}
				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				rightRotate(root,node->parent->parent);
			}
		}
		else
		{
			tmpNode = node->parent->parent->left;
			if(tmpNode->color == RED)
			{
				node->parent->color = BLACK;
				tmpNode->color = BLACK;
				node->parent->parent->color = RED;
				node = node->parent->parent;
			}
			else 
			{
				if(node == node->parent->left)
				{
					node = node->parent;
					rightRotate(root, node);
				}
				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				leftRotate(root, node->parent->parent);
			}
		}
	}
	(*root)->color = BLACK;
}

void leftRotate(TNODE **root, TNODE *node)
{
	TNODE *r_child;
	r_child = node->right;
	node->right = r_child->left;
	if(r_child->left!=pleaf)
		r_child->left->parent = node;
	r_child->parent = node->parent;
	if(node->parent == pleaf) *root = r_child;
	else if(node == node->parent->left) node->parent->left = r_child;
	else node->parent->right = r_child;
	r_child->left = node;
	node->parent = r_child;
}

void rightRotate(TNODE **root, TNODE *node)
{	
	TNODE *l_child;
	l_child = node->left;
	node->left = l_child->right;
	if(l_child->right!=pleaf)
		l_child->right->parent = node;
	l_child->parent = node->parent;
	if(node->parent == pleaf) *root = l_child;
	else if(node == node->parent->left) node->parent->left = l_child;
	else node->parent->right = l_child;
	l_child->right = node;
	node->parent = l_child;
}

TNODE *searchNode(TNODE *root, char *value)
{
	TNODE *tmpNode;
	int cmpResult;
	tmpNode = root;
	while(tmpNode!=pleaf)
	{ 
		cmpResult = strCmp(value, tmpNode->value->value);
		if(cmpResult == 0)	
			break;
		tmpNode = (cmpResult < 0)?tmpNode->left:tmpNode->right;
	}
	return tmpNode;
}

void deleteTree(TNODE *root)
{
	if(root->left!=pleaf)
		deleteTree(root->left);
	if(root->right!=pleaf)
		deleteTree(root->right);
	free(root->value->altvalue);
	free(root->value->value);
	free(root->value);
	free(root);
}

TNODE *deleteSpec(TNODE **root, TNODE *node)
{
	TNODE *tmpNode;
	TNODE *replaceNode;
	if(node->left == pleaf || node->right==pleaf)
		tmpNode = node;
	else
		tmpNode = tree_successor(node);
	if(tmpNode->left!=pleaf)
		replaceNode = tmpNode->left;
	else
		replaceNode = tmpNode->right;
	replaceNode->parent = tmpNode->parent;
	if(replaceNode->parent==pleaf)
		*root = replaceNode;
	else if(tmpNode == tmpNode->parent->left)
			tmpNode->parent->left = replaceNode;
		else 
			tmpNode->parent->right = replaceNode;
	if(node!=tmpNode)
		node->value = tmpNode->value;
	if(tmpNode->color == BLACK)
		RB_deleteFixUp(*root, replaceNode);
	return tmpNode;
}

TNODE *tree_successor(TNODE *node)
{
	TNODE *tmpNode;
	if(node->right!=pleaf)
		return tree_min(node->right);
	tmpNode = node->parent;
	while(tmpNode!=pleaf && node==tmpNode->right)
	{
		node = tmpNode;
		tmpNode = tmpNode->parent;
	}
	return tmpNode;
}

TNODE *tree_min(TNODE *node)
{
	while(node->left!=pleaf)
		node=node->left;
	return node;
}

TNODE *tree_predecessor(TNODE *node)
{
	TNODE *tmp;
	if(node->left!=pleaf)
		return tree_max(node->left);
	tmp = node->parent;
	while(tmp!=pleaf && node==tmp->left)
	{
		node = tmp;
		tmp = tmp->parent;
	}
	return tmp;
}

TNODE *tree_max(TNODE *node)
{
	while(node->right!=pleaf)
		node=node->right;
	return node;
}

void RB_deleteFixUp(TNODE *root, TNODE *node)
{
	TNODE *tmpNode;
	while(node != root && node->color == BLACK)
	{
		if(node == node->parent->left)
		{
			tmpNode = node->parent->right;
			if(tmpNode->color == RED)
			{
				tmpNode->color = BLACK;
				node->parent->color = RED;
				leftRotate(&root,node->parent);
				tmpNode = node->parent->right;
			}
			if(tmpNode->left->color == BLACK && tmpNode->right->color == BLACK)
			{
				tmpNode->color = RED;
				node = node->parent;
			}
			else 
			{
				if(tmpNode->right->color == BLACK)
				{
					tmpNode->left->color = BLACK;
					tmpNode->color = RED;
					rightRotate(&root, tmpNode);
					tmpNode = node->parent->right;
				}
				tmpNode->color = node->parent->color;
				node->parent->color = BLACK;
				tmpNode->right->color = BLACK;
				leftRotate(&root,node->parent);
				node = root;
			}
		}
		else
		{
			tmpNode = node->parent->left;
			if(tmpNode->color == RED)
			{
				tmpNode->color = BLACK;
				node->parent->color = RED;
				rightRotate(&root,node->parent);
				tmpNode = node->parent->left;
			}
			if(tmpNode->right->color == BLACK && tmpNode->left->color == BLACK)
			{
				tmpNode->color = RED;
				node = node->parent;
			}
			else 
			{
				if(tmpNode->left->color == BLACK)
				{
					tmpNode->right->color = BLACK;
					tmpNode->color = RED;
					leftRotate(&root,tmpNode);
					tmpNode = node->parent->left;
				}
				tmpNode->color = node->parent->color;
				node->parent->color = BLACK;
				tmpNode->left->color = BLACK;
				rightRotate(&root,node->parent);
				node = root;
			}
		}
	}
	node->color = BLACK;
}
