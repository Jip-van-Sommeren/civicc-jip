#include <stdio.h>
#include "palm/dbug.h"
#include <stdbool.h>
#include <string.h>
#include "ccngen/enum.h"
#include "ccngen/ast.h"
/**
 * @fn VarTypeToString
 */
char *VarTypeToString(enum Type type)
{
    char *tmp;
    switch (type)
    {
    case CT_int:
        tmp = "int";
        break;
    case CT_float:
        tmp = "float";
        break;
    case CT_bool:
        tmp = "bool";
        break;
    case CT_void:
        tmp = "void";
        break;
    case CT_NULL:
        tmp = "unkown";
        break;
        // DBUG_ASSERT(false, "unknown type detected!");
    }
    return tmp;
}

char *BinopToString(enum BinOpType type)
{
    char *tmp;
    switch (type)
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
    return tmp;
}

char *MonopToString(enum MonOpType type)
{
    char *tmp;
    switch (type)
    {
    case MO_not:
        tmp = "!";
        break;
    case MO_neg:
        tmp = "-";
        break;
    case MO_NULL:
        DBUG_ASSERT(false, "unknown monop detected!");
    }
    return tmp;
}

int checkExprDimension(node_st *dims)
{
    int dimCount = 0;
    while (dims != NULL)
    {
        dimCount++;
        dims = EXPRS_NEXT(dims);
    }
    return dimCount;
}

int checkParamDimension(node_st *dims)
{
    int dimCount = 0;
    while (dims != NULL)
    {
        dimCount++;
        dims = IDS_NEXT(dims);
    }
    return dimCount;
}
