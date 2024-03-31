/**
 * @file booleancast.c
 * @brief Generate ternary nodes for bool casts.
 *
 * Generate the ternary nodes.
 * @author Jip van Sommeren
 * @date 31-03-24
 */

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
#include "symboltable/sahelper.h"

void BCinit()
{
    return;
}

void BCfini()
{
    return;
}

static int count = 0;
node_st *BCcast(node_st *node)
{
    // Check if we are casting from boolean to int
    if (CAST_TYPE(node) == CT_int && getType(CAST_EXPR(node)) == CT_bool)
    {
        // The condition is the boolean expression itself
        node_st *condition = CAST_EXPR(node);

        // The 'then' part returns 1 for true
        node_st *thenExpr = ASTnum(1);

        // The 'else' part returns 0 for false
        node_st *elseExpr = ASTnum(0);

        // Create a new Ternary node representing the cast
        node_st *newTern = ASTtern(condition, thenExpr, elseExpr);

        // Since the original cast node is being replaced, you might need to free its memory
        MEMfree(node->data.N_cast);
        MEMfree(node);
        TERN_COUNT(newTern) = count;
        count += 2;
        TRAVchildren(newTern);
        return newTern;
    }
    if (CAST_TYPE(node) == CT_float && getType(CAST_EXPR(node)) == CT_bool)
    {
        // The condition is the boolean expression itself
        node_st *condition = CAST_EXPR(node);

        // The 'then' part returns 1 for true
        node_st *thenExpr = ASTfloat(1.0);

        // The 'else' part returns 0 for false
        node_st *elseExpr = ASTfloat(0.0);

        // Create a new Ternary node representing the cast
        node_st *newTern = ASTtern(condition, thenExpr, elseExpr);

        // Since the original cast node is being replaced, you might need to free its memory
        MEMfree(node->data.N_cast);
        MEMfree(node);
        TERN_COUNT(newTern) = count;
        count += 2;
        TRAVchildren(newTern);
        return newTern;
    }
    if (CAST_TYPE(node) == CT_bool && getType(CAST_EXPR(node)) == CT_int)
    {
        // The condition checks if the expression is not equal to zero
        node_st *condition = ASTbinop(CAST_EXPR(node), ASTnum(0), BO_ne);

        // The 'then' part for true (expression is not zero)
        node_st *thenExpr = ASTbool(true);

        // The 'else' part for false (expression is zero)
        node_st *elseExpr = ASTbool(false);

        // Create a new Ternary node representing the cast
        node_st *newTern = ASTtern(condition, thenExpr, elseExpr);

        // Since the original cast node is being replaced, you might need to free its memory
        MEMfree(node->data.N_cast);
        MEMfree(node);
        TERN_COUNT(newTern) = count;
        count += 2;
        TRAVchildren(newTern);
        return newTern;
    }
    if (CAST_TYPE(node) == CT_bool && getType(CAST_EXPR(node)) == CT_float)
    {
        // The condition checks if the expression is not equal to zero
        node_st *condition = ASTbinop(CAST_EXPR(node), ASTfloat(0.0), BO_ne);

        // The 'then' part for true (expression is not zero)
        node_st *thenExpr = ASTbool(true);

        // The 'else' part for false (expression is zero)
        node_st *elseExpr = ASTbool(false);

        // Create a new Ternary node representing the cast
        node_st *newTern = ASTtern(condition, thenExpr, elseExpr);

        // Since the original cast node is being replaced, you might need to free its memory
        MEMfree(node->data.N_cast);
        MEMfree(node);
        TERN_COUNT(newTern) = count;
        count += 2;
        TRAVchildren(newTern);
        return newTern;
    }

    // For cases that do not involve casting from boolean to int, just traverse the children
    TRAVchildren(node);
    return node;
}
