#include "palm/hash_table.h"
#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "ccngen/enum.h"
#include "ccngen/trav_data.h"
#include "stdio.h"
#include "string.h"
#include "palm/dbug.h"
#include "vartypetostring.h"

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
    case NT_PARAM:
        tmp = PARAM_TYPE(node);
        break;
    default:
        tmp = 0;
        break;
    }
    return tmp;
}

char *getName(node_st *node)
{
    static char buffer[64];
    char *tmp = NULL;
    switch (NODE_TYPE(node))
    {
    case NT_BOOL:
        tmp = BOOL_VAL(node) ? "true" : "false";

        break;
    case NT_FLOAT:
        sprintf(buffer, "%e", FLOAT_VAL(node));
        return buffer;
    case NT_NUM:
        sprintf(buffer, "%d", NUM_VAL(node));
        return buffer;
        // Add cases for other node types as needed

    case NT_VAR:
        tmp = SYMBOLENTRY_NAME(VAR_SYMBOLENTRY(node));
        break;
    case NT_VARLET:
        tmp = SYMBOLENTRY_NAME(VARLET_SYMBOLENTRY(node));
        break;
    case NT_MONOP:
        tmp = MonopToString(MONOP_OP((node)));
        break;
    case NT_BINOP:
        tmp = BinopToString(BINOP_OP((node)));
        break;
    case NT_FUNCALL:
        tmp = SYMBOLENTRY_NAME(FUNCALL_SYMBOLENTRY(node));
        break;
    case NT_PARAM:
        tmp = PARAM_NAME(node);
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
    // Retrieve the list of expressions (arguments) passed in the function call
    node_st *exprs = FUNCALL_FUN_ARGS(node);
    // Retrieve the entry from the symbol table for the function being called
    node_st *entry = FUNCALL_SYMBOLENTRY(node);
    // Retrieve the list of parameters defined for the function
    node_st *params = SYMBOLENTRY_PARAMS(entry);

    int countArgs = 0, countParams = 0;
    enum Type argType, paramType;
    // Iterate over both lists simultaneously
    while (exprs != NULL || params != NULL)
    {
        TRAVdo(EXPRS_EXPR(exprs));
        argType = getType(EXPRS_EXPR(exprs));
        paramType = getType(PARAMS_PARAM(params));

        countArgs++;
        countParams++;
        printf("argname: %s\n", getName(EXPRS_EXPR(exprs)));
        if (argType != paramType)
        {
            printf("%d is of type %s, expected %s\n", countArgs, VarTypeToString(argType), VarTypeToString(paramType));
        }
        if (exprs != NULL)
        {
            exprs = EXPRS_NEXT(exprs);
        }
        if (params != NULL)
        {
            params = PARAMS_NEXT(params);
        }
    }
    // Check if the number of arguments matches the number of parameters
    if (countArgs != countParams)
    {
        printf("Argument count mismatch: Expected %d, got %d\n", countParams, countArgs);
    }

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
        printf("return type %s\n", VarTypeToString(getType(returnExpr)));
    }
    else if ((returnExpr == NULL && RETURN_TYPE(node) == CT_void))
    {
        printf("return type %s\n", VarTypeToString(CT_void));
    }
    else
    {
        printf("invalid return type\n");
    }

    return node;
}