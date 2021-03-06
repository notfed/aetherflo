#include "symbols.h"
#include <stdio.h>
#include <unordered_map>
#include <memory>
#include <string>
#include "gc/include/gc_cpp.h"
#include "log/minlog.h"

using namespace Symbols;
using namespace std;

int SymbolTable::last_sequence = 0;

SymbolTable* Symbols::global_symbol_table = new SymbolTable();
SymbolTable* Symbols::current_symbol_table = Symbols::global_symbol_table;

Object::Object(int i) : kind(SymbolInt), iVal(i), fVal(nullptr)
{
}

Object::Object(string s) : kind(SymbolString), sVal(s), fVal(nullptr)
{
}

Object::Object(ProcedureDeclaration* p, SymbolTable* closure) : kind(SymbolProcedure), fVal(p), closure(closure)
{
}

Type Object::GetKind()
{
    return this->kind;
}

string Object::GetString()
{
    return this->sVal;
}

int Object::GetInt()
{
    return this->iVal;
}

void Object::SetInt(int val)
{
    iVal = val;
}

ProcedureDeclaration* Object::GetProcedure()
{
    return this->fVal;
}

SymbolTable::SymbolTable() : sequence(++last_sequence)
{
}

SymbolTable::SymbolTable(const SymbolTable& cloneFrom) : sequence(++last_sequence)
{

    this->symbols = cloneFrom.symbols;
    minlog::debug("Copied closure %d to %d (size %d->%d)\n", cloneFrom.sequence, this->sequence, (int)cloneFrom.symbols.size(), (int)this->symbols.size());
}

Object* SymbolTable::Get(string name)
{
    auto iter = this->symbols.find(name);
    bool found = (iter != this->symbols.end());
    if(found) {
        return iter->second;
    } else {
        minlog::fatal("error: reference to non-existent symbol '%s'\n", name.c_str());
        exit(1);
    }
}

void SymbolTable::Set(string name, Object* object)
{
    this->symbols[name] = object;
}

bool SymbolTable::Contains(const char* name)
{
  auto iter = this->symbols.find(name);
  bool found = (iter != this->symbols.end());
  return found;
}

SymbolTableStateGuard::SymbolTableStateGuard()
{
    this->prev_symbol_table = Symbols::current_symbol_table;
}

SymbolTableStateGuard::~SymbolTableStateGuard()
{
    minlog::debug("Switching back to closure %d\n", this->prev_symbol_table->sequence);
    Symbols::current_symbol_table = this->prev_symbol_table;
}
