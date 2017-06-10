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
        Object* object = current_symbol_table->Get(name);
        if(object->GetKind() != SymbolInt)
        {
            fprintf(stderr, "error: variable '%s' was not an int\n", name);
            exit(1);
        }
        int val = object->GetInt();
        return val;
    }
    catch(std::out_of_range e)
    {
        fprintf(stderr, "error: use of undeclared variable '%s'\n", name);
        exit(1);
    }
}

void Id::Assign(int val) { // TODO: Allow more than just int
    Object* object = new Object(val);
    current_symbol_table->Set(this->name, object);
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

ProcedureDeclaration::ProcedureDeclaration(Id *id, forward_list<Id*>* arguments, Statements *statements)
    : id(id), arguments(arguments), statements(statements) { }

void ProcedureDeclaration::Execute()
{

    // Create object for procedure, with current symbol table as its closure
    Object* object = new Object(this, current_symbol_table);

    // Place this object in the current symbol table
    current_symbol_table->Set(this->id->name, object);

    // Note: The rest is not needed.
    //  the ProcedureDeclaration closure does not contain arguments or variables
    //  until it is called:

    // Create closure for this procedure definition
    // shared_ptr<SymbolTable> closure = make_shared<SymbolTable>(*current_symbol_table);

    // Place this object in the closure
    //closure->Set(this->id->name, object);
    

/* TODO: For debugging
    // fprintf(stderr, "Created function '%s' in closure %d\n", this->id->name, current_symbol_table->sequence); // TODO: Debugging

    // Get desired procedure object
    shared_ptr<Object> object2 = current_symbol_table->Get(this->id->name);

    // Assert  it's really a procedure (TODO: For debug)
    if(object2->GetKind() != SymbolProcedure)
    {
            fprintf(stderr, "error: '%s' failed to save into closure %d)\n", this->id->name, current_symbol_table->sequence);
            exit(1);
    }
*/
}

ProcedureCall::ProcedureCall(Id *id, forward_list<ProcedureCallArgument*>* arguments) : id(id), arguments(arguments)
{
}

typedef forward_list<Id*>::iterator fdaIter;
typedef forward_list<ProcedureCallArgument*>::iterator fcaIter;
typedef forward_list<Id*> fda;
typedef forward_list<ProcedureCallArgument*> fca;

void ProcedureCall::Execute()
{
    try
    {
        // Get procedure object out of current symbol table
        Object* object = current_symbol_table->Get(this->id->name);

        // Assert  it's really a procedure
        if(object->GetKind() != SymbolProcedure)
        {
            fprintf(stderr, "error: '%s' was not a procedure (closure %d)\n", this->id->name, current_symbol_table->sequence);
            exit(1);
        }

        // Create a list with all all FCD and FCA mappings
        forward_list<shared_ptr<pair<fca,fda>>> argList; // TODO: Wtf?

        /* TODO: Need Object class before this will work
        auto a = this->arguments;
        auto b = object->fVal->arguments;
        for(pair<fcaIter,fdaIter> i(a->begin(),b->begin()); 
            i.first !=  a->end() && i.second != b->end();
            ++i.first, ++i.second)
        {
            fprintf(stderr, "created new closure with arg=%s\n", (*i.second)->id->name); // TODO: Just doing this for debugging
            string argName = (*i.second)->id->name;
            shared_ptr<Symbol> argValue = make_shared<Symbol>(argName, (*i.first)->expression->Evaluate()); // TODO: Need to do this before entering closure!!!!!
            // ^ TODO:
            argList.push_front(make_shared<pair<fca,fda>>(argName,argValue));
        }
        */

        // Backup the current symbol table
        SymbolTableStateGuard guard();

        // Pull the procedure's closure
        SymbolTable* closure = object->closure;

        // Create a new closure with arguments added
        SymbolTable* closureWithArgs = new SymbolTable(*closure);

        // Pull out the FCA and FDA mappings and put them in the closure
        /* TODO: Need Objects before this will work 
        for(shared_ptr<pair<fca,fda>> argNameAndValue : argList)
        {
            string argName = argNameAndValue->second;
            int argValue = argNameAndValue->first;
            closureWithArgs->Set(argName, argValue); 
        }
        */

        current_symbol_table = closureWithArgs;

        // Execute the procedure
        //fprintf(stderr, "executing procedure '%s'\n", this->id->name); // TODO: Just doing this for debugging
        ProcedureDeclaration* assignment = object->GetProcedure();
        assignment->statements->Execute();
    }
    catch(std::out_of_range e)
    {
        fprintf(stderr, "error: use of undeclared function '%s'\n", this->id->name);
        exit(1);
    }
}

ProcedureCallArgument::ProcedureCallArgument(Expression* expression) : expression(expression)
{
}
