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
buylists       { return token::BUYLISTS; }
list           { return token::LIST; }
strategies     { return token::STRATEGIES; }
strategy       { return token::STRATEGY; }
playorder      { return token::PLAYORDER; }
end            { return token::END; }
not            { return token::NOT; }
if             { return token::IF; }
options        { return token::OPTIONS; }
for            { return token::FOR; }
choose         { return token::CHOOSE; }
option         { return token::OPTION; }
ignore         { return token::IGNORE; }
optional       { return token::OPTIONAL; }
->             { return token::ARROW; }
[0-9]+         {
  yylval->emplace<int>(atoi(yytext));
  return token::INT;
}
[a-zA-Z0-9]+   {
  yylval->emplace<std::string>(yytext);
  return token::STRING;
}
.              { return yytext[0]; }
%%

