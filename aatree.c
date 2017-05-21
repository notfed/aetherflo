#include <stdlib.h> /* malloc, free */
#include <string.h> /* strcmp, strdup */
#include <stdio.h>  /* printf */

#include "aatree.h"

/* This is an AA-aatree implementation. */
/* Source: https://github.com/ccxvii/minilibs */
/* Public Domain */


static aatree sentinel = { "", NULL, &sentinel, &sentinel, 0 };

void aatree_print(aatree *node, int level)
{
	int i;
	if (node->left != &sentinel)
		aatree_print(node->left, level + 1);
	for (i = 0; i < level; ++i)
		putchar(' ');
	printf("%s = %s (%d)\n", node->key, (char*)node->value, node->level);
	if (node->right != &sentinel)
		aatree_print(node->right, level + 1);
}

static aatree *aatree_make(const char *key, void *value)
{
	aatree *node = malloc(sizeof(aatree));
	node->key = strdup(key);
	node->value = value;
	node->left = node->right = &sentinel;
	node->level = 1;
	return node;
}

void *aatree_lookup(aatree *node, const char *key)
{
	if (node) {
		while (node != &sentinel) {
			int c = strcmp(key, node->key);
			if (c == 0)
				return node->value;
			else if (c < 0)
				node = node->left;
			else
				node = node->right;
		}
	}
	return NULL;
}

static aatree *aatree_skew(aatree *node)
{
	if (node->left->level == node->level) {
		aatree *save = node;
		node = node->left;
		save->left = node->right;
		node->right = save;
	}
	return node;
}

static aatree *aatree_split(aatree *node)
{
	if (node->right->right->level == node->level) {
		aatree *save = node;
		node = node->right;
		save->right = node->left;
		node->left = save;
		node->level++;
	}
	return node;
}

aatree *aatree_insert(aatree *node, const char *key, void *value)
{
	if (node && node != &sentinel) {
		int c = strcmp(key, node->key);
		if (c < 0)
			node->left = aatree_insert(node->left, key, value);
		else
			node->right = aatree_insert(node->right, key, value);
		node = aatree_skew(node);
		node = aatree_split(node);
		return node;
	}
	return aatree_make(key, value);
}

void aatree_free(aatree *node)
{
	if (node && node != &sentinel) {
		aatree_free(node->left);
		aatree_free(node->right);
		free(node->key);
		free(node);
	}
}

