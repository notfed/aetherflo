%{

#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern int yyparse();
extern FILE* yyin;

void yyerror(const char* s);

%}

%union {
  int ival;
  int sval;
}



%token T_ID
%token<ival> T_INT

%token T_AMPERSAND
%token T_LBRACE T_RBRACE
%token T_PIPE T_COLON T_QUESTION T_AFTERCOND
%token T_PLUS T_MINUS T_MULTIPLY T_DIVIDE T_LPAREN T_RPAREN
%token T_NEWLINE T_QUIT
%token T_RELOP
%left T_PLUS T_MINUS
%left T_MULTIPLY T_DIVIDE

%type<ival> expr
%type<ival> factor
%type<ival> term
%type<bool> condexpr

%start stmts

%%

stmts   : line stmts
	| /* e */    
        ;
line    : stmt T_NEWLINE
	;
stmt	: assn 
        | cond 
        | print        
        ;
print   : T_AMPERSAND T_ID            { printf("print\n"); }
	;
assn    : T_LBRACE expr T_RBRACE T_PIPE T_COLON T_ID   { printf("stored %d\n",$2); }
	;
cond    : T_LPAREN condexpr T_RPAREN T_QUESTION T_QUESTION T_LPAREN stmt T_RPAREN T_PIPE T_AFTERCOND { return 1; }
	;
condexpr : T_ID T_RELOP T_ID    { return 0; }
	 | T_ID T_RELOP T_INT   { return 0; }
         | T_INT T_RELOP T_ID   { return 0; }
         ;
expr	: expr '+' term		{ $$ = $1 + $3; printf("x"); }
     	| term
	;
term	: term '*' factor 	{ $$ = $1 * $3; }
     	| factor
	;
factor	: T_LPAREN expr T_RPAREN		{ printf("factor\n"); $$ = $2; }
       	| T_INT
	;
%%

int main() {
	yyin = stdin;

	do { 
		yyparse();
	} while(!feof(yyin));
	return 0;
}

void yyerror(const char* s) {
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}
