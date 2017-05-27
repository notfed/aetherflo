#include "ast.h"
#include "aatree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

using namespace Ast;

typedef struct aatree_item
{
    int val;
} aatree_item;

static aatree* symbols = NULL;

int Val::Evaluate()
{
    return val;
}

int Id::Evaluate()
{
    aatree_item* item = (aatree_item*)(aatree_lookup(symbols,ast->data.name));
    if(item == NULL) {
        fprintf(stderr, "error: use of undeclared variable '%s'\n", ast->data.name);
        exit(1);
    } 
    return item->val;
}

int Expression::Evaluate()
{
    if(strcmp(op,"+")==0) {
        return left->Evaluate()+right->Evaluate();
    } else if(strcmp(op,"-")==0) {
        return left->Evaluate()-right->Evaluate();
    } else if(strcmp(op,"*")==0) {
        return left->Evaluate()*right->Evaluate();
    } else if(strcmp(op,"/")==0) {
        return left->Evaluate()/right->Evaluate();
    } else if(strcmp(op,"<")==0) {
        return left->Evaluate()<right->Evaluate();
    } else if(strcmp(op,"<=")==0) {
        return left->Evaluate()<=right->Evaluate();
    } else if(strcmp(op,">")==0) {
        return left->Evaluate()>right->Evaluate();
    } else if(strcmp(op,">=")==0) {
        return left->Evaluate()>=right->Evaluate();
    } else if(strcmp(op,"==")==0) {
        return left->Evaluate()==right->Evaluate();
    } else if(strcmp(op,"!=")==0) {
        return left->Evaluate()!=right->Evaluate();
    } else {
        fprintf(stderr,"fatal error: invalid op '%s'\n", op);
        exit(1);
    }
}

void Statements::Execute()
{
    this.thisStatement->Execute();
    this.childStatements->Execute();
}

void Print::Execute()
{
    int result = expr->Evaluate();
    printf("%d\n", result);
}

void Conditional::Execute()
{
    int result = condition->Evaluate();
    if(result) {
        statement->Execute();
    }
}
void Assignment::Execute()
{
	/* If variable already exists, update it in-place */
        aatree_item* item = (aatree_item*)(aatree_lookup(symbols,name));
        if(item != NULL) {
	    item->val = val;
        }
	/* Otherwise, create variable anew */
        else {
            aatree_item* newNode = new aatree_item();
            newNode->val = val;
            symbols = aatree_insert(symbols,name,newNode);
        }
}
