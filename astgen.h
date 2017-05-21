#ifndef ASTGEN_H
#define ASTGEN_H

typedef struct AstElement
{
    enum {ekEmpty, ekId, ekVal, ekOp, ekStatements, ekExpression, ekBoolExpression, ekAssignment, ekConditional, ekPrint} kind;
    union
    {
            int val;
            char* name;
            char* symbol;

            struct
            {
              struct AstElement* childStatements;
              struct AstElement* thisStatement;
            } statements;

            struct
            {
              struct AstElement *left, *right;
              struct AstElement *op;
            } expression;

            struct
            {
              struct AstElement *left, *right;
              struct AstElement* relop;
            } boolexpression;

            struct
            {
              struct AstElement* name;
              struct AstElement* right;
            } assignment;

            struct
            {
              struct AstElement* condition;
              struct AstElement* statement;
            } conditional;

            struct
            {
              struct AstElement* expr;
            } print;

        } data;
} AstElement;

void execute(struct AstElement* ast);

struct AstElement* makeEmpty();
struct AstElement* makeVal(int val);
struct AstElement* makeId(char *name);
struct AstElement* makeOp(char *symbol);

struct AstElement* makeStatement(struct AstElement* childStatements, struct AstElement* thisStatement);
struct AstElement* makeExp(struct AstElement* left, struct AstElement* right, struct AstElement* op);
struct AstElement* makeBoolExp(struct AstElement* left, struct AstElement* relop, struct AstElement* right);
struct AstElement* makeAssignment(struct AstElement* name, struct AstElement* right);
struct AstElement* makeConditional(struct AstElement*condition, struct AstElement* statement);
struct AstElement* makePrint(struct AstElement* expr);
#endif
