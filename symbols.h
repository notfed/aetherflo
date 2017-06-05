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

    enum Type {SymbolInt,SymbolString,SymbolProcedure};

    class Object
    {
    private:
        Type kind;
        string sVal;
        int iVal;
    public:
        shared_ptr<SymbolTable> closure; // TODO: Make private
        ProcedureDeclaration* const fVal;

        Object(string s);
        Object(int i);
        Object(ProcedureDeclaration* p, shared_ptr<SymbolTable> closure);

        Type GetKind();
        string GetString();
        int GetInt();
        ProcedureDeclaration* GetProcedure();
    };

    class SymbolTable
    {
    private:
        unordered_map<string, shared_ptr<Object>> symbols;
        static int last_sequence; // TODO: For debugging purposes
    public:
        const int sequence; // TODO: For debugging purposes
        SymbolTable();
        SymbolTable(const SymbolTable& cloneFrom);
        shared_ptr<Object> Get(string name);
        void Set(string name, shared_ptr<Object> symbol);
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
