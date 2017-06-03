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

    class SymbolTable;

    enum SymbolKind {SymbolInt,SymbolString,SymbolProcedure};

    class Object
    {
    };

    class Symbol
    {
    private:
        string name;
        SymbolKind kind;
        string sVal;
        int iVal;
    public:
        shared_ptr<SymbolTable> closure; // TODO: Make private
        FunctionAssignment* const fVal;

        Symbol(string name, string s);
        Symbol(string name, int i);
        Symbol(string name, FunctionAssignment* p, shared_ptr<SymbolTable> closure);

        string GetName();

        SymbolKind GetKind();
        string GetString();
        int GetInt();
        FunctionAssignment* GetProcedure();
    };

    class SymbolTable
    {
    private:
        unordered_map<string, shared_ptr<Symbol>> symbols;
        static int last_sequence; // TODO: For debugging purposes
    public:
        const int sequence; // TODO: For debugging purposes
        SymbolTable();
        SymbolTable(SymbolTable* cloneFrom);
        shared_ptr<Symbol> Get(string name);
        void Set(string name, shared_ptr<Symbol> symbol);
    };

    class SymbolTableStateGuard
    {
    private:
        shared_ptr<SymbolTable> prev_symbol_table;
    public:
        SymbolTableStateGuard();
        ~SymbolTableStateGuard();
    };

    extern shared_ptr<SymbolTable> global_symbol_table;
    extern shared_ptr<SymbolTable> current_symbol_table;
}

#endif
