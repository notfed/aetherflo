#include "symbols.h"
#include <stdio.h>
#include <unordered_map>
#include <memory>
#include <string>

using namespace Symbols;
using namespace std;

SymbolTable global_symbol_table(nullptr);

int SymbolInt::GetVal()
{
    return this->val;
}

string SymbolString::GetVal()
{
    return this->val;
}

Statement* SymbolProcedure::GetVal()
{
    return this->procedure;
}

/*
void SymbolInt::Visit(SymbolVisitor* visitor)
{
    visitor->Visit(this, this->val);
}

void SymbolString::Visit(SymbolVisitor* visitor)
{
    visitor->Visit(this, this->val);
}

void SymbolProcedure::Visit(SymbolVisitor* visitor)
{
    visitor->Visit(this, this->procedure);
}
*/

SymbolInt::SymbolInt(string id, int val)
{
    this->name = name;
    this->val = val;
}

SymbolString::SymbolString(string id, const char* val)
{
    this->name = name;
    this->val = val;
}

SymbolProcedure::SymbolProcedure(string id, Statement* statement)
{
    this->name = name;
    this->procedure = statement;
}

SymbolTable::SymbolTable(shared_ptr<SymbolTable> parent)
{
    this->parent = parent;
}

shared_ptr<Symbol> SymbolTable::Get(string name)
{
    SymbolTable* cur = this;
    do
    {
        auto iter = cur->symbols.find(name);
        bool found = (iter != cur->symbols.end());
        if(found) {
            return iter->second;
        } else {
            cur = cur->parent.get();
        }
    }
    while(cur != nullptr);

    fprintf(stderr,"error: reference to non-existent symbol '%s'\n", name.c_str());
    exit(1);
}

void SymbolTable::Set(string name, shared_ptr<Symbol> symbol)
{
    this->symbols[name] = symbol;
};
