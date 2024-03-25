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
#include "vardefhelper.h"
#include "symboltable/sahelper.h"

void VDRAinit()
{

    return;
}

void VDRAfini()
{

    return;
}

node_st *VDRAprogram(node_st *node)
{
    node_st *assignStmts = NULL; // Placeholder for the first assignment in the list
    node_st *lastDecls = NULL;

    node_st *assignStmtsTail = NULL; // Tail of the list.

    node_st *decls = PROGRAM_DECLS(node);
    while (decls != NULL)
    {
        if (NODE_TYPE(DECLS_DECL(decls)) == NT_GLOBDEF)
        {
            // Extract the global definition node.
            node_st *globDef = DECLS_DECL(decls);

            // Create an assignment statement for the global variable initialization.
            node_st *var = ASTvarlet(NULL, strdup(GLOBDEF_NAME(globDef)));
            node_st *assign = ASTassign(var, makeExpr(GLOBDEF_INIT(globDef)));

            // Append the assignment to the list.
            if (assignStmts == NULL)
            {
                // This is the first assignment statement.
                assignStmts = ASTstmts(assign, NULL);
                assignStmtsTail = assignStmts; // The tail is the same as head now.
            }
            else
            {
                // Append to the tail and update the tail pointer.
                node_st *newStmtsNode = ASTstmts(assign, NULL);
                STMTS_NEXT(assignStmtsTail) = newStmtsNode; // Append to the end.
                assignStmtsTail = newStmtsNode;             // Update the tail.
            }

            freeInitExpr(GLOBDEF_INIT(DECLS_DECL(decls)));
            GLOBDEF_INIT(DECLS_DECL(decls)) = NULL;
        }
        else
        {
            TRAVopt(DECLS_DECL(decls));
        }
        lastDecls = decls;
        decls = DECLS_NEXT(decls);
    }

    // Create the __init function definition node with the assignments as its body.
    node_st *initFunBody = ASTfunbody(NULL, NULL, assignStmts);
    node_st *initFunDef = ASTfundef(initFunBody, NULL, strdup("__init"), CT_void, false);
    // Assume there's a way t

    DECLS_NEXT(lastDecls) = ASTdecls(initFunDef, NULL);

    return node; // Return the modified AST
}

node_st *VDRAfunbody(node_st *node)
{
    node_st *varDeclsNode = FUNBODY_VARDECLS(node);
    node_st *prevVardeclsNode = NULL;
    int i = 1;
    while (varDeclsNode != NULL)
    {
        node_st *vardecl = VARDECLS_VARDECL(varDeclsNode);
        int dimsCount = 0;
        if (VARDECL_DIMS(vardecl) != NULL)
        {
            dimsCount += checkExprDimension(VARDECL_DIMS(vardecl));
            char str[20];
            node_st *fun_args = NULL;
            node_st *fun_argsTail = NULL;
            node_st *exprs = VARDECL_DIMS(vardecl);
            while (exprs != NULL)
            {
                // no need to make new temp var if expr is already a var
                if (VARDECL_INIT(vardecl) == NULL)
                {
                    appendExprAndUpdateTail(&fun_args, &fun_argsTail, makeExpr(EXPRS_EXPR(exprs)));
                }
                if (NODE_TYPE(EXPRS_EXPR(exprs)) != NT_VAR)
                {

                    sprintf(str, "tmp_%d", i);
                    // hardcode type, array index has to be int
                    node_st *newVarDecl = ASTvardecl(NULL, makeExpr(EXPRS_EXPR(exprs)), strdup(str), CT_int);
                    EXPRS_EXPR(exprs) = makeVarNode(EXPRS_EXPR(exprs), str);

                    i++;
                    node_st *newVardeclsNode = ASTvardecls(newVarDecl, NULL);

                    if (prevVardeclsNode == NULL)
                    {
                        // This means we're inserting right at the start
                        VARDECLS_NEXT(newVardeclsNode) = varDeclsNode;
                        FUNBODY_VARDECLS(node) = newVardeclsNode;
                    }
                    else
                    {
                        // Inserting in the middle or at the end
                        VARDECLS_NEXT(newVardeclsNode) = VARDECLS_NEXT(prevVardeclsNode);
                        VARDECLS_NEXT(prevVardeclsNode) = newVardeclsNode;
                    }

                    prevVardeclsNode = newVardeclsNode;
                }
                exprs = EXPRS_NEXT(exprs);
            }
            if (VARDECL_INIT(vardecl) != NULL)
            {
                sprintf(str, "tmp_%d", i);
                node_st *newVarDecl = ASTvardecl(NULL, makeExpr(VARDECL_INIT(vardecl)), strdup(str), CT_int);
                VARDECL_INIT(vardecl) = makeVarNode(VARDECL_INIT(vardecl), str);
                i++;

                node_st *newVardeclsNode = ASTvardecls(newVarDecl, NULL);

                if (prevVardeclsNode == NULL)
                {
                    // This means we're inserting right at the start
                    VARDECLS_NEXT(newVardeclsNode) = varDeclsNode;
                    FUNBODY_VARDECLS(node) = newVardeclsNode;
                }
                else
                {
                    // Inserting in the middle or at the end
                    VARDECLS_NEXT(newVardeclsNode) = VARDECLS_NEXT(prevVardeclsNode);
                    VARDECLS_NEXT(prevVardeclsNode) = newVardeclsNode;
                }
            }
            else if (VARDECL_INIT(vardecl) == NULL)
            {

                VARDECL_INIT(vardecl) = ASTfuncall(fun_args, strdup("__allocate"));
            }
        }
        prevVardeclsNode = varDeclsNode;
        // Keep names free for for loops in next traversal
        i += dimsCount;

        varDeclsNode = VARDECLS_NEXT(varDeclsNode);
    }
    return node;
}
