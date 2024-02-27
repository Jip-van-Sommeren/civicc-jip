%{


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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


%}

%union {
 char               *id;
 int                 cint;
 float               cflt;
 enum BinOpType     cbinop;
 enum MonOpType     cmonop;
 enum Type           ctype;
 node_st             *node;
}

%locations

%token BRACKET_L BRACKET_R COMMA SEMICOLON
%token MINUS PLUS STAR SLASH PERCENT LE LT GE GT EQ NE OR AND
%token NOT NEG
%token TRUEVAL FALSEVAL LET
%token INT_TYPE FLOAT_TYPE BOOL_TYPE VOID_TYPE

%token <cint> NUM
%token <cflt> FLOAT
%token <id> ID

%type <node> intval floatval boolval constant expr
%type <node> stmts stmt assign varlet program
%type <cbinop> binop
%type <cmonop> monop
%type <ctype> dtype
%type <node> decls decl globdecl


%start program

%%


program: decls
         {
           parseresult = ASTprogram($1);
         }
         | stmts
         {
           parseresult = ASTprogram($1);
         }
         ;

decls: decl decls
       {
         $$ = ASTdecls($1, $2);
       }
     | decl
       {
         $$ = ASTdecls($1, NULL);
       }
     ;

decl: globdecl
        {
          $$ = $1;
        }
      ;

globdecl: dtype ID[name] SEMICOLON
          {
            $$ = ASTglobdecl($name, $1);
            AddLocToNode($$, &@1, &@name);
          }
         ;

stmts: stmt stmts
        {
          $$ = ASTstmts($1, $2);
        }
      | stmt
        {
          $$ = ASTstmts($1, NULL);
        }
        ;

stmt: assign
       {
         $$ = $1;
       }
       ;

assign: varlet LET expr SEMICOLON
        {
          $$ = ASTassign($1, $3);
        }
        ;

varlet: ID
        {
          $$ = ASTvarlet($1);
          AddLocToNode($$, &@1, &@1);
        }
        ;


expr: constant
      {
        $$ = $1;
      }
    | ID
      {
        $$ = ASTvar($1);
      }
    | BRACKET_L expr[left] binop[type] expr[right] BRACKET_R
      {
        $$ = ASTbinop( $left, $right, $type);
        AddLocToNode($$, &@left, &@right);
      }
    | monop[type] expr[operand]
      {
        $$ = ASTmonop($operand, $type);
        AddLocToNode($$, &@type, &@operand);
      }
    ;

constant: floatval
          {
            $$ = $1;
          }
        | intval
          {
            $$ = $1;
          }
        | boolval
          {
            $$ = $1;
          }
        ;

floatval: FLOAT
           {
             $$ = ASTfloat($1);
           }
         ;

intval: NUM
        {
          $$ = ASTnum($1);
        }
      ;

boolval: TRUEVAL
         {
           $$ = ASTbool(true);
         }
       | FALSEVAL
         {
           $$ = ASTbool(false);
         }
       ;

binop: PLUS      { $$ = BO_add; }
     | MINUS     { $$ = BO_sub; }
     | STAR      { $$ = BO_mul; }
     | SLASH     { $$ = BO_div; }
     | PERCENT   { $$ = BO_mod; }
     | LE        { $$ = BO_le; }
     | LT        { $$ = BO_lt; }
     | GE        { $$ = BO_ge; }
     | GT        { $$ = BO_gt; }
     | EQ        { $$ = BO_eq; }
     | OR        { $$ = BO_or; }
     | AND       { $$ = BO_and; }
     ;

monop: NOT { $$ = MO_not; }
     | NEG { $$ = MO_neg; }
     ;

dtype: INT_TYPE { $$ = CT_int; }
     | FLOAT_TYPE { $$ = CT_float; }
     | BOOL_TYPE { $$ = CT_bool; }
     | VOID_TYPE { $$ = CT_void; }
     ;

%%

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
