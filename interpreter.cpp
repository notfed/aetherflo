#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "interpreter.h"
#include "symbols.h"
#include <forward_list>
#include <iostream>

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
        shared_ptr<Symbol> symbol(current_symbol_table->Get(name));
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

Op::Op(const char *glyph) : glyph(glyph) {
}

Statements::Statements(Statement *statement, Statements *childStatements) 
    : statement(statement), childStatements(childStatements)
{
}

void Statements::Execute()
{
    // fprintf(stderr, "Inside statements\n"); // TODO: Debug
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
        fprintf(stderr,"fatal error: invalid op '%s'\n", op);
        exit(1);
    }
}

FunctionAssignment::FunctionAssignment(Id *id, forward_list<FuncDefArgument*>* arguments, Statements *statements)
    : id(id), arguments(arguments), statements(statements) { }

void FunctionAssignment::Execute()
{
    // Create closure for this procedure definition
    shared_ptr<SymbolTable> closure(current_symbol_table);

    // Create symbol for procedure
    shared_ptr<Symbol> symbol = make_shared<Symbol>(this->id->name, this, closure);

    // Place this symbol in the current symbol table
    current_symbol_table->Set(this->id->name, symbol);

    // fprintf(stderr, "Created function '%s' in closure %d\n", this->id->name, current_symbol_table->sequence); // TODO: Debugging

    // Get desired procedure symbol
    shared_ptr<Symbol> symbol2 = current_symbol_table->Get(this->id->name);

    // Assert  it's really a procedure (TODO: For debug)
    if(symbol2->GetKind() != SymbolProcedure)
    {
            fprintf(stderr, "error: '%s' failed to save into closure %d)\n", symbol->GetName().c_str(), current_symbol_table->sequence);
            exit(1);
    }
}

FunctionCall::FunctionCall(Id *id, forward_list<FuncCallArgument*>* arguments) : id(id), arguments(arguments)
{
}

typedef forward_list<FuncDefArgument*>::iterator fdaIter;
typedef forward_list<FuncCallArgument*>::iterator fcaIter;

void FunctionCall::Execute()
{
    try
    {
        // Get desired procedure symbol
        shared_ptr<Symbol> symbol = current_symbol_table->Get(this->id->name);

        // Assert  it's really a procedure
        if(symbol->GetKind() != SymbolProcedure)
        {
            fprintf(stderr, "error: '%s' was not a procedure (closure %d)\n", symbol->GetName().c_str(), current_symbol_table->sequence);
            exit(1);
        }

        // Backup the current symbol table
        SymbolTableStateGuard guard();

        // Pull the procedure's closure
        shared_ptr<SymbolTable> closure = symbol->closure;

        // Create a new closure with arguments added
        shared_ptr<SymbolTable> closureWithArgs = make_shared<SymbolTable>(closure.get());

        // Shove all arguments into this new closureWithArgs
        auto a = this->arguments;
        auto b = symbol->fVal->arguments;
        for(pair<fcaIter,fdaIter> i(a->begin(),b->begin()); 
            i.first !=  a->end() && i.second != b->end();
            ++i.first, ++i.second)
        {
            fprintf(stderr, "created new closure with arg=%s\n", (*i.second)->id->name); // TODO: Just doing this for debugging
            auto argName = (*i.second)->id->name;
            auto argValue = make_shared<Symbol>(argName, (*i.first)->expression->Evaluate()); // TODO: Need to do this before entering closure!!!!!
            closureWithArgs->Set(argName, argValue); 
        }

        current_symbol_table = closureWithArgs;

        // Execute the procedure
        //fprintf(stderr, "executing procedure '%s'\n", this->id->name); // TODO: Just doing this for debugging
        FunctionAssignment* assignment = symbol->GetProcedure();
        assignment->statements->Execute();
    }
    catch(std::out_of_range e)
    {
        fprintf(stderr, "error: use of undeclared function '%s'\n", this->id->name);
        exit(1);
    }
}

FuncDefArgument::FuncDefArgument(Id* id) : id(id)
{
}

FuncCallArgument::FuncCallArgument(Expression* expression) : expression(expression)
{
}
