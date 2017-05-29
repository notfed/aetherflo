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
    private:
        int val;
    public:
        Val(int val);
        virtual int Evaluate();
    };
    
    class Id : public Expression
    {
        friend class FunctionAssignment;
        friend class FunctionCall;
    private:
        const char* name;
    public:
        Id(const char *name);
        virtual int Evaluate();
        void Assign(int val);
        const char* GetName();
    };
    
    class Op
    {
    private:
        friend class ExpressionNode;
        const char* symbol;
    public:
        Op(const char *symbol);
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

    struct FuncDefArgument
    {  
        FuncDefArgument(Id* id);
        Id* id;
    };

    struct FuncCallArgument
    {  
        FuncCallArgument(Expression* expression);
        Expression* expression;
    };

    class FunctionAssignment : public Statement
    {
    private:
        Id *id;
        std::forward_list<FuncDefArgument*>* arguments;
        Statements *statements;
    public:
        FunctionAssignment(Id *id, std::forward_list<FuncDefArgument*>* arguments, Statements *statements);
        virtual void Execute();
    };

    class FunctionCall : public Statement
    {
    private:
        std::forward_list<FuncCallArgument*>* arguments;
        Id* id;
    public:
        FunctionCall(Id *id, std::forward_list<FuncCallArgument*>* arguments);
        virtual void Execute();
    };

}

#endif
