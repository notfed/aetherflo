#include "symbols.h"
#include <stdio.h>
#include <unordered_map>
#include <memory>
#include <string>

using namespace Symbols;
using namespace std;

int SymbolTable::last_sequence = 0; // TODO: For debugging

shared_ptr<SymbolTable> Symbols::global_symbol_table = make_shared<SymbolTable>();
shared_ptr<SymbolTable> Symbols::current_symbol_table(Symbols::global_symbol_table);

Symbol::Symbol(string name ,int i) : name(name), kind(SymbolInt), iVal(i), fVal(nullptr)
{
}

Symbol::Symbol(string name, string s) : name(name), kind(SymbolString), sVal(s), fVal(nullptr)
{
}

Symbol::Symbol(string name, FunctionAssignment* p, shared_ptr<SymbolTable> closure) : name(name), kind(SymbolProcedure), fVal(p), closure(closure)
{
}

string Symbol::GetName()
{
    return this->name;
}

SymbolKind Symbol::GetKind()
{
    return this->kind;
}

string Symbol::GetString()
{
    return this->sVal;
}

int Symbol::GetInt()
{
    return this->iVal;
}

FunctionAssignment* Symbol::GetProcedure()
{
    return this->fVal;
}

SymbolTable::SymbolTable() : sequence(++last_sequence)
{
}

SymbolTable::SymbolTable(SymbolTable* cloneFrom) : sequence(++last_sequence)
{
    for(auto it : cloneFrom->symbols)
    {
        symbols[it.first] = it.second;
    }
}

shared_ptr<Symbol> SymbolTable::Get(string name)
{
    auto iter = this->symbols.find(name);
    bool found = (iter != this->symbols.end());
    if(found) {
        return iter->second;
    } else {
        fprintf(stderr,"error: reference to non-existent symbol '%s'\n", name.c_str());
        exit(1);
    }
}

void SymbolTable::Set(string name, shared_ptr<Symbol> symbol)
{
    this->symbols[name] = symbol;
}

SymbolTableStateGuard::SymbolTableStateGuard()
{
    this->prev_symbol_table = Symbols::current_symbol_table;
}

SymbolTableStateGuard::~SymbolTableStateGuard()
{
    Symbols::current_symbol_table = this->prev_symbol_table;
}
