#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

namespace Ast
{
    class Val
    {
    private:
        int val;
    public:
        inline Val(int val) {
            this.val = val;
        }
    };
    
    class Id
    {
    private:
        char* name;
    public:
        inline Id(char *name) {
            this.name = strdup(name);
        }
    };
    
    class Op
    {
    private:
        char* symbol;
    public:
        inline Id(char *name) {
            this.symbol = strdup(name);
        }
    };

    class Expression
    {
    private:
        Expression *left;
        Expression *right;
        Op *op;
    public:
        inline Expression(Expression *left, Expression *right, Op* op) {
            this.left = left;
            this.right = right;
            this.op = op;
        }
        int Evaluate();
    };

    class Statement
    {
    public:
        virtual void Execute() = 0;
    };

    class Statements
    {
    private:
        Statement *thisStatement;
        Statements *childStatements;
    public:
        inline Statements(Statement *thisStatement, Statements *childStatements) {
            this.thisStatement = thisStatement;
            this.childStatements = childStatements;
        }
    };
    
    class Assignment : public Statement
    {
    private:
        Id *name;
        Expression *right;
    public:
        inline Assignment(Id * name, Expression* right) {
            this.name = name;
            this.right = right;
        }
    };
    
    class Conditional : public Statement
    {
    private:
        Expression *condition;
        Statement *right;
    public:
        inline Conditional(Expression* condition, Statement* right) {
            this.condition = condition;
            this.right = right;
        }
    };
    
    class Print : public Statement
    {
    private:
        Expression *expr;
    public:
        inline Print(Expression *expr) {
            this.expr = expr;
        }
    };
    
}
