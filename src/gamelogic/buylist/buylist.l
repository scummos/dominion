%{

#include "buylist.tab.hpp"
#define YY_DECL int yylex(yy::parser::semantic_type* yylval, yy::parser::location_type* location)

int yycolumn = 1;

#define YY_USER_ACTION location->begin.line = location->end.line = yylineno; \
    location->begin.column = yycolumn; location->end.column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;

using token = yy::parser::token;

%}


%option noyywrap
%option nodefault
%option yylineno

%%

[ \t] ;
[\n]           { yycolumn = 1; }
buylist        { return token::BUYLIST; }
list           { return token::LIST; }
end            { return token::END; }
not            { return token::NOT; }
if             { return token::IF; }
[0-9]+         {
  yylval->emplace<int>(atoi(yytext));
  return token::INT;
}
[a-zA-Z0-9]+   {
  std::cerr << "parsed string: " << yytext << std::endl;
  yylval->emplace<std::string>(yytext);
  return token::STRING;
}
.              { return yytext[0]; }
%%

