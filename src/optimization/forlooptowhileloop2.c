/**
 * @file forlooptowhileloop2.c
 * @brief For loop to While loop generation.
 *
 * This file contains the replacement of for loop to a while loop and
 * the corresponding steps such as making the update assigns.
 *
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
#include "vartypetostring.h"
#include "palm/ctinfo.h"
#include "varinitregassignment/vardefhelper.h"
#include "arrayparam/arrayhelpers.h"

node_st *FTWTfor(node_st *node)
{
    TRAVchildren(node);
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
    node_st *varCond = ASTvar(NULL, strdup(FOR_VAR(node)));
    VAR_TYPE(varCond) = CT_int;
    VAR_INDEX(varCond) = FOR_VARINDEX(node);
    node_st *condExpr = ASTbinop(varCond, CCNcopy(FOR_STOP(node)), loopExtreme);
    node_st *bodyStmts = CCNcopy(FOR_BLOCK(node));

    // Transform for loop into while loop
    node_st *whileNode = ASTwhile(condExpr, bodyStmts);

    // If there's an update expression, it needs to be appended to the end of the while body

    node_st *var = ASTvar(NULL, strdup(FOR_VAR(node)));
    VAR_TYPE(var) = CT_int;
    VAR_INDEX(var) = FOR_VARINDEX(node);

    node_st *binopUpdate = ASTbinop(var, updateExpr, loopUpdater);
    BINOP_TYPE(binopUpdate) = CT_int;
    node_st *varlet = ASTvarlet(NULL, strdup(FOR_VAR(node)));
    VARLET_TYPE(varlet) = CT_int;
    VARLET_INDEX(varlet) = FOR_VARINDEX(node);
    node_st *updateAssign = ASTassign(varlet, binopUpdate);
    ASSIGN_UPDATE(updateAssign) = true;
    // Assuming you have a function to append statements to a block
    node_st *newStmtsNode = ASTstmts(updateAssign, NULL);

    insertStmtsAtEnd(&WHILE_BLOCK(whileNode), newStmtsNode);

    CCNfree(node);
    return whileNode;
}