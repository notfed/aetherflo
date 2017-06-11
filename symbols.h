#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "interpreter.h"
#include <unordered_map>
#include <memory>
#include <string>
#include "gc/include/gc_cpp.h"

using namespace std;
using namespace Interpreter;

namespace Symbols
{

    class SymbolTable;

    enum Type {SymbolInt,SymbolString,SymbolProcedure};

    class Object : public gc
    {
    private:
        Type kind;
        string sVal;
        int iVal;
    public:
        SymbolTable* closure; // TODO: Make private
        ProcedureDeclaration* const fVal;

        Object(string s);
        Object(int i);
        Object(ProcedureDeclaration* p, SymbolTable* closure);

        Type GetKind();
        string GetString();

        int GetInt();
        void SetInt(int val);

        ProcedureDeclaration* GetProcedure();
    };

    class SymbolTable : public gc
    {
    private:
        unordered_map<string, Object*> symbols; // TODO: Change to const char*?
        static int last_sequence; // TODO: For debugging purposes
    public:
        const int sequence; // TODO: For debugging purposes
        SymbolTable();
        SymbolTable(const SymbolTable& cloneFrom);
        Object* Get(string name);
        bool Contains(const char* name);
        void Set(string name, Object* symbol);
    };

    class SymbolTableStateGuard
    {
    private:
        SymbolTable* prev_symbol_table;
    public:
        SymbolTableStateGuard();
        ~SymbolTableStateGuard();
    };

    extern SymbolTable* global_symbol_table;
    extern SymbolTable* current_symbol_table;
}

#endif
