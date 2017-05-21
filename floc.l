%option noyywrap

%{
#include <stdio.h>

#define YY_DECL int yylex()

#include "floc.tab.h"

%}

%%

[ \t]	; // ignore all whitespace
[0-9]+		{ yylval.val = atoi(yytext); return T_INT;}
\n		{return T_NEWLINE;}
"+"		{return T_PLUS;}
"-"		{return T_MINUS;}
"*"		{return T_MULTIPLY;}
"/"		{return T_DIVIDE;}
"("		{return T_LPAREN;}
")"		{return T_RPAREN;}
"{"             {return T_LBRACE;}
"}"             {return T_RBRACE;}
"|"             {return T_PIPE;}
":"             {return T_COLON;}
"?"             {return T_QUESTION;}
"&"             {return T_AMPERSAND;}
(<|>|==|!=)     {return T_RELOP;}
[a-zA-Z]+       { yylval.name = yytext; return T_ID;}

%%
