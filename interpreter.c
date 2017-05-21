#include "interpreter.h"
#include "aatree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static int evaluateInt(struct AstElement* ast);
static int evaluateBool(struct AstElement* ast);

typedef struct aatree_item
{
    int val;
} aatree_item;

static aatree symbols;

static void* checkAlloc(size_t sz)
{
  void* result = calloc(sz, 1);
  if(!result)
  {
    fprintf(stderr, "alloc failed\n");
    exit(1);
  }
}


void interpret_statements(AstElement* ast)
{
        execute(ast->data.statements.thisStatement);
        execute(ast->data.statements.childStatements);
}
void interpret_print(AstElement* ast)
{
        int result = evaluateInt(ast->data.print.expr);
        printf("Print: %d\n", result);
}
void interpret_conditional(AstElement* ast)
{
        printf("Conditional\n");
}
void interpret_assignment(AstElement* ast)
{

	/* Fix our two operands: name and value */
        char* name = ast->data.assignment.name->data.name;
        int val = evaluateInt(ast->data.assignment.right);

        printf("Assignment to %s\n", name);

	/* If variable already exists, update it in-place */
        aatree* node = aatree_lookup(&symbols,name);
        if(node != NULL) {
            aatree_item* item = (aatree_item*)(node->value);
	    item->val = val;
        }
	/* Otherwise, create variable anew */
        else {
            aatree_item* newNode = checkAlloc(sizeof(aatree_item));
            newNode->val = val;
            aatree_insert(&symbols,name,newNode);
        }
}

void execute(struct AstElement* ast)
{
    switch(ast->kind) 
    {
        case ekStatements: interpret_statements(ast); break;
        case ekPrint: interpret_print(ast); break;
        case ekAssignment: interpret_assignment(ast); break;
        case ekConditional: interpret_conditional(ast); break;
    }
}
int evaluateInt(struct AstElement* ast)
{
    if(ast->kind == ekVal) {
        return ast->data.val;
    } else if(ast->kind == ekId) {
        aatree* node = aatree_lookup(&symbols,ast->data.name);
        if(node == NULL) {
            fprintf(stderr, "error: use of undeclared variable '%s'\n", ast->data.name);
            exit(1);
        } 
        aatree_item* item = (aatree_item*)(node->value);
	return item->val;
    } else if(ast->kind == ekExpression) {

        AstElement* left = ast->data.expression.left;
        char* op = ast->data.expression.op->data.symbol;
        AstElement* right = ast->data.expression.left;

        if(strcmp(op,"+")) {
            return evaluateInt(left)+evaluateInt(right);
        } else if(strcmp(op,"-")) {
            return evaluateInt(left)-evaluateInt(right);
        } else if(strcmp(op,"*")) {
            return evaluateInt(left)*evaluateInt(right);
        } else if(strcmp(op,"/")) {
            return evaluateInt(left)/evaluateInt(right);
        } else {
            fprintf(stderr,"fatal error: invalid op\n");
            exit(1);
        }
    } else {
        fprintf(stderr,"fatal error: invalid expression\n");
        exit(1);
    }
}
/* 0 is false, 1 is true */
int evaluateBool(struct AstElement* ast)
{
    if(ast->kind == ekVal) {
        return ast->data.val;
    } else if(ast->kind == ekId) {
        fprintf(stderr, "error: boolean variables not supported\n");
        exit(1);
    } else if(ast->kind == ekExpression) {

        AstElement* left = ast->data.expression.left;
        char* op = ast->data.expression.op->data.symbol;
        AstElement* right = ast->data.expression.left;

        if(strcmp(op,"<")) {
            return evaluateBool(left)<evaluateBool(right);
        } else if(strcmp(op,"<=")) {
            return evaluateBool(left)<=evaluateBool(right);
        } else if(strcmp(op,">")) {
            return evaluateBool(left)>evaluateBool(right);
        } else if(strcmp(op,">=")) {
            return evaluateBool(left)>=evaluateBool(right);
        } else {
            fprintf(stderr,"fatal error: invalid op\n");
            exit(1);
        }

    }
}
