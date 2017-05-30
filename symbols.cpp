#include "symbols.h"
#include <stdio.h>
#include <unordered_map>
#include <memory>
#include <string>

using namespace Symbols;
using namespace std;

SymbolTable Symbols::global_symbol_table;
SymbolTable* Symbols::current_symbol_table = &global_symbol_table;

Symbol::Symbol(string name ,int i)
{
    this->name = name;
    this->kind = SymbolInt;
    this->iVal = i;
}
Symbol::Symbol(string name, string s)
{
    this->name = name;
    this->kind = SymbolString;
    this->sVal = s;
}
Symbol::Symbol(string name, Statement* p)
{
    this->name = name;
    this->kind = SymbolProcedure;
    this->pVal = p;
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
Statement* Symbol::GetProcedure()
{
    return this->pVal;
}

SymbolTable::SymbolTable()
{
}

SymbolTable::SymbolTable(shared_ptr<SymbolTable>& parent) 
{
    // TODO: Copy parent.symbol into symbol
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
