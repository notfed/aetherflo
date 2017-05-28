#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "interpreter.h"

using namespace Interpreter;

static std::unordered_map<std::string, int> symbols;

Id::Id(const char *name) {
    this->name = strdup(name);
}

int Id::Evaluate()
{
    const char* name = this->name;
    
    try
    {
        return symbols.at(name);
    }
    catch(std::out_of_range e)
    {
        fprintf(stderr, "error: use of undeclared variable '%s'\n", name);
        exit(1);
    }
}

void Id::Assign(int val) {
    symbols[this->name] = val;
}

Val::Val(int val) {
    this->val = val;
}

int Val::Evaluate()
{
    return this->val;
}

Op::Op(const char *symbol) {
    this->symbol = strdup(symbol);
}

Statements::Statements(Statement *statement, Statements *childStatements) {
    this->statement = statement;
    this->childStatements = childStatements;
}

void Statements::Execute()
{
    if(this->statement != nullptr)
    {
        this->statement->Execute();
    }
    if(this->childStatements != nullptr)
    {
        this->childStatements->Execute();
    }
}

Assignment::Assignment(Id *id, Expression* right) {
    this->id= id;
    this->right = right;
}

void Assignment::Execute() {
    int val = this->right->Evaluate();
    this->id->Assign(val);
}

Conditional::Conditional(Expression* condition, Statement* statement) {
    this->condition = condition;
    this->statement = statement;
}

void Conditional::Execute()
{
    int result = this->condition->Evaluate();
    if(result) {
        this->statement->Execute();
    }
}

Print::Print(Expression *expr) {
    this->expr = expr;
}

void Print::Execute()
{
    int result = this->expr->Evaluate();
    printf("%d\n", result);
}

ExpressionNode::ExpressionNode(Expression *left, Expression *right, Op* op) {
    this->left = left;
    this->right = right;
    this->op = op;
}

int ExpressionNode::Evaluate()
{
    const char* op = this->op->symbol;
    int left = this->left->Evaluate();
    int right = this->right->Evaluate();

    if(strcmp(op,"+")==0) {
        return left+right;
    } else if(strcmp(op,"-")==0) {
        return left-right;
    } else if(strcmp(op,"*")==0) {
        return left*right;
    } else if(strcmp(op,"/")==0) {
        return left/right;
    } else if(strcmp(op,"<")==0) {
        return left<right;
    } else if(strcmp(op,"<=")==0) {
        return left<=right;
    } else if(strcmp(op,">")==0) {
        return left>right;
    } else if(strcmp(op,">=")==0) {
        return left>=right;
    } else if(strcmp(op,"==")==0) {
        return left==right;
    } else if(strcmp(op,"!=")==0) {
        return left!=right;
    } else {
        fprintf(stderr,"fatal error: invalid op '%s'\n", op);
        exit(1);
    }
}
