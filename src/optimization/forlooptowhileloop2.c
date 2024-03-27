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
#include "vartypetostring.h"
#include "palm/ctinfo.h"
#include "varinitregassignment/vardefhelper.h"
#include "arrayparam/arrayhelpers.h"

node_st *FTWTfor(node_st *node)
{
    TRAVchildren(node);
    // node_st *initExpr = FOR_START_EXPR(node);
    enum BinOpType loopUpdater = BO_add;
    enum BinOpType loopExtreme = BO_lt;
    node_st *step = NULL;

    if (FOR_STEP(node) != NULL && NODE_TYPE(FOR_STEP(node)) == NT_MONOP && MONOP_OP(FOR_STEP(node)) == MO_neg)
    {
        loopUpdater = BO_sub;
        loopExtreme = BO_gt;
        step = ASTnum((NUM_VAL(MONOP_OPERAND((FOR_STEP(node))))));
    }
    else if (FOR_STEP(node) != NULL)
    {
        step = CCNcopy(FOR_STEP(node));
    }
    else
    {
        step = ASTnum(1);
    }
    node_st *updateExpr = step;
    node_st *condExpr = ASTbinop(ASTvar(NULL, strdup(FOR_VAR(node))), CCNcopy(FOR_STOP(node)), loopExtreme);
    node_st *bodyStmts = CCNcopy(FOR_BLOCK(node));

    // Transform for loop into while loop
    node_st *whileNode = ASTwhile(condExpr, bodyStmts);

    // If there's an update expression, it needs to be appended to the end of the while body
    if (updateExpr)
    {
        node_st *binopUpdate = ASTbinop(ASTvar(NULL, strdup(FOR_VAR(node))), updateExpr, loopUpdater);
        node_st *updateAssign = ASTassign(ASTvarlet(NULL, strdup(FOR_VAR(node))), binopUpdate);
        // Assuming you have a function to append statements to a block
        node_st *newStmtsNode = ASTstmts(updateAssign, NULL);

        STMTS_NEXT(newStmtsNode) = WHILE_BLOCK(whileNode);
        WHILE_BLOCK(whileNode) = newStmtsNode;
    }
    CCNfree(node);
    return whileNode;
}