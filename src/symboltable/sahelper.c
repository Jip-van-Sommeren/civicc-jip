#include "ccngen/ast.h"
#include "ccngen/enum.h"
#include "stdio.h"
#include "string.h"
#include "vartypetostring.h"

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

enum Type getBinOpResultType(enum BinOpType op, enum Type leftType, enum Type rightType)
{
    // Check for type compatibility
    if (leftType != rightType)
    {
        return CT_NULL; // In this simple model, operands must be of the same type
    }
    if (leftType == CT_void)
    {
        return CT_NULL;
    }

    switch (op)
    {
    case BO_add:
    case BO_mul:
        if (leftType == CT_int || leftType == CT_float || leftType == CT_bool)
        {
            return leftType;
        }
        break;
    case BO_sub:
    case BO_div:
        // These operations are valid for int and float types and return a result of the same type
        if (leftType == CT_int || leftType == CT_float)
        {
            return leftType;
        }
        break;
    case BO_mod:
        // Modulo is only valid for integers
        if (leftType == CT_int)
        {
            return leftType;
        }
        break;
    case BO_lt:
    case BO_le:
    case BO_gt:
    case BO_ge:
        if (leftType == CT_int || leftType == CT_float)
        {
            return CT_bool;
        }
        break;
    case BO_eq:

    case BO_ne:
        // Relational operators are valid for int and float, but always return a boolean
        if (leftType == CT_int || leftType == CT_float || leftType == CT_bool)
        {
            return CT_bool;
        }
        break;
    case BO_and:
    case BO_or:
        // Logical operators are only valid for booleans and return a boolean
        if (leftType == CT_bool)
        {
            return leftType;
        }
        break;
    default:
        break;
    }
    return CT_NULL;
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
    case NT_ARREXPR:
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