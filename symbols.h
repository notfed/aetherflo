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

/*
    class Symbol;

    class SymbolVisitor
    {
    public:
        virtual void Visit(Symbol* symbol, int val) = 0;
        virtual void Visit(Symbol* symbol, string& val) = 0;
        virtual void Visit(Symbol* symbol, Statement* val) = 0;
    };
*/

    class Symbol
    {
    public:
        //virtual void Visit(SymbolVisitor* visitor) = 0;
    };

    class SymbolInt : public Symbol
    {
    private:
        string name;
        int val;
    public:
        SymbolInt(string name, int val);
        //virtual void Visit(SymbolVisitor* visitor);
        int GetVal();
    };

    class SymbolString : public Symbol
    {
    private:
        string name;
        string val;
    public:
        SymbolString(string name, const char* val);
        //virtual void Visit(SymbolVisitor* visitor);
        string GetVal();
    };

    class SymbolProcedure : public Symbol
    {
    private:
        string name;
        Statement* procedure;
    public:
        SymbolProcedure(string name, Statement* procedure);
        //virtual void Visit(SymbolVisitor* visitor);
        Statement* GetVal();
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
