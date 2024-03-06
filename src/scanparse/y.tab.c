/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "palm/memory.h"
#include "palm/ctinfo.h"
#include "palm/dbug.h"
#include "palm/str.h"
#include "ccngen/ast.h"
#include "ccngen/enum.h"
#include "global/globals.h"

static node_st *parseresult = NULL;
extern int yylex();
int yyerror(char *errname);
extern FILE *yyin;
void AddLocToNode(node_st *node, void *begin_loc, void *end_loc);



#line 94 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    BRACKET_L = 258,
    BRACKET_R = 259,
    COMMA = 260,
    SEMICOLON = 261,
    CURLY_BRACKET_L = 262,
    CURLY_BRACKET_R = 263,
    SQUARE_BRACKET_L = 264,
    SQUARE_BRACKET_R = 265,
    MINUS = 266,
    PLUS = 267,
    STAR = 268,
    SLASH = 269,
    MOD = 270,
    LE = 271,
    LT = 272,
    GE = 273,
    GT = 274,
    EQ = 275,
    NE = 276,
    OR = 277,
    AND = 278,
    NOT = 279,
    TRUEVAL = 280,
    FALSEVAL = 281,
    LET = 282,
    INT_TYPE = 283,
    FLOAT_TYPE = 284,
    BOOL_TYPE = 285,
    VOID_TYPE = 286,
    EXPORT = 287,
    RETURN = 288,
    IF = 289,
    ELSE = 290,
    WHILE = 291,
    DO = 292,
    FOR = 293,
    EXTERN = 294,
    NUM = 295,
    FLOAT = 296,
    ID = 297,
    UMINUS = 298,
    THEN = 299
  };
#endif
/* Tokens.  */
#define BRACKET_L 258
#define BRACKET_R 259
#define COMMA 260
#define SEMICOLON 261
#define CURLY_BRACKET_L 262
#define CURLY_BRACKET_R 263
#define SQUARE_BRACKET_L 264
#define SQUARE_BRACKET_R 265
#define MINUS 266
#define PLUS 267
#define STAR 268
#define SLASH 269
#define MOD 270
#define LE 271
#define LT 272
#define GE 273
#define GT 274
#define EQ 275
#define NE 276
#define OR 277
#define AND 278
#define NOT 279
#define TRUEVAL 280
#define FALSEVAL 281
#define LET 282
#define INT_TYPE 283
#define FLOAT_TYPE 284
#define BOOL_TYPE 285
#define VOID_TYPE 286
#define EXPORT 287
#define RETURN 288
#define IF 289
#define ELSE 290
#define WHILE 291
#define DO 292
#define FOR 293
#define EXTERN 294
#define NUM 295
#define FLOAT 296
#define ID 297
#define UMINUS 298
#define THEN 299

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 25 "parser.y"

 char               *id;
 int                 cint;
 float               cflt;
 enum Type           ctype;
 node_st             *node;

#line 242 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  16
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   494

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  45
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  99
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  195

#define YYUNDEFTOK  2
#define YYMAXUTOK   299


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    76,    76,    84,    89,    97,   102,   110,   114,   120,
     127,   132,   140,   144,   152,   158,   165,   169,   174,   183,
     188,   196,   204,   209,   217,   221,   228,   233,   238,   244,
     264,   268,   273,   277,   287,   288,   293,   297,   305,   309,
     316,   321,   329,   333,   341,   345,   349,   353,   357,   361,
     366,   372,   377,   383,   389,   393,   404,   411,   415,   422,
     429,   434,   442,   450,   455,   462,   467,   474,   480,   488,
     490,   492,   494,   496,   498,   500,   502,   504,   506,   508,
     510,   512,   514,   516,   518,   522,   526,   530,   534,   540,
     544,   548,   554,   560,   566,   570,   578,   579,   580,   581
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "BRACKET_L", "BRACKET_R", "COMMA",
  "SEMICOLON", "CURLY_BRACKET_L", "CURLY_BRACKET_R", "SQUARE_BRACKET_L",
  "SQUARE_BRACKET_R", "MINUS", "PLUS", "STAR", "SLASH", "MOD", "LE", "LT",
  "GE", "GT", "EQ", "NE", "OR", "AND", "NOT", "TRUEVAL", "FALSEVAL", "LET",
  "INT_TYPE", "FLOAT_TYPE", "BOOL_TYPE", "VOID_TYPE", "EXPORT", "RETURN",
  "IF", "ELSE", "WHILE", "DO", "FOR", "EXTERN", "NUM", "FLOAT", "ID",
  "UMINUS", "THEN", "$accept", "program", "decls", "exprs",
  "exprs_nonempty", "arrexpr", "ids", "ids_nonempty", "funcall", "cast",
  "decl", "fundef", "compound_statement", "blocks", "blocks_nonempty",
  "block", "funbody", "globdef", "globdecl", "stmts", "param", "params",
  "params_nonempty", "stmt", "return", "exprstmt", "ifelse", "while",
  "stmt_block", "dowhile", "for", "assign", "vardecl", "var_decl_tail",
  "varlet", "var", "expr", "constants", "floatval", "intval", "boolval",
  "ctype", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299
};
# endif

#define YYPACT_NINF (-108)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-68)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -3,  -108,  -108,  -108,  -108,   158,   158,    20,  -108,    -3,
    -108,  -108,  -108,   -19,    22,    -4,  -108,  -108,    13,    18,
      23,    71,   158,  -108,   195,   158,  -108,   195,  -108,    12,
    -108,  -108,  -108,    26,  -108,    -1,   158,   195,   195,   195,
    -108,  -108,  -108,  -108,    15,  -108,  -108,  -108,  -108,   355,
    -108,  -108,  -108,  -108,    29,   373,    38,    39,    76,   158,
      23,  -108,    98,    74,  -108,   458,  -108,  -108,   195,  -108,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,    76,  -108,  -108,   100,    67,  -108,  -108,
      84,   195,   195,  -108,    57,    58,    58,  -108,  -108,  -108,
     136,   136,   136,   136,   120,   120,   471,   160,  -108,  -108,
     158,   199,   109,   110,   119,   111,    36,  -108,    67,  -108,
    -108,   108,  -108,  -108,  -108,  -108,  -108,  -108,  -108,  -108,
    -108,    93,   391,     0,    79,   458,   458,  -108,    81,  -108,
     409,   195,   195,  -108,  -108,    88,   101,  -108,  -108,   195,
    -108,   195,    63,  -108,   138,  -108,   258,   278,   159,   139,
     116,   427,   105,  -108,   195,  -108,   119,   119,  -108,  -108,
     195,   137,  -108,   123,   445,   134,  -108,   298,   195,     5,
    -108,   119,   176,   337,  -108,  -108,  -108,   195,   238,   119,
     195,  -108,   318,   119,  -108
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,    96,    97,    98,    99,     0,     0,     0,     2,     4,
      18,    17,    16,     0,     0,     0,     1,     3,     0,     0,
      11,     0,    41,    31,     0,    41,    30,     0,    12,     0,
      10,    35,    42,     0,    40,     0,     0,     6,     0,     0,
      94,    95,    93,    92,    68,    88,    87,    84,    86,     0,
      85,    89,    90,    91,     0,     0,     0,     0,     0,     0,
      11,    39,     0,     0,     5,     7,    83,    82,     6,    33,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    32,    13,     0,    23,    19,    43,
       0,     0,     0,     9,     0,    70,    69,    71,    72,    73,
      77,    76,    79,    78,    74,    75,    81,    80,    20,    34,
       0,     0,     0,     0,     0,     0,    68,    27,    29,    22,
      24,     0,    28,    50,    45,    46,    47,    48,    49,    44,
      26,     0,     0,     0,     0,    15,     8,    14,     0,    52,
       0,     0,     0,    36,    58,     0,     0,    25,    21,     0,
      53,     6,     0,    38,     0,    51,     0,     0,     0,     0,
       0,     0,     0,    65,     0,    63,     0,     0,    57,    37,
       0,     0,    62,     0,     0,    54,    56,     0,     0,     0,
      66,     0,     0,     0,    64,    55,    59,     0,     0,     0,
       0,    60,     0,     0,    61
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -108,  -108,   181,   -66,  -108,  -108,   131,  -108,  -108,  -108,
    -108,   -75,   124,  -108,  -108,    85,  -108,  -108,  -108,  -108,
     135,   184,  -108,   -83,  -108,  -108,  -108,  -108,  -107,  -108,
    -108,  -108,  -108,    32,  -108,  -108,   -24,  -108,  -108,  -108,
    -108,     1
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     7,     8,    63,    64,    45,    29,    30,    46,    47,
       9,    10,    88,   118,   119,   120,   121,    11,    12,   158,
      32,    33,    34,   144,   123,   124,   125,   126,   145,   127,
     128,   129,   130,   165,   131,    48,   132,    50,    51,    52,
      53,    35
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      49,    13,    94,    55,   122,    20,    14,    15,    60,   151,
      13,   163,   117,    65,    66,    67,    22,    56,    68,    23,
      16,    25,    57,    18,    26,     1,     2,     3,     4,     5,
      58,    59,   164,    83,    59,   122,     6,    62,    21,    68,
      24,    61,   152,   117,    65,    27,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   175,
     176,   137,    92,   -67,    19,    28,    22,   135,   136,   163,
      36,    72,    73,    74,   185,   169,    37,    31,    38,    92,
      85,    86,   191,    87,    93,   162,   194,   140,   133,    56,
     164,    39,    40,    41,   134,     1,     2,     3,     4,   110,
     111,   112,    91,   113,   114,   115,   109,    42,    43,   116,
      92,   138,   141,   142,   146,   173,   148,   156,   157,   133,
     149,   153,    36,   154,   159,   161,   143,    65,    37,   160,
      38,    70,    71,    72,    73,    74,    75,    76,    77,    78,
     174,    25,   170,    39,    40,    41,   177,    70,    71,    72,
      73,    74,   111,   112,   183,   113,   114,   115,   171,    42,
      43,   116,    36,   188,   178,   179,   192,   168,    37,   181,
      38,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,   186,    39,    40,    41,     1,     2,     3,     4,
      17,    90,   111,   112,    89,   113,   114,   115,    36,    42,
      43,   116,    36,   147,    37,   139,    38,   108,    37,    54,
      38,   184,     0,     0,     0,     0,     0,     0,     0,    39,
      40,    41,     0,    39,    40,    41,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    42,    43,    44,     0,    42,
      43,    44,   189,   190,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,   166,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,   167,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,   182,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,   193,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,   187,     0,     0,     0,     0,     0,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    69,     0,     0,     0,     0,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    84,
       0,     0,     0,     0,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,   150,     0,     0,
       0,     0,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,   155,     0,     0,     0,     0,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,   172,     0,     0,     0,     0,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,   180,     0,     0,     0,     0,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,     0,    82
};

static const yytype_int16 yycheck[] =
{
      24,     0,    68,    27,    87,     9,     5,     6,     9,     9,
       9,     6,    87,    37,    38,    39,     3,     5,     3,     6,
       0,     3,    10,    42,     6,    28,    29,    30,    31,    32,
       4,     5,    27,     4,     5,   118,    39,    36,    42,     3,
      27,    42,    42,   118,    68,    27,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,   166,
     167,     4,     5,    27,    42,    42,     3,    91,    92,     6,
       3,    13,    14,    15,   181,   158,     9,     6,    11,     5,
      42,    42,   189,     7,    10,   151,   193,   111,    87,     5,
      27,    24,    25,    26,    10,    28,    29,    30,    31,    32,
      33,    34,     4,    36,    37,    38,     6,    40,    41,    42,
       5,   110,     3,     3,     3,    10,     8,   141,   142,   118,
      27,    42,     3,    42,    36,   149,     7,   151,     9,    28,
      11,    11,    12,    13,    14,    15,    16,    17,    18,    19,
     164,     3,     3,    24,    25,    26,   170,    11,    12,    13,
      14,    15,    33,    34,   178,    36,    37,    38,    42,    40,
      41,    42,     3,   187,    27,    42,   190,     8,     9,    35,
      11,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,     6,    24,    25,    26,    28,    29,    30,    31,
       9,    60,    33,    34,    59,    36,    37,    38,     3,    40,
      41,    42,     3,   118,     9,     6,    11,    83,     9,    25,
      11,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      25,    26,    -1,    24,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    40,    41,    42,    -1,    40,
      41,    42,     4,     5,    -1,    -1,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,     4,    -1,    -1,    -1,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,     4,    -1,    -1,    -1,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,     4,    -1,    -1,    -1,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,     4,    -1,    -1,    -1,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,     5,    -1,    -1,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,     6,    -1,    -1,    -1,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,     6,
      -1,    -1,    -1,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,     6,    -1,    -1,
      -1,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,     6,    -1,    -1,    -1,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,     6,    -1,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,     6,    -1,    -1,    -1,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    23
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    28,    29,    30,    31,    32,    39,    46,    47,    55,
      56,    62,    63,    86,    86,    86,     0,    47,    42,    42,
       9,    42,     3,     6,    27,     3,     6,    27,    42,    51,
      52,     6,    65,    66,    67,    86,     3,     9,    11,    24,
      25,    26,    40,    41,    42,    50,    53,    54,    80,    81,
      82,    83,    84,    85,    66,    81,     5,    10,     4,     5,
       9,    42,    86,    48,    49,    81,    81,    81,     3,     6,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,     4,     6,    42,    42,     7,    57,    65,
      51,     4,     5,    10,    48,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    57,     6,
      32,    33,    34,    36,    37,    38,    42,    56,    58,    59,
      60,    61,    68,    69,    70,    71,    72,    74,    75,    76,
      77,    79,    81,    86,    10,    81,    81,     4,    86,     6,
      81,     3,     3,     7,    68,    73,     3,    60,     8,    27,
       6,     9,    42,    42,    42,     6,    81,    81,    64,    36,
      28,    81,    48,     6,    27,    78,     4,     4,     8,    68,
       3,    42,     6,    10,    81,    73,    73,    81,    27,    42,
       6,    35,     4,    81,    78,    73,     6,     5,    81,     4,
       5,    73,    81,     4,    73
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    45,    46,    47,    47,    48,    48,    49,    49,    50,
      51,    51,    52,    52,    53,    54,    55,    55,    55,    56,
      56,    57,    58,    58,    59,    59,    60,    60,    60,    61,
      62,    62,    62,    62,    63,    63,    64,    64,    65,    65,
      66,    66,    67,    67,    68,    68,    68,    68,    68,    68,
      68,    69,    69,    70,    71,    71,    72,    73,    73,    74,
      75,    75,    76,    77,    77,    78,    78,    79,    80,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    82,
      82,    82,    83,    84,    85,    85,    86,    86,    86,    86
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     1,     0,     1,     3,     3,
       1,     0,     1,     3,     4,     4,     1,     1,     1,     6,
       7,     3,     1,     0,     1,     2,     1,     1,     1,     1,
       4,     3,     6,     5,     7,     4,     0,     2,     5,     2,
       1,     0,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     3,     2,     2,     5,     7,     5,     3,     1,     7,
      10,    12,     4,     3,     6,     1,     3,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 77 "parser.y"
         {
           parseresult = ASTprogram((yyvsp[0].node));
         }
#line 1728 "y.tab.c"
    break;

  case 3:
#line 85 "parser.y"
       {
         (yyval.node) = ASTdecls((yyvsp[-1].node), (yyvsp[0].node));
       }
#line 1736 "y.tab.c"
    break;

  case 4:
#line 90 "parser.y"
      {
          (yyval.node) = NULL;
      }
#line 1744 "y.tab.c"
    break;

  case 5:
#line 98 "parser.y"
       {
         (yyval.node) = (yyvsp[0].node);
       }
#line 1752 "y.tab.c"
    break;

  case 6:
#line 103 "parser.y"
      {
        (yyval.node) = NULL;
      }
#line 1760 "y.tab.c"
    break;

  case 7:
#line 111 "parser.y"
    {
      (yyval.node) = ASTexprs((yyvsp[0].node), NULL);
    }
#line 1768 "y.tab.c"
    break;

  case 8:
#line 115 "parser.y"
  {
    (yyval.node) = ASTexprs((yyvsp[0].node), (yyvsp[-2].node));
  }
#line 1776 "y.tab.c"
    break;

  case 9:
#line 121 "parser.y"
         {
           (yyval.node) = ASTarrexpr((yyvsp[-1].node));
         }
#line 1784 "y.tab.c"
    break;

  case 10:
#line 128 "parser.y"
       {
         (yyval.node) = (yyvsp[0].node);
       }
#line 1792 "y.tab.c"
    break;

  case 11:
#line 133 "parser.y"
      {
        (yyval.node) = NULL;
      }
#line 1800 "y.tab.c"
    break;

  case 12:
#line 141 "parser.y"
    {
      (yyval.node) = ASTids(NULL, (yyvsp[0].id));
    }
#line 1808 "y.tab.c"
    break;

  case 13:
#line 145 "parser.y"
  {
    (yyval.node) = ASTids((yyvsp[-2].node), (yyvsp[0].id));
  }
#line 1816 "y.tab.c"
    break;

  case 14:
#line 153 "parser.y"
     {
        (yyval.node) = ASTfuncall((yyvsp[-1].node), (yyvsp[-3].id));
      }
#line 1824 "y.tab.c"
    break;

  case 15:
#line 159 "parser.y"
    {
      (yyval.node) = ASTcast((yyvsp[0].node), (yyvsp[-2].ctype));
    }
#line 1832 "y.tab.c"
    break;

  case 16:
#line 166 "parser.y"
       {
         (yyval.node) = (yyvsp[0].node);
      }
#line 1840 "y.tab.c"
    break;

  case 17:
#line 170 "parser.y"
    {
       (yyval.node) = (yyvsp[0].node);
     }
#line 1848 "y.tab.c"
    break;

  case 18:
#line 175 "parser.y"
      {
        (yyval.node) = (yyvsp[0].node);
      }
#line 1856 "y.tab.c"
    break;

  case 19:
#line 184 "parser.y"
        {
          (yyval.node) = ASTfundef((yyvsp[0].node), (yyvsp[-2].node), (yyvsp[-4].id), (yyvsp[-5].ctype), false);
        }
#line 1864 "y.tab.c"
    break;

  case 20:
#line 189 "parser.y"
        {
          (yyval.node) = ASTfundef((yyvsp[0].node), (yyvsp[-2].node), (yyvsp[-4].id), (yyvsp[-5].ctype), true);
        }
#line 1872 "y.tab.c"
    break;

  case 21:
#line 197 "parser.y"
    {
      (yyval.node) = (yyvsp[-1].node);
    }
#line 1880 "y.tab.c"
    break;

  case 22:
#line 205 "parser.y"
       {
         (yyval.node) = (yyvsp[0].node);
       }
#line 1888 "y.tab.c"
    break;

  case 23:
#line 210 "parser.y"
      {
        (yyval.node) = NULL;
      }
#line 1896 "y.tab.c"
    break;

  case 24:
#line 218 "parser.y"
    {
      (yyval.node) = ASTblocks((yyvsp[0].node), NULL);
    }
#line 1904 "y.tab.c"
    break;

  case 25:
#line 222 "parser.y"
  {
    (yyval.node) = ASTblocks((yyvsp[0].node), (yyvsp[-1].node));
  }
#line 1912 "y.tab.c"
    break;

  case 26:
#line 229 "parser.y"
      {
        (yyval.node) = (yyvsp[0].node);
      }
#line 1920 "y.tab.c"
    break;

  case 27:
#line 234 "parser.y"
      {
        (yyval.node) = (yyvsp[0].node);
      }
#line 1928 "y.tab.c"
    break;

  case 28:
#line 239 "parser.y"
      {
        (yyval.node) = (yyvsp[0].node);
      }
#line 1936 "y.tab.c"
    break;

  case 29:
#line 245 "parser.y"
    {
        (yyval.node) = ASTfunbody((yyvsp[0].node));
    }
#line 1944 "y.tab.c"
    break;

  case 30:
#line 265 "parser.y"
    {
        (yyval.node) = ASTglobdef(NULL, NULL, (yyvsp[-1].id), (yyvsp[-2].ctype), true);
    }
#line 1952 "y.tab.c"
    break;

  case 31:
#line 269 "parser.y"
    {
        (yyval.node) = ASTglobdef(NULL, NULL, (yyvsp[-1].id), (yyvsp[-2].ctype), false);
    }
#line 1960 "y.tab.c"
    break;

  case 32:
#line 274 "parser.y"
    {
        (yyval.node) = ASTglobdef(NULL, (yyvsp[-1].node), (yyvsp[-3].id), (yyvsp[-4].ctype), true);
    }
#line 1968 "y.tab.c"
    break;

  case 33:
#line 278 "parser.y"
    {
        (yyval.node) = ASTglobdef(NULL, (yyvsp[-1].node), (yyvsp[-3].id), (yyvsp[-4].ctype), false);
    }
#line 1976 "y.tab.c"
    break;

  case 34:
#line 287 "parser.y"
                                                                    { (yyval.node) = ASTglobdecl((yyvsp[-3].node), (yyvsp[-1].id), (yyvsp[-5].ctype)); }
#line 1982 "y.tab.c"
    break;

  case 35:
#line 288 "parser.y"
                                { (yyval.node) = ASTglobdecl(NULL, (yyvsp[-1].id), (yyvsp[-2].ctype)); }
#line 1988 "y.tab.c"
    break;

  case 36:
#line 294 "parser.y"
    {
     (yyval.node) = NULL;
    }
#line 1996 "y.tab.c"
    break;

  case 37:
#line 298 "parser.y"
  {
    (yyval.node) = ASTstmts((yyvsp[0].node), (yyvsp[-1].node));
  }
#line 2004 "y.tab.c"
    break;

  case 38:
#line 306 "parser.y"
      {
        (yyval.node) = ASTparam((yyvsp[-2].node), (yyvsp[0].id), (yyvsp[-4].ctype));
      }
#line 2012 "y.tab.c"
    break;

  case 39:
#line 310 "parser.y"
      {
        (yyval.node) = ASTparam(NULL, (yyvsp[0].id), (yyvsp[-1].ctype));
      }
#line 2020 "y.tab.c"
    break;

  case 40:
#line 317 "parser.y"
       {
         (yyval.node) = (yyvsp[0].node);
       }
#line 2028 "y.tab.c"
    break;

  case 41:
#line 322 "parser.y"
      {
        (yyval.node) = NULL;
      }
#line 2036 "y.tab.c"
    break;

  case 42:
#line 330 "parser.y"
    {
      (yyval.node) = ASTparams((yyvsp[0].node), NULL);
    }
#line 2044 "y.tab.c"
    break;

  case 43:
#line 334 "parser.y"
  {
    (yyval.node) = ASTparams((yyvsp[0].node), (yyvsp[-2].node));
  }
#line 2052 "y.tab.c"
    break;

  case 44:
#line 342 "parser.y"
       {
         (yyval.node) = (yyvsp[0].node);
       }
#line 2060 "y.tab.c"
    break;

  case 45:
#line 346 "parser.y"
       {
         (yyval.node) = (yyvsp[0].node);
       }
#line 2068 "y.tab.c"
    break;

  case 46:
#line 350 "parser.y"
       {
         (yyval.node) = (yyvsp[0].node);
       }
#line 2076 "y.tab.c"
    break;

  case 47:
#line 354 "parser.y"
       {
        (yyval.node) = (yyvsp[0].node);
       }
#line 2084 "y.tab.c"
    break;

  case 48:
#line 358 "parser.y"
       {
         (yyval.node) = (yyvsp[0].node);
       }
#line 2092 "y.tab.c"
    break;

  case 49:
#line 362 "parser.y"
       {
         (yyval.node) = (yyvsp[0].node);
       }
#line 2100 "y.tab.c"
    break;

  case 50:
#line 367 "parser.y"
       {
         (yyval.node) = (yyvsp[0].node);
       }
#line 2108 "y.tab.c"
    break;

  case 51:
#line 373 "parser.y"
      {
        (yyval.node) = ASTreturn((yyvsp[-1].node));
      }
#line 2116 "y.tab.c"
    break;

  case 52:
#line 378 "parser.y"
      {
        (yyval.node) = ASTreturn(NULL);
      }
#line 2124 "y.tab.c"
    break;

  case 53:
#line 384 "parser.y"
           {
             (yyval.node) = ASTexprstmt((yyvsp[-1].node));
           }
#line 2132 "y.tab.c"
    break;

  case 54:
#line 390 "parser.y"
      {
        (yyval.node) = ASTifelse((yyvsp[-2].node), (yyvsp[0].node), NULL);
      }
#line 2140 "y.tab.c"
    break;

  case 55:
#line 394 "parser.y"
      {
       (yyval.node) = ASTifelse((yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node));
      }
#line 2148 "y.tab.c"
    break;

  case 56:
#line 405 "parser.y"
          {
            (yyval.node) = ASTwhile((yyvsp[-2].node), (yyvsp[0].node));
          }
#line 2156 "y.tab.c"
    break;

  case 57:
#line 412 "parser.y"
    {
      (yyval.node) = (yyvsp[-1].node);
    }
#line 2164 "y.tab.c"
    break;

  case 58:
#line 416 "parser.y"
    {
      (yyval.node) = ASTstmts((yyvsp[0].node), NULL);
    }
#line 2172 "y.tab.c"
    break;

  case 59:
#line 423 "parser.y"
    {
      (yyval.node) = ASTdowhile((yyvsp[-2].node), (yyvsp[-5].node));
    }
#line 2180 "y.tab.c"
    break;

  case 60:
#line 430 "parser.y"
        {
          (yyval.node) = ASTfor((yyvsp[-4].node), (yyvsp[-2].node), NULL, (yyvsp[0].node), (yyvsp[-6].id));
        }
#line 2188 "y.tab.c"
    break;

  case 61:
#line 435 "parser.y"
        {
          (yyval.node) = ASTfor((yyvsp[-6].node), (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node), (yyvsp[-8].id));
        }
#line 2196 "y.tab.c"
    break;

  case 62:
#line 443 "parser.y"
        {
          (yyval.node) = ASTassign((yyvsp[-3].node), (yyvsp[-1].node));
        }
#line 2204 "y.tab.c"
    break;

  case 63:
#line 451 "parser.y"
    {
        (yyval.node) = ASTvardecl(NULL, (yyvsp[0].node), (yyvsp[-1].id), (yyvsp[-2].ctype));
    }
#line 2212 "y.tab.c"
    break;

  case 64:
#line 456 "parser.y"
    {
        (yyval.node) = ASTvardecl((yyvsp[-3].node), (yyvsp[0].node), (yyvsp[-1].id), (yyvsp[-5].ctype));
    }
#line 2220 "y.tab.c"
    break;

  case 65:
#line 463 "parser.y"
    {
      (yyval.node) = NULL;
    }
#line 2228 "y.tab.c"
    break;

  case 66:
#line 468 "parser.y"
  {
    (yyval.node) = (yyvsp[-1].node);
  }
#line 2236 "y.tab.c"
    break;

  case 67:
#line 475 "parser.y"
    {
      (yyval.node) = ASTvarlet(NULL, (yyvsp[0].id));
    }
#line 2244 "y.tab.c"
    break;

  case 68:
#line 481 "parser.y"
    {
      (yyval.node) = ASTvar(NULL, (yyvsp[0].id));
    }
#line 2252 "y.tab.c"
    break;

  case 69:
#line 489 "parser.y"
      { (yyval.node) = ASTbinop((yyvsp[-2].node), (yyvsp[0].node), BO_add); }
#line 2258 "y.tab.c"
    break;

  case 70:
#line 491 "parser.y"
      { (yyval.node) = ASTbinop((yyvsp[-2].node), (yyvsp[0].node), BO_sub); }
#line 2264 "y.tab.c"
    break;

  case 71:
#line 493 "parser.y"
      { (yyval.node) = ASTbinop((yyvsp[-2].node), (yyvsp[0].node), BO_mul); }
#line 2270 "y.tab.c"
    break;

  case 72:
#line 495 "parser.y"
      { (yyval.node) = ASTbinop((yyvsp[-2].node), (yyvsp[0].node), BO_div); }
#line 2276 "y.tab.c"
    break;

  case 73:
#line 497 "parser.y"
      { (yyval.node) = ASTbinop((yyvsp[-2].node), (yyvsp[0].node), BO_mod); }
#line 2282 "y.tab.c"
    break;

  case 74:
#line 499 "parser.y"
      { (yyval.node) = ASTbinop((yyvsp[-2].node), (yyvsp[0].node), BO_eq); }
#line 2288 "y.tab.c"
    break;

  case 75:
#line 501 "parser.y"
      { (yyval.node) = ASTbinop((yyvsp[-2].node), (yyvsp[0].node), BO_ne); }
#line 2294 "y.tab.c"
    break;

  case 76:
#line 503 "parser.y"
      { (yyval.node) = ASTbinop((yyvsp[-2].node), (yyvsp[0].node), BO_lt); }
#line 2300 "y.tab.c"
    break;

  case 77:
#line 505 "parser.y"
      { (yyval.node) = ASTbinop((yyvsp[-2].node), (yyvsp[0].node), BO_le); }
#line 2306 "y.tab.c"
    break;

  case 78:
#line 507 "parser.y"
      { (yyval.node) = ASTbinop((yyvsp[-2].node), (yyvsp[0].node), BO_gt); }
#line 2312 "y.tab.c"
    break;

  case 79:
#line 509 "parser.y"
      { (yyval.node) = ASTbinop((yyvsp[-2].node), (yyvsp[0].node), BO_ge); }
#line 2318 "y.tab.c"
    break;

  case 80:
#line 511 "parser.y"
      { (yyval.node) = ASTbinop((yyvsp[-2].node), (yyvsp[0].node), BO_and); }
#line 2324 "y.tab.c"
    break;

  case 81:
#line 513 "parser.y"
      { (yyval.node) = ASTbinop((yyvsp[-2].node), (yyvsp[0].node), BO_or); }
#line 2330 "y.tab.c"
    break;

  case 82:
#line 515 "parser.y"
      { (yyval.node) = ASTmonop((yyvsp[0].node), MO_not); }
#line 2336 "y.tab.c"
    break;

  case 83:
#line 517 "parser.y"
      { (yyval.node) = ASTmonop((yyvsp[0].node), MO_neg); }
#line 2342 "y.tab.c"
    break;

  case 84:
#line 519 "parser.y"
    {
      (yyval.node) = (yyvsp[0].node);
   }
#line 2350 "y.tab.c"
    break;

  case 85:
#line 523 "parser.y"
      {
        (yyval.node) = (yyvsp[0].node);
      }
#line 2358 "y.tab.c"
    break;

  case 86:
#line 527 "parser.y"
      {
        (yyval.node) = (yyvsp[0].node);
      }
#line 2366 "y.tab.c"
    break;

  case 87:
#line 531 "parser.y"
     {
        (yyval.node) = (yyvsp[0].node);
     }
#line 2374 "y.tab.c"
    break;

  case 88:
#line 535 "parser.y"
      {
        (yyval.node) = (yyvsp[0].node);
    }
#line 2382 "y.tab.c"
    break;

  case 89:
#line 541 "parser.y"
          {
            (yyval.node) = (yyvsp[0].node);
          }
#line 2390 "y.tab.c"
    break;

  case 90:
#line 545 "parser.y"
          {
            (yyval.node) = (yyvsp[0].node);
          }
#line 2398 "y.tab.c"
    break;

  case 91:
#line 549 "parser.y"
          {
            (yyval.node) = (yyvsp[0].node);
          }
#line 2406 "y.tab.c"
    break;

  case 92:
#line 555 "parser.y"
           {
            (yyval.node) = ASTfloat((yyvsp[0].cflt));
           }
#line 2414 "y.tab.c"
    break;

  case 93:
#line 561 "parser.y"
        {
          (yyval.node) = ASTnum((yyvsp[0].cint));
        }
#line 2422 "y.tab.c"
    break;

  case 94:
#line 567 "parser.y"
         {
           (yyval.node) = ASTbool(true);
         }
#line 2430 "y.tab.c"
    break;

  case 95:
#line 571 "parser.y"
         {
           (yyval.node) = ASTbool(false);
         }
#line 2438 "y.tab.c"
    break;

  case 96:
#line 578 "parser.y"
                { (yyval.ctype) = CT_int; }
#line 2444 "y.tab.c"
    break;

  case 97:
#line 579 "parser.y"
                  { (yyval.ctype) = CT_float; }
#line 2450 "y.tab.c"
    break;

  case 98:
#line 580 "parser.y"
                 { (yyval.ctype) = CT_bool; }
#line 2456 "y.tab.c"
    break;

  case 99:
#line 581 "parser.y"
                 { (yyval.ctype) = CT_void; }
#line 2462 "y.tab.c"
    break;


#line 2466 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
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


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 584 "parser.y"


void AddLocToNode(node_st *node, void *begin_loc, void *end_loc)
{
    // Needed because YYLTYPE unpacks later than top-level decl.
    YYLTYPE *loc_b = (YYLTYPE*)begin_loc;
    YYLTYPE *loc_e = (YYLTYPE*)end_loc;
    NODE_BLINE(node) = loc_b->first_line;
    NODE_BCOL(node) = loc_b->first_column;
    NODE_ELINE(node) = loc_e->last_line;
    NODE_ECOL(node) = loc_e->last_column;
}

int yyerror(char *error)
{
  CTI(CTI_ERROR, true, "line %d, col %d\nError parsing source code: %s\n",
            global.line, global.col, error);
  CTIabortOnError();
  return 0;
}

node_st *SPdoScanParse(node_st *root)
{
    DBUG_ASSERT(root == NULL, "Started parsing with existing syntax tree.");
    yyin = fopen(global.input_file, "r");
    if (yyin == NULL) {
        CTI(CTI_ERROR, true, "Cannot open file '%s'.", global.input_file);
        CTIabortOnError();
    }
    yyparse();
    return parseresult;
}
