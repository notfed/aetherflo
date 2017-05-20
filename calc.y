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

%token T_PLUS T_MINUS T_MULTIPLY T_DIVIDE T_LEFT T_RIGHT
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
line    : stmt '\n'   
	;
stmt	: assn 
        | cond 
        | print        
        ;
print   : '&' T_ID            { printf("print\n"); }
	;
assn    : '{' expr '}' '|' ':' T_ID   { printf("stored %d\n",$2); }
	;
cond    : '(' condexpr ')' '?' '?' '(' stmt ')' '|' '>' '>' { return 1; }
	;
condexpr : T_ID T_RELOP T_ID
	 | T_ID T_RELOP T_INT
         | T_INT T_RELOP T_ID
         ;
expr	: expr '+' term		{ $$ = $1 + $3; }
     	| term
	;
term	: term '*' factor 	{ $$ = $1 * $3; }
     	| factor
	;
factor	: '(' expr ')'		{ $$ = $2; }
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
