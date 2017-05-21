#include "astgen.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static void* checkAlloc(size_t sz)
{
  void* result = calloc(sz, 1);
  if(!result)
  {
    fprintf(stderr, "alloc failed\n");
    exit(1);
  }
}
struct AstElement* makeStatement(struct AstElement* childStatements, struct AstElement* thisStatement)
{
	struct AstElement* result = checkAlloc(sizeof(AstElement));
	result->kind = ekStatements;
	result->data.statements.thisStatement = childStatements;
	result->data.statements.childStatements = childStatements;
	return result;
}

struct AstElement* makeExp(struct AstElement* left, struct AstElement* right, char* op)
{
	struct AstElement* result = checkAlloc(sizeof(AstElement));
	result->kind = ekExpression;
	return result;
}

struct AstElement* makeBoolExp(struct AstElement* left, char* relop, struct AstElement* right)
{
	struct AstElement* result = checkAlloc(sizeof(AstElement));
	result->kind = ekBoolExpression;
	return result;
}

struct AstElement* makeAssignment(char*name, struct AstElement* right)
{
	struct AstElement* result = checkAlloc(sizeof(AstElement));
	result->kind = ekAssignment;
	return result;
}

struct AstElement* makeConditional(struct AstElement*condition, struct AstElement* statement)
{
	struct AstElement* result = checkAlloc(sizeof(AstElement));
	result->kind = ekConditional;
	return result;
}

struct AstElement* makePrint(struct AstElement* expr)
{
	struct AstElement* result = checkAlloc(sizeof(AstElement));
	result->kind = ekPrint;
	return result;
}
struct AstElement* makeEmpty()
{
	struct AstElement* result = checkAlloc(sizeof(AstElement));
	result->kind = ekEmpty;
	return result;
}

struct AstElement* makeVal(int val)
{
	struct AstElement* result = checkAlloc(sizeof(AstElement));
	result->kind = ekVal;
	return result;
}
struct AstElement* makeId(char* name)
{
	struct AstElement* result = checkAlloc(sizeof(AstElement));
	result->kind = ekId;
	return result;
}
void execute(struct AstElement* ast)
{
	printf("TODO: Execute...");
}
