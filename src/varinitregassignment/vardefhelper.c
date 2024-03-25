#include <stdio.h>
#include "ccngen/ast.h"
#include "string.h"
#include "palm/memory.h"
#include "ccngen/enum.h"
#include "ccn/ccn.h"

node_st *makeExpr(node_st *expr)
{
    node_st *entry;
    switch (NODE_TYPE(expr))
    {
    case NT_NUM:
        entry = ASTnum(NUM_VAL(expr));
        break;
    case NT_FLOAT:
        entry = ASTfloat(FLOAT_VAL(expr));
        break;
    case NT_BOOL:
        entry = ASTbool(BOOL_VAL(expr));
        break;
    case NT_FUNCALL:
        entry = ASTfuncall(FUNCALL_FUN_ARGS(expr), strdup(FUNCALL_NAME(expr)));
        FUNCALL_TYPE(entry) = FUNCALL_TYPE(expr);
        FUNCALL_SYMBOLENTRY(entry) = FUNCALL_SYMBOLENTRY(expr);
        break;
    case NT_VAR:
        entry = ASTvar(NULL, strdup(VAR_NAME(expr)));
        VAR_TYPE(entry) = VAR_TYPE(expr);
        VAR_SYMBOLENTRY(entry) = VAR_SYMBOLENTRY(expr);
        break;
    case NT_BINOP:
        entry = ASTbinop(BINOP_LEFT(expr), BINOP_RIGHT(expr), BINOP_OP(expr));
        BINOP_TYPE(entry) = BINOP_TYPE(expr);
        break;
    case NT_MONOP:
        entry = ASTmonop(MONOP_OPERAND(expr), MONOP_OP(expr));
        MONOP_TYPE(entry) = MONOP_TYPE(expr);
        break;
    case NT_CAST:
        entry = ASTcast(CAST_EXPR(expr), CAST_TYPE(expr));
        break;
    case NT_ARREXPR:
        entry = ASTarrexpr(ARREXPR_EXPRS(expr));
        break;
    default:
        break;
    }

    return entry;
}

node_st *makeVarNodeNoFree(node_st *expr, char *name)
{
    node_st *entry;
    switch (NODE_TYPE(expr))
    {
    case NT_NUM:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = CT_int;
        break;
    case NT_FLOAT:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = CT_float;
        break;
    case NT_BOOL:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = CT_bool;
        break;
    case NT_FUNCALL:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = FUNCALL_TYPE(expr);
        VAR_SYMBOLENTRY(entry) = FUNCALL_SYMBOLENTRY(expr);
        break;
    case NT_VAR:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = VAR_TYPE(expr);
        VAR_SYMBOLENTRY(entry) = VAR_SYMBOLENTRY(expr);
        break;
    case NT_VARLET:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = VARLET_TYPE(expr);
        VAR_SYMBOLENTRY(entry) = VARLET_SYMBOLENTRY(expr);
        break;
    case NT_BINOP:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = BINOP_TYPE(expr);
        break;
    case NT_MONOP:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = MONOP_TYPE(expr);
        break;
    case NT_CAST:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = CAST_TYPE(expr);
        break;
    case NT_ARREXPR:
        entry = ASTvar(NULL, strdup(name));
        break;
    default:
        break;
    }
    return entry;
}

node_st *makeVarNode(node_st *expr, char *name)
{
    node_st *entry;
    switch (NODE_TYPE(expr))
    {
    case NT_NUM:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = CT_int;
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_num);
        MEMfree(expr);
        break;
    case NT_FLOAT:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = CT_float;
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_float);
        MEMfree(expr);
        break;
    case NT_BOOL:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = CT_bool;
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_bool);
        MEMfree(expr);
        break;
    case NT_FUNCALL:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = FUNCALL_TYPE(expr);
        VAR_SYMBOLENTRY(entry) = FUNCALL_SYMBOLENTRY(expr);
        MEMfree(expr->data.N_funcall->name);
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_funcall);
        MEMfree(expr);
        break;
    case NT_VAR:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = VAR_TYPE(expr);
        VAR_SYMBOLENTRY(entry) = VAR_SYMBOLENTRY(expr);
        MEMfree(expr->data.N_var->name);
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_var);
        MEMfree(expr);
        break;
    case NT_VARLET:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = VARLET_TYPE(expr);
        VAR_SYMBOLENTRY(entry) = VARLET_SYMBOLENTRY(expr);
        MEMfree(expr->data.N_varlet->name);
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_varlet);
        MEMfree(expr);
        break;
    case NT_BINOP:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = BINOP_TYPE(expr);
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_binop);
        MEMfree(expr);
        break;
    case NT_MONOP:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = MONOP_TYPE(expr);
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_monop);
        MEMfree(expr);
        break;
    case NT_CAST:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = CAST_TYPE(expr);
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_cast);
        MEMfree(expr);
        break;
    case NT_ARREXPR:
        entry = ASTvar(NULL, strdup(name));
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_arrexpr);
        MEMfree(expr);
        break;
    default:
        break;
    }
    return entry;
}

void freeInitExpr(node_st *expr)
{
    switch (NODE_TYPE(expr))
    {
    case NT_NUM:
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_num);
        MEMfree(expr);
        break;
    case NT_FLOAT:
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_float);
        MEMfree(expr);
        break;
    case NT_BOOL:
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_bool);
        MEMfree(expr);
        break;
    case NT_FUNCALL:
        MEMfree(expr->data.N_funcall->name);
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_funcall);
        MEMfree(expr);
        break;
    case NT_VAR:
        MEMfree(expr->data.N_var->name);
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_var);
        MEMfree(expr);
        break;
    case NT_BINOP:
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_binop);
        MEMfree(expr);
        break;
    case NT_MONOP:
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_monop);
        MEMfree(expr);
        break;
    case NT_CAST:
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_cast);
        MEMfree(expr);
        break;
    case NT_ARREXPR:
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_arrexpr);
        MEMfree(expr);
        break;
    default:
        break;
    }
}

node_st *exprsToExprs(node_st *dims)
{
    node_st *exprsHead = NULL;     // Head of the new exprs list
    node_st *lastExprsNode = NULL; // To keep track of the last node in the new list

    while (dims != NULL)
    {
        // Assuming dims is a list of Expr nodes

        TRAVdo(EXPRS_EXPR(dims));
        node_st *currentExpr = makeExpr(EXPRS_EXPR(dims)); // Extract the current expression from dims

        // Create a new Exprs node with the current expression
        node_st *newExprsNode = ASTexprs(currentExpr, NULL);

        if (exprsHead == NULL)
        {
            // If this is the first node, set it as the head of the list
            exprsHead = newExprsNode;
        }
        else
        {
            // Otherwise, link it to the last node in the list
            EXPRS_NEXT(lastExprsNode) = newExprsNode;
        }

        // Update the last node to the current one
        lastExprsNode = newExprsNode;

        // Move to the next set of dimensions
        dims = EXPRS_NEXT(dims);
    }

    return exprsHead;
}

node_st *idsToExprs(node_st *dims)
{
    node_st *exprsHead = NULL;     // Head of the new exprs list
    node_st *lastExprsNode = NULL; // To keep track of the last node in the new list

    while (dims != NULL)
    {
        // Assuming dims is a list of Expr nodes
        node_st *currentExpr = ASTvar(NULL, strdup(IDS_NAME(dims)));
        VAR_TYPE(currentExpr) = CT_int;

        // Create a new Exprs node with the current expression
        node_st *newExprsNode = ASTexprs(currentExpr, NULL);

        if (exprsHead == NULL)
        {
            // If this is the first node, set it as the head of the list
            exprsHead = newExprsNode;
        }
        else
        {
            // Otherwise, link it to the last node in the list
            EXPRS_NEXT(lastExprsNode) = newExprsNode;
        }

        // Update the last node to the current one
        lastExprsNode = newExprsNode;

        // Move to the next set of dimensions
        dims = IDS_NEXT(dims);
    }

    return exprsHead;
}

void appendExprAndUpdateTail(node_st **exprList, node_st **exprListTail, node_st *newExpr)
{
    // Check if the list is empty
    if (*exprList == NULL)
    {
        // This is the first expression in the list.
        *exprList = ASTexprs(newExpr, NULL);
        *exprListTail = *exprList; // The tail is the first node itself.
    }
    else
    {
        // There are already expressions in the list. Append the new expression.
        node_st *newExprNode = ASTexprs(newExpr, NULL);
        EXPRS_NEXT(*exprListTail) = newExprNode; // Append new expression to the end.
        *exprListTail = newExprNode;             // Update the tail to the new node.
    }
}
