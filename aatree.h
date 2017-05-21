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

struct aatree *aatree_insert(struct aatree *node, const char *key, void *value);
void *aatree_lookup(struct aatree *node, const char *key);
void aatree_free(struct aatree *node);

#endif
