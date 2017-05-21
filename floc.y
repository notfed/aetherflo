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

%token T_AMPERSAND
%token T_LBRACE T_RBRACE
%token T_LPAREN T_RPAREN
%token T_PIPE T_COLON T_QUESTION T_AFTERCOND
%token T_NEWLINE T_QUIT
%left T_PLUS T_MINUS
%left T_MULTIPLY T_DIVIDE

%type<ast> stmts stmt print assn cond expr term factor boolexpr line

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
print   : T_AMPERSAND expr            { $$ = makePrint($2); }
	;
assn    : T_LBRACE expr T_RBRACE T_PIPE T_COLON T_ID   { $$ = makeAssignment(makeId($6),$2); }
	;
cond    : T_LPAREN boolexpr T_RPAREN T_QUESTION T_QUESTION T_LPAREN stmt T_RPAREN { $$ = makeConditional($2,$7); }
	;

boolexpr : expr T_RELOP expr { $$ = makeBoolExp($1,makeOp($2),$3); }
         ;

expr	: expr '+' term		{ $$ = makeExp($1,$3, makeOp("+")); }
     	| term
	;
term	: term '*' factor 	{ $$ = makeExp($1,$3,makeOp("*")); }
     	| factor
	;
factor	: T_LPAREN expr T_RPAREN  { $$ = $2; }
       	| T_INT  { $$ = makeVal($1); }
       	| T_ID   { $$ = makeId($1); }
	;
%%

extern int yy_flex_debug;
int main() {
        yy_flex_debug = 0;
	yyin = stdin;

	do { 
		yyparse();
	} while(!feof(yyin));
	return 0;
}

void yyerror(const char* s) {
	fprintf(stderr, "error: %s: line %d\n", s, yylineno);
	exit(1);
}
