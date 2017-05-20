#ifndef ASTGEN_H
#define ASTGEN_H

struct
{
    enum {ekId, ekNumber, ekStatements, ekExpression, ekCondExpression, ekAssignment, ekConditional, ekPrint} kind;
    union
    {
            int val;
            char* name;

            struct
            {
              int count;
              struct AstElement* childStatements;
              struct AstElement* thisStatement;
            } statements;

            struct
            {
              struct AstElement *left, *right;
              char op;
            } expression;

            struct
            {
              struct AstElement *left, *right;
              char* op;
            } boolexpression;

            struct
            {
              char*name;
              struct AstElement* right;
            } assignment;

            struct
            {
              struct AstElement* condition;
              struct AstElement* statement;
            } conditonal;

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

struct AstElement* makeStatement(struct AstElement* childStatements, struct AstElement* thisStatement);
struct AstElement* makeExp(struct AstElement* left, struct AstElement* right, char* op);
struct AstElement* makeBoolExp(struct AstElement* left, char* relop, struct AstElement* right);
struct AstElement* makeAssignment(char*name, struct AstElement* right);
struct AstElement* makeConditional(struct AstElement*condition, struct AstElement* statement);
struct AstElement* makePrint(struct AstElement* expr);
#endif
