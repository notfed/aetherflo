#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "symbols.h"
#include "interpreter.h"
#include <memory>
#include <unordered_map>

namespace Symbols
{
    class SymbolVisitor
    {
    public:
        virtual void Visit(Symbol* symbol, int val) = 0;
        virtual void Visit(Symbol* symbol, const char *val) = 0;
        virtual void Visit(Symbol* symbol, Statement *val) = 0;
    };

    class Symbol
    {
    public:
        virtual void Visit(SymbolVisitor* visitor) = 0;
    };

    class SymbolInt : public Symbol
    {
    private:
        Id* id;
        int val;
    public:
        SymbolInt(Id* id, int val);
        virtual void Visit(SymbolVisitor* visitor);
    };

    class SymbolString : public Symbol
    {
    private:
        Id* id;
        const char* val;
    public:
        SymbolString(Id* id, const char* val);
        virtual void Visit(SymbolVisitor* visitor);
    };

    class SymbolProcedure : public Symbol
    {
    private:
        Id* id;
        Statement *procedure;
    public:
        SymbolString(Id* id, Statement* procedure);
        virtual void Visit(SymbolVisitor* visitor);
    };

    class SymbolTable
    {
    private:
        shared_ptr<SymbolTable> parent;
        std::unordered_map<std::string, shared_ptr<Symbol>> symbols;
    public:
        SymbolTable(shared_ptr<SymbolTable> parent);
        shared_ptr<Symbol> Find(Id id);
    };
}

#endif
