%{

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ccngen/ast.h"
#include "ccngen/enum.h"
#include "parser.h"
#include "palm/str.h"
#include "global/globals.h"
#include "palm/ctinfo.h"
#include <limits.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

// Use for error messages, see parser.y
extern int yyerror(char *errname);

#define FILTER(token) \
  global.col += yyleng;  \
  return( token);

#define LINEBUF_SIZE 256
#define YY_NO_UNPUT

static inline void token_action();

int yycolumn = 1;
#define YY_USER_ACTION token_action();


%}

%p 6000 /* set positions to 6000 */
%e 2000 /* set nodes to 1100 */
%option noyywrap
%option nounput
%option yylineno


%%

"("                        { FILTER( BRACKET_L); }
")"                        { FILTER( BRACKET_R); }
","                        { FILTER( COMMA); }
";"                        { FILTER( SEMICOLON); }
"{"                        { FILTER( CURLY_BRACKET_L); }
"}"                        { FILTER( CURLY_BRACKET_R); }
"["                        { FILTER( SQUARE_BRACKET_L); }
"]"                        { FILTER( SQUARE_BRACKET_R); }

"-"                        { FILTER( MINUS); }
"+"                        { FILTER( PLUS);  }
"*"                        { FILTER( STAR);  }
"/"                        { FILTER( SLASH);  }
"%"                        { FILTER( MOD);  }

"<="                       { FILTER( LE);    }
"<"                        { FILTER( LT);    }
">="                       { FILTER( GE);    }
">"                        { FILTER( GT);    }
"=="                       { FILTER( EQ);    }
"!="                       { FILTER( NE);    }
"&&"                       { FILTER( AND);    }
"||"                       { FILTER( OR);    }

"="                        { FILTER( LET);    }
"!"                        { FILTER( NOT);    }

"true"                     { FILTER( TRUEVAL); }
"false"                    { FILTER( FALSEVAL); }

"int"                      { FILTER( INT_TYPE); }
"float"                    { FILTER( FLOAT_TYPE); }
"bool"                     { FILTER( BOOL_TYPE); }
"void"                     { FILTER( VOID_TYPE); }

"export"                   { FILTER( EXPORT); }
"extern"                   { FILTER( EXTERN); }
"return"                   { FILTER( RETURN); }
"if"                       { FILTER( IF); }
"else"                     { FILTER( ELSE); }
"while"                    { FILTER( WHILE); }
"do"                       { FILTER( DO); }
"for"                      { FILTER( FOR); }

[A-Za-z][A-Za-z0-9_]*      { yylval.id = STRcpy(yytext);
                             FILTER( ID);
                           }

[0-9]+                     { char *endptr;
long int num = strtol(yytext, &endptr, 10);

// Check if the whole string was valid and no overflow/underflow occurred
if (*endptr == '\0' && errno != ERANGE) {
    // Check if it's within the 32-bit int range
    if (num >= INT_MIN && num <= INT_MAX) {
        yylval.cint = (int)num;
        FILTER(NUM);
    } else {
        // Handle out-of-range error
        fprintf(stderr, "Number out of range for 32-bit int: %s\n", yytext);
        // You might want to set an error flag or take other appropriate action here
    }
} else {
    // Handle invalid input
    fprintf(stderr, "Invalid integer: %s\n", yytext);
    // You might want to set an error flag or take other appropriate action here
}
                           }

[0-9]+"."[0-9]+([eE][-+]?[0-9]+)?   { yylval.cflt = atof(yytext);
                                     FILTER( FLOAT);
                                   }

"/*"                        {
                                char c, prev = 0;
                                while((c = input()) != 0) {
                                    if(c == '\n') {
                                        global.line++; // Increment line counter
                                        yycolumn = 1; // Reset column counter for the new line
                                    }
                                    if(prev == '*' && c == '/') break; // End of comment
                                    prev = c;
                                }
                            }

"//" {
    char c;
    while((c = input()) != '\n' && c != EOF); /* Ignore single line comments */
}

\n.*                       {
                             yycolumn = 1;
                             global.line += 1;
                             global.col = 0;
                             yyless(1);
                           }

[ \t]                      { global.col += yyleng;
                           }
%%

static inline void token_action() {
    yylloc.first_line = yylloc.last_line = yylineno;
    yylloc.first_column = yycolumn;
    yylloc.last_column = yycolumn + yyleng - 1;
    yycolumn += yyleng;
}
