#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "ccngen/enum.h"

node_st *SRbinop(node_st *node)
{
    TRAVchildren(node);

    if (BINOP_OP(node) == BO_mul)
    {
        node_st *left = BINOP_LEFT(node);
        node_st *right = BINOP_RIGHT(node);
        int factor = 0;
        node_st *var_node = NULL;

        if (NODE_TYPE(left) == NT_NUM && (NUM_VAL(left) == 2 || NUM_VAL(left) == 3))
        {
            factor = NUM_VAL(left);
            var_node = right;
        }
        else if (NODE_TYPE(right) == NT_NUM && (NUM_VAL(right) == 2 || NUM_VAL(right) == 3))
        {
            factor = NUM_VAL(right);
            var_node = left;
        }

        if (factor > 0)
        {
            node = CCNfree(node);
            node = ASTbinop(var_node, var_node, BO_add);
            if (factor == 3)
            {
                node = ASTbinop(node, var_node, BO_add);
            }
        }
    }

    return node;
}