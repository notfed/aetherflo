#include "symbols.h"
#include <stdio.h>
#include <unordered_map>

using namespace Symbols;

static SymbolTable global_symbols(nullptr);

void SymbolInt::Visit(SymbolVisitor* visitor)
{
    visitor->Visit(this->val);
}

void SymbolString::Visit(SymbolVisitor* visitor)
{
    visitor->Visit(this->val);
}

void SymbolProcedure::Visit(SymbolVisitor* visitor)
{
    visitor->Visit(this->val);
}

SymbolInt::SymbolInt(Id* id, int val)
{
    this->id = id;
    this->val = val;
}

SymbolString::SymbolString(Id* id, const char* val)
{
    this->id = id;
    this->val = val;
}

SymbolProcedure::SymbolProcedure(Id* id, Statement* statement)
{
    this->id = id;
    this->statement = statement;
}

SymbolTable::SymbolTable(shared_ptr<SymbolTable> parent)
{
    this->parent = parent;
}

shared_ptr<Symbol> SymbolTable::Find(Id id)
{
    SymbolTable* cur = this;
    do
    {
        auto iter = cur->symbols.find(id->name);
        bool found == (iter != cur->symbols.end());
        if(found) {
            return iter->second;
        } else {
            cur = cur->parent;
        }
    }
    while(cur != nullptr);

    fprintf(stderr,"error: reference to non-existent symbol '%s'");
    exit(1);
}
