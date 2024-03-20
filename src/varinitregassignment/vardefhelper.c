#include <stdio.h>
#include "ccngen/ast.h"
#include "string.h"
#include "palm/memory.h"
#include "ccngen/enum.h"
#include "ccn/ccn.h"

node_st *makeExpr(node_st *expr)
{
    node_st *entry;
    if (NODE_TYPE(expr) == NT_NUM)
    {
        entry = ASTnum(NUM_VAL(expr));
    }
    else if (NODE_TYPE(expr) == NT_FUNCALL)
    {
        entry = ASTfuncall(FUNCALL_FUN_ARGS(expr), strdup(FUNCALL_NAME(expr)));
        FUNCALL_TYPE(entry) = FUNCALL_TYPE(expr);
        FUNCALL_SYMBOLENTRY(entry) = FUNCALL_SYMBOLENTRY(expr);
    }
    else if (NODE_TYPE(expr) == NT_VAR)
    {
        entry = ASTvar(NULL, strdup(VAR_NAME(expr)));
        VAR_TYPE(entry) = VAR_TYPE(expr);
        VAR_SYMBOLENTRY(entry) = VAR_SYMBOLENTRY(expr);
    }
    return entry;
}

node_st *makeVarNode(node_st *expr, char *name)
{
    node_st *entry;
    if (NODE_TYPE(expr) == NT_NUM)
    {
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = CT_int;
        MEMfree(expr->data.N_num);
        MEMfree(expr);
    }
    else if (NODE_TYPE(expr) == NT_FUNCALL)
    {

        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = FUNCALL_TYPE(expr);
        VAR_SYMBOLENTRY(entry) = FUNCALL_SYMBOLENTRY(expr);
        MEMfree(expr->data.N_funcall->name);
        MEMfree(expr->data.N_funcall);
        MEMfree(expr);
    }
    else if (NODE_TYPE(expr) == NT_VAR)
    {
        entry = ASTvar(NULL, strdup(VAR_NAME(expr)));
        VAR_TYPE(entry) = VAR_TYPE(expr);
        VAR_SYMBOLENTRY(entry) = VAR_SYMBOLENTRY(expr);
        MEMfree(expr->data.N_var->name);
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_var);
        MEMfree(expr);
    }
    return entry;
}

void freeInitExpr(node_st *expr)
{
    if (NODE_TYPE(expr) == NT_NUM)
    {
        MEMfree(expr->data.N_num);
        MEMfree(expr);
    }
    else if (NODE_TYPE(expr) == NT_FUNCALL)
    {

        MEMfree(expr->data.N_funcall->name);
        MEMfree(expr->data.N_funcall);
        MEMfree(expr);
    }
    else if (NODE_TYPE(expr) == NT_VAR)
    {
        MEMfree(expr->data.N_var->name);
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_var);
        MEMfree(expr);
    }
}