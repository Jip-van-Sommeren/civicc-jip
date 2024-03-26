#include "palm/hash_table.h"
#include "palm/memory.h"
#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "ccngen/enum.h"
#include "ccngen/trav_data.h"
#include "stdio.h"
#include "string.h"
#include "palm/dbug.h"
#include "vartypetostring.h"
#include "palm/ctinfo.h"
#include "varinitregassignment/vardefhelper.h"
#include "arrayhelpers.h"

#define MAX_DIMENSIONS 5

void FAinit()
{
    return;
}

void FAfini()
{

    return;
}

node_st *FAfunbody(node_st *node)
{
    node_st *stmts = FUNBODY_STMTS(node); // Assuming this gets the starting point for checking
    LoopInfo loopInfo = {0, {NULL}};
    while (stmts != NULL)
    {
        processStatementNode(STMTS_STMT(stmts), &loopInfo);
        stmts = STMTS_NEXT(stmts); // Move to the next statement in the function body
    }
    printf("Max Depth: %d\n", loopInfo.depth);
    TRAVchildren(node);
    return node;
}

node_st *FAfuncall(node_st *node)
{
    printf("herre");
    // Check if the function call is "__allocate"
    if (strcmp(FUNCALL_NAME(node), "__allocate") == 0)
    {
        node_st *exprs = FUNCALL_FUN_ARGS(node); // Get the arguments of the function call
        node_st *productExpr = NULL;             // This will hold the product of dimensions

        while (exprs != NULL)
        {
            if (productExpr == NULL)
            {
                // For the first dimension, simply copy it as the starting point of the product
                productExpr = CCNcopy(EXPRS_EXPR(exprs));
            }
            else
            {
                // For subsequent dimensions, multiply them with the product expression so far
                productExpr = ASTbinop(productExpr, CCNcopy(EXPRS_EXPR(exprs)), BO_mul);
            }
            exprs = EXPRS_NEXT(exprs); // Move to the next argument
        }

        // Once the product of all dimensions is calculated, replace the function call arguments
        // with this single product expression
        CCNfree(FUNCALL_FUN_ARGS(node));
        FUNCALL_FUN_ARGS(node) = ASTexprs(productExpr, NULL);
    }
    return node;
}

node_st *FAvardecl(node_st *node)
{
    if (VARDECL_DIMS(node) != NULL)
    {
        CCNfree(VARDECL_DIMS(node));
        VARDECL_DIMS(node) = NULL;
    }
    return node;
}

node_st *FAparam(node_st *node)
{
    if (PARAM_DIMS(node) != NULL)
    {
        CCNfree(PARAM_DIMS(node));
        PARAM_DIMS(node) = NULL;
    }
    return node;
}