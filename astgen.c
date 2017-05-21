#include "astgen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void execute(struct AstElement* ast)
{
	printf("TODO: Execute...\n");
}

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
	result->data.statements.thisStatement = thisStatement;
	result->data.statements.childStatements = childStatements;
	return result;
}

struct AstElement* makeExp(struct AstElement* left, struct AstElement* right, struct AstElement* op)
{
	struct AstElement* result = checkAlloc(sizeof(AstElement));
	result->kind = ekExpression;
	result->data.expression.left = left;
	result->data.expression.right = right;
	result->data.expression.op = op;
	return result;
}

struct AstElement* makeBoolExp(struct AstElement* left, struct AstElement* relop, struct AstElement* right)
{
	struct AstElement* result = checkAlloc(sizeof(AstElement));
	result->kind = ekBoolExpression;
	result->data.boolexpression.left = left;
	result->data.boolexpression.relop = relop;
	result->data.boolexpression.right = right;
	return result;
}

struct AstElement* makeAssignment(struct AstElement* name, struct AstElement* right)
{
	struct AstElement* result = checkAlloc(sizeof(AstElement));
	result->kind = ekAssignment;
	result->data.assignment.name = name;
	result->data.assignment.right= right;
	return result;
}

struct AstElement* makeConditional(struct AstElement*condition, struct AstElement* statement)
{
	struct AstElement* result = checkAlloc(sizeof(AstElement));
	result->kind = ekConditional;
	result->data.conditional.condition=condition;
	result->data.conditional.statement=statement;
	return result;
}

struct AstElement* makePrint(struct AstElement* expr)
{
	struct AstElement* result = checkAlloc(sizeof(AstElement));
	result->kind = ekPrint;
	result->data.print.expr = expr;
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
	result->data.val = val;
	return result;
}
struct AstElement* makeId(char* name)
{
	struct AstElement* result = checkAlloc(sizeof(AstElement));
	result->kind = ekId;
	result->data.name = checkAlloc(strlen(name)+1);
	strcpy(result->data.name, name);
	return result;
}
struct AstElement* makeOp(char *symbol)
{
	struct AstElement* result = checkAlloc(sizeof(AstElement));
	result->kind = ekOp;
	result->data.symbol = checkAlloc(strlen(symbol)+1);
	strcpy(result->data.symbol, symbol);
	return result;
}
