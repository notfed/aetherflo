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

const char* Id::GetName() {
    return this->name;
}

Val::Val(int val) {
    this->val = val;
}

Op::Op(const char *symbol) {
    this->symbol = strdup(symbol);
}

Statements::Statements(Statement *statement, Statements *childStatements) {
    this->statement = statement;
    this->childStatements = childStatements;
}

ExpressionNode::ExpressionNode(Expression *left, Expression *right, Op* op) {
    this->left = left;
    this->right = right;
    this->op = op;
}

Assignment::Assignment(Id *id, Expression* right) {
    this->id= id;
    this->right = right;
}

Conditional::Conditional(Expression* condition, Statement* statement) {
    this->condition = condition;
    this->statement = statement;
}

Print::Print(Expression *expr) {
    this->expr = expr;
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

int Val::Evaluate()
{
    return this->val;
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


void Print::Execute()
{
    int result = this->expr->Evaluate();
    printf("%d\n", result);
}

void Conditional::Execute()
{
    int result = this->condition->Evaluate();
    if(result) {
        this->statement->Execute();
    }
}
void Assignment::Execute()
{
    const char* name = this->id->GetName();
    int val = this->right->Evaluate();
    symbols[name] = val;
}
