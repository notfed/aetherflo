%{

#include "ast.h"
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
  struct AstElement* ast;
}

%token<val> T_INT
%token<name> T_ID

%token<op> T_PLUS T_MINUS T_MULTIPLY T_DIVIDE
%token<op> T_RELOP

%token T_NEWLINE

%left '+' '-'
%left '*' '/'
%nonassoc '<' '>' T_LE T_GE T_EQ T_NE

%type<ast> stmts stmt print assn cond expr factor line

%start program

%%

program : stmts        { execute($1); }

stmts   : line stmts   { $$ = makeStatement($1,$2); }
	| /* e */      { $$ = makeEmpty(); }
        ;
line    : stmt T_NEWLINE { $$ = $1; }
	;
stmt	: assn 
        | cond
        | print     
        ;
print   : '&' expr       { $$ = makePrint($2); }
	;
assn    : '{' expr '}' '|' ':' T_ID   { $$ = makeAssignment(makeId($6),$2); }
	;
cond    : '(' expr ')' '?' '?' '(' stmt ')' { $$ = makeConditional($2,$7); }
	;

expr    : expr '<' expr { $$ = makeExp($1,$3, makeOp("<")); }
     	| expr '>' expr { $$ = makeExp($1,$3, makeOp(">")); }
     	| expr T_EQ expr { $$ = makeExp($1,$3, makeOp("==")); }
     	| expr T_NE expr { $$ = makeExp($1,$3, makeOp("!=")); }
     	| expr T_LE expr { $$ = makeExp($1,$3, makeOp("<=")); }
     	| expr T_GE expr { $$ = makeExp($1,$3, makeOp(">=")); }
     	| expr '+' expr { $$ = makeExp($1,$3, makeOp("+")); }
     	| expr '-' expr { $$ = makeExp($1,$3, makeOp("-")); }
     	| expr '*' expr { $$ = makeExp($1,$3, makeOp("*")); }
     	| expr '/' expr { $$ = makeExp($1,$3, makeOp("/")); }
        | factor
	;

factor	: '(' expr ')' { $$ = $2; }
       	| T_INT  { $$ = makeVal($1); }
       	| T_ID   { $$ = makeId($1); }
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
