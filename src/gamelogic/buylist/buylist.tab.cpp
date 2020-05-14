// A Bison parser, made by GNU Bison 3.5.3.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.



// First part of user prologue.
#line 10 "buylist.ypp"

    #define YYERROR_VERBOSE
    #include <cstdio>
    #include <iostream>
    #include "buylist.h"
    #include "buylist.tab.hpp"
    using namespace std;

    typedef struct yy_buffer_state *YY_BUFFER_STATE;
    extern YY_BUFFER_STATE yy_scan_string(char const* base);
    extern void yyrestart(FILE *input_file);
    extern int yycolumn, yylineno;
    extern void yy_delete_buffer(yy_buffer_state*);
    extern int yylex(yy::parser::semantic_type*, yy::parser::location_type*);
    extern "C" {
        extern FILE *yyin;
    }

    extern int parse_buylist(char* fn, BuylistCollection& ret, StrategyCollection& strats);

#line 61 "buylist.tab.cpp"


#include "buylist.tab.hpp"




#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 157 "buylist.tab.cpp"


  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  parser::parser (BuylistCollection& ret_yyarg, StrategyCollection& strats_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      yy_lac_established_ (false),
      ret (ret_yyarg),
      strats (strats_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/

  // basic_symbol.
#if 201103L <= YY_CPLUSPLUS
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (basic_symbol&& that)
    : Base (std::move (that))
    , value ()
    , location (std::move (that.location))
  {
    switch (this->type_get ())
    {
      case 38: // list_entry
        value.move< BuyAction > (std::move (that.value));
        break;

      case 36: // list
        value.move< Buylist > (std::move (that.value));
        break;

      case 54: // card_name
        value.move< CardId > (std::move (that.value));
        break;

      case 51: // condition
      case 53: // condition_expr
        value.move< Condition::Ptr > (std::move (that.value));
        break;

      case 44: // cardoption
        value.move< GenericOption > (std::move (that.value));
        break;

      case 45: // cardoptions
        value.move< GenericOptions > (std::move (that.value));
        break;

      case 43: // playorder
        value.move< Playorder > (std::move (that.value));
        break;

      case 55: // reactoption_name
        value.move< ReactKind > (std::move (that.value));
        break;

      case 40: // strat
      case 41: // strat_body
        value.move< Strategy > (std::move (that.value));
        break;

      case 48: // cardoption_tagged_expr
        value.move< TaggedExpr > (std::move (that.value));
        break;

      case 26: // INT
        value.move< int > (std::move (that.value));
        break;

      case 46: // cardoption_expr
      case 47: // cardoption_expr_value
      case 56: // expr
      case 57: // constant
        value.move< std::any > (std::move (that.value));
        break;

      case 27: // STRING
        value.move< std::string > (std::move (that.value));
        break;

      case 37: // list_entries
        value.move< std::vector<BuyAction> > (std::move (that.value));
        break;

      case 35: // lists
        value.move< std::vector<Buylist> > (std::move (that.value));
        break;

      case 42: // playorders
        value.move< std::vector<Playorder> > (std::move (that.value));
        break;

      case 39: // strats
        value.move< std::vector<Strategy> > (std::move (that.value));
        break;

      case 49: // cardoption_tagged_expr_list
        value.move< std::vector<TaggedExpr> > (std::move (that.value));
        break;

      case 50: // cardoption_expr_list
      case 52: // func_arglist
        value.move< std::vector<std::any> > (std::move (that.value));
        break;

      default:
        break;
    }

  }
#endif

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->type_get ())
    {
      case 38: // list_entry
        value.copy< BuyAction > (YY_MOVE (that.value));
        break;

      case 36: // list
        value.copy< Buylist > (YY_MOVE (that.value));
        break;

      case 54: // card_name
        value.copy< CardId > (YY_MOVE (that.value));
        break;

      case 51: // condition
      case 53: // condition_expr
        value.copy< Condition::Ptr > (YY_MOVE (that.value));
        break;

      case 44: // cardoption
        value.copy< GenericOption > (YY_MOVE (that.value));
        break;

      case 45: // cardoptions
        value.copy< GenericOptions > (YY_MOVE (that.value));
        break;

      case 43: // playorder
        value.copy< Playorder > (YY_MOVE (that.value));
        break;

      case 55: // reactoption_name
        value.copy< ReactKind > (YY_MOVE (that.value));
        break;

      case 40: // strat
      case 41: // strat_body
        value.copy< Strategy > (YY_MOVE (that.value));
        break;

      case 48: // cardoption_tagged_expr
        value.copy< TaggedExpr > (YY_MOVE (that.value));
        break;

      case 26: // INT
        value.copy< int > (YY_MOVE (that.value));
        break;

      case 46: // cardoption_expr
      case 47: // cardoption_expr_value
      case 56: // expr
      case 57: // constant
        value.copy< std::any > (YY_MOVE (that.value));
        break;

      case 27: // STRING
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      case 37: // list_entries
        value.copy< std::vector<BuyAction> > (YY_MOVE (that.value));
        break;

      case 35: // lists
        value.copy< std::vector<Buylist> > (YY_MOVE (that.value));
        break;

      case 42: // playorders
        value.copy< std::vector<Playorder> > (YY_MOVE (that.value));
        break;

      case 39: // strats
        value.copy< std::vector<Strategy> > (YY_MOVE (that.value));
        break;

      case 49: // cardoption_tagged_expr_list
        value.copy< std::vector<TaggedExpr> > (YY_MOVE (that.value));
        break;

      case 50: // cardoption_expr_list
      case 52: // func_arglist
        value.copy< std::vector<std::any> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }



  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->type_get ())
    {
      case 38: // list_entry
        value.move< BuyAction > (YY_MOVE (s.value));
        break;

      case 36: // list
        value.move< Buylist > (YY_MOVE (s.value));
        break;

      case 54: // card_name
        value.move< CardId > (YY_MOVE (s.value));
        break;

      case 51: // condition
      case 53: // condition_expr
        value.move< Condition::Ptr > (YY_MOVE (s.value));
        break;

      case 44: // cardoption
        value.move< GenericOption > (YY_MOVE (s.value));
        break;

      case 45: // cardoptions
        value.move< GenericOptions > (YY_MOVE (s.value));
        break;

      case 43: // playorder
        value.move< Playorder > (YY_MOVE (s.value));
        break;

      case 55: // reactoption_name
        value.move< ReactKind > (YY_MOVE (s.value));
        break;

      case 40: // strat
      case 41: // strat_body
        value.move< Strategy > (YY_MOVE (s.value));
        break;

      case 48: // cardoption_tagged_expr
        value.move< TaggedExpr > (YY_MOVE (s.value));
        break;

      case 26: // INT
        value.move< int > (YY_MOVE (s.value));
        break;

      case 46: // cardoption_expr
      case 47: // cardoption_expr_value
      case 56: // expr
      case 57: // constant
        value.move< std::any > (YY_MOVE (s.value));
        break;

      case 27: // STRING
        value.move< std::string > (YY_MOVE (s.value));
        break;

      case 37: // list_entries
        value.move< std::vector<BuyAction> > (YY_MOVE (s.value));
        break;

      case 35: // lists
        value.move< std::vector<Buylist> > (YY_MOVE (s.value));
        break;

      case 42: // playorders
        value.move< std::vector<Playorder> > (YY_MOVE (s.value));
        break;

      case 39: // strats
        value.move< std::vector<Strategy> > (YY_MOVE (s.value));
        break;

      case 49: // cardoption_tagged_expr_list
        value.move< std::vector<TaggedExpr> > (YY_MOVE (s.value));
        break;

      case 50: // cardoption_expr_list
      case 52: // func_arglist
        value.move< std::vector<std::any> > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_type.
  parser::by_type::by_type ()
    : type (empty_symbol)
  {}

#if 201103L <= YY_CPLUSPLUS
  parser::by_type::by_type (by_type&& that)
    : type (that.type)
  {
    that.clear ();
  }
#endif

  parser::by_type::by_type (const by_type& that)
    : type (that.type)
  {}

  parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  void
  parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  void
  parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  int
  parser::by_type::type_get () const YY_NOEXCEPT
  {
    return type;
  }


  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_number_type
  parser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[+state];
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 38: // list_entry
        value.YY_MOVE_OR_COPY< BuyAction > (YY_MOVE (that.value));
        break;

      case 36: // list
        value.YY_MOVE_OR_COPY< Buylist > (YY_MOVE (that.value));
        break;

      case 54: // card_name
        value.YY_MOVE_OR_COPY< CardId > (YY_MOVE (that.value));
        break;

      case 51: // condition
      case 53: // condition_expr
        value.YY_MOVE_OR_COPY< Condition::Ptr > (YY_MOVE (that.value));
        break;

      case 44: // cardoption
        value.YY_MOVE_OR_COPY< GenericOption > (YY_MOVE (that.value));
        break;

      case 45: // cardoptions
        value.YY_MOVE_OR_COPY< GenericOptions > (YY_MOVE (that.value));
        break;

      case 43: // playorder
        value.YY_MOVE_OR_COPY< Playorder > (YY_MOVE (that.value));
        break;

      case 55: // reactoption_name
        value.YY_MOVE_OR_COPY< ReactKind > (YY_MOVE (that.value));
        break;

      case 40: // strat
      case 41: // strat_body
        value.YY_MOVE_OR_COPY< Strategy > (YY_MOVE (that.value));
        break;

      case 48: // cardoption_tagged_expr
        value.YY_MOVE_OR_COPY< TaggedExpr > (YY_MOVE (that.value));
        break;

      case 26: // INT
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case 46: // cardoption_expr
      case 47: // cardoption_expr_value
      case 56: // expr
      case 57: // constant
        value.YY_MOVE_OR_COPY< std::any > (YY_MOVE (that.value));
        break;

      case 27: // STRING
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case 37: // list_entries
        value.YY_MOVE_OR_COPY< std::vector<BuyAction> > (YY_MOVE (that.value));
        break;

      case 35: // lists
        value.YY_MOVE_OR_COPY< std::vector<Buylist> > (YY_MOVE (that.value));
        break;

      case 42: // playorders
        value.YY_MOVE_OR_COPY< std::vector<Playorder> > (YY_MOVE (that.value));
        break;

      case 39: // strats
        value.YY_MOVE_OR_COPY< std::vector<Strategy> > (YY_MOVE (that.value));
        break;

      case 49: // cardoption_tagged_expr_list
        value.YY_MOVE_OR_COPY< std::vector<TaggedExpr> > (YY_MOVE (that.value));
        break;

      case 50: // cardoption_expr_list
      case 52: // func_arglist
        value.YY_MOVE_OR_COPY< std::vector<std::any> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 38: // list_entry
        value.move< BuyAction > (YY_MOVE (that.value));
        break;

      case 36: // list
        value.move< Buylist > (YY_MOVE (that.value));
        break;

      case 54: // card_name
        value.move< CardId > (YY_MOVE (that.value));
        break;

      case 51: // condition
      case 53: // condition_expr
        value.move< Condition::Ptr > (YY_MOVE (that.value));
        break;

      case 44: // cardoption
        value.move< GenericOption > (YY_MOVE (that.value));
        break;

      case 45: // cardoptions
        value.move< GenericOptions > (YY_MOVE (that.value));
        break;

      case 43: // playorder
        value.move< Playorder > (YY_MOVE (that.value));
        break;

      case 55: // reactoption_name
        value.move< ReactKind > (YY_MOVE (that.value));
        break;

      case 40: // strat
      case 41: // strat_body
        value.move< Strategy > (YY_MOVE (that.value));
        break;

      case 48: // cardoption_tagged_expr
        value.move< TaggedExpr > (YY_MOVE (that.value));
        break;

      case 26: // INT
        value.move< int > (YY_MOVE (that.value));
        break;

      case 46: // cardoption_expr
      case 47: // cardoption_expr_value
      case 56: // expr
      case 57: // constant
        value.move< std::any > (YY_MOVE (that.value));
        break;

      case 27: // STRING
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case 37: // list_entries
        value.move< std::vector<BuyAction> > (YY_MOVE (that.value));
        break;

      case 35: // lists
        value.move< std::vector<Buylist> > (YY_MOVE (that.value));
        break;

      case 42: // playorders
        value.move< std::vector<Playorder> > (YY_MOVE (that.value));
        break;

      case 39: // strats
        value.move< std::vector<Strategy> > (YY_MOVE (that.value));
        break;

      case 49: // cardoption_tagged_expr_list
        value.move< std::vector<TaggedExpr> > (YY_MOVE (that.value));
        break;

      case 50: // cardoption_expr_list
      case 52: // func_arglist
        value.move< std::vector<std::any> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 38: // list_entry
        value.copy< BuyAction > (that.value);
        break;

      case 36: // list
        value.copy< Buylist > (that.value);
        break;

      case 54: // card_name
        value.copy< CardId > (that.value);
        break;

      case 51: // condition
      case 53: // condition_expr
        value.copy< Condition::Ptr > (that.value);
        break;

      case 44: // cardoption
        value.copy< GenericOption > (that.value);
        break;

      case 45: // cardoptions
        value.copy< GenericOptions > (that.value);
        break;

      case 43: // playorder
        value.copy< Playorder > (that.value);
        break;

      case 55: // reactoption_name
        value.copy< ReactKind > (that.value);
        break;

      case 40: // strat
      case 41: // strat_body
        value.copy< Strategy > (that.value);
        break;

      case 48: // cardoption_tagged_expr
        value.copy< TaggedExpr > (that.value);
        break;

      case 26: // INT
        value.copy< int > (that.value);
        break;

      case 46: // cardoption_expr
      case 47: // cardoption_expr_value
      case 56: // expr
      case 57: // constant
        value.copy< std::any > (that.value);
        break;

      case 27: // STRING
        value.copy< std::string > (that.value);
        break;

      case 37: // list_entries
        value.copy< std::vector<BuyAction> > (that.value);
        break;

      case 35: // lists
        value.copy< std::vector<Buylist> > (that.value);
        break;

      case 42: // playorders
        value.copy< std::vector<Playorder> > (that.value);
        break;

      case 39: // strats
        value.copy< std::vector<Strategy> > (that.value);
        break;

      case 49: // cardoption_tagged_expr_list
        value.copy< std::vector<TaggedExpr> > (that.value);
        break;

      case 50: // cardoption_expr_list
      case 52: // func_arglist
        value.copy< std::vector<std::any> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 38: // list_entry
        value.move< BuyAction > (that.value);
        break;

      case 36: // list
        value.move< Buylist > (that.value);
        break;

      case 54: // card_name
        value.move< CardId > (that.value);
        break;

      case 51: // condition
      case 53: // condition_expr
        value.move< Condition::Ptr > (that.value);
        break;

      case 44: // cardoption
        value.move< GenericOption > (that.value);
        break;

      case 45: // cardoptions
        value.move< GenericOptions > (that.value);
        break;

      case 43: // playorder
        value.move< Playorder > (that.value);
        break;

      case 55: // reactoption_name
        value.move< ReactKind > (that.value);
        break;

      case 40: // strat
      case 41: // strat_body
        value.move< Strategy > (that.value);
        break;

      case 48: // cardoption_tagged_expr
        value.move< TaggedExpr > (that.value);
        break;

      case 26: // INT
        value.move< int > (that.value);
        break;

      case 46: // cardoption_expr
      case 47: // cardoption_expr_value
      case 56: // expr
      case 57: // constant
        value.move< std::any > (that.value);
        break;

      case 27: // STRING
        value.move< std::string > (that.value);
        break;

      case 37: // list_entries
        value.move< std::vector<BuyAction> > (that.value);
        break;

      case 35: // lists
        value.move< std::vector<Buylist> > (that.value);
        break;

      case 42: // playorders
        value.move< std::vector<Playorder> > (that.value);
        break;

      case 39: // strats
        value.move< std::vector<Strategy> > (that.value);
        break;

      case 49: // cardoption_tagged_expr_list
        value.move< std::vector<TaggedExpr> > (that.value);
        break;

      case 50: // cardoption_expr_list
      case 52: // func_arglist
        value.move< std::vector<std::any> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    /// Discard the LAC context in case there still is one left from a
    /// previous invocation.
    yy_lac_discard_ ("init");

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      {
        if (!yy_lac_establish_ (yyla.type_get ()))
           goto yyerrlab;
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        if (!yy_lac_establish_ (yyla.type_get ()))
           goto yyerrlab;

        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    yy_lac_discard_ ("shift");
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case 38: // list_entry
        yylhs.value.emplace< BuyAction > ();
        break;

      case 36: // list
        yylhs.value.emplace< Buylist > ();
        break;

      case 54: // card_name
        yylhs.value.emplace< CardId > ();
        break;

      case 51: // condition
      case 53: // condition_expr
        yylhs.value.emplace< Condition::Ptr > ();
        break;

      case 44: // cardoption
        yylhs.value.emplace< GenericOption > ();
        break;

      case 45: // cardoptions
        yylhs.value.emplace< GenericOptions > ();
        break;

      case 43: // playorder
        yylhs.value.emplace< Playorder > ();
        break;

      case 55: // reactoption_name
        yylhs.value.emplace< ReactKind > ();
        break;

      case 40: // strat
      case 41: // strat_body
        yylhs.value.emplace< Strategy > ();
        break;

      case 48: // cardoption_tagged_expr
        yylhs.value.emplace< TaggedExpr > ();
        break;

      case 26: // INT
        yylhs.value.emplace< int > ();
        break;

      case 46: // cardoption_expr
      case 47: // cardoption_expr_value
      case 56: // expr
      case 57: // constant
        yylhs.value.emplace< std::any > ();
        break;

      case 27: // STRING
        yylhs.value.emplace< std::string > ();
        break;

      case 37: // list_entries
        yylhs.value.emplace< std::vector<BuyAction> > ();
        break;

      case 35: // lists
        yylhs.value.emplace< std::vector<Buylist> > ();
        break;

      case 42: // playorders
        yylhs.value.emplace< std::vector<Playorder> > ();
        break;

      case 39: // strats
        yylhs.value.emplace< std::vector<Strategy> > ();
        break;

      case 49: // cardoption_tagged_expr_list
        yylhs.value.emplace< std::vector<TaggedExpr> > ();
        break;

      case 50: // cardoption_expr_list
      case 52: // func_arglist
        yylhs.value.emplace< std::vector<std::any> > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2:
#line 79 "buylist.ypp"
                  { }
#line 1344 "buylist.tab.cpp"
    break;

  case 3:
#line 82 "buylist.ypp"
                                           {
      ret = BuylistCollection({yystack_[4].value.as < std::vector<Buylist> > ()});
      strats = StrategyCollection({yystack_[1].value.as < std::vector<Strategy> > ()});
    }
#line 1353 "buylist.tab.cpp"
    break;

  case 4:
#line 88 "buylist.ypp"
             {
      yystack_[1].value.as < std::vector<Buylist> > ().push_back(yystack_[0].value.as < Buylist > ());
      yylhs.value.as < std::vector<Buylist> > () = yystack_[1].value.as < std::vector<Buylist> > ();
    }
#line 1362 "buylist.tab.cpp"
    break;

  case 5:
#line 92 "buylist.ypp"
         {
      yylhs.value.as < std::vector<Buylist> > () = std::vector<Buylist>{yystack_[0].value.as < Buylist > ()};
    }
#line 1370 "buylist.tab.cpp"
    break;

  case 6:
#line 97 "buylist.ypp"
                        {
      yylhs.value.as < Buylist > () = Buylist(yystack_[1].value.as < std::vector<BuyAction> > ());
    }
#line 1378 "buylist.tab.cpp"
    break;

  case 7:
#line 100 "buylist.ypp"
                                    {
      yylhs.value.as < Buylist > () = Buylist(yystack_[2].value.as < Condition::Ptr > (), yystack_[1].value.as < std::vector<BuyAction> > ());
    }
#line 1386 "buylist.tab.cpp"
    break;

  case 8:
#line 105 "buylist.ypp"
                                         {
      yystack_[2].value.as < std::vector<BuyAction> > ().push_back(yystack_[0].value.as < BuyAction > ());
      yylhs.value.as < std::vector<BuyAction> > () = yystack_[2].value.as < std::vector<BuyAction> > ();
    }
#line 1395 "buylist.tab.cpp"
    break;

  case 9:
#line 109 "buylist.ypp"
               {
      yylhs.value.as < std::vector<BuyAction> > () = {yystack_[0].value.as < BuyAction > ()};
    }
#line 1403 "buylist.tab.cpp"
    break;

  case 10:
#line 112 "buylist.ypp"
           {
      yylhs.value.as < std::vector<BuyAction> > () = {};
    }
#line 1411 "buylist.tab.cpp"
    break;

  case 13:
#line 120 "buylist.ypp"
            {
      yylhs.value.as < BuyAction > () = BuyAction(yystack_[0].value.as < CardId > ());
    }
#line 1419 "buylist.tab.cpp"
    break;

  case 14:
#line 123 "buylist.ypp"
                        {
      yylhs.value.as < BuyAction > () = BuyAction(yystack_[0].value.as < Condition::Ptr > (), yystack_[1].value.as < CardId > ());
    }
#line 1427 "buylist.tab.cpp"
    break;

  case 15:
#line 126 "buylist.ypp"
                          {
      auto cond = createCondition("Not", {createCondition("Has", {yystack_[3].value.as < CardId > (), yystack_[1].value.as < int > ()})});
      yylhs.value.as < BuyAction > () = BuyAction(cond, yystack_[3].value.as < CardId > ());
    }
#line 1436 "buylist.tab.cpp"
    break;

  case 16:
#line 130 "buylist.ypp"
                                    {
      auto cond = createCondition("AllOf", {yystack_[0].value.as < Condition::Ptr > (), createCondition("Not", {createCondition("Has", {yystack_[4].value.as < CardId > (), yystack_[2].value.as < int > ()})})});
      yylhs.value.as < BuyAction > () = BuyAction(cond, yystack_[4].value.as < CardId > ());
    }
#line 1445 "buylist.tab.cpp"
    break;

  case 17:
#line 136 "buylist.ypp"
                    {
      yylhs.value.as < Condition::Ptr > () = yystack_[0].value.as < Condition::Ptr > ();
    }
#line 1453 "buylist.tab.cpp"
    break;

  case 18:
#line 139 "buylist.ypp"
                          {
      yylhs.value.as < Condition::Ptr > () = createCondition("Not", {yystack_[0].value.as < Condition::Ptr > ()});
    }
#line 1461 "buylist.tab.cpp"
    break;

  case 19:
#line 144 "buylist.ypp"
                              {
      try {
        yylhs.value.as < Condition::Ptr > () = createCondition(yystack_[3].value.as < std::string > (), yystack_[1].value.as < std::vector<std::any> > ());
      }
      catch (InvalidConditionError e) {
        throw parser::syntax_error(yylhs.location, e.error);
      }
    }
#line 1474 "buylist.tab.cpp"
    break;

  case 20:
#line 154 "buylist.ypp"
       {
      yylhs.value.as < std::vector<std::any> > () = std::vector<std::any>{yystack_[0].value.as < std::any > ()};
    }
#line 1482 "buylist.tab.cpp"
    break;

  case 21:
#line 157 "buylist.ypp"
                          {
      yystack_[2].value.as < std::vector<std::any> > ().push_back(yystack_[0].value.as < std::any > ());
      yylhs.value.as < std::vector<std::any> > () = yystack_[2].value.as < std::vector<std::any> > ();
    }
#line 1491 "buylist.tab.cpp"
    break;

  case 22:
#line 163 "buylist.ypp"
           {
      yylhs.value.as < std::any > () = std::any(yystack_[0].value.as < std::any > ());
    }
#line 1499 "buylist.tab.cpp"
    break;

  case 23:
#line 166 "buylist.ypp"
                   {
      yylhs.value.as < std::any > () = std::any(yystack_[0].value.as < Condition::Ptr > ());
    }
#line 1507 "buylist.tab.cpp"
    break;

  case 24:
#line 171 "buylist.ypp"
      {
      yylhs.value.as < std::any > () = std::any(yystack_[0].value.as < int > ());
    }
#line 1515 "buylist.tab.cpp"
    break;

  case 25:
#line 174 "buylist.ypp"
              {
      yylhs.value.as < std::any > () = std::any(yystack_[0].value.as < CardId > ());
    }
#line 1523 "buylist.tab.cpp"
    break;

  case 26:
#line 179 "buylist.ypp"
         {
      yylhs.value.as < CardId > () = cardId(yystack_[0].value.as < std::string > ());
      if (yylhs.value.as < CardId > () == CardId::Invalid) {
        throw parser::syntax_error(yylhs.location, "Invalid card name");
      }
    }
#line 1534 "buylist.tab.cpp"
    break;

  case 27:
#line 187 "buylist.ypp"
         {
      yylhs.value.as < ReactKind > () = reactKind(yystack_[0].value.as < std::string > ());
      if (yylhs.value.as < ReactKind > () == ReactKind::Invalid) {
        throw parser::syntax_error(yylhs.location, "Invalid reaction kind");
      }
    }
#line 1545 "buylist.tab.cpp"
    break;

  case 28:
#line 195 "buylist.ypp"
               {
      yystack_[1].value.as < std::vector<Strategy> > ().push_back(yystack_[0].value.as < Strategy > ());
      yylhs.value.as < std::vector<Strategy> > () = yystack_[1].value.as < std::vector<Strategy> > ();
    }
#line 1554 "buylist.tab.cpp"
    break;

  case 29:
#line 199 "buylist.ypp"
          {
      yylhs.value.as < std::vector<Strategy> > () = std::vector<Strategy>{yystack_[0].value.as < Strategy > ()};
    }
#line 1562 "buylist.tab.cpp"
    break;

  case 30:
#line 204 "buylist.ypp"
                          {
      yylhs.value.as < Strategy > () = yystack_[1].value.as < Strategy > ();
    }
#line 1570 "buylist.tab.cpp"
    break;

  case 31:
#line 207 "buylist.ypp"
                                      {
      yystack_[1].value.as < Strategy > ().setCondition(yystack_[2].value.as < Condition::Ptr > ());
      yylhs.value.as < Strategy > () = yystack_[1].value.as < Strategy > ();
    }
#line 1579 "buylist.tab.cpp"
    break;

  case 32:
#line 213 "buylist.ypp"
                                       {
      yylhs.value.as < Strategy > () = Strategy(yystack_[2].value.as < std::vector<Playorder> > (), yystack_[0].value.as < GenericOptions > ());
    }
#line 1587 "buylist.tab.cpp"
    break;

  case 33:
#line 216 "buylist.ypp"
                             {
      yylhs.value.as < Strategy > () = Strategy(yystack_[1].value.as < std::vector<Playorder> > (), {});
    }
#line 1595 "buylist.tab.cpp"
    break;

  case 34:
#line 219 "buylist.ypp"
                {
      yylhs.value.as < Strategy > () = Strategy({}, yystack_[0].value.as < GenericOptions > ());
    }
#line 1603 "buylist.tab.cpp"
    break;

  case 35:
#line 222 "buylist.ypp"
           {
      yylhs.value.as < Strategy > () = Strategy();
    }
#line 1611 "buylist.tab.cpp"
    break;

  case 36:
#line 227 "buylist.ypp"
                           {
      yystack_[2].value.as < std::vector<Playorder> > ().push_back(yystack_[0].value.as < Playorder > ());
      yylhs.value.as < std::vector<Playorder> > () = yystack_[2].value.as < std::vector<Playorder> > ();
    }
#line 1620 "buylist.tab.cpp"
    break;

  case 37:
#line 231 "buylist.ypp"
              {
      yylhs.value.as < std::vector<Playorder> > () = std::vector<Playorder>({yystack_[0].value.as < Playorder > ()});
    }
#line 1628 "buylist.tab.cpp"
    break;

  case 38:
#line 236 "buylist.ypp"
              {
        yylhs.value.as < Playorder > () = Playorder(yystack_[0].value.as < CardId > ());
      }
#line 1636 "buylist.tab.cpp"
    break;

  case 39:
#line 239 "buylist.ypp"
                 {
        yylhs.value.as < Playorder > () = Playorder(yystack_[0].value.as < std::string > ());
      }
#line 1644 "buylist.tab.cpp"
    break;

  case 40:
#line 244 "buylist.ypp"
                         {
      yystack_[1].value.as < GenericOptions > ().push_back(yystack_[0].value.as < GenericOption > ());
      yylhs.value.as < GenericOptions > () = yystack_[1].value.as < GenericOptions > ();
    }
#line 1653 "buylist.tab.cpp"
    break;

  case 41:
#line 248 "buylist.ypp"
               {
      yylhs.value.as < GenericOptions > () = std::vector<GenericOption>{yystack_[0].value.as < GenericOption > ()};
    }
#line 1661 "buylist.tab.cpp"
    break;

  case 42:
#line 253 "buylist.ypp"
                                            {
      yylhs.value.as < GenericOption > () = GenericCardOption{yystack_[2].value.as < CardId > (), {}, yystack_[1].value.as < std::any > ()};
    }
#line 1669 "buylist.tab.cpp"
    break;

  case 43:
#line 256 "buylist.ypp"
                                                        {
      yylhs.value.as < GenericOption > () = GenericCardOption{yystack_[3].value.as < CardId > (), yystack_[2].value.as < Condition::Ptr > (), yystack_[1].value.as < std::any > ()};
    }
#line 1677 "buylist.tab.cpp"
    break;

  case 44:
#line 259 "buylist.ypp"
                                                       {
      yylhs.value.as < GenericOption > () = GenericReactOption{yystack_[2].value.as < ReactKind > (), {}, yystack_[1].value.as < std::any > ()};
    }
#line 1685 "buylist.tab.cpp"
    break;

  case 45:
#line 262 "buylist.ypp"
                                                                 {
      yylhs.value.as < GenericOption > () = GenericReactOption{yystack_[3].value.as < ReactKind > (), yystack_[2].value.as < Condition::Ptr > (), yystack_[1].value.as < std::any > ()};
    }
#line 1693 "buylist.tab.cpp"
    break;

  case 46:
#line 267 "buylist.ypp"
                        {
      yylhs.value.as < std::any > () = yystack_[0].value.as < std::any > ();
    }
#line 1701 "buylist.tab.cpp"
    break;

  case 47:
#line 270 "buylist.ypp"
                                                   {
      // only parse as list if it has at least 1 comma
      yystack_[2].value.as < std::vector<std::any> > ().push_back(yystack_[0].value.as < std::any > ());
      yylhs.value.as < std::any > () = yystack_[2].value.as < std::vector<std::any> > ();
    }
#line 1711 "buylist.tab.cpp"
    break;

  case 48:
#line 275 "buylist.ypp"
                           {
      yylhs.value.as < std::any > () = yystack_[0].value.as < TaggedExpr > ();
    }
#line 1719 "buylist.tab.cpp"
    break;

  case 49:
#line 278 "buylist.ypp"
                                                           {
      yystack_[2].value.as < std::vector<TaggedExpr> > ().push_back(yystack_[0].value.as < TaggedExpr > ());
      yylhs.value.as < std::any > () = yystack_[2].value.as < std::vector<TaggedExpr> > ();
    }
#line 1728 "buylist.tab.cpp"
    break;

  case 50:
#line 284 "buylist.ypp"
                            {
      yylhs.value.as < std::any > () = std::make_pair(yystack_[2].value.as < CardId > (), yystack_[0].value.as < CardId > ());
    }
#line 1736 "buylist.tab.cpp"
    break;

  case 51:
#line 287 "buylist.ypp"
              {
      yylhs.value.as < std::any > () = std::any(yystack_[0].value.as < CardId > ());
    }
#line 1744 "buylist.tab.cpp"
    break;

  case 52:
#line 292 "buylist.ypp"
                                                 {
      yystack_[2].value.as < std::vector<std::any> > ().push_back(yystack_[0].value.as < std::any > ());
      yylhs.value.as < std::vector<std::any> > () = yystack_[2].value.as < std::vector<std::any> > ();
    }
#line 1753 "buylist.tab.cpp"
    break;

  case 53:
#line 296 "buylist.ypp"
                          {
      yylhs.value.as < std::vector<std::any> > () = std::vector<std::any>{yystack_[0].value.as < std::any > ()};
    }
#line 1761 "buylist.tab.cpp"
    break;

  case 54:
#line 301 "buylist.ypp"
                    {
      yylhs.value.as < TaggedExpr > () = TaggedExpr{OptionTag::ChooseOption, 1};
    }
#line 1769 "buylist.tab.cpp"
    break;

  case 55:
#line 304 "buylist.ypp"
                          {
      yylhs.value.as < TaggedExpr > () = TaggedExpr{OptionTag::ChooseOption, yystack_[0].value.as < int > ()};
    }
#line 1777 "buylist.tab.cpp"
    break;

  case 56:
#line 307 "buylist.ypp"
                      {
      yylhs.value.as < TaggedExpr > () = TaggedExpr{OptionTag::ChooseOption, -1};
    }
#line 1785 "buylist.tab.cpp"
    break;

  case 57:
#line 310 "buylist.ypp"
                                        {
      yylhs.value.as < TaggedExpr > () = TaggedExpr{OptionTag::Discard, yystack_[1].value.as < std::any > ()};
    }
#line 1793 "buylist.tab.cpp"
    break;

  case 58:
#line 313 "buylist.ypp"
                                     {
      yylhs.value.as < TaggedExpr > () = TaggedExpr{OptionTag::Keep, yystack_[1].value.as < std::any > ()};
    }
#line 1801 "buylist.tab.cpp"
    break;

  case 59:
#line 316 "buylist.ypp"
                                      {
      yylhs.value.as < TaggedExpr > () = TaggedExpr{OptionTag::Trash, yystack_[1].value.as < std::any > ()};
    }
#line 1809 "buylist.tab.cpp"
    break;

  case 60:
#line 321 "buylist.ypp"
                                                         {
      yystack_[2].value.as < std::vector<TaggedExpr> > ().push_back(yystack_[0].value.as < TaggedExpr > ());
      yylhs.value.as < std::vector<TaggedExpr> > () = yystack_[2].value.as < std::vector<TaggedExpr> > ();
    }
#line 1818 "buylist.tab.cpp"
    break;

  case 61:
#line 325 "buylist.ypp"
                           {
      yylhs.value.as < std::vector<TaggedExpr> > () = std::vector<TaggedExpr>{yystack_[0].value.as < TaggedExpr > ()};
    }
#line 1826 "buylist.tab.cpp"
    break;


#line 1830 "buylist.tab.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[+yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yy_error_token_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yy_error_token_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      yy_lac_discard_ ("error recovery");
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  bool
  parser::yy_lac_check_ (int yytoken) const
  {
    // Logically, the yylac_stack's lifetime is confined to this function.
    // Clear it, to get rid of potential left-overs from previous call.
    yylac_stack_.clear ();
    // Reduce until we encounter a shift and thereby accept the token.
#if YYDEBUG
    YYCDEBUG << "LAC: checking lookahead " << yytname_[yytoken] << ':';
#endif
    std::ptrdiff_t lac_top = 0;
    while (true)
      {
        state_type top_state = (yylac_stack_.empty ()
                                ? yystack_[lac_top].state
                                : yylac_stack_.back ());
        int yyrule = yypact_[+top_state];
        if (yy_pact_value_is_default_ (yyrule)
            || (yyrule += yytoken) < 0 || yylast_ < yyrule
            || yycheck_[yyrule] != yytoken)
          {
            // Use the default action.
            yyrule = yydefact_[+top_state];
            if (yyrule == 0)
              {
                YYCDEBUG << " Err\n";
                return false;
              }
          }
        else
          {
            // Use the action from yytable.
            yyrule = yytable_[yyrule];
            if (yy_table_value_is_error_ (yyrule))
              {
                YYCDEBUG << " Err\n";
                return false;
              }
            if (0 < yyrule)
              {
                YYCDEBUG << " S" << yyrule << '\n';
                return true;
              }
            yyrule = -yyrule;
          }
        // By now we know we have to simulate a reduce.
        YYCDEBUG << " R" << yyrule - 1;
        // Pop the corresponding number of values from the stack.
        {
          std::ptrdiff_t yylen = yyr2_[yyrule];
          // First pop from the LAC stack as many tokens as possible.
          std::ptrdiff_t lac_size = std::ptrdiff_t (yylac_stack_.size ());
          if (yylen < lac_size)
            {
              yylac_stack_.resize (std::size_t (lac_size - yylen));
              yylen = 0;
            }
          else if (lac_size)
            {
              yylac_stack_.clear ();
              yylen -= lac_size;
            }
          // Only afterwards look at the main stack.
          // We simulate popping elements by incrementing lac_top.
          lac_top += yylen;
        }
        // Keep top_state in sync with the updated stack.
        top_state = (yylac_stack_.empty ()
                     ? yystack_[lac_top].state
                     : yylac_stack_.back ());
        // Push the resulting state of the reduction.
        state_type state = yy_lr_goto_state_ (top_state, yyr1_[yyrule]);
        YYCDEBUG << " G" << state;
        yylac_stack_.push_back (state);
      }
  }

  // Establish the initial context if no initial context currently exists.
  bool
  parser::yy_lac_establish_ (int yytoken)
  {
    /* Establish the initial context for the current lookahead if no initial
       context is currently established.

       We define a context as a snapshot of the parser stacks.  We define
       the initial context for a lookahead as the context in which the
       parser initially examines that lookahead in order to select a
       syntactic action.  Thus, if the lookahead eventually proves
       syntactically unacceptable (possibly in a later context reached via a
       series of reductions), the initial context can be used to determine
       the exact set of tokens that would be syntactically acceptable in the
       lookahead's place.  Moreover, it is the context after which any
       further semantic actions would be erroneous because they would be
       determined by a syntactically unacceptable token.

       yy_lac_establish_ should be invoked when a reduction is about to be
       performed in an inconsistent state (which, for the purposes of LAC,
       includes consistent states that don't know they're consistent because
       their default reductions have been disabled).

       For parse.lac=full, the implementation of yy_lac_establish_ is as
       follows.  If no initial context is currently established for the
       current lookahead, then check if that lookahead can eventually be
       shifted if syntactic actions continue from the current context.  */
    if (!yy_lac_established_)
      {
#if YYDEBUG
        YYCDEBUG << "LAC: initial context established for "
                 << yytname_[yytoken] << '\n';
#endif
        yy_lac_established_ = true;
        return yy_lac_check_ (yytoken);
      }
    return true;
  }

  // Discard any previous initial lookahead context.
  void
  parser::yy_lac_discard_ (const char* evt)
  {
   /* Discard any previous initial lookahead context because of Event,
      which may be a lookahead change or an invalidation of the currently
      established initial context for the current lookahead.

      The most common example of a lookahead change is a shift.  An example
      of both cases is syntax error recovery.  That is, a syntax error
      occurs when the lookahead is syntactically erroneous for the
      currently established initial context, so error recovery manipulates
      the parser stacks to try to find a new initial context in which the
      current lookahead is syntactically acceptable.  If it fails to find
      such a context, it discards the lookahead.  */
    if (yy_lac_established_)
      {
        YYCDEBUG << "LAC: initial context discarded due to "
                 << evt << '\n';
        yy_lac_established_ = false;
      }
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    std::ptrdiff_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
         In the first two cases, it might appear that the current syntax
         error should have been detected in the previous state when
         yy_lac_check was invoked.  However, at that time, there might
         have been a different syntax error that discarded a different
         initial context during error recovery, leaving behind the
         current lookahead.
    */
    if (!yyla.empty ())
      {
        symbol_number_type yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];

#if YYDEBUG
        // Execute LAC once. We don't care if it is successful, we
        // only do it for the sake of debugging output.
        if (!yy_lac_established_)
          yy_lac_check_ (yytoken);
#endif

        int yyn = yypact_[+yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            for (int yyx = 0; yyx < yyntokens_; ++yyx)
              if (yyx != yy_error_token_ && yyx != yy_undef_token_
                  && yy_lac_check_ (yyx))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -54;

  const signed char parser::yytable_ninf_ = -62;

  const signed char
  parser::yypact_[] =
  {
      51,    34,    59,   -54,     6,     9,   -54,   -54,    -2,   -54,
      -1,   -54,    52,    -7,    48,   -54,    53,    33,   -54,   -54,
     -54,   -54,    52,     0,    55,   -54,    68,   -54,    45,   -54,
     -54,    56,    37,     7,   -54,   -54,    33,    -9,   -54,   -54,
     -54,   -54,    73,   -18,    66,    67,    82,   -54,    35,    25,
     -54,   -54,    45,   -54,   -54,    61,     1,   -54,   -54,    52,
      62,   -54,   -54,    85,   -54,   -54,    35,   -18,    22,   -54,
      22,   -54,    35,   -54,    60,    63,    64,    75,    79,    92,
      70,    71,    72,    74,    46,    89,    98,    46,    46,    46,
      46,    78,   -54,   -54,    54,    52,   100,    52,   -54,   101,
      76,    77,    80,   -54,    83,    86,   -54,   -54,   -54,   -54,
     -54,   -54
  };

  const signed char
  parser::yydefact_[] =
  {
       0,     0,     0,     2,    10,     0,     5,     1,     0,    26,
       0,     9,    10,    13,     0,     4,     0,     0,    17,     6,
      12,    11,     0,     0,     0,    14,     0,    18,     0,     8,
       7,     0,    35,     0,    29,    24,    26,     0,    23,    25,
      20,    22,    15,     0,     0,     0,     0,    41,    34,    35,
       3,    28,     0,    19,    16,     0,     0,    37,    38,     0,
       0,    30,    40,     0,    21,    39,    33,     0,     0,    27,
       0,    31,    32,    36,     0,     0,     0,     0,     0,     0,
      46,    48,     0,     0,     0,    51,     0,     0,     0,     0,
       0,    54,    56,    42,     0,     0,     0,     0,    44,     0,
       0,     0,     0,    55,    49,    47,    43,    50,    45,    57,
      59,    58
  };

  const signed char
  parser::yypgoto_[] =
  {
     -54,   -54,   102,   103,    88,   -54,    84,    69,   -54,    57,
     -46,    47,   -53,    21,    26,   -54,   -54,   -12,   -54,    49,
      -4,   -54,    81,   -54,   -54,   -54,   -54
  };

  const signed char
  parser::yydefgoto_[] =
  {
      -1,     5,     6,    10,    11,    33,    34,    46,    56,    57,
      47,    48,    79,    80,    81,    82,    83,    12,    37,    38,
      85,    70,    40,    41,     2,     3,    22
  };

  const signed char
  parser::yytable_[] =
  {
      13,    25,    62,     8,    19,    30,    66,    16,    13,     9,
      20,    20,    50,     4,    14,    55,     8,    86,    13,    52,
      49,    32,    24,    53,    39,    17,    62,    21,    21,    67,
      54,    96,     8,     9,    99,   100,   101,   102,     4,    58,
      43,    44,    74,    75,    76,    77,    78,     8,    39,     9,
      45,    44,    43,    44,     1,    68,    84,    18,    87,     7,
      45,    26,    45,    58,    28,    27,    74,    75,    76,    77,
      78,    35,    36,     9,    74,    75,    76,    77,    78,     9,
      17,    31,    32,     8,    59,    60,    42,    61,    65,    69,
      71,    88,    91,   107,    89,    90,    92,    93,   -53,   -61,
      94,    97,    95,    98,   103,   106,   108,    15,   109,   110,
      29,   -60,   111,    72,   -52,    23,   105,    51,    63,     0,
     104,     0,     0,     0,    73,     0,     0,     0,     0,     0,
       0,     0,     0,    64
  };

  const signed char
  parser::yycheck_[] =
  {
       4,    13,    48,    10,     5,     5,     5,     9,    12,    27,
      11,    11,     5,     4,     5,    33,    10,    70,    22,    28,
      32,    14,    29,    32,    28,    27,    72,    28,    28,    28,
      42,    84,    10,    27,    87,    88,    89,    90,     4,    43,
      15,    16,    20,    21,    22,    23,    24,    10,    52,    27,
      25,    16,    15,    16,     3,    59,    68,     8,    70,     0,
      25,    13,    25,    67,    31,    16,    20,    21,    22,    23,
      24,    26,    27,    27,    20,    21,    22,    23,    24,    27,
      27,    26,    14,    10,    18,    18,    30,     5,    27,    27,
       5,    31,    17,    97,    31,    31,    17,     5,    28,    28,
      28,    12,    28,     5,    26,     5,     5,     5,    32,    32,
      22,    28,    32,    66,    28,    12,    95,    33,    49,    -1,
      94,    -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    52
  };

  const signed char
  parser::yystos_[] =
  {
       0,     3,    58,    59,     4,    35,    36,     0,    10,    27,
      37,    38,    51,    54,     5,    36,     9,    27,    53,     5,
      11,    28,    60,    37,    29,    51,    13,    53,    31,    38,
       5,    26,    14,    39,    40,    26,    27,    52,    53,    54,
      56,    57,    30,    15,    16,    25,    41,    44,    45,    51,
       5,    40,    28,    32,    51,    33,    42,    43,    54,    18,
      18,     5,    44,    41,    56,    27,     5,    28,    54,    27,
      55,     5,    45,    43,    20,    21,    22,    23,    24,    46,
      47,    48,    49,    50,    51,    54,    46,    51,    31,    31,
      31,    17,    17,     5,    28,    28,    46,    12,     5,    46,
      46,    46,    46,    26,    48,    47,     5,    54,     5,    32,
      32,    32
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    34,    58,    59,    35,    35,    36,    36,    37,    37,
      37,    60,    60,    38,    38,    38,    38,    51,    51,    53,
      52,    52,    56,    56,    57,    57,    54,    55,    39,    39,
      40,    40,    41,    41,    41,    41,    42,    42,    43,    43,
      45,    45,    44,    44,    44,    44,    46,    46,    46,    46,
      47,    47,    50,    50,    48,    48,    48,    48,    48,    48,
      49,    49
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     1,     6,     2,     1,     3,     4,     3,     1,
       0,     1,     1,     1,     2,     4,     5,     2,     3,     4,
       1,     3,     1,     1,     1,     1,     1,     1,     2,     1,
       3,     4,     4,     3,     1,     0,     3,     1,     1,     2,
       2,     1,     5,     6,     5,     6,     1,     3,     1,     3,
       3,     1,     3,     1,     2,     3,     2,     4,     4,     4,
       3,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "$end", "error", "$undefined", "BUYLISTS", "LIST", "END", "COMMENT",
  "LPAREN", "RPAREN", "NOT", "IF", "NEWLINE", "ARROW", "STRATEGIES",
  "STRATEGY", "PLAYORDER", "OPTIONS", "OPTION", "FOR", "OPTIONAL",
  "TAG_DISCARD", "TAG_TRASH", "TAG_KEEP", "TAG_CHOOSE", "TAG_IGNORE",
  "REACTIONS", "INT", "STRING", "','", "'['", "']'", "'('", "')'", "':'",
  "$accept", "lists", "list", "list_entries", "list_entry", "strats",
  "strat", "strat_body", "playorders", "playorder", "cardoption",
  "cardoptions", "cardoption_expr", "cardoption_expr_value",
  "cardoption_tagged_expr", "cardoption_tagged_expr_list",
  "cardoption_expr_list", "condition", "func_arglist", "condition_expr",
  "card_name", "reactoption_name", "expr", "constant", "buylist",
  "buylist_section", "list_separator", YY_NULLPTR
  };

#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,    79,    79,    82,    88,    92,    97,   100,   105,   109,
     112,   117,   117,   120,   123,   126,   130,   136,   139,   144,
     154,   157,   163,   166,   171,   174,   179,   187,   195,   199,
     204,   207,   213,   216,   219,   222,   227,   231,   236,   239,
     244,   248,   253,   256,   259,   262,   267,   270,   275,   278,
     284,   287,   292,   296,   301,   304,   307,   310,   313,   316,
     321,   325
  };

  // Print the state stack on the debug stream.
  void
  parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser::yy_reduce_print_ (int yyrule)
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  parser::token_number_type
  parser::yytranslate_ (int t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const token_number_type
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      31,    32,     2,     2,    28,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    33,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    29,     2,    30,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27
    };
    const int user_token_number_max_ = 282;

    if (t <= 0)
      return yyeof_;
    else if (t <= user_token_number_max_)
      return translate_table[t];
    else
      return yy_undef_token_;
  }

} // yy
#line 2485 "buylist.tab.cpp"

#line 329 "buylist.ypp"


int parse_buylist(char const* text, BuylistCollection& ret, StrategyCollection& strats) {
  auto buf = yy_scan_string(text);
  yycolumn = 1;
  yylineno = 1;

  int ok = 0;
  try {
    yy::parser p(ret, strats);
    // p.set_debug_level(1);
    ok = p.parse();
  }
  catch (...) {
    yy_delete_buffer(buf);
    throw;
  }

  yy_delete_buffer(buf);
  return ok;
}

void yy::parser::error (const location_type& l, const std::string& m) {
  cerr << "Parse error in line " << l << ": " << m << endl;
  throw BuylistParseError{l.begin.line, l.end.line, l.begin.column, l.end.column, m};
}

