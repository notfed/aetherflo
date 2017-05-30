#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "interpreter.h"
#include "symbols.h"
#include <forward_list>

using namespace Interpreter;
using namespace Symbols;
using namespace std;

Id::Id(const char *name) : name(name) {
}

int Id::Evaluate() // TODO: Allow more than just int
{
    const char* name = this->name;
    try
    {
        shared_ptr<Symbol> symbol = current_symbol_table->Get(name);
        if(symbol->GetKind() != SymbolInt)
        {
            fprintf(stderr, "error: variable '%s' was not an int\n", name);
            exit(1);
        }
        int val = symbol.get()->GetInt();
        return val;
    }
    catch(std::out_of_range e)
    {
        fprintf(stderr, "error: use of undeclared variable '%s'\n", name);
        exit(1);
    }
}

void Id::Assign(int val) { // TODO: Allow more than just int
    shared_ptr<Symbol> symbol = make_shared<Symbol>(this->name, val);
    current_symbol_table->Set(this->name, symbol);
}

Val::Val(int val) : val(val) {
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
    Statements *nexts = this;
    while(nexts != nullptr)
    {
       nexts->statement->Execute();
       nexts = nexts->childStatements;
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

FunctionAssignment::FunctionAssignment(Id *id, forward_list<FuncDefArgument*>* arguments, Statements *statements)
{
    this->id = id;
    this->arguments = arguments;
    this->statements = statements;
}

void FunctionAssignment::Execute()
{
    shared_ptr<Symbol> symbol = make_shared<Symbol>(this->id->name, this->statements);
    current_symbol_table->Set(this->id->name, symbol);
}

FunctionCall::FunctionCall(Id *id, forward_list<FuncCallArgument*>* arguments)
{
    this->id = id;
    this->arguments = arguments;
}

void FunctionCall::Execute()
{
    try
    {
        shared_ptr<Symbol> symbol = current_symbol_table->Get(this->id->name);
        if(symbol->GetKind() != SymbolProcedure)
        {
            fprintf(stderr, "error: '%s' was not a procedure\n", symbol->GetName().c_str());
            exit(1);
        }
        Statement* statement = symbol.get()->GetProcedure();
        statement->Execute();
    }
    catch(std::out_of_range e)
    {
        fprintf(stderr, "error: use of undeclared function '%s'\n", this->id->name);
        exit(1);
    }
}

FuncDefArgument::FuncDefArgument(Id* id)
{
    this->id = id;
}

FuncCallArgument::FuncCallArgument(Expression* expression)
{
    this->expression = expression;
}
