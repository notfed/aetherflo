#ifndef INTERPRETER_H
#define INTERPRETER_H

namespace Interpreter
{
    class Evaluatable 
    {
    public:
        virtual int Evaluate() = 0;
    };

    class Val : Evaluatable
    {
    private:
        int val;
    public:
        Val(int val);
    };
    
    class Id : Evaluatable
    {
    private:
        char* name;
    public:
        Id(char *name);
    };
    
    class Op
    {
    private:
        char* symbol;
    public:
        Op(char *symbol);
    };

    class Expression : public Evaluatable
    {
    private:
        Evaluatable *left;
        Evaluatable *right;
        Op *op;
    public:
        Expression(Evaluatable *left, Evaluatable *right, Op* op);
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
    };

    class EmptyStatement : public Statement
    {
    };
    
    class Assignment : public Statement
    {
    private:
        Id *name;
        Expression *right;
    public:
        Assignment(Id * name, Expression* right);
    };
    
    class Conditional : public Statement
    {
    private:
        Expression *condition;
        Statement *right;
    public:
        Conditional(Expression* condition, Statement* right);
    };
    
    class Print : public Statement
    {
    private:
        Expression *expr;
    public:
        Print(Expression *expr);
    };
}

#endif
