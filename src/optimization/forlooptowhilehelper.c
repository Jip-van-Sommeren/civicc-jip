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

void insertNewShit(node_st **head, node_st **varDeclHead, node_st *node)
{
    node_st *forNode = node;
    // if its a newly declared var in the loop we also need to make new vardecl, its not new obviously not needed/+.
    if (FOR_NEWVAR(forNode))
    {
        node_st *vardecl = ASTvardecl(NULL, NULL, strdup(FOR_VAR(forNode)), CT_int);
        node_st *tempVardecls = ASTvardecls(vardecl, NULL);
        insertVarDecl(varDeclHead, tempVardecls, NULL);
    }
    node_st *varlet = ASTvarlet(NULL, strdup(FOR_VAR(forNode)));
    VARLET_TYPE(varlet) = CT_int;
    node_st *newAssign = ASTassign(varlet, CCNcopy(FOR_START_EXPR(forNode)));
    node_st *tmpStmts = ASTstmts(newAssign, NULL);
    insertStmts(head, tmpStmts, NULL);
}

void processForLoop(node_st **head, node_st **varDeclHead, node_st *prev, node_st *node)
{
    if (!node)
        return;

    node_st *innerStmts = NULL;
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
    case NT_DOWHILE:

        innerStmts = returnStmtsBlock(node);
        while (innerStmts != NULL)
        {
            processForLoop(head, varDeclHead, prev, STMTS_STMT(innerStmts));
            innerStmts = STMTS_NEXT(innerStmts);
        }
        break;
    case NT_FOR:
        insertNewShit(head, varDeclHead, node);
        innerStmts = returnStmtsBlock(node);
        while (innerStmts != NULL)
        {
            processForLoop(head, varDeclHead, prev, STMTS_STMT(innerStmts));
            innerStmts = STMTS_NEXT(innerStmts);
        }
        break;

        break;
    default:
        break;
    }
}