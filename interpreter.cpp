#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "interpreter.h"
#include "symbols.h"
#include <forward_list>
#include <iostream>
#include <memory>
#include "log/minlog.h"

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
        Object* object = current_symbol_table->Get(name);
        if(object->GetKind() != SymbolInt)
        {
            minlog::fatal("error: variable '%s' was not an int\n", name);
            exit(1);
        }
        int val = object->GetInt();
        return val;
    }
    catch(std::out_of_range e)
    {
        minlog::fatal("error: use of undeclared variable '%s'\n", name);
        exit(1);
    }
}

void Id::Assign(int val) { // TODO: Allow more than just int

    Object* object = new Object(val);

    if(current_symbol_table->Contains(this->name)) {
      current_symbol_table->Get(this->name)->SetInt(val);
    } else {
      current_symbol_table->Set(this->name, object);
    }
}

Val::Val(int val) : val(val) {
}

int Val::Evaluate()
{
    return this->val;
}

Op::Op(const char *glyph) : glyph(glyph) {
}

Statements::Statements(Statement *statement, Statements *childStatements) 
    : statement(statement), childStatements(childStatements)
{
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

Assignment::Assignment(Id *id, Expression* right) : id(id), right(right) {
}

void Assignment::Execute() {
    int val = this->right->Evaluate();
    this->id->Assign(val);
    minlog::debug("Assigning %s=%d\n", this->id->name, val);
}

Conditional::Conditional(Expression* condition, Statement* statement) 
    : condition(condition), statement(statement) { }

void Conditional::Execute()
{
    int result = this->condition->Evaluate();
    if(result) {
        this->statement->Execute();
    }
}

Print::Print(Expression *expr) : expr(expr) { }

void Print::Execute()
{
    int result = this->expr->Evaluate();
    printf("%d\n", result);
}

ExpressionNode::ExpressionNode(Expression *left, Expression *right, Op* op) 
    : left(left), right(right), op(op) { }

int ExpressionNode::Evaluate()
{
    const char* op = this->op->glyph;
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
        minlog::fatal("fatal error: invalid op '%s'\n", op);
        exit(1);
    }
}

ProcedureDeclaration::ProcedureDeclaration(Id *id, forward_list<Id*>* arguments, Statements *statements)
    : id(id), arguments(arguments), statements(statements) { }

void ProcedureDeclaration::Execute()
{
    // Clone the current symbol table into a closure
    SymbolTable* closure = new SymbolTable(*current_symbol_table);

    // Output some debugging information
    int argumentsSize = 0;
    for(auto iter : *this->arguments) ++argumentsSize;
    minlog::debug("Declaring procedure '%s'(%d args) (in closure %d with closure %d)...\n", this->id->name, argumentsSize, current_symbol_table->sequence, closure->sequence);

    // Create object for procedure, with newly created closure as its closure
    Object* procedure = new Object(this, closure);

    // Place the procedure in its own closure
    closure->Set(this->id->name, procedure);

    // Place this object in the current symbol table
    current_symbol_table->Set(this->id->name, procedure);
}

ProcedureCall::ProcedureCall(Id *id, forward_list<ProcedureCallArgument*>* arguments) : id(id), arguments(arguments)
{
}

typedef forward_list<Id*> argDefList_t;
typedef forward_list<Id*>::iterator argDefListIter_t;
typedef forward_list<ProcedureCallArgument*> argCallList_t;
typedef forward_list<ProcedureCallArgument*>::iterator argCallListIter_t;

void ProcedureCall::Execute()
{
    try
    {

        // Get procedure object out of current symbol table
        minlog::debug("Fetching procedure '%s' (from closure %d)...\n", this->id->name, current_symbol_table->sequence);
        Object* procedure = current_symbol_table->Get(this->id->name);
        minlog::debug("Calling procedure '%s' (using closure %d)...\n", this->id->name, procedure->closure->sequence);

        // Assert  it's really a procedure
        if(procedure->GetKind() != SymbolProcedure)
        {
            minlog::debug("error: '%s' was not a procedure (closure %d)\n", this->id->name, current_symbol_table->sequence);
            exit(1);
        }

        // Clone the procedure's closure, to which we will add arguments
        SymbolTable* closureWithArgs = new SymbolTable(*procedure->closure);

        // Populate the closureWithArgs with arguments
        argCallList_t* a = this->arguments;
        argDefList_t* b = procedure->fVal->arguments;

        for(pair<argCallListIter_t,argDefListIter_t> i(a->begin(),b->begin()); 
            i.first !=  a->end() && i.second != b->end();
            ++i.first, ++i.second)
        {
            ProcedureCallArgument* callArgument = (*i.first);
            Id* declaredArgument = (*i.second);
            minlog::debug("  arg='%s'\n", declaredArgument->name);
            const char *argName = declaredArgument->name;
            Object* argValue = new Object(callArgument->expression->Evaluate());
            closureWithArgs->Set(argName, argValue);
        }

        // Backup the current symbol table
        SymbolTableStateGuard guard;

        // Use our new closureWithArgs during our function call
        minlog::debug("Switching to closure %d\n", closureWithArgs->sequence);
        current_symbol_table = closureWithArgs;

        // Execute the procedure
        minlog::debug("  executing %s\n", this->id->name);
        ProcedureDeclaration* assignment = procedure->GetProcedure();
        assignment->statements->Execute();
    }
    catch(std::out_of_range e)
    {
        (stderr, "error: use of undeclared function '%s'\n", this->id->name);
        exit(1);
    }
}

ProcedureCallArgument::ProcedureCallArgument(Expression* expression) : expression(expression)
{
}
