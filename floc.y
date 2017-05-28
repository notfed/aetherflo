%{

#include "interpreter.h"
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern int yyparse();
extern FILE* yyin;
extern int yylineno;

void yyerror(const char* s);

int yydebug=1;

#define YYPARSE_PARAM astDest

%}

%union {
  char* name;
  char* op;
  int val;
  Statements* statements_node;
  Statement* statement_node;
  Evaluatable* evaluatable_node;
}

%token<val> T_INT
%token<name> T_ID

%token<op> T_PLUS T_MINUS T_MULTIPLY T_DIVIDE
%token<op> T_RELOP

%token T_NEWLINE

%left '+' '-'
%left '*' '/'
%nonassoc '<' '>' T_LE T_GE T_EQ T_NE

%type<statements_node> stmts
%type<statement_node> stmt print assn cond line
%type<evaluatable_node> expr factor

%start program

%%

program : stmts        { $1->Execute(); }

stmts   : line stmts   { $$ = new Statements($1,$2); }
	| /* e */      { $$ = new EmptyStatement(); }
        ;
line    : stmt T_NEWLINE { $$ = $1; }
	;
stmt	: assn 
        | cond
        | print     
        ;
print   : '&' expr       { $$ = new Print($2); }
	;
assn    : '{' expr '}' '|' ':' T_ID   { $$ = new Assignment(makeId($6),$2); }
	;
cond    : '(' expr ')' '?' '?' '(' stmt ')' { $$ = new Conditional($2,$7); }
	;

expr    : expr '<' expr { $$ = new Expression($1,$3, new Op("<")); }
     	| expr '>' expr { $$ = new Expression($1,$3, new Op(">")); }
     	| expr T_EQ expr { $$ = new Expression($1,$3, new Op("==")); }
     	| expr T_NE expr { $$ = new Expression($1,$3, new Op("!=")); }
     	| expr T_LE expr { $$ = new Expression($1,$3, new Op("<=")); }
     	| expr T_GE expr { $$ = new Expression($1,$3, new Op(">=")); }
     	| expr '+' expr { $$ = new Expression($1,$3, new Op("+")); }
     	| expr '-' expr { $$ = new Expression($1,$3, new Op("-")); }
     	| expr '*' expr { $$ = new Expression($1,$3, new Op("*")); }
     	| expr '/' expr { $$ = new Expression($1,$3, new Op("/")); }
        | factor
	;

factor	: '(' expr ')' { $$ = $2; }
       	| T_INT  { $$ = new Val($1); }
       	| T_ID   { $$ = new Id($1); }
	;
%%

extern int yy_flex_debug;
int main() {
    yy_flex_debug = 0;
    yyparse();
    return 0;
}

void yyerror(const char* s) {
    fprintf(stderr, "error: %s: line %d\n", s, yylineno);
    exit(1);
}
