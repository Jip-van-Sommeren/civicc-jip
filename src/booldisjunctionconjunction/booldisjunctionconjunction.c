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
#include "palm/ctinfo.h"
#include "vartypetostring.h"

void DBCinit()
{
    return;
}

void DBCfini()
{
    return;
}

node_st *DBCbinop(node_st *node)
{
    // Base case: if the node is not a binary operation, simply traverse its children
    // Process binary operations that are either conjunctions or disjunctions
    if ((BINOP_TYPE(node) == CT_bool) && (BINOP_OP(node) == BO_and || BINOP_OP(node) == BO_or || BINOP_OP(node) == BO_mul || BINOP_OP(node) == BO_add))
    {
        node_st *condExpr = BINOP_LEFT(node);
        node_st *thenExpr, *elseExpr;

        if (BINOP_OP(node) == BO_and || BINOP_OP(node) == BO_mul)
        {
            // For conjunctions: if left operand is true, evaluate right operand
            thenExpr = BINOP_RIGHT(node);
            elseExpr = ASTbool(false);
        }
        else
        { // BO_or
            // For disjunctions: if left operand is false, evaluate right operand
            thenExpr = ASTbool(true);
            elseExpr = BINOP_RIGHT(node);
        }

        // Create a new Tern node with the constructed condition, then, and else branches
        node_st *ternNode = ASTtern(condExpr, thenExpr, elseExpr);
        MEMfree(node->data.N_binop);
        MEMfree(node);
        return ternNode;
    }

    // For other types of binary operations, just traverse the children
    return node;
}
