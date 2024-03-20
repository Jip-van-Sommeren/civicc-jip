/**
 * @file
 *
 * This file contains the code for the Print traversal.
 * The traversal has the uid: PRT
 *
 *
 */

#include <stdio.h>
#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "palm/dbug.h"
#include "ccngen/enum.h"
#include "vartypetostring.h"

/**
 * @fn PRTprogram
 */
node_st *PRTprogram(node_st *node)
{
    printf("Program:\n");
    TRAVdo(PROGRAM_SYMBOLTABLE(node));
    TRAVdo(PROGRAM_DECLS(node));
    return node;
}

/**
 * @fn PRTdecls
 */
node_st *PRTdecls(node_st *node)
{
    TRAVdo(DECLS_DECL(node));

    // If there is another expression in the list, print a comma and continue printing

    node_st *decls = DECLS_NEXT(node);

    while (decls != NULL)
    {
        TRAVdo(DECLS_DECL(decls));
        printf(", ");
        // Move to the next set of declessions
        decls = DECLS_NEXT(decls);
    }

    return node;
}

/**
 * @fn PRTexprs
 */
node_st *PRTexprs(node_st *node)
{
    // Print the first expression in the list
    TRAVdo(EXPRS_EXPR(node));

    // If there is another expression in the list, print a comma and continue printing

    node_st *exprs = EXPRS_NEXT(node);

    while (exprs != NULL)
    {
        printf(", ");
        TRAVdo(EXPRS_EXPR(exprs));
        // Move to the next set of expressions
        exprs = EXPRS_NEXT(exprs);
    }

    return node;
}

/**
 * @fn PRTarrExpr
 */
node_st *PRTarrexpr(node_st *node)
{
    printf("[ ");
    // Print the expressions that make up the array expression
    TRAVopt(ARREXPR_EXPRS(node));
    printf(" ]");
    return node;
}

/**
 * @fn PRTids
 */
node_st *PRTids(node_st *node)
{
    // Print the name of the identifier
    printf("%s", IDS_NAME(node));

    // If there is another expression in the list, print a comma and continue printing

    node_st *ids = IDS_NEXT(node);

    while (ids != NULL)
    {
        printf(", ");
        printf("%s", IDS_NAME(ids));
        // Move to the next set of idsessions
        ids = IDS_NEXT(ids);
    }

    return node;
}

/**
 * @fn PRTexprStmt
 */
node_st *PRTexprstmt(node_st *node)
{
    // Print the expression part of the statement
    TRAVdo(EXPRSTMT_EXPR(node));
    printf(";\n");
    return node;
}

/**
 * @fn PRTreturn
 */
node_st *PRTreturn(node_st *node)
{
    printf("return");
    // If there is an expression to return, print it
    if (RETURN_EXPR(node) != NULL)
    {
        printf(" ");
        TRAVdo(RETURN_EXPR(node));
    }
    printf(";\n");
    return node;
}

/**
 * @fn PRTfunCall
 */
node_st *PRTfuncall(node_st *node)
{
    printf("%s(", FUNCALL_NAME(node));
    TRAVopt(FUNCALL_FUN_ARGS(node));
    printf(")");
    return node;
}

/**
 * @fn PRTcast
 */
node_st *PRTcast(node_st *node)
{

    printf("Cast to %s: (", VarTypeToString(CAST_TYPE(node)));
    TRAVdo(CAST_EXPR(node));
    printf(")\n");
    return node;
}

/**
 * @fn PRTfunDef
 */
node_st *PRTfundef(node_st *node)
{
    // Directly integrate the type conversion logic

    printf("Function Definition: %s\n", FUNDEF_NAME(node));
    printf("Return Type: %s\n", VarTypeToString(FUNDEF_TYPE(node)));
    TRAVopt(FUNDEF_BODY(node));
    TRAVopt(FUNDEF_PARAMS(node));
    printf("%-20s %-15s %-10s %-10s\n", "Name", "Type", "Scope", "Line No");
    printf("-------------------------------------------------------------\n");
    TRAVopt(FUNDEF_SYMBOLTABLE(node));
    printf("-------------------------------------------------------------\n");
    return node;
}

/**
 * @fn PRTfunBody
 */
node_st *PRTfunbody(node_st *node)
{
    printf("Function block:\n");
    TRAVopt(FUNBODY_VARDECLS(node));

    TRAVopt(FUNBODY_LOCAL_FUNDEFS(node));

    TRAVopt(FUNBODY_STMTS(node));
    return node;
}

/**
 * @fn PRTifElse
 */
node_st *PRTifelse(node_st *node)
{
    printf("if (");
    TRAVopt(IFELSE_COND(node));
    printf(") ");
    TRAVdo(IFELSE_THEN(node));
    if (IFELSE_ELSE_BLOCK(node) != NULL)
    {
        printf(" else ");
        TRAVdo(IFELSE_ELSE_BLOCK(node));
    }
    return node;
}

/**
 * @fn PRTwhile
 */
node_st *PRTwhile(node_st *node)
{
    printf("while (");
    TRAVdo(WHILE_COND(node));
    printf(") {\n");
    TRAVdo(WHILE_BLOCK(node));
    printf("}\n");
    return node;
}

/**
 * @fn PRTdoWhile
 */
node_st *PRTdowhile(node_st *node)
{
    printf("do {\n");
    // Print the block of statements
    TRAVopt(DOWHILE_BLOCK(node));
    printf("} while (");
    // Print the condition expression
    TRAVdo(DOWHILE_COND(node));
    printf(");\n");
    return node;
}

/**
 * @fn PRTfor
 */
node_st *PRTfor(node_st *node)
{
    printf("for (");
    // Print the start expression of the for loop
    TRAVdo(FOR_START_EXPR(node));
    printf("; ");
    // Print the stop condition of the for loop
    TRAVdo(FOR_STOP(node));
    printf("; ");
    // Print the step expression if it exists
    if (FOR_STEP(node) != NULL)
    {
        TRAVdo(FOR_STEP(node));
    }
    printf(") {\n");
    // Print the block of statements
    TRAVopt(FOR_BLOCK(node));
    printf("}\n");
    return node;
}

/**
 * @fn PRTglobDecl
 */
node_st *PRTglobdecl(node_st *node)
{

    printf("%s %s;\n", VarTypeToString(GLOBDECL_TYPE(node)), GLOBDECL_NAME(node));
    // Assuming dims is an optional child
    TRAVopt(GLOBDECL_DIMS(node));
    TRAVopt(GLOBDECL_PARAMS(node));
    printf("\n");
    return node;
}

/**
 * @fn PRTglobDef
 */
node_st *PRTglobdef(node_st *node)
{
    printf("Global Definition: %s\n", GLOBDEF_NAME(node));
    // Assuming dims and init are optional children
    TRAVopt(GLOBDEF_DIMS(node));
    if (GLOBDEF_INIT(node) != NULL)
    {
        printf(" = ");
        TRAVdo(GLOBDEF_INIT(node));
    }
    printf(";\n");
    return node;
}

/**
 * @fn PRTvardecls
 */
node_st *PRTvardecls(node_st *node)
{
    // Print the current function definition
    TRAVdo(VARDECLS_VARDECL(node));

    // If there is another expression in the list, print a comma and continue printing

    node_st *vardecls = VARDECLS_NEXT(node);

    while (vardecls != NULL)
    {
        TRAVdo(VARDECLS_VARDECL(vardecls));
        // Move to the next set of declessions
        vardecls = VARDECLS_NEXT(vardecls);
    }

    return node;
}

/**
 * @fn PRTsymboltable
 */
node_st *PRTsymboltable(node_st *node)
{
    // Print the current function definition
    TRAVdo(SYMBOLTABLE_ENTRY(node));

    // If there is another expression in the list, print a comma and continue printing

    node_st *symboltable = SYMBOLTABLE_NEXT(node);

    while (symboltable != NULL)
    {
        TRAVdo(SYMBOLTABLE_ENTRY(symboltable));
        // Move to the next set of declessions
        symboltable = SYMBOLTABLE_NEXT(symboltable);
    }

    return node;
}

/**
 * @fn PRTsymboltable
 */
node_st *PRTsymbolentry(node_st *node)
{
    // Print the current function definition
    char *typestr = VarTypeToString(SYMBOLENTRY_TYPE(node));
    printf("%-20s %-15s %-10d %-10d\n", SYMBOLENTRY_NAME(node), typestr, SYMBOLENTRY_SCOPELEVEL(node), SYMBOLENTRY_DECLAREDATLINE(node));
    return node;
}
/**
 * @fn PRTfundefs
 */
node_st *PRTfundefs(node_st *node)
{
    // Print the current function definition
    TRAVdo(FUNDEFS_FUNDEF(node));

    // If there is another expression in the list, print a comma and continue printing

    node_st *fundefs = FUNDEFS_NEXT(node);

    while (fundefs != NULL)
    {
        TRAVdo(FUNDEFS_FUNDEF(fundefs));
        // Move to the next set of declessions
        fundefs = FUNDEFS_NEXT(fundefs);
    }

    return node;
}
/**
 * @fn PRTparam
 */
node_st *PRTparam(node_st *node)
{

    printf("Parameter: %s, Type: %s", PARAM_NAME(node), VarTypeToString(PARAM_TYPE(node)));

    // If the parameter has dimensions (for array types), print them.
    if (PARAM_DIMS(node) != NULL)
    {
        printf("[");
        TRAVopt(PARAM_DIMS(node)); // Assuming this would print the dimensions or size of the array.
        printf("]");
    }

    printf("\n");
    return node;
}

/**
 * @fn PRTparams
 */
node_st *PRTparams(node_st *node)
{
    // Print the current function definition
    TRAVdo(PARAMS_PARAM(node));

    // If there is another expression in the list, print a comma and continue printing

    node_st *params = PARAMS_NEXT(node);

    while (params != NULL)
    {
        TRAVdo(PARAMS_PARAM(params));
        // Move to the next set of declessions
        params = PARAMS_NEXT(params);
    }

    return node;
}

/**
 * @fn PRTvarDecl
 */
node_st *PRTvardecl(node_st *node)
{
    printf("%s", VarTypeToString(VARDECL_TYPE(node)));
    // Assuming dims and init are optional children
    if (VARDECL_DIMS(node) != NULL)
    {
        printf("[");
        TRAVopt(VARDECL_DIMS(node));
        printf("]");
    }
    printf(" %s", VARDECL_NAME(node));
    if (VARDECL_INIT(node) != NULL)
    {
        printf(" = ");
        TRAVdo(VARDECL_INIT(node));
    }
    printf(";\n");
    return node;
}

/**
 * @fn PRTstmts
 */
node_st *PRTstmts(node_st *node)
{
    TRAVdo(STMTS_STMT(node));

    // If there is another expression in the list, print a comma and continue printing

    node_st *stmts = STMTS_NEXT(node);

    while (stmts != NULL)
    {
        TRAVdo(STMTS_STMT(stmts));
        // Move to the next set of declessions
        stmts = STMTS_NEXT(stmts);
    }

    return node;
}

/**
 * @fn PRTassign
 */
node_st *PRTassign(node_st *node)
{
    printf("Assignment to %s = ", VARLET_NAME(ASSIGN_LET(node))); // Assuming VARLET_NAME gets the name of the variable
    TRAVdo(ASSIGN_EXPR(node));
    printf(";\n");
    return node;
}

/**
 * @fn PRTbinop
 */
node_st *PRTbinop(node_st *node)
{
    char *tmp = BinopToString(BINOP_OP(node));
    printf("( ");

    TRAVleft(node);

    printf(" %s ", tmp);

    TRAVright(node);

    printf(")(%d:%d-%d)", NODE_BLINE(node), NODE_BCOL(node), NODE_ECOL(node));

    return node;
}

/**
 * @fn PRTmonop
 */
node_st *PRTmonop(node_st *node)
{
    char *tmp = MonopToString(MONOP_OP(node));
    printf("%s", tmp);

    TRAVopt(MONOP_OPERAND(node));
    return node;
}

/**
 * @fn PRTvarlet
 */
node_st *PRTvarlet(node_st *node)
{
    printf("%s(%d:%d)", VARLET_NAME(node), NODE_BLINE(node), NODE_BCOL(node));
    return node;
}

/**
 * @fn PRTvar
 */
node_st *PRTvar(node_st *node)
{
    printf("%s", VAR_NAME(node));
    return node;
}

/**
 * @fn PRTnum
 */
node_st *PRTnum(node_st *node)
{
    printf("%d", NUM_VAL(node));
    return node;
}

/**
 * @fn PRTfloat
 */
node_st *PRTfloat(node_st *node)
{
    printf("%f", FLOAT_VAL(node));
    return node;
}

/**
 * @fn PRTbool
 */
node_st *PRTbool(node_st *node)
{
    char *bool_str = BOOL_VAL(node) ? "true" : "false";
    printf("%s", bool_str);
    return node;
}

node_st *PRTtern(node_st *node)
{
    printf("here\n");

    TRAVopt(TERN_COND(node));
    printf("? ");
    TRAVopt(TERN_THEN_EXPR(node));
    printf(": ");
    TRAVopt(TERN_ELSE_EXPR(node));
    printf("\n");
    return node;
}