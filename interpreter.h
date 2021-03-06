#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <forward_list>

namespace Interpreter
{

    class Expression 
    {
    public:
        virtual int Evaluate() = 0;
    };

    class Val : public Expression
    {
    public:
        const int val;
        Val(int val);
        virtual int Evaluate();
    };
    
    class Id : public Expression
    {
    public:
        const char* const name;
        Id(const char *name);
        virtual int Evaluate();
        void Assign(int val);
    };
    
    class Op
    {
    public:
        const char* const glyph;
        Op(const char *glyph);
    };

    class ExpressionNode : public Expression
    {
    private:
        Expression *left;
        Expression *right;
        Op *op;
    public:
        ExpressionNode(Expression *left, Expression *right, Op* op);
        virtual int Evaluate();
    };

    class Statement
    {
    public:
        virtual void Execute() = 0;
    };

    class Statements : public Statement
    {
    private:
        Statement *statement;
        Statements *childStatements;
    public:
        Statements(Statement *statement, Statements *childStatements);
        virtual void Execute();
    };

    class Assignment : public Statement
    {
    private:
        Id *id;
        Expression *right;
    public:
        Assignment(Id *id, Expression* right);
        virtual void Execute();
    };
    
    class Conditional : public Statement
    {
    private:
        Expression *condition;
        Statement *statement;
    public:
        Conditional(Expression *condition, Statement *statement);
        virtual void Execute();
    };
    
    class Print : public Statement
    {
    private:
        Expression *expr;
    public:
        Print(Expression *expr);
        virtual void Execute();
    };

    struct ProcedureCallArgument
    {  
        ProcedureCallArgument(Expression* expression);
        Expression* const expression;
    };

    class ProcedureDeclaration : public Statement
    {
    public:
        Id* const id;
        Statements* const statements;
        std::forward_list<Id*>* const arguments;

        ProcedureDeclaration(Id *id, std::forward_list<Id*>* arguments, Statements *statements);
        virtual void Execute();
    };

    class ProcedureCall : public Statement
    {
    private:
        std::forward_list<ProcedureCallArgument*>* arguments;
        Id* id;
    public:
        ProcedureCall(Id *id, std::forward_list<ProcedureCallArgument*>* arguments);
        virtual void Execute();
    };

}

#endif
