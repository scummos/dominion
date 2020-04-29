%{
#include "buylist.tab.hpp"
#define YY_DECL int yylex(yy::parser::semantic_type* yylval, yy::parser::location_type* location)

using token = yy::parser::token;
%}
%option noyywrap
%option nodefault
%option yylineno

%%

[ \t\n] ;
buylist        { return token::BUYLIST; }
list           { return token::LIST; }
end            { return token::END; }
not            { return token::NOT; }
if             { return token::IF; }
[0-9]+         {
    yylval->build<int>(atoi(yytext));
    return token::INT;
}
[a-zA-Z0-9]+   {
  yylval->build<std::string>(yytext);
  return token::STRING;
}
.              { return yytext[0]; }
%%

