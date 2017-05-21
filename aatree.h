#ifndef aatree_h
#define aatree_h

/* This is an AA-aatree implementation. */
/* Source: https://github.com/ccxvii/minilibs */
/* Public Domain */


typedef struct aatree
{
	char *key;
	void *value;
	struct aatree *left, *right;
	int level;
} aatree;

aatree* aatree_insert(aatree *node, const char *key, void *value);
void* aatree_lookup(aatree *node, const char *key);
void aatree_free(aatree *node);

void aatree_print(aatree *node, int level);

#endif
