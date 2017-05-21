#include "interpreter.h"
#include "aatree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static int evaluateExpr(struct AstElement* ast);

typedef struct aatree_item
{
    int val;
} aatree_item;

static aatree* symbols = NULL;

static void* checkAlloc(size_t sz)
{
  void* result = calloc(sz, 1);
  if(!result)
  {
    fprintf(stderr, "alloc failed\n");
    exit(1);
  }
  return result;
}

void interpret_statements(AstElement* ast)
{
        printf("-Statement\n");
        execute(ast->data.statements.thisStatement);
        execute(ast->data.statements.childStatements);
}
void interpret_print(AstElement* ast)
{
        printf("-Print\n");
        int result = evaluateExpr(ast->data.print.expr);
        printf("%d\n", result);
}
void interpret_conditional(AstElement* ast)
{
        printf("-Conditional\n");
        int result = evaluateExpr(ast->data.conditional.condition);
        if(result) {
            execute(ast->data.conditional.statement);
        }
}
void interpret_assignment(AstElement* ast)
{
        printf("-Assignment\n");

	/* Fix our two operands: name and value */
        char* name = ast->data.assignment.name->data.name;
        int val = evaluateExpr(ast->data.assignment.right);

        printf("Assignment %s=%d\n", name, val);

	/* If variable already exists, update it in-place */
        aatree* node = aatree_lookup(symbols,name);
        if(node != NULL) {
            aatree_item* item = (aatree_item*)(node->value);
	    item->val = val;
            printf("(Updated %s to %d)", name, val);
        }
	/* Otherwise, create variable anew */
        else {
            aatree_item* newNode = checkAlloc(sizeof(aatree_item));
            newNode->val = val;
            symbols = aatree_insert(symbols,name,newNode);

            /* Just some debugging stuff to prove it worked; remove later */
            printf("(New var %s set to %d)\n", name, val);
            void* doubleCheck = aatree_lookup(symbols,name);
            if(doubleCheck == NULL ) {
                printf("(Var %s failed to update)\n", name);
            } else {
                aatree_item* item = (aatree_item*)(doubleCheck);
                printf("(Var %s is now %d)\n", name, item->val); 
            }
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
        default: 
            fprintf(stderr,"error: fatal: attempt to execute unexecutable node\n");
    }
}
int evaluateExpr(struct AstElement* ast)
{
    if(ast->kind == ekVal) {
        return ast->data.val;
    } else if(ast->kind == ekId) {
        aatree_item* item = (aatree_item*)(aatree_lookup(symbols,ast->data.name));
        if(item == NULL) {
            fprintf(stderr, "error: use of undeclared variable '%s'\n", ast->data.name);
            exit(1);
        } 
	return item->val;
    } else if(ast->kind == ekExpression) {

        AstElement* left = ast->data.expression.left;
        char* op = ast->data.expression.op->data.symbol;
        AstElement* right = ast->data.expression.right;

        if(strcmp(op,"+")==0) {
            return evaluateExpr(left)+evaluateExpr(right);
        } else if(strcmp(op,"-")==0) {
            return evaluateExpr(left)-evaluateExpr(right);
        } else if(strcmp(op,"*")==0) {
            return evaluateExpr(left)*evaluateExpr(right);
        } else if(strcmp(op,"/")==0) {
            return evaluateExpr(left)/evaluateExpr(right);
        } else if(strcmp(op,"<")==0) {
            return evaluateExpr(left)<evaluateExpr(right);
        } else if(strcmp(op,"<=")==0) {
            return evaluateExpr(left)<=evaluateExpr(right);
        } else if(strcmp(op,">")==0) {
            return evaluateExpr(left)>evaluateExpr(right);
        } else if(strcmp(op,">=")==0) {
            return evaluateExpr(left)>=evaluateExpr(right);
        } else if(strcmp(op,"==")==0) {
            return evaluateExpr(left)==evaluateExpr(right);
        } else if(strcmp(op,"!=")==0) {
            return evaluateExpr(left)!=evaluateExpr(right);
        } else {
            fprintf(stderr,"fatal error: invalid op '%s'\n", op);
            exit(1);
        }
    } else {
        fprintf(stderr,"fatal error: invalid expression\n");
        exit(1);
    }
}
