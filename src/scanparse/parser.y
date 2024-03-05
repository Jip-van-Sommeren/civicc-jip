%{


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


%}

%union {
 char               *id;
 int                 cint;
 float               cflt;
 enum Type           ctype;
 node_st             *node;
}

%locations

%token BRACKET_L BRACKET_R COMMA SEMICOLON CURLY_BRACKET_L CURLY_BRACKET_R SQUARE_BRACKET_L SQUARE_BRACKET_R
%token MINUS PLUS STAR SLASH MOD LE LT GE GT EQ NE OR AND
%token NOT
%token TRUEVAL FALSEVAL LET
%token INT_TYPE FLOAT_TYPE BOOL_TYPE VOID_TYPE
%token EXPORT RETURN IF ELSE WHILE DO FOR EXTERN

%token <cint> NUM
%token <cflt> FLOAT
%token <id> ID


%type <node> intval floatval boolval constants expr exprs
%type <node> stmts stmt varlet program
%type <node> fundef vardecl 
%type <node> param params
%type <ctype> ctype
%type <node> decls decl funbody
%type <node> arrexpr
%type <node> globdecl globdef funcall exprstmt for stmt_block var_decl_tail
%type <node> ids
%type <node> assign ifelse while dowhile return cast var compound_statement 
%type <node> params_nonempty ids_nonempty exprs_nonempty blocks_nonempty
%type <node> block blocks


%left OR
%left AND
%left EQ NE
%left LT LE GT GE
%left PLUS MINUS
%left STAR SLASH MOD
%right NOT
%right UMINUS
%right THEN ELSE 

%start program

%%


program: decls
         {
           parseresult = ASTprogram($1);
         }
         ;


decls:
        decl decls
       {
         $$ = ASTdecls($1, $2);
       }
     |
      %empty
      {
          $$ = NULL;
      }



exprs:
        exprs_nonempty
       {
         $$ = $1;
       }
    |
       %empty 
      {
        $$ = NULL;
      }
     ;


exprs_nonempty:
    expr
    {
      $$ = ASTexprs($1, NULL);
    }
  | exprs COMMA expr
  {
    $$ = ASTexprs($3, $1);
  }
  ;     

arrexpr: SQUARE_BRACKET_L exprs SQUARE_BRACKET_R
         {
           $$ = ASTarrexpr($2);
         }
       ;

ids:
        ids_nonempty
       {
         $$ = $1;
       }
    |
       %empty 
      {
        $$ = NULL;
      }
     ;


ids_nonempty:
    ID
    {
      $$ = ASTids(NULL, $1);
    }
  | ids COMMA ID
  {
    $$ = ASTids($1, $3);
  }
  ;



funcall: ID BRACKET_L exprs BRACKET_R
     {
        $$ = ASTfuncall($3, $1);
      }
      ;

cast: BRACKET_L ctype BRACKET_R expr
    {
      $$ = ASTcast($4, $2);
    }
    ;

decl: 
     globdecl
       {
         $$ = $1;
      }
     | globdef
    {
       $$ = $1;
     }
      | 
      fundef
      {
        $$ = $1;
      }
      ;



fundef:
        ctype ID BRACKET_L params BRACKET_R compound_statement
        {
          $$ = ASTfundef($6, $4, $2, $1, false);
        }
        |
        EXPORT ctype ID BRACKET_L params BRACKET_R compound_statement
        {
          $$ = ASTfundef($7, $5, $3, $2, true);
        }
        ;


compound_statement:
    CURLY_BRACKET_L funbody CURLY_BRACKET_R
    {
      $$ = $2;
    }
  ;


blocks:
        blocks_nonempty
       {
         $$ = $1;
       }
    |
       %empty
      {
        $$ = NULL;
      }
     ;


blocks_nonempty:
    block
    {
      $$ = ASTblocks($1, NULL);
    }
  | blocks block
  {
    $$ = ASTblocks($2, $1);
  }
  ;

block: 
      vardecl
      {
        $$ = $1;
      }
      |
      fundef
      {
        $$ = $1;
      }
      |
      stmt
      {
        $$ = $1;
      }

funbody:
    blocks
    {
        $$ = ASTfunbody($1);
    }
    ;




//opt_init:
//    LET expr { $$ = $2; }
//    | %empty { $$ = NULL; }
//    ;

//opt_arr_init:
//    EQUAL arr_expr { $$ = $2; }
//    | /* empty */ { $$ = NULL; }
//    ;

globdef:
    EXPORT ctype ID SEMICOLON
    {
        $$ = ASTglobdef(NULL, NULL, $3, $2, true);
    }
    | ctype ID SEMICOLON
    {
        $$ = ASTglobdef(NULL, NULL, $2, $1, false);
    }
    |
    EXPORT ctype ID LET expr SEMICOLON
    {
        $$ = ASTglobdef(NULL, $5, $3, $2, true);
    }
    | ctype ID LET expr SEMICOLON
    {
        $$ = ASTglobdef(NULL, $4, $2, $1, false);
    }
    ;




globdecl:
    EXTERN ctype SQUARE_BRACKET_L ids SQUARE_BRACKET_R ID SEMICOLON { $$ = ASTglobdecl($4, $6, $2); }
    | EXTERN ctype ID SEMICOLON { $$ = ASTglobdecl(NULL, $3, $2); }
;


stmts:
    %empty
    {
     $$ = NULL;
    }
  | stmts stmt
  {
    $$ = ASTstmts($2, $1);
  }
  ;



param: ctype SQUARE_BRACKET_L ids SQUARE_BRACKET_R ID
      {
        $$ = ASTparam($3, $5, $1);
      }
    | ctype ID
      {
        $$ = ASTparam(NULL, $2, $1);
      }
      ;

params:
        params_nonempty
       {
         $$ = $1;
       }
    |
       %empty 
      {
        $$ = NULL;
      }
     ;


params_nonempty:
    param
    {
      $$ = ASTparams($1, NULL);
    }
  | params COMMA param
  {
    $$ = ASTparams($3, $1);
  }
  ;


stmt:
        assign
       {
         $$ = $1;
       }
       | exprstmt
       {
         $$ = $1;
       }
       | ifelse
       {
         $$ = $1;
       }
       | while
       {
        $$ = $1;
       }
       | dowhile
       {
         $$ = $1;
       }
       | for
       {
         $$ = $1;
       }
       |
      return
       {
         $$ = $1;
       }
       ;

return: RETURN expr SEMICOLON
      {
        $$ = ASTreturn($2);
      }
      |
      RETURN SEMICOLON
      {
        $$ = ASTreturn(NULL);
      }
      ;

exprstmt: expr SEMICOLON
           {
             $$ = ASTexprstmt($1);
           }
           ;

ifelse: IF BRACKET_L expr BRACKET_R stmt_block            %prec THEN
      {
        $$ = ASTifelse($3, $5, NULL);
      }
     | IF BRACKET_L expr BRACKET_R stmt_block ELSE stmt_block
      {
       $$ = ASTifelse($3, $5, $7);
      }
      ;






while: WHILE BRACKET_L expr BRACKET_R stmt_block
          {
            $$ = ASTwhile($3, $5);
          }
          ;

stmt_block:
    CURLY_BRACKET_L stmts CURLY_BRACKET_R
    {
      $$ = $2;
    }
  | stmt
    {
      $$ = ASTstmts($1, NULL);
    }
  ;

dowhile:
    DO stmt_block WHILE BRACKET_L expr BRACKET_R SEMICOLON
    {
      $$ = ASTdowhile($5, $2);
    }
    ;


for:   FOR BRACKET_L INT_TYPE ID LET expr COMMA expr BRACKET_R stmt_block
        {
          $$ = ASTfor($6, $8, NULL, $10, $4);
        }
        |
        FOR BRACKET_L INT_TYPE ID LET expr COMMA expr COMMA expr BRACKET_R stmt_block
        {
          $$ = ASTfor($6, $8, $10, $12, $4);
        }
        ;



assign: varlet LET expr SEMICOLON
        {
          $$ = ASTassign($1, $3);
        }
       ;


vardecl:
    ctype ID var_decl_tail
    {
        $$ = ASTvardecl(NULL, $3, $2, $1);
    }
    |
        ctype SQUARE_BRACKET_L exprs SQUARE_BRACKET_R ID var_decl_tail
    {
        $$ = ASTvardecl($3, $6, $5, $1);
    }
  ;

var_decl_tail:
    SEMICOLON
    {
      $$ = NULL;
    }

  | LET expr SEMICOLON
  {
    $$ = $2;
  }
  ;


varlet: ID
    {
      $$ = ASTvarlet(NULL, $1);
    }
    ;

var: ID
    {
      $$ = ASTvar(NULL, $1);
    }
    ;



expr: expr PLUS expr
      { $$ = ASTbinop($1, $3, BO_add); }
    | expr MINUS expr
      { $$ = ASTbinop($1, $3, BO_sub); }
    | expr STAR expr
      { $$ = ASTbinop($1, $3, BO_mul); }
    | expr SLASH expr
      { $$ = ASTbinop($1, $3, BO_div); }
    | expr MOD expr
      { $$ = ASTbinop($1, $3, BO_mod); }
    | expr EQ expr
      { $$ = ASTbinop($1, $3, BO_eq); }
    | expr NE expr
      { $$ = ASTbinop($1, $3, BO_ne); }
    | expr LT expr
      { $$ = ASTbinop($1, $3, BO_lt); }
    | expr LE expr
      { $$ = ASTbinop($1, $3, BO_le); }
    | expr GT expr
      { $$ = ASTbinop($1, $3, BO_gt); }
    | expr GE expr
      { $$ = ASTbinop($1, $3, BO_ge); }
    | expr AND expr
      { $$ = ASTbinop($1, $3, BO_and); }
    | expr OR expr
      { $$ = ASTbinop($1, $3, BO_or); }
    | NOT expr
      { $$ = ASTmonop($2, MO_not); }
    | MINUS expr %prec UMINUS
      { $$ = ASTmonop($2, MO_neg); }
    | cast
    {
      $$ = $1;
   }
    | constants
      {
        $$ = $1;
      }
    | var
      {
        $$ = $1;
      }
      | funcall
     {
        $$ = $1;
     }
    | arrexpr
      {
        $$ = $1;
    }
    ;

 constants: floatval
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



ctype: INT_TYPE { $$ = CT_int; }
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
