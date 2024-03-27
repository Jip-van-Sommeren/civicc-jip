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

node_st *generateFlattenedIndexExpr(node_st **dims, node_st **indices, int numDims)
{
    if (numDims < 1)
    {
        return NULL; // Handle error or invalid input
    }

    node_st *flattenedIndexExpr = CCNcopy(indices[numDims - 1]); // Start with the last index as is

    // Traverse dimensions in reverse order, starting from the second-last dimension
    for (int i = numDims - 2; i >= 0; --i)
    {
        // Begin with the current index
        node_st *productExpr = CCNcopy(indices[i]);

        // Multiply it by the size of all subsequent dimensions
        for (int j = i + 1; j < numDims; ++j)
        {
            productExpr = ASTbinop(productExpr, CCNcopy(dims[j]), BO_mul);
        }

        // Always add the product to the cumulative flattened index expression
        if (i < numDims - 1)
        {
            flattenedIndexExpr = ASTbinop(productExpr, flattenedIndexExpr, BO_add);
        }
    }

    return flattenedIndexExpr;
}

node_st **extractIndicesFromVarlet(node_st *varlet, int numDims)
{
    // Dynamically allocate an array to hold pointers to node_st structures (index expressions).
    node_st **indices = (node_st **)malloc(numDims * sizeof(node_st *));

    if (indices == NULL)
    {
        // Handle memory allocation failure if needed
        return NULL;
    }

    // Initialize the array to ensure all pointers are NULL initially.
    for (int i = 0; i < numDims; ++i)
    {
        indices[i] = NULL;
    }

    node_st *currentExpr = VARLET_INDICES(varlet); // Starting point of the index expressions.
    int indexCount = 0;                            // To keep track of the number of indices processed.

    // Loop through the index expressions and fill the indices array.
    while (currentExpr != NULL && indexCount < numDims)
    {
        indices[indexCount] = EXPRS_EXPR(currentExpr);
        currentExpr = EXPRS_NEXT(currentExpr);
        indexCount++;
    }

    // Return the array of index expressions.
    return indices;
}

node_st **extractIndicesFromVar(node_st *var, int numDims)
{
    // Dynamically allocate an array to hold pointers to node_st structures (index expressions).
    node_st **indices = (node_st **)malloc(numDims * sizeof(node_st *));

    if (indices == NULL)
    {
        // Handle memory allocation failure if needed
        return NULL;
    }

    // Initialize the array to ensure all pointers are NULL initially.
    for (int i = 0; i < numDims; ++i)
    {
        indices[i] = NULL;
    }

    node_st *currentExpr = VAR_INDICES(var); // Starting point of the index expressions.
    int indexCount = 0;                      // To keep track of the number of indices processed.

    // Loop through the index expressions and fill the indices array.
    while (currentExpr != NULL && indexCount < numDims)
    {
        indices[indexCount] = EXPRS_EXPR(currentExpr);
        currentExpr = EXPRS_NEXT(currentExpr);
        indexCount++;
    }

    // Return the array of index expressions.
    return indices;
}

void transformAssignForFlattenedArray(node_st *assignStmt, node_st **dims, int numDimsVarlet, int numDimsVar)
{
    node_st *varlet = ASSIGN_LET(assignStmt);
    if (VARLET_INDICES(varlet) != NULL && numDimsVarlet > 1)
    {

        node_st **indices = extractIndicesFromVarlet(varlet, numDimsVarlet); // You'll need to implement extractIndicesFromVarlet.

        // Generate the expression for the flattened index.
        node_st *flattenedIndexExpr = generateFlattenedIndexExpr(dims, indices, numDimsVarlet);

        // Replace the indices in varlet with the new single flattened index.
        CCNfree(VARLET_INDICES(varlet));
        free(indices);
        VARLET_INDICES(varlet) = ASTexprs(flattenedIndexExpr, NULL);
    }
    node_st *var = ASSIGN_EXPR(assignStmt);
    if (NODE_TYPE(var) == NT_VAR && VAR_INDICES(var) != NULL && numDimsVar > 1)
    {
        node_st *dimsVar[numDimsVar];
        node_st **indices = extractIndicesFromVar(var, numDimsVar);

        node_st *flattenedIndexExpr = NULL;
        // extract vardims from decl to get correct order
        if (SYMBOLENTRY_DIMS(VAR_SYMBOLENTRY(var)) != NULL)
        {
            node_st *exprs = SYMBOLENTRY_DIMS(VAR_SYMBOLENTRY(var));
            int i = 0;
            while (exprs != NULL)
            {
                dimsVar[i] = EXPRS_EXPR(exprs);
                i++;
                exprs = EXPRS_NEXT(exprs);
            }
            flattenedIndexExpr = generateFlattenedIndexExpr(dimsVar, indices, numDimsVar);
        }
        else
        {
            flattenedIndexExpr = generateFlattenedIndexExpr(dims, indices, numDimsVar);
        }

        // Generate the expression for the flattened index.

        // Replace the indices in varlet with the new single flattened index.
        CCNfree(VAR_INDICES(var));
        free(indices);
        VAR_INDICES(var) = ASTexprs(flattenedIndexExpr, NULL);
    }
}

node_st *returnStmtsBlock(node_st *node)
{
    node_st *stmts = NULL;
    switch (NODE_TYPE(node))
    {
    case NT_FOR:
        stmts = FOR_BLOCK(node);
        break;
    case NT_WHILE:
        stmts = WHILE_BLOCK(node);
        break;
    case NT_DOWHILE:
        stmts = DOWHILE_BLOCK(node);
        break;

    default:
        break;
    }
    return stmts;
}

node_st *extractLoopStopExpr(node_st *node)
{
    node_st *stopExpr = NULL;
    switch (NODE_TYPE(node))
    {
    case NT_FOR:
        stopExpr = FOR_STOP(node);
        break;
    case NT_WHILE:
        if (NODE_TYPE(DOWHILE_COND(node)) == NT_BOOL)
        {
            stopExpr = WHILE_BLOCK(node);
        }
        else
        {
            stopExpr = BINOP_RIGHT(DOWHILE_COND(node));
        }
        break;
    case NT_DOWHILE:
        if (NODE_TYPE(DOWHILE_COND(node)) == NT_BOOL)
        {
            stopExpr = DOWHILE_BLOCK(node);
        }
        else
        {
            stopExpr = BINOP_RIGHT(DOWHILE_COND(node));
        }
        break;

    default:
        break;
    }
    return stopExpr;
}

node_st *extractLoopStartExpr(node_st *node)
{
    node_st *startExpr = NULL;
    switch (NODE_TYPE(node))
    {
    case NT_FOR:
        startExpr = ASTvar(NULL, strdup(FOR_VAR(node)));
        VAR_TYPE(startExpr) = CT_int;
        break;
    case NT_WHILE:
        startExpr = BINOP_LEFT(WHILE_COND(node));
        break;
    case NT_DOWHILE:
        startExpr = BINOP_LEFT(DOWHILE_COND(node));
        break;

    default:
        break;
    }
    return startExpr;
}

void processStatementNode(node_st *node, LoopInfo *loopInfo)
{
    if (!node)
        return;

    LoopInfo localLoopInfo = *loopInfo;

    node_st *innerStmts = NULL;
    switch (NODE_TYPE(node))
    {
    case NT_IFELSE:
        innerStmts = IFELSE_THEN(node);
        while (innerStmts != NULL)
        {
            processStatementNode(STMTS_STMT(innerStmts), &localLoopInfo);
            innerStmts = STMTS_NEXT(innerStmts);
        }
        innerStmts = IFELSE_ELSE_BLOCK(node);
        while (innerStmts != NULL)
        {
            processStatementNode(STMTS_STMT(innerStmts), &localLoopInfo);
            innerStmts = STMTS_NEXT(innerStmts);
        }
        break;
    case NT_FOR:
    case NT_WHILE:
    case NT_DOWHILE:
        localLoopInfo.depth++;
        // Assuming returnStmtsBlock(node) correctly retrieves the block of statements inside the loop
        // and assuming you have a function to extract the loop stop condition as an expression
        if (localLoopInfo.depth <= MAX_DIMENSIONS)
        {
            localLoopInfo.dims[localLoopInfo.depth - 1] = extractLoopStopExpr(node);
            // localLoopInfo.indeces[localLoopInfo.depth - 1] = extractLoopStartExpr(node);
        }
        innerStmts = returnStmtsBlock(node);
        while (innerStmts != NULL)
        {
            processStatementNode(STMTS_STMT(innerStmts), &localLoopInfo);
            innerStmts = STMTS_NEXT(innerStmts);
        }
        break;
    case NT_ASSIGN:
        // Here you can apply your logic for index transformation
        if (localLoopInfo.depth > 1)
        { // Ensure there is at least one loop surrounding this assignment
            int dimsCountVarlet = checkExprDimension(VARLET_INDICES(ASSIGN_LET(node)));
            int dimsCountVar = 0;
            if (NODE_TYPE(ASSIGN_EXPR(node)) == NT_VAR)
            {
                dimsCountVar = checkExprDimension(VAR_INDICES(ASSIGN_EXPR(node)));
            }
            transformAssignForFlattenedArray(node, localLoopInfo.dims, dimsCountVarlet, dimsCountVar);
        }
        break;
    default:
        break;
    }

    if (localLoopInfo.depth > loopInfo->depth)
    {
        loopInfo->depth = localLoopInfo.depth; // Update max depth if the local depth is greater
    }
}

void collectValues(node_st *expr, node_st **valuesTail, node_st **flattenedValues)
{
    if (expr == NULL)
        return;

    if (NODE_TYPE(expr) == NT_ARREXPR)
    {
        node_st *nestedExprs = ARREXPR_EXPRS(expr);
        while (nestedExprs != NULL)
        {
            collectValues(EXPRS_EXPR(nestedExprs), valuesTail, flattenedValues);
            nestedExprs = EXPRS_NEXT(nestedExprs);
        }
    }
    else
    {
        node_st *newNode = ASTexprs(CCNcopy(expr), NULL);
        if (*valuesTail == NULL)
        {
            *flattenedValues = newNode;
            *valuesTail = newNode;
        }
        else
        {
            EXPRS_NEXT(*valuesTail) = newNode;
            *valuesTail = newNode;
        }
    }
}
