#include "palm/hash_table.h"
#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "ccngen/enum.h"
#include "ccngen/trav_data.h"
#include "stdio.h"
#include "string.h"
#include "palm/dbug.h"

#define TABLE_SIZE 128
#define STACK_SIZE 3
bool getMonOpResultType(enum MonOpType op, enum Type operandType)
{
    switch (op)
    {
    case MO_neg:
        // Negation is valid for int and float types and returns a result of the same type
        if (operandType == CT_int || operandType == CT_float)
        {
            return true;
        }
        break;
    case MO_not:
        // Logical NOT is only valid for booleans and returns a boolean
        if (operandType == CT_bool)
        {
            return true;
        }
        break;
    default:
        break;
    }
    return false; // If the operation is not defined for the given type
}

bool getBinOpResultType(enum BinOpType op, enum Type leftType, enum Type rightType)
{
    // Check for type compatibility
    if (leftType != rightType)
    {
        return false; // In this simple model, operands must be of the same type
    }
    if (leftType == CT_void)
    {
        return false;
    }

    switch (op)
    {
    case BO_add:
    case BO_sub:
    case BO_mul:
    case BO_div:
        // These operations are valid for int and float types and return a result of the same type
        if (leftType == CT_int || leftType == CT_float)
        {
            return true;
        }
        break;
    case BO_mod:
        // Modulo is only valid for integers
        if (leftType == CT_int)
        {
            return true;
        }
        break;
    case BO_lt:
    case BO_le:
    case BO_gt:
    case BO_ge:
    case BO_eq:
    case BO_ne:
        // Relational operators are valid for int and float, but always return a boolean
        if (leftType == CT_int || leftType == CT_float)
        {
            return true;
        }
        break;
    case BO_and:
    case BO_or:
        // Logical operators are only valid for booleans and return a boolean
        if (leftType == CT_bool)
        {
            return true;
        }
        break;
    default:
        break;
    }
    return false;
}

enum Type getType(node_st *node)
{
    int tmp;
    switch (NODE_TYPE(node))
    {
    case NT_BOOL:
        tmp = CT_bool;
        break;
    case NT_FLOAT:
        tmp = CT_float;
        break;
    case NT_NUM:
        tmp = CT_int;
        break;
    case NT_VAR:
        tmp = SYMBOLENTRY_TYPE(VAR_SYMBOLENTRY(node));
        break;
    case NT_VARLET:
        tmp = SYMBOLENTRY_TYPE(VARLET_SYMBOLENTRY(node));
        break;
    case NT_MONOP:
        tmp = MONOP_TYPE(node);
        break;
    case NT_BINOP:
        tmp = BINOP_TYPE(node);
        break;
    case NT_FUNCALL:
        tmp = SYMBOLENTRY_TYPE(FUNCALL_SYMBOLENTRY(node));
        break;
    case NT_CAST:
        tmp = CAST_TYPE(node);
        break;
    default:
        tmp = 0;
        break;
    }
    return tmp;
}
void SAinit()
{
    return;
}

void SAfini()
{
    return;
}

node_st *SAprogram(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *SAfundef(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *SAbool(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *SAfloat(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *SAnum(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *SAvar(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *SAmonop(node_st *node)
{
    TRAVchildren(node);
    enum Type type = getType(MONOP_OPERAND(node));
    if (getMonOpResultType(MONOP_OP(node), type))
    {
        MONOP_TYPE(node) = type;
    }
    else
    {
        printf("incorrect monop operation");
    }
    return node;
}

node_st *SAbinop(node_st *node)

{
    TRAVchildren(node);
    node_st *left = BINOP_LEFT(node);
    node_st *right = BINOP_RIGHT(node);
    enum Type typeLeft = getType(left);
    enum Type typeRight = getType(right);
    if (getBinOpResultType(BINOP_OP(node), typeLeft, typeRight))
    {
        BINOP_TYPE(node) = typeLeft;
    }
    else
    {
        printf("incorrect types here!\n");
    }

    return node;
}

node_st *SAcast(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *SAfuncall(node_st *node)
{
    TRAVchildren(node);
    node_st *exprs = FUNCALL_FUN_ARGS(node);
    int count = 0;
    if (exprs != NULL)
    {
        TRAVdo(EXPRS_EXPR(exprs));
        count++;
        printf("input: %d type: %d\n", count, getType(EXPRS_EXPR(exprs)));

        // Directly update exprs to its next node to correctly move forward in the list
        exprs = EXPRS_NEXT(exprs);

        while (exprs != NULL)
        {
            TRAVdo(EXPRS_EXPR(exprs));
            count++;
            printf("input: %d type: %d\n", count, getType(EXPRS_EXPR(exprs)));
            // Move to the next set of expressions
            exprs = EXPRS_NEXT(exprs);
        }
    }
    printf("count: %d\n", count);
    return node;
}
node_st *SAarrexpr(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *SAreturn(node_st *node)
{
    TRAVchildren(node);
    node_st *returnExpr = RETURN_EXPR(node);

    if ((returnExpr != NULL && getType(returnExpr) == RETURN_TYPE(node)))
    {
        printf("return type %d\n", getType(returnExpr));
    }
    else if ((returnExpr == NULL && RETURN_TYPE(node) == CT_void))
    {
        printf("return type %d\n", CT_void);
    }
    else
    {
        printf("invalid return type\n");
    }

    return node;
}