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

/**
 * @fn PRTprogram
 */
node_st *PRTprogram(node_st *node)
{
    printf("Program:\n");
    TRAVopt(PROGRAM_DECLS(node));
    return node;
}

/**
 * @fn PRTdecls
 */
node_st *PRTdecls(node_st *node)
{
    TRAVdo(DECLS_DECL(node));
    if (DECLS_NEXT(node) != NULL)
    {
        TRAVdo(DECLS_NEXT(node));
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
    if (EXPRS_NEXT(node) != NULL)
    {
        printf(", ");
        TRAVdo(EXPRS_NEXT(node));
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

    // If there is another identifier in the list, print a comma and continue printing
    if (IDS_NEXT(node) != NULL)
    {
        printf(", ");
        TRAVdo(IDS_NEXT(node));
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
    printf("Function Call: %s(", FUNCALL_NAME(node));
    TRAVopt(FUNCALL_FUN_ARGS(node));
    printf(");\n");
    return node;
}

/**
 * @fn PRTcast
 */
node_st *PRTcast(node_st *node)
{
    // Directly integrate the type conversion logic
    char *typeStr = "unknown";
    switch (CAST_TYPE(node))
    {
    case CT_int:
        typeStr = "int";
        break;
    case CT_float:
        typeStr = "float";
        break;
    case CT_bool:
        typeStr = "bool";
        break;
    case CT_void:
        typeStr = "void";
        break;
    case CT_NULL:
        DBUG_ASSERT(false, "unknown type detected!");
        // Add cases for other types as necessary
    }

    printf("Cast to %s: (", typeStr);
    TRAVdo(CAST_EXPR(node));
    printf(")\n");
    return node;
}

// /**
//  * @fn PRTfunDefs
//  */
// node_st *PRTfundefs(node_st *node)
// {
//     // Print the current function definition
//     TRAVdo(FUNDEFS_FUNDEF(node));

//     // If there is another function definition in the list, continue printing
//     if (FUNDEFS_NEXT(node) != NULL)
//     {
//         TRAVdo(FUNDEFS_NEXT(node));
//     }

//     return node;
// }

/**
 * @fn PRTfunDef
 */
node_st *PRTfundef(node_st *node)
{
    // Directly integrate the type conversion logic
    char *typeStr = "unknown";
    switch (FUNDEF_TYPE(node))
    {
    case CT_int:
        typeStr = "int";
        break;
    case CT_float:
        typeStr = "float";
        break;
    case CT_bool:
        typeStr = "bool";
        break;
    case CT_void:
        typeStr = "void";
        break;
    case CT_NULL:
        DBUG_ASSERT(false, "unknown type detected!");
        // Add cases for other types as necessary
    }

    printf("Function Definition: %s\n", FUNDEF_NAME(node));
    printf("Return Type: %s\n", typeStr);
    TRAVopt(FUNDEF_BODY(node));
    TRAVopt(FUNDEF_PARAMS(node));
    return node;
}

/**
 * @fn PRTfunBody
 */
node_st *PRTfunbody(node_st *node)
{
    printf("Function block:\n");
    TRAVopt(FUNBODY_VARDECLS(node));
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
    char *typeStr = "unknown";
    switch (GLOBDECL_TYPE(node))
    {
    case CT_int:
        typeStr = "int";
        break;
    case CT_float:
        typeStr = "float";
        break;
    case CT_bool:
        typeStr = "bool";
        break;
    case CT_void:
        typeStr = "void";
        break;
    case CT_NULL:
        DBUG_ASSERT(false, "unknown type detected!");
        // Add cases for other types as necessary
    }
    printf("%s %s;\n", typeStr, GLOBDECL_NAME(node));
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
// /**
//  * @fn PRTvardecls
//  */
// node_st *PRTvardecls(node_st *node)
// {
//     // Print the current function definition
//     // TRAVdo(VARDECLS_VARDECL(node));

//     // // If there is another function definition in the list, continue printing
//     // if (VARDECLS_NEXT(node) != NULL)
//     // {
//     //     TRAVdo(VARDECLS_NEXT(node));
//     // }

//     return node;
// }

// /**
//  * @fn PRTvardecls
//  */
// node_st *PRTblocks(node_st *node)
// {
//     // Print the current function definition
//     TRAVdo(BLOCKS_BLOCK(node));

//     // If there is another function definition in the list, continue printing
//     if (BLOCKS_NEXT(node) != NULL)
//     {
//         TRAVdo(BLOCKS_NEXT(node));
//     }

//     return node;
// }

/**
 * @fn PRTvardecls
 */
node_st *PRTvardecls(node_st *node)
{
    // Print the current function definition
    TRAVdo(VARDECLS_VARDECL(node));

    // If there is another function definition in the list, continue printing
    if (VARDECLS_NEXT(node) != NULL)
    {
        TRAVdo(VARDECLS_NEXT(node));
    }

    return node;
}

/**
 * @fn PRTvardecls
 */
node_st *PRTfundefs(node_st *node)
{
    // Print the current function definition
    TRAVdo(FUNDEFS_FUNDEF(node));

    // If there is another function definition in the list, continue printing
    if (FUNDEFS_NEXT(node) != NULL)
    {
        TRAVdo(FUNDEFS_NEXT(node));
    }

    return node;
}
/**
 * @fn PRTparam
 */
node_st *PRTparam(node_st *node)
{
    // Directly integrate the type conversion logic
    char *typeStr = "unknown";
    switch (PARAM_TYPE(node))
    {
    case CT_int:
        typeStr = "int";
        break;
    case CT_float:
        typeStr = "float";
        break;
    case CT_bool:
        typeStr = "bool";
        break;
    case CT_void:
        typeStr = "void";
        break;
    case CT_NULL:
        DBUG_ASSERT(false, "unknown type detected!");
        // Add cases for other types as necessary
    }

    printf("Parameter: %s, Type: %s", PARAM_NAME(node), typeStr);

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

    // If there is another function definition in the list, continue printing
    if (PARAMS_NEXT(node) != NULL)
    {
        TRAVdo(PARAMS_NEXT(node));
    }

    return node;
}

/**
 * @fn PRTvarDecl
 */
node_st *PRTvardecl(node_st *node)
{
    printf("Variable Declaration: %s\n", VARDECL_NAME(node));
    printf("Variable Type: %d\n", VARDECL_TYPE(node));
    // Assuming dims and init are optional children
    TRAVopt(VARDECL_DIMS(node));
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
    if (STMTS_NEXT(node) != NULL)
    {
        TRAVdo(STMTS_NEXT(node));
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
    char *tmp = NULL;
    printf("( ");

    TRAVleft(node);

    switch (BINOP_OP(node))
    {
    case BO_add:
        tmp = "+";
        break;
    case BO_sub:
        tmp = "-";
        break;
    case BO_mul:
        tmp = "*";
        break;
    case BO_div:
        tmp = "/";
        break;
    case BO_mod:
        tmp = "%";
        break;
    case BO_lt:
        tmp = "<";
        break;
    case BO_le:
        tmp = "<=";
        break;
    case BO_gt:
        tmp = ">";
        break;
    case BO_ge:
        tmp = ">=";
        break;
    case BO_eq:
        tmp = "==";
        break;
    case BO_ne:
        tmp = "!=";
        break;
    case BO_or:
        tmp = "||";
        break;
    case BO_and:
        tmp = "&&";
        break;
    case BO_NULL:
        DBUG_ASSERT(false, "unknown binop detected!");
    }

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
    switch (MONOP_OP(node))
    {
    case MO_not:
        printf("!");
        break;
    case MO_neg:
        printf("-");
        break;
    case MO_NULL:
        DBUG_ASSERT(false, "unknown monop detected!");
    }

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
