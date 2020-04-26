%{
    #define YYERROR_VERBOSE
    #include <cstdio>
    #include <iostream>
    using namespace std;

    // stuff from flex that bison needs to know about:
    extern "C" int yylex();
    extern "C" int yyparse();
    extern "C" int parse_buylist(char const* fn);
    extern int yylineno;
    extern FILE *yyin;

    void yyerror(const char *s);
%}

%locations

%union {
  int ival;
  char *sval;
}

// define the constant-string tokens:
%token BUYLIST LIST END COMMENT LPAREN RPAREN NOT IF COMMA

// define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:
%token <ival> INT
%token <sval> STRING

%%

buylist:
  buylist_section {
      cout << "done" << endl;
    }
  ;
buylist_section:
  BUYLIST lists END
  ;
lists:
  lists list
  | list
  ;
list:
  LIST list_entries END
  | LIST condition list_entries END
  ;
list_entries:
  list_entries list_separator list_entry
  | list_entry
  | %empty
  ;
list_separator:
  ',' | '\n'
  ;
list_entry:
  card_name
  | card_name condition
  ;
card_name:
  STRING {
    cout << "card name:" << $1 << std::endl;
  }
  ;
condition:
  IF condition_expr
  | IF NOT condition_expr
  ;
condition_expr:
  funccall
  ;
funccall:
  STRING '(' func_arglist ')'
  ;
func_arglist:
  expr ',' func_arglist
  | expr
  ;
expr:
  constant
  | funccall
  ;
constant:
  INT
  | cardname
  | cardname_query
  ;
cardname:
  STRING
  ;
cardname_query:
  STRING ':' cardname
%%

int parse_buylist(char const* fn) {

  // open a file handle to a particular file:
  FILE *myfile = fopen(fn, "r");
  // make sure it's valid:
  if (!myfile) {
    cout << "error opening file" << endl;
    return -1;
  }
  // Set flex to read from it instead of defaulting to STDIN:
  yyin = myfile;

  // Parse through the input:
  int ret = yyparse();
  fclose(myfile);
  return ret;
}

void yyerror(const char *s) {
  cout << "Parse error in line " << yylineno << ": " << s << endl;
  exit(-1);
}
