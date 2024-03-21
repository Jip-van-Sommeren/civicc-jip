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
    node_st *assignStmts = NULL;

    node_st *decls = PROGRAM_DECLS(node);
    node_st *lastDecls = NULL;
    while (decls != NULL)
    {
        if (NODE_TYPE(DECLS_DECL(decls)) == NT_GLOBDEF)
        {
            // Extract the global definition node
            node_st *globDef = DECLS_DECL(decls);

            // Create an assignment statement for the global variable initialization
            node_st *var = ASTvarlet(NULL, strdup(GLOBDEF_NAME(globDef)));
            node_st *assign = ASTassign(var, makeExpr(GLOBDEF_INIT(globDef)));

            // Add the assignment to the list

            assignStmts = ASTstmts(assign, assignStmts); // This is the first assignment

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

    node_st *initFunBody = ASTfunbody(NULL, NULL, assignStmts);
    node_st *initFunDef = ASTfundef(initFunBody, NULL, strdup("__init"), CT_void, false);
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
            node_st *exprs = VARDECL_DIMS(vardecl);

            while (exprs != NULL)
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

                exprs = EXPRS_NEXT(exprs);
            }
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
            prevVardeclsNode = newVardeclsNode;
        }
        // Keep names free for for loops in next traversal
        i += dimsCount;

        varDeclsNode = VARDECLS_NEXT(varDeclsNode);
    }
    return node;
}
