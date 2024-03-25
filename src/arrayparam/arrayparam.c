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

void APinit()
{
    return;
}

void APfini()
{

    return;
}

node_st *APfundef(node_st *node)
{
    node_st *currentParamsNode = FUNDEF_PARAMS(node);
    node_st *prevParamsNode = NULL; // Keep track of the previous Params node in the linked list
    // pseudo fundef __init can be skipped
    if (strcmp(FUNDEF_NAME(node), "__init") == 0)
    {
        return node;
    }
    // node_st *symbolTable = FUNDEF_SYMBOLTABLE(node);
    while (currentParamsNode != NULL)

    {
        node_st *param = PARAMS_PARAM(currentParamsNode);
        // Check if the current parameter is an array with dimensions
        if (PARAM_DIMS(param) != NULL)
        {
            node_st *ids = PARAM_DIMS(param);

            while (ids != NULL)
            {
                // For each dimension, create a new parameter
                char *id = IDS_NAME(ids);
                node_st *newParam = ASTparam(NULL, strdup(id), PARAM_TYPE(param));

                // dims has to be int type so hardcode it.
                // node_st *entry = ASTsymbolentry(NULL, strdup(id), CT_int, NODE_BLINE(param), SYMBOLENTRY_SCOPELEVEL(SYMBOLTABLE_ENTRY(FUNDEF_SYMBOLTABLE((node)))), NODE_TYPE(param), 0);
                // node_st *tempSymbolTable = ASTsymboltable(entry, NULL);
                // // insert at start of symboltable, since order does not matter in this case
                // SYMBOLTABLE_NEXT(tempSymbolTable) = SYMBOLTABLE_NEXT(symbolTable);
                // SYMBOLTABLE_NEXT(symbolTable) = tempSymbolTable;
                // Create a new Params node for the new parameter
                node_st *newParamsNode = ASTparams(newParam, NULL);

                if (prevParamsNode == NULL)
                {
                    // This means we're inserting right at the start
                    PARAMS_NEXT(newParamsNode) = currentParamsNode;
                    FUNDEF_PARAMS(node) = newParamsNode;
                }
                else
                {
                    // Inserting in the middle or at the end
                    PARAMS_NEXT(newParamsNode) = PARAMS_NEXT(prevParamsNode);
                    PARAMS_NEXT(prevParamsNode) = newParamsNode;
                }

                // Update prevParamsNode to be the newly inserted Params node
                prevParamsNode = newParamsNode;

                ids = IDS_NEXT(ids);
            }
        }
        else
        {
            // No dimensions to add, so just move to the next Params node
            prevParamsNode = currentParamsNode;
        }

        // Move to the next Params node in the list
        currentParamsNode = PARAMS_NEXT(currentParamsNode);
    }
    TRAVchildren(node);

    return node;
}

node_st *APfuncall(node_st *node)
{
    node_st *exprsNode = FUNCALL_FUN_ARGS(node);
    node_st *prevExprsNode = NULL; // Keep track of the previous Params node in the linked list
    // edge case for pseudo allocate since it is not a real funcall

    if (strcmp(FUNCALL_NAME(node), "__allocate") == 0)
    {
        return node;
    }
    while (exprsNode != NULL)
    {
        node_st *expr = EXPRS_EXPR(exprsNode);

        // Check if the current parameter is an array with dimensions
        if (NODE_TYPE(expr) == NT_VAR && SYMBOLENTRY_DIMS(VAR_SYMBOLENTRY(expr)) != NULL)
        {

            node_st *exprs = SYMBOLENTRY_DIMS(VAR_SYMBOLENTRY(expr));

            while (exprs != NULL)
            {
                //     // For each dimension, create a new parameter
                node_st *expr = EXPRS_EXPR(exprs);
                node_st *new_expr = NULL;
                if (NODE_TYPE(expr) == NT_NUM)
                {
                    new_expr = ASTnum(NUM_VAL(expr));
                }
                else if (NODE_TYPE(expr) == NT_VAR)
                {
                    new_expr = ASTvar(NULL, strdup(VAR_NAME(expr)));
                }
                else
                {
                    printf("invalid type\n");
                }

                //     // Create a new Params node for the new parameter
                node_st *newExprsNode = ASTexprs(new_expr, NULL);

                if (prevExprsNode == NULL)
                {
                    // This means we're inserting right at the start
                    EXPRS_NEXT(newExprsNode) = exprsNode;
                    FUNCALL_FUN_ARGS(node) = newExprsNode;
                }
                else
                {
                    // Inserting in the middle or at the end
                    EXPRS_NEXT(newExprsNode) = EXPRS_NEXT(prevExprsNode);
                    EXPRS_NEXT(prevExprsNode) = newExprsNode;
                }

                // Update prevExprsNode to be the newly inserted Params node
                prevExprsNode = newExprsNode;

                exprs = EXPRS_NEXT(exprs);
            }
        }

        else
        {
            // No dimensions to add, so just move to the next Params node
            prevExprsNode = exprsNode;
        }

        // Move to the next Params node in the list
        exprsNode = EXPRS_NEXT(exprsNode);
    }
    TRAVchildren(node);
    return node;
}
