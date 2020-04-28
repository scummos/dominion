%{
#include "buylist.tab.hpp"
#define YY_DECL extern "C" int yylex()
%}
%option noyywrap
%option nodefault
%option yylineno

%%
[ \t\n] ;
buylist        { return BUYLIST; }
list           { return LIST; }
end            { return END; }
not            { return NOT; }
if             { return IF; }
[0-9]+         { yylval.ival = atoi(yytext); return INT; }
[a-zA-Z0-9]+   {
  yylval.sval = strdup(yytext);
  return STRING;
}
.              { return yytext[0]; }
%%

