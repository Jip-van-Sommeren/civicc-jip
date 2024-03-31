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
#include "palm/ctinfo.h"
#include "sahelper.h"

#define TABLE_SIZE 128
#define STACK_SIZE 3

void returnTypeError(node_st *returnExpr, node_st *returnNode)
{
    if (returnExpr == NULL)
    {
        CTI(CTI_ERROR, true, "return type void, expected type %s\n", VarTypeToString(RETURN_TYPE(returnNode)));
    }

    else
    {
        CTI(CTI_ERROR, true, "return type %s, expected type %s\n", VarTypeToString(getType(returnExpr)), VarTypeToString(RETURN_TYPE(returnNode)));
    }
    CTIabortOnError();
}

void paramTypeError(node_st *expr, node_st *param)
{
    CTI(CTI_ERROR, true, "arg %s is type %s, expected type %s\n", getName(expr), VarTypeToString(getType(expr)), VarTypeToString(getType(param)));
    CTIabortOnError();
}

void paramLengthError(int countArgs, int countParams, char *name)
{
    CTI(CTI_ERROR, true, "%s got %d arguments, expected %d\n", name, countArgs, countParams);
    CTIabortOnError();
}

void castTypeError()
{
    CTI(CTI_ERROR, true, "cannot cast to void type\n");
    CTIabortOnError();
}

void binopTypeError(enum Type typeLeft, enum Type typeRight, enum BinOpType op)
{
    CTI(CTI_ERROR, true, "cannot perform %s on type %s and %s\n", BinopToString(op), VarTypeToString(typeLeft), VarTypeToString(typeRight));
    CTIabortOnError();
}

void monopTypeError(enum Type type, enum MonOpType op)
{
    CTI(CTI_ERROR, true, "%s invalid for type %s\n", MonopToString(op), VarTypeToString(type));
    CTIabortOnError();
}

void vardeclTypeError(node_st *init, node_st *node)
{
    CTI(CTI_ERROR, true, "%s declared as type %s, value is of type %s\n", VARDECL_NAME(node), VarTypeToString(VARDECL_TYPE(node)), VarTypeToString(getType(init)));
    CTIabortOnError();
}

void vardeclDimsError(node_st *expr)
{
    CTI(CTI_ERROR, true, "array dimensions can only declared with type int not with type %s\n", VarTypeToString(getType(expr)));
    CTIabortOnError();
}

void arrayTypeError(enum Type exprType, enum Type expectedType)
{
    CTI(CTI_ERROR, true, "array element is of type %s, expected type %s", VarTypeToString(exprType), VarTypeToString(expectedType));
    CTIabortOnError();
}

void funcallInputError(int argDim, int paramDim, enum Type argType, enum Type paramType)
{
    CTI(CTI_ERROR, true, "expected %s [%d] but argument is of type %s [%d]", VarTypeToString(paramType), paramDim, VarTypeToString(argType), argDim);
    CTIabortOnError();
}

void incorrectDimsArrayError()
{
    CTI(CTI_ERROR, true, "array expression does not match declared dimensions");
    CTIabortOnError();
}

void assignTypeError(node_st *expr, node_st *varlet)
{
    CTI(CTI_ERROR, true, "arg %s is type %s, expected type %s\n", getName(expr), VarTypeToString(getType(expr)), VarTypeToString(getType(varlet)));
    CTIabortOnError();
}

void intOutOfRangeError()
{
    CTI(CTI_ERROR, true, "integer out of range\n");
    CTIabortOnError();
}

void floatOutOfRangeError()
{
    CTI(CTI_ERROR, true, "integer out of range\n");
    CTIabortOnError();
}

int countArrayDimensions(node_st *arrExpr)
{

    if (NODE_TYPE(arrExpr) == NT_ARREXPR)
    {
        // Get the first list of expressions within the ArrExpr node.
        node_st *exprList = ARREXPR_EXPRS(arrExpr);
        if (exprList == NULL)
        {
            // Empty list, so this is a 1-dimensional array with 0 elements.
            return 1;
        }

        node_st *firstExpr = EXPRS_EXPR(exprList);
        if (firstExpr == NULL || NODE_TYPE(firstExpr) != NT_ARREXPR)
        {
            // The list contains non-ArrExpr elements, indicating this is a 1D array.
            return 1;
        }

        // The first element of the list is an ArrExpr, indicating that this is at least a 2D array.
        // Recursively count the dimensions of the first ArrExpr in the list to handle nested arrays.
        return 1 + countArrayDimensions(firstExpr);
    }
    else if (NODE_TYPE(arrExpr) == NT_VAR)
    {
        return (SYMBOLENTRY_DIMSCOUNT(VAR_SYMBOLENTRY(arrExpr)));
    }

    return 0;
}

bool isExprList(node_st *node)
{
    // Check if the node type is an ArrExpr, indicating a list of expressions.
    return NODE_TYPE(node) == NT_ARREXPR;
}

bool checkExprList(node_st *exprs, int expectedCount, enum Type type)
{
    int count = 0;
    while (exprs != NULL)
    {
        count++;
        if (getType(EXPRS_EXPR(exprs)) != type)
        {
            arrayTypeError(getType(EXPRS_EXPR(exprs)), type);
        }
        exprs = EXPRS_NEXT(exprs); // Traverse through the linked list of expressions.
    }
    return count == expectedCount;
}

bool checkInitializer(node_st *init, int expectedDims[], int currentDim, int totalDims, enum Type type)
{
    // Base case: if we've processed all dimensions, there should be no more initializers.
    if (currentDim >= totalDims)
    {
        return false;
    }

    if (isExprList(init))
    {
        node_st *exprList = ARREXPR_EXPRS(init); // Get the list of expressions within the ArrExpr node.

        if (currentDim == totalDims - 1)
        {
            // We're at the last dimension, so we expect a flat list of expressions.
            return checkExprList(exprList, expectedDims[currentDim], type);
        }
        else
        {
            // Nested list case: each element in the list should itself be a list matching the next dimension.
            int count = 0;
            while (exprList != NULL)
            {
                node_st *expr = EXPRS_EXPR(exprList); // Get the current expression, which should be an ArrExpr.
                if (!isExprList(expr) || !checkInitializer(expr, expectedDims, currentDim + 1, totalDims, type))
                {
                    return false; // Nested list doesn't match expected dimensions or is not a list.
                }
                count++;
                exprList = EXPRS_NEXT(exprList); // Move to the next expression in the list.
            }
            return count == expectedDims[currentDim]; // Ensure the count matches the expected dimension size.
        }
    }
    return false; // If init is not an expression list, it's an invalid initializer for an array.
}

bool checkArrayDimensions(node_st *dims, node_st *init, enum Type type)
{
    // Step 1: Extract expected dimensions from dims list.
    int expectedDims[2]; // 2D max array
    int dimCount = 0;
    while (dims != NULL)
    {
        TRAVdo(EXPRS_EXPR(dims)); // Traverse the expression to do any necessary analysis on it
        if (getType(EXPRS_EXPR(dims)) != CT_int)
        {
            vardeclDimsError(EXPRS_EXPR(dims));
        }
        // reuse dimcount for array indexing
        expectedDims[dimCount++] = NUM_VAL(EXPRS_EXPR(dims));
        dims = EXPRS_NEXT(dims); // Assuming you have a way to iterate to the next dimension.
    }

    // Step 2: Check the initializer against expected dimensions.
    return checkInitializer(init, expectedDims, 0, dimCount, type);
}
void checkIntegerRange(int value)
{
    if (value < INT32_MIN || value > INT32_MAX)
    {
        intOutOfRangeError();
    }
}

void SAinit()
{
    return;
}

void SAfini()
{
    return;
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
        monopTypeError(type, MONOP_OP(node));
    }
    return node;
}

node_st *SAglobdef(node_st *node)
{
    TRAVchildren(node);
    node_st *init = GLOBDEF_INIT(node);

    node_st *dims = GLOBDEF_DIMS(node);
    if (init != NULL && NODE_TYPE(init) == NT_NUM)
    {

        int num = NUM_VAL(init);
        checkIntegerRange(num);
    }
    if (dims == NULL && init != NULL && (GLOBDEF_TYPE(node) != getType(init)))
    {
        vardeclTypeError(init, node);
    }
    // Check if array declaration has correct dims
    if (dims != NULL && init != NULL && NODE_TYPE(init) == NT_ARREXPR && !checkArrayDimensions(dims, init, GLOBDEF_TYPE(node)))
    {
        incorrectDimsArrayError();
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

    enum Type resultType = getBinOpResultType(BINOP_OP(node), typeLeft, typeRight);
    printf("node type here %d\n", BINOP_OP(node));
    if (resultType != CT_NULL)
    {
        BINOP_TYPE(node) = resultType;
    }
    else
    {
        binopTypeError(typeLeft, typeRight, BINOP_OP(node));
    }

    return node;
}

node_st *SAcast(node_st *node)
{
    if (CAST_TYPE(node) == CT_void)
    {
        castTypeError();
    }
    TRAVchildren(node);
    return node;
}

node_st *SAfuncall(node_st *node)
{
    if (strcmp(FUNCALL_NAME(node), "__allocate") == 0)
    {
        return node;
    }
    // Retrieve the list of expressions (arguments) passed in the function call
    node_st *entry = FUNCALL_SYMBOLENTRY(node);
    // Retrieve the list of parameters defined for the function
    if (SYMBOLENTRY_PARAMS(entry) == NULL && FUNCALL_FUN_ARGS(node))
    {
        TRAVchildren(node);
        return node;
    }
    node_st *exprs = FUNCALL_FUN_ARGS(node);
    // Retrieve the entry from the symbol table for the function being called
    node_st *params = SYMBOLENTRY_PARAMS(entry);

    int countArgs = 0, countParams = 0;
    enum Type argType, paramType;
    // Iterate over both lists simultaneously
    while (exprs != NULL || params != NULL)
    {
        TRAVdo(EXPRS_EXPR(exprs));
        argType = getType(EXPRS_EXPR(exprs));
        paramType = getType(PARAMS_PARAM(params));
        if (PARAM_DIMS(PARAMS_PARAM(params)) != NULL)
        {

            int argDim = countArrayDimensions(EXPRS_EXPR(exprs));
            int paramDim = checkParamDimension(PARAM_DIMS(PARAMS_PARAM(params)));
            if (argDim != paramDim)
            {
                funcallInputError(argDim, paramDim, argType, paramType);
            }
        }

        countArgs++;
        countParams++;
        if (argType != paramType)
        {
            paramTypeError(EXPRS_EXPR(exprs), PARAMS_PARAM(params));
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
        paramLengthError(countArgs, countParams, FUNCALL_NAME(node));
    }
    TRAVchildren(node);
    return node;
}

node_st *SAvardecl(node_st *node)
{
    TRAVchildren(node);
    node_st *init = VARDECL_INIT(node);

    node_st *dims = VARDECL_DIMS(node);
    if (init != NULL && NODE_TYPE(init) == NT_NUM)
    {
        long long num = NUM_VAL(init);
        checkIntegerRange(num);
    }
    if (dims == NULL && init != NULL && (VARDECL_TYPE(node) != getType(init)))
    {
        vardeclTypeError(init, node);
    }
    // Check if array declaration has correct dims
    if (dims != NULL && init != NULL && NODE_TYPE(init) == NT_ARREXPR && !checkArrayDimensions(dims, init, VARDECL_TYPE(node)))
    {
        incorrectDimsArrayError();
    }
    return node;
}

node_st *SAreturn(node_st *node)
{
    TRAVchildren(node);
    node_st *returnExpr = RETURN_EXPR(node);

    // Check for a correct return type: either non-void with a matching type or void with no return expression
    if (!((returnExpr != NULL && getType(returnExpr) == RETURN_TYPE(node)) ||
          (returnExpr == NULL && RETURN_TYPE(node) == CT_void)))
    {
        returnTypeError(returnExpr, node);
    }

    return node;
}

node_st *SAassign(node_st *node)
{
    TRAVchildren(node);
    node_st *varlet = ASSIGN_LET(node);

    node_st *expr = ASSIGN_EXPR(node);
    if (NODE_TYPE(expr) == NT_BINOP)
    {
        printf("node type: %d\n", BINOP_OP(expr));
    }

    if (NODE_TYPE(expr) == NT_FUNCALL && strcmp(FUNCALL_NAME(expr), "__allocate") == 0)
    {
        return node;
    }
    if (NODE_TYPE(expr) == NT_BINOP &&
        NODE_TYPE(BINOP_LEFT(expr)) == NT_VAR &&
        strcmp(VARLET_NAME(varlet), VAR_NAME(BINOP_LEFT(expr))) == 0 && NODE_TYPE(BINOP_RIGHT(expr)) == NT_NUM &&
        (BINOP_OP(expr) == BO_sub || BINOP_OP(expr) == BO_add) &&
        !SYMBOLENTRY_GLOBAL(VAR_SYMBOLENTRY(BINOP_LEFT(expr))))
    {

        ASSIGN_UPDATE(node) = true;
    }
    if (getType(expr) != getType(varlet))
    {
        assignTypeError(expr, varlet);
    }

    return node;
}