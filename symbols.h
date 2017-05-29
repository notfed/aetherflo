#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "interpreter.h"
#include <unordered_map>
#include <memory>
#include <string>

using namespace std;
using namespace Interpreter;

namespace Symbols
{

    enum SymbolKind {SymbolInt,SymbolString,SymbolProcedure};

    class Symbol
    {
    private:
        string name;
        SymbolKind kind;
        string sVal;
        union
        {
            int iVal;
            Statement* pVal;
        };
    public:
        Symbol(string name, string s);
        Symbol(string name, int i);
        Symbol(string name, Statement* p);

        string GetName();

        SymbolKind GetKind();
        string GetString();
        int GetInt();
        Statement* GetProcedure();
    };

    class SymbolTable
    {
    private:
        shared_ptr<SymbolTable> parent;
        unordered_map<string, shared_ptr<Symbol>> symbols;
    public:
        SymbolTable(shared_ptr<SymbolTable> parent);
        shared_ptr<Symbol> Get(string name);
        void Set(string name, shared_ptr<Symbol> symbol);
    };

    extern SymbolTable global_symbol_table;

}

#endif
