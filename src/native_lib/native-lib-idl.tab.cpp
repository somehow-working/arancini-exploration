// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "native-lib-idl.tab.h"


// Unqualified %code blocks.
#line 36 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"


	#include <arancini/native_lib/native-lib.h>
	#include <arancini/native_lib/native-lib-idl.h>


	#define yylex scanner.lex


#line 56 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"


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
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 7 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
namespace arancini { namespace native_lib {
#line 149 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"

  /// Build a parser object.
  Parser::Parser (arancini::native_lib::Scanner& scanner_yyarg, arancini::native_lib::NativeLibs& driver_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      yy_lac_established_ (false),
      scanner (scanner_yyarg),
      driver (driver_yyarg)
  {}

  Parser::~Parser ()
  {}

  Parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_idl: // idl
      case symbol_kind::S_defs: // defs
      case symbol_kind::S_def: // def
      case symbol_kind::S_libdef: // libdef
      case symbol_kind::S_ccdef: // ccdef
      case symbol_kind::S_fndef: // fndef
      case symbol_kind::S_maybe_attrlist: // maybe_attrlist
      case symbol_kind::S_attrs: // attrs
      case symbol_kind::S_attr: // attr
      case symbol_kind::S_full_typedef: // full_typedef
      case symbol_kind::S_const_typedef: // const_typedef
      case symbol_kind::S_typedef: // typedef
      case symbol_kind::S_ptrdef: // ptrdef
      case symbol_kind::S_params: // params
      case symbol_kind::S_param: // param
        value.copy< idl_ast_node > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }




  template <typename Base>
  Parser::symbol_kind_type
  Parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  Parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_idl: // idl
      case symbol_kind::S_defs: // defs
      case symbol_kind::S_def: // def
      case symbol_kind::S_libdef: // libdef
      case symbol_kind::S_ccdef: // ccdef
      case symbol_kind::S_fndef: // fndef
      case symbol_kind::S_maybe_attrlist: // maybe_attrlist
      case symbol_kind::S_attrs: // attrs
      case symbol_kind::S_attr: // attr
      case symbol_kind::S_full_typedef: // full_typedef
      case symbol_kind::S_const_typedef: // const_typedef
      case symbol_kind::S_typedef: // typedef
      case symbol_kind::S_ptrdef: // ptrdef
      case symbol_kind::S_params: // params
      case symbol_kind::S_param: // param
        value.move< idl_ast_node > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.move< std::string > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  Parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  Parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  Parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  Parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  Parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  Parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  Parser::symbol_kind_type
  Parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  Parser::symbol_kind_type
  Parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  Parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Parser::symbol_kind_type
  Parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  Parser::stack_symbol_type::stack_symbol_type ()
  {}

  Parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_idl: // idl
      case symbol_kind::S_defs: // defs
      case symbol_kind::S_def: // def
      case symbol_kind::S_libdef: // libdef
      case symbol_kind::S_ccdef: // ccdef
      case symbol_kind::S_fndef: // fndef
      case symbol_kind::S_maybe_attrlist: // maybe_attrlist
      case symbol_kind::S_attrs: // attrs
      case symbol_kind::S_attr: // attr
      case symbol_kind::S_full_typedef: // full_typedef
      case symbol_kind::S_const_typedef: // const_typedef
      case symbol_kind::S_typedef: // typedef
      case symbol_kind::S_ptrdef: // ptrdef
      case symbol_kind::S_params: // params
      case symbol_kind::S_param: // param
        value.YY_MOVE_OR_COPY< idl_ast_node > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  Parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_idl: // idl
      case symbol_kind::S_defs: // defs
      case symbol_kind::S_def: // def
      case symbol_kind::S_libdef: // libdef
      case symbol_kind::S_ccdef: // ccdef
      case symbol_kind::S_fndef: // fndef
      case symbol_kind::S_maybe_attrlist: // maybe_attrlist
      case symbol_kind::S_attrs: // attrs
      case symbol_kind::S_attr: // attr
      case symbol_kind::S_full_typedef: // full_typedef
      case symbol_kind::S_const_typedef: // const_typedef
      case symbol_kind::S_typedef: // typedef
      case symbol_kind::S_ptrdef: // ptrdef
      case symbol_kind::S_params: // params
      case symbol_kind::S_param: // param
        value.move< idl_ast_node > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_idl: // idl
      case symbol_kind::S_defs: // defs
      case symbol_kind::S_def: // def
      case symbol_kind::S_libdef: // libdef
      case symbol_kind::S_ccdef: // ccdef
      case symbol_kind::S_fndef: // fndef
      case symbol_kind::S_maybe_attrlist: // maybe_attrlist
      case symbol_kind::S_attrs: // attrs
      case symbol_kind::S_attr: // attr
      case symbol_kind::S_full_typedef: // full_typedef
      case symbol_kind::S_const_typedef: // const_typedef
      case symbol_kind::S_typedef: // typedef
      case symbol_kind::S_ptrdef: // ptrdef
      case symbol_kind::S_params: // params
      case symbol_kind::S_param: // param
        value.copy< idl_ast_node > (that.value);
        break;

      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_idl: // idl
      case symbol_kind::S_defs: // defs
      case symbol_kind::S_def: // def
      case symbol_kind::S_libdef: // libdef
      case symbol_kind::S_ccdef: // ccdef
      case symbol_kind::S_fndef: // fndef
      case symbol_kind::S_maybe_attrlist: // maybe_attrlist
      case symbol_kind::S_attrs: // attrs
      case symbol_kind::S_attr: // attr
      case symbol_kind::S_full_typedef: // full_typedef
      case symbol_kind::S_const_typedef: // const_typedef
      case symbol_kind::S_typedef: // typedef
      case symbol_kind::S_ptrdef: // ptrdef
      case symbol_kind::S_params: // params
      case symbol_kind::S_param: // param
        value.move< idl_ast_node > (that.value);
        break;

      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.move< std::string > (that.value);
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
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  Parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  Parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::operator() ()
  {
    return parse ();
  }

  int
  Parser::parse ()
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

    // Discard the LAC context in case there still is one left from a
    // previous invocation.
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
    YY_STACK_PRINT ();

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
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, &yyla.location));
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

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        if (!yy_lac_establish_ (yyla.kind ()))
          goto yyerrlab;
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        if (!yy_lac_establish_ (yyla.kind ()))
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
      case symbol_kind::S_idl: // idl
      case symbol_kind::S_defs: // defs
      case symbol_kind::S_def: // def
      case symbol_kind::S_libdef: // libdef
      case symbol_kind::S_ccdef: // ccdef
      case symbol_kind::S_fndef: // fndef
      case symbol_kind::S_maybe_attrlist: // maybe_attrlist
      case symbol_kind::S_attrs: // attrs
      case symbol_kind::S_attr: // attr
      case symbol_kind::S_full_typedef: // full_typedef
      case symbol_kind::S_const_typedef: // const_typedef
      case symbol_kind::S_typedef: // typedef
      case symbol_kind::S_ptrdef: // ptrdef
      case symbol_kind::S_params: // params
      case symbol_kind::S_param: // param
        yylhs.value.emplace< idl_ast_node > ();
        break;

      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        yylhs.value.emplace< std::string > ();
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
  case 2: // idl: defs
#line 78 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
          { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_ROOT); yylhs.value.as < idl_ast_node > ().add_child(std::move(yystack_[0].value.as < idl_ast_node > ())); driver.idl_commit(yylhs.value.as < idl_ast_node > ()); }
#line 811 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 3: // defs: defs def
#line 80 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
               { yylhs.value.as < idl_ast_node > () = yystack_[1].value.as < idl_ast_node > (); yylhs.value.as < idl_ast_node > ().add_child(std::move(yystack_[0].value.as < idl_ast_node > ())); }
#line 817 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 4: // defs: def
#line 81 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
          { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_DEFS);yylhs.value.as < idl_ast_node > ().add_child(std::move(yystack_[0].value.as < idl_ast_node > ())); }
#line 823 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 5: // def: libdef
#line 84 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
            { yylhs.value.as < idl_ast_node > () = yystack_[0].value.as < idl_ast_node > (); }
#line 829 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 6: // def: ccdef
#line 85 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
           { yylhs.value.as < idl_ast_node > () = yystack_[0].value.as < idl_ast_node > (); }
#line 835 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 7: // def: fndef
#line 86 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
           { yylhs.value.as < idl_ast_node > () = yystack_[0].value.as < idl_ast_node > (); }
#line 841 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 8: // libdef: T_LIBRARY STRING_LITERAL T_SEMI
#line 89 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                                        { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_LIBDEF); yylhs.value.as < idl_ast_node > ().value = yystack_[1].value.as < std::string > (); }
#line 847 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 9: // ccdef: T_CALLCONV IDENTIFIER T_SEMI
#line 91 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                                    { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_CCDEF); yylhs.value.as < idl_ast_node > ().value = yystack_[1].value.as < std::string > (); }
#line 853 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 10: // fndef: maybe_attrlist full_typedef IDENTIFIER T_LPAREN params T_RPAREN T_SEMI
#line 93 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                                                                              { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_FNDEF); yylhs.value.as < idl_ast_node > ().value = yystack_[4].value.as < std::string > (); yylhs.value.as < idl_ast_node > ().add_child(std::move(yystack_[5].value.as < idl_ast_node > ()));  yylhs.value.as < idl_ast_node > ().add_child(std::move(yystack_[2].value.as < idl_ast_node > ())); }
#line 859 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 11: // maybe_attrlist: %empty
#line 95 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_ATTRS); }
#line 865 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 12: // maybe_attrlist: T_LBRACKET attrs T_RBRACKET
#line 96 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                                            { yylhs.value.as < idl_ast_node > () = yystack_[1].value.as < idl_ast_node > (); }
#line 871 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 13: // attrs: attr
#line 99 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
            { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_ATTRS);  yylhs.value.as < idl_ast_node > ().add_child(std::move(yystack_[0].value.as < idl_ast_node > ())); }
#line 877 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 14: // attrs: attrs T_COMMA attr
#line 100 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                          { yylhs.value.as < idl_ast_node > () = yystack_[2].value.as < idl_ast_node > ();  yylhs.value.as < idl_ast_node > ().add_child(std::move(yystack_[0].value.as < idl_ast_node > ())); }
#line 883 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 15: // attr: IDENTIFIER
#line 103 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                 { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_ATTR); yylhs.value.as < idl_ast_node > ().value = yystack_[0].value.as < std::string > (); }
#line 889 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 16: // full_typedef: maybe_attrlist const_typedef
#line 106 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                                           { yylhs.value.as < idl_ast_node > () = yystack_[0].value.as < idl_ast_node > (); }
#line 895 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 17: // const_typedef: typedef
#line 108 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                                { yylhs.value.as < idl_ast_node > () = yystack_[0].value.as < idl_ast_node > (); yylhs.value.as < idl_ast_node > ().is_const = 0; }
#line 901 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 18: // const_typedef: T_CONST typedef
#line 109 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                                { yylhs.value.as < idl_ast_node > () = yystack_[0].value.as < idl_ast_node > (); yylhs.value.as < idl_ast_node > ().is_const = 1; }
#line 907 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 19: // typedef: ptrdef
#line 112 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                { yylhs.value.as < idl_ast_node > () = yystack_[0].value.as < idl_ast_node > (); }
#line 913 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 20: // typedef: T_I8
#line 113 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_TYPEDEF); yylhs.value.as < idl_ast_node > ().width = 8; yylhs.value.as < idl_ast_node > ().tc = 1; }
#line 919 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 21: // typedef: T_I16
#line 114 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_TYPEDEF); yylhs.value.as < idl_ast_node > ().width = 16; yylhs.value.as < idl_ast_node > ().tc = 1; }
#line 925 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 22: // typedef: T_I32
#line 115 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_TYPEDEF); yylhs.value.as < idl_ast_node > ().width = 32; yylhs.value.as < idl_ast_node > ().tc = 1; }
#line 931 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 23: // typedef: T_I64
#line 116 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_TYPEDEF); yylhs.value.as < idl_ast_node > ().width = 64; yylhs.value.as < idl_ast_node > ().tc = 1; }
#line 937 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 24: // typedef: T_ILONG
#line 117 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_TYPEDEF); yylhs.value.as < idl_ast_node > ().width = 64; yylhs.value.as < idl_ast_node > ().tc = 1; }
#line 943 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 25: // typedef: T_U1
#line 118 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_TYPEDEF); yylhs.value.as < idl_ast_node > ().width = 1; yylhs.value.as < idl_ast_node > ().tc = 2; }
#line 949 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 26: // typedef: T_U8
#line 119 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_TYPEDEF); yylhs.value.as < idl_ast_node > ().width = 8; yylhs.value.as < idl_ast_node > ().tc = 2; }
#line 955 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 27: // typedef: T_U16
#line 120 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_TYPEDEF); yylhs.value.as < idl_ast_node > ().width = 16; yylhs.value.as < idl_ast_node > ().tc = 2; }
#line 961 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 28: // typedef: T_U32
#line 121 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_TYPEDEF); yylhs.value.as < idl_ast_node > ().width = 32; yylhs.value.as < idl_ast_node > ().tc = 2; }
#line 967 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 29: // typedef: T_U64
#line 122 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_TYPEDEF); yylhs.value.as < idl_ast_node > ().width = 64; yylhs.value.as < idl_ast_node > ().tc = 2; }
#line 973 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 30: // typedef: T_ULONG
#line 123 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_TYPEDEF); yylhs.value.as < idl_ast_node > ().width = 64; yylhs.value.as < idl_ast_node > ().tc = 2; }
#line 979 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 31: // typedef: T_F32
#line 124 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_TYPEDEF); yylhs.value.as < idl_ast_node > ().width = 32; yylhs.value.as < idl_ast_node > ().tc = 3; }
#line 985 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 32: // typedef: T_F64
#line 125 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_TYPEDEF); yylhs.value.as < idl_ast_node > ().width = 64; yylhs.value.as < idl_ast_node > ().tc = 3; }
#line 991 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 33: // typedef: T_STRING
#line 126 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_TYPEDEF); yylhs.value.as < idl_ast_node > ().width = 64; yylhs.value.as < idl_ast_node > ().tc = 4; }
#line 997 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 34: // typedef: T_VOID
#line 127 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_TYPEDEF); yylhs.value.as < idl_ast_node > ().width = 0; yylhs.value.as < idl_ast_node > ().tc = 0; }
#line 1003 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 35: // typedef: T_CPLX
#line 128 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_TYPEDEF); yylhs.value.as < idl_ast_node > ().width = 0; yylhs.value.as < idl_ast_node > ().tc = 8; }
#line 1009 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 36: // ptrdef: T_PTR
#line 131 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
              { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_TYPEDEF); yylhs.value.as < idl_ast_node > ().width = 64; yylhs.value.as < idl_ast_node > ().tc = 5; }
#line 1015 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 37: // ptrdef: T_PTR T_LPAREN IDENTIFIER T_RPAREN
#line 132 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                                           { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_TYPEDEF); yylhs.value.as < idl_ast_node > ().width = 64; yylhs.value.as < idl_ast_node > ().tc = 5; }
#line 1021 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 38: // params: params T_COMMA param
#line 135 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                                { yylhs.value.as < idl_ast_node > () = yystack_[2].value.as < idl_ast_node > ();  yylhs.value.as < idl_ast_node > ().add_child(std::move(yystack_[0].value.as < idl_ast_node > ())); }
#line 1027 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 39: // params: param
#line 136 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                                { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_PARAMS);  yylhs.value.as < idl_ast_node > ().add_child(std::move(yystack_[0].value.as < idl_ast_node > ())); }
#line 1033 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 40: // params: %empty
#line 137 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                                { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_PARAMS); }
#line 1039 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;

  case 41: // param: full_typedef IDENTIFIER
#line 140 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
                               { yylhs.value.as < idl_ast_node > () = idl_ast_node(IANT_PARAM); yylhs.value.as < idl_ast_node > ().value = yystack_[0].value.as < std::string > ();  yylhs.value.as < idl_ast_node > ().add_child(std::move(yystack_[1].value.as < idl_ast_node > ())); }
#line 1045 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"
    break;


#line 1049 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"

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
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
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
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
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
    {
      stack_symbol_type error_token;

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
    YY_STACK_PRINT ();
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
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  const char *
  Parser::symbol_name (symbol_kind_type yysymbol)
  {
    static const char *const yy_sname[] =
    {
    "end of file", "error", "invalid token", "T_LIBRARY", "T_CALLCONV",
  "T_STRING", "T_FD", "T_CPLX", "T_CONST", "T_PTR", "T_VOID", "T_I8",
  "T_I16", "T_I32", "T_I64", "T_ILONG", "T_U1", "T_U8", "T_U16", "T_U32",
  "T_U64", "T_ULONG", "T_F32", "T_F64", "T_STAR", "T_SEMI", "T_LPAREN",
  "T_RPAREN", "T_LBRACKET", "T_RBRACKET", "T_COMMA", "STRING_LITERAL",
  "IDENTIFIER", "$accept", "idl", "defs", "def", "libdef", "ccdef",
  "fndef", "maybe_attrlist", "attrs", "attr", "full_typedef",
  "const_typedef", "typedef", "ptrdef", "params", "param", YY_NULLPTR
    };
    return yy_sname[yysymbol];
  }



  // Parser::context.
  Parser::context::context (const Parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  Parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

#if YYDEBUG
    // Execute LAC once. We don't care if it is successful, we
    // only do it for the sake of debugging output.
    if (!yyparser_.yy_lac_established_)
      yyparser_.yy_lac_check_ (yyla_.kind ());
#endif

    for (int yyx = 0; yyx < YYNTOKENS; ++yyx)
      {
        symbol_kind_type yysym = YY_CAST (symbol_kind_type, yyx);
        if (yysym != symbol_kind::S_YYerror
            && yysym != symbol_kind::S_YYUNDEF
            && yyparser_.yy_lac_check_ (yysym))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = yysym;
          }
      }
    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }




  bool
  Parser::yy_lac_check_ (symbol_kind_type yytoken) const
  {
    // Logically, the yylac_stack's lifetime is confined to this function.
    // Clear it, to get rid of potential left-overs from previous call.
    yylac_stack_.clear ();
    // Reduce until we encounter a shift and thereby accept the token.
#if YYDEBUG
    YYCDEBUG << "LAC: checking lookahead " << symbol_name (yytoken) << ':';
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
        YYCDEBUG << " G" << int (state);
        yylac_stack_.push_back (state);
      }
  }

  // Establish the initial context if no initial context currently exists.
  bool
  Parser::yy_lac_establish_ (symbol_kind_type yytoken)
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
    if (yy_lac_established_)
      return true;
    else
      {
#if YYDEBUG
        YYCDEBUG << "LAC: initial context established for "
                 << symbol_name (yytoken) << '\n';
#endif
        yy_lac_established_ = true;
        return yy_lac_check_ (yytoken);
      }
  }

  // Discard any previous initial lookahead context.
  void
  Parser::yy_lac_discard_ (const char* event)
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
                 << event << '\n';
        yy_lac_established_ = false;
      }
  }


  int
  Parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
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

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

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
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char Parser::yypact_ninf_ = -27;

  const signed char Parser::yytable_ninf_ = -41;

  const signed char
  Parser::yypact_[] =
  {
      -2,   -26,   -25,    15,    54,     0,   -27,   -27,   -27,   -27,
      27,    31,    32,   -27,    22,   -27,   -27,   -27,     1,    26,
     -27,   -27,   -27,    15,   -27,   -27,    20,    33,   -27,   -27,
     -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,
     -27,   -27,   -27,   -27,   -27,    34,   -27,   -27,    29,    18,
      35,    36,    23,   -27,   -27,   -27,    38,    27,   -27,   -27
  };

  const signed char
  Parser::yydefact_[] =
  {
      11,     0,     0,     0,     0,    11,     4,     5,     6,     7,
      11,     0,     0,    15,     0,    13,     1,     3,     0,     0,
       8,     9,    12,     0,    33,    35,     0,    36,    34,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    16,    17,    19,     0,    14,    18,     0,    11,
       0,     0,     0,    39,    37,    41,     0,    11,    10,    38
  };

  const signed char
  Parser::yypgoto_[] =
  {
     -27,   -27,   -27,    59,   -27,   -27,   -27,    44,   -27,    42,
      56,   -27,    41,   -27,   -27,    12
  };

  const signed char
  Parser::yydefgoto_[] =
  {
       0,     4,     5,     6,     7,     8,     9,    18,    14,    15,
      51,    42,    43,    44,    52,    53
  };

  const signed char
  Parser::yytable_[] =
  {
      -2,     1,     2,     1,     2,    11,    24,    12,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    24,     3,    25,     3,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    10,   -40,     3,    13,   -40,    10,
      56,    22,    23,    57,    16,     3,    20,    21,    45,    48,
      49,    50,    54,    58,    17,    46,    19,    47,    55,    59
  };

  const signed char
  Parser::yycheck_[] =
  {
       0,     3,     4,     3,     4,    31,     5,    32,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,     5,    28,     7,    28,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,     0,    27,    28,    32,    30,     5,
      27,    29,    30,    30,     0,    28,    25,    25,    32,    26,
      26,    32,    27,    25,     5,    23,    10,    26,    32,    57
  };

  const signed char
  Parser::yystos_[] =
  {
       0,     3,     4,    28,    34,    35,    36,    37,    38,    39,
      40,    31,    32,    32,    41,    42,     0,    36,    40,    43,
      25,    25,    29,    30,     5,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    44,    45,    46,    32,    42,    45,    26,    26,
      32,    43,    47,    48,    27,    32,    27,    30,    25,    48
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    33,    34,    35,    35,    36,    36,    36,    37,    38,
      39,    40,    40,    41,    41,    42,    43,    44,    44,    45,
      45,    45,    45,    45,    45,    45,    45,    45,    45,    45,
      45,    45,    45,    45,    45,    45,    46,    46,    47,    47,
      47,    48
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     1,     2,     1,     1,     1,     1,     3,     3,
       7,     0,     3,     1,     3,     1,     2,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     3,     1,
       0,     2
  };




#if YYDEBUG
  const unsigned char
  Parser::yyrline_[] =
  {
       0,    78,    78,    80,    81,    84,    85,    86,    89,    91,
      93,    95,    96,    99,   100,   103,   106,   108,   109,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   131,   132,   135,   136,
     137,   140
  };

  void
  Parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  Parser::yy_reduce_print_ (int yyrule) const
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

  Parser::symbol_kind_type
  Parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    return static_cast<symbol_kind_type> (t);
  }

#line 7 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"
} } // arancini::native_lib
#line 1653 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.tab.cpp"

#line 142 "/home/ole/mnt/arancini-exploration/src/native_lib/native-lib-idl.y"

// Bison expects us to provide implementation - otherwise linker complains
void arancini::native_lib::Parser::error(const arancini::native_lib::location &loc , const std::string &message) {

		std::cerr << "Error: " << message << "\nLocation: " << loc << '\n';
}
