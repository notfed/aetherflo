#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

class Expression
{
private:
    Expression *left;
    Expression *right;
    Op *op;
public:
    Expression(Expression *left, Expression *right, Op* op) {
        this.left = left;
        this.right = right;
        this.op = op;
    }
};

class Statements
{
private:
    Statement *thisStatement;
    Statements *childStatements;
public:
    Statements(Statement *thisStatement, Statements *childStatements) {
        this.thisStatement = thisStatement;
        this.childStatements = childStatements;
    }
};

class Assignment : Statement
{
private:
    Id *name;
    Expression *right;
public:
    Assignment(Id * name, Expression* right) {
        this.name = name;
        this.right = right;
    }
};

class Conditional : Statement
{
private:
    Expression *condition;
    Statement *right;
public:
    Conditional(Expression* condition, Statement* right) {
        this.condition = condition;
        this.right = right;
    }
};

class Print : Statement
{
private:
    Expression *expr;
public:
    Print(Expression *expr) {
        this.expr = expr;
    }
};

class Val
{
private:
    int val;
public:
    Val(int val) {
        this.val = val;
    }
};

class Id
{
private:
    char* name;
public:
    Id(char *name) {
        this.name = strdup(name);
    }
};

class Op
{
private:
    char* symbol;
public:
    Id(char *name) {
        this.symbol = strdup(name);
    }
};
}
