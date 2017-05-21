#include <stdlib.h> /* malloc, free */
#include <string.h> /* strcmp, strdup */

#include "aatree.h"

/* This is an AA-aatree implementation. */
/* Source: https://github.com/ccxvii/minilibs */
/* Public Domain */


static struct aatree sentinel = { "", NULL, &sentinel, &sentinel, 0 };

static struct aatree *aatree_make(const char *key, void *value)
{
	struct aatree *node = malloc(sizeof(struct aatree));
	node->key = strdup(key);
	node->value = value;
	node->left = node->right = &sentinel;
	node->level = 1;
	return node;
}

void *aatree_lookup(struct aatree *node, const char *key)
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

static struct aatree *aatree_skew(struct aatree *node)
{
	if (node->left->level == node->level) {
		struct aatree *save = node;
		node = node->left;
		save->left = node->right;
		node->right = save;
	}
	return node;
}

static struct aatree *aatree_split(struct aatree *node)
{
	if (node->right->right->level == node->level) {
		struct aatree *save = node;
		node = node->right;
		save->right = node->left;
		node->left = save;
		node->level++;
	}
	return node;
}

struct aatree *aatree_insert(struct aatree *node, const char *key, void *value)
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

void aatree_free(struct aatree *node)
{
	if (node && node != &sentinel) {
		aatree_free(node->left);
		aatree_free(node->right);
		free(node->key);
		free(node);
	}
}

#ifdef TEST
#include <stdio.h>
static void aatree_print(struct aatree *node, int level)
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

int main(int argc, char **argv)
{
	struct aatree *args = NULL;
	int i;
	for (i = 0; i < argc; ++i) {
		char buf[10];
		sprintf(buf, "%d", i);
		args = aatree_insert(args, argv[i], strdup(buf));
	}
	aatree_print(args, 0);
	return 0;
}
#endif
