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
 bool               boolean;
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

%type <boolean> opt_export opt_extern
%type <node> intval floatval boolval constants expr exprs
%type <node> stmts stmt varlet program
%type <node> fundef fundefs vardecl vardecls
%type <node> param params
%type <ctype> ctype
%type <node> decls decl funbody
//%type globdecl globdef funcall exprstmt for arrexpr
%type <node> ids
%type <node> assign ifelse while dowhile return cast var

%left OR
%left AND
%left EQ NE
%left LT LE GT GE
%left PLUS MINUS
%left STAR SLASH MOD
%right NOT
%right UMINUS

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
        expr COMMA exprs
       {
         $$ = ASTexprs($1, $3);
       }
    |
     %empty
       {
         $$ = NULL;
       }
     ;

//arrexpr: SQUARE_BRACKET_L exprs SQUARE_BRACKET_R
//         {
//           $$ = $2;
//         }
//       | SQUARE_BRACKET_L expr SQUARE_BRACKET_R
//         {
//           $$ = ASTarrexpr($2);
//         }
//         ;



ids: ID
      {
       $$ = ASTids(NULL, $1);
      }
    | ids COMMA ID
      {
        $$ = ASTids($1, $3);
      }
      ;


//funcall: ID BRACKET_L exprs BRACKET_R SEMICOLON
//     {
//        $$ = ASTfuncall($1);
//      }
//      ;

cast: BRACKET_L ctype BRACKET_R expr SEMICOLON
    {
      $$ = ASTcast($4, $2);
    }
    ;

decl: 
//     globdecl
//       {
//         $$ = $1;
//      }
//     | globdef
//    {
//       $$ = $1;
//     }
//      | 
      fundef
      {
        $$ = $1;
      }
      ;



fundef: 
        opt_export ctype ID BRACKET_L params BRACKET_R CURLY_BRACKET_L funbody CURLY_BRACKET_R
        {
          $$ = ASTfundef($8, $5, $3, $2, $1);
        }
        ;

opt_export: EXPORT
    {
      $$ = true;
    }
    | %empty
    {
      $$ = false;
    }
    ;

opt_extern: EXTERN
    {
      $$ = true;
    }
    | %empty
    {
      $$ = false;
    }
    ;

fundefs:
        fundef fundefs
       {
         $$ = ASTfundefs($1, $2);
       }
    |
     %empty
       {
         $$ = NULL;
       }
     ;

funbody: vardecls fundefs stmts
         {
           $$ = ASTfunbody($1, $2, $3);
         }
         ;


//globdef: 
//            opt_export ctype ID expr SEMICOLON
//       {
//          $$ = ASTglobdef($3, $2);
//       }
//
//        ;




//globdecl: EXTERN ctype ID SEMICOLON
//         {
//           $$ = ASTglobdecl($3, $2);
//         }
//       | EXTERN ctype SQUARE_BRACKET_L ids SQUARE_BRACKET_R ID SEMICOLON
//         {
//           $$ = ASTglobdecl($6, $2);
//        }
//         ;

stmts:
        stmt stmts
        {
         $$ = ASTstmts($1, $2);
        }
        |
        %empty
        {
          $$ = NULL;
       }
       ;




param: ctype SQUARE_BRACKET_L ids SQUARE_BRACKET_R ID
      {
        $$ = ASTparam($5, $1);
      }
    | ctype ID
      {
        $$ = ASTparam($2, $1);
      }
      ;

params:
        param COMMA params
       {
         $$ = ASTparams($1, $3);
       }
    |
     param
       {
         $$ = ASTparams($1, NULL);
       }
    |
     %empty
       {
         $$ = NULL;
       }
     ;

stmt: 
        assign
       {
         $$ = $1;
       }
//       | exprstmt
//       {
//         $$ = $1;
//       }
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
//       | for
//       {
//         $$ = $1;
//       }
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

//exprstmt: expr SEMICOLON
//           {
//             $$ = ASTexprstmt($1);
//           }
//           ;

ifelse: IF BRACKET_L expr BRACKET_R stmts
      {
        $$ = ASTifelse($3, $5, NULL);
      }
     | IF BRACKET_L expr BRACKET_R stmts ELSE stmts
      {
       $$ = ASTifelse($3, $5, $7);
      }
      ;



while: WHILE BRACKET_L expr BRACKET_R CURLY_BRACKET_L stmts CURLY_BRACKET_R
          {
            $$ = ASTwhile($3, $6);
          }
          ;

dowhile: DO CURLY_BRACKET_L stmts CURLY_BRACKET_R WHILE BRACKET_L expr BRACKET_R SEMICOLON
        {
          $$ = ASTdowhile($7, $3);
        }
        ;

//for:   FOR BRACKET_L ctype varlet LET expr COMMA expr BRACKET_R CURLY_BRACKET_L stmts CURLY_BRACKET_R
//        {
//          $$ = ASTfor($6, $8);
//        }
//        ;



assign: varlet LET expr SEMICOLON
        {
          $$ = ASTassign($1, $3);
        }
       ;

vardecls:
        vardecl vardecls
       {
         $$ = ASTvardecls($1, $2);
       }
    |
     %empty
       {
         $$ = NULL;
       }
     ;


vardecl:
    ctype ID SQUARE_BRACKET_L exprs SQUARE_BRACKET_R LET expr SEMICOLON { $$ = ASTvardecl($4, $7, $2, $1); }
    | ctype ID SQUARE_BRACKET_L exprs SQUARE_BRACKET_R SEMICOLON { $$ = ASTvardecl($4, NULL, $2, $1); }
    | ctype ID LET expr SEMICOLON { $$ = ASTvardecl(NULL, $4, $2, $1); }
    | ctype ID SEMICOLON { $$ = ASTvardecl(NULL, NULL, $2, $1); }
;

varlet: ID
    {
      $$ = ASTvarlet($1);
    }
    ;

var: ID
    {
      $$ = ASTvar($1);
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
    | BRACKET_L expr BRACKET_R
      { $$ = $2; }
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
//    | funcall
//     {
//        $$ = $1;
//     }
//    | arrexpr
//      {
//        $$ = $1;
//    }
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
