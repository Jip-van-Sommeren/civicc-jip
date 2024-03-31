/**
 * @file forlooptowhileloop.c
 * @brief For loop to While loop generation.
 *
 * This file contains the initital steps for the conversion of for loop to
 * while loop, it sets the initializers for the loop iterators.
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

void insertNewAssigns(node_st **head, node_st **varDeclHead, node_st *node)
{
    node_st *forNode = node;
    int count = 0;
    // if its a newly declared var in the loop we also need to make new vardecl, its not new obviously not needed/+.

    if (FOR_NEWVAR(node))
    {
        node_st *vardecl = ASTvardecl(NULL, NULL, strdup(FOR_VAR(forNode)), CT_int);
        node_st *tempVardecls = ASTvardecls(vardecl, NULL);
        count = insertVarDeclAtEndAndReturnCount(varDeclHead, tempVardecls);
        FOR_VARINDEX(node) = count;
    }

    node_st *varlet = ASTvarlet(NULL, strdup(FOR_VAR(forNode)));

    VARLET_INDEX(varlet) = count;

    VARLET_TYPE(varlet) = CT_int;
    node_st *newAssign = ASTassign(varlet, CCNcopy(FOR_START_EXPR(forNode)));
    ASSIGN_UPDATE(newAssign) = false;
    node_st *tmpStmts = ASTstmts(newAssign, NULL);
    insertStmts(head, tmpStmts, NULL);
}

void processForLoop(node_st **head, node_st **varDeclHead, node_st *prev, node_st *node)
{
    if (!node)
        return;

    node_st *innerStmts = NULL;
    node_st *prevInner = NULL;
    switch (NODE_TYPE(node))
    {
    case NT_IFELSE:
        innerStmts = IFELSE_THEN(node);
        while (innerStmts != NULL)
        {
            processForLoop(head, varDeclHead, prev, STMTS_STMT(innerStmts));
            innerStmts = STMTS_NEXT(innerStmts);
        }
        innerStmts = IFELSE_ELSE_BLOCK(node);
        while (innerStmts != NULL)
        {
            processForLoop(head, varDeclHead, prev, STMTS_STMT(innerStmts));
            innerStmts = STMTS_NEXT(innerStmts);
        }
        break;
    case NT_WHILE:
        innerStmts = returnStmtsBlock(node);
        while (innerStmts != NULL)
        {
            processForLoop(&WHILE_BLOCK(node), varDeclHead, prevInner, STMTS_STMT(innerStmts));
            prevInner = innerStmts;
            innerStmts = STMTS_NEXT(innerStmts);
        }
        break;
    case NT_DOWHILE:

        innerStmts = returnStmtsBlock(node);
        while (innerStmts != NULL)
        {
            processForLoop(&DOWHILE_BLOCK(node), varDeclHead, prevInner, STMTS_STMT(innerStmts));
            prevInner = innerStmts;
            innerStmts = STMTS_NEXT(innerStmts);
        }
        break;
    case NT_FOR:
        insertNewAssigns(head, varDeclHead, node);
        innerStmts = returnStmtsBlock(node);
        while (innerStmts != NULL)
        {
            processForLoop(&FOR_BLOCK(node), varDeclHead, prevInner, STMTS_STMT(innerStmts));
            prev = innerStmts;
            innerStmts = STMTS_NEXT(innerStmts);
        }
        break;

        break;
    default:
        break;
    }
}
