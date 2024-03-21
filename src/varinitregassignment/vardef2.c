#include "palm/memory.h"
#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "stdio.h"
#include "string.h"
#include "palm/dbug.h"
#include "palm/ctinfo.h"
#include "vartypetostring.h"
#include "vardefhelper.h"

void VDRATinit()
{
    return;
}

void VDRATfini()
{
    return;
}

node_st *VDRATprogram(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *VDRATfunbody(node_st *node)
{

    node_st *assignStmts = FUNBODY_STMTS(node); // Works if FUNBODY_STMS(node) is NULL or already has stmts in it.

    node_st *vardecls = FUNBODY_VARDECLS(node);
    node_st *prevVardeclsNode = NULL;

    while (vardecls != NULL)
    {
        node_st *vardecl = VARDECLS_VARDECL(vardecls);

        node_st *varlet = ASTvarlet(NULL, strdup(VARDECL_NAME(vardecl)));
        node_st *assign = ASTassign(varlet, makeExpr(VARDECL_INIT(vardecl)));

        if (VARDECL_DIMS(vardecl) != NULL)
        {
            char str[20];
            node_st *exprs = VARDECL_DIMS(vardecl);

            // set forloop and dims as zero, will be made in the loop
            node_st *forloop = NULL;
            node_st *dims = NULL;
            // make exprs node for pseudo __allocate funargs
            node_st *fun_args = NULL;
            while (exprs != NULL)
            {
                // logic to add loop vardecls
                int i;
                // get dimscount for variable name logic, since they have been left free in prev traversal
                int dimsCount = checkExprDimension(VARDECL_DIMS(vardecl));

                sscanf(VAR_NAME(EXPRS_EXPR(exprs)), "tmp_%d", &i);

                sprintf(str, "tmp_%d", (i + 1 + dimsCount));
                // hardcode type, loop index has to be int
                // make vardecls for loop iteration and insert them
                node_st *newVarDecl = ASTvardecl(NULL, NULL, strdup(str), CT_int);
                node_st *newVardeclsNode = ASTvardecls(newVarDecl, NULL);
                // make vars for varlet indeces and recursively insert
                node_st *var = ASTvar(NULL, strdup(str));
                dims = ASTexprs(var, dims);

                fun_args = ASTexprs(makeExpr(EXPRS_EXPR(exprs)), fun_args);
                if (forloop == NULL)
                {
                    forloop = ASTfor(ASTnum(0), makeExpr(EXPRS_EXPR(exprs)), NULL, ASTstmts(assign, NULL), strdup(str));
                }
                else
                {
                    forloop = ASTfor(ASTnum(0), makeExpr(EXPRS_EXPR(exprs)), NULL, ASTstmts(forloop, NULL), strdup(str));
                }
                // If this is the first new VarDecls node, set it as the start of the new list
                if (prevVardeclsNode == NULL)
                {
                    // This means we're inserting right at the start
                    VARDECLS_NEXT(newVardeclsNode) = vardecls;
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
            // set varlet indeces;
            VARLET_INDICES(varlet) = dims;
            node_st *funcall = ASTfuncall(fun_args, strdup("__allocate"));
            FUNCALL_TYPE(funcall) = CT_void;
            // make exprStmt to insert into the statements
            node_st *exprStmt = ASTexprstmt(funcall);
            assignStmts = ASTstmts(forloop, assignStmts);
            assignStmts = ASTstmts(exprStmt, assignStmts);
        }
        else
        {
            //
            assignStmts = ASTstmts(assign, assignStmts);
            // free the vardecl init after the assign and set to zero
            prevVardeclsNode = vardecls;
        }
        // free the vardecl init after the assign and set to zero
        freeInitExpr(VARDECL_INIT(VARDECLS_VARDECL(vardecls)));
        VARDECL_INIT(VARDECLS_VARDECL(vardecls)) = NULL;

        // lastDecls = vardecls;
        vardecls = VARDECLS_NEXT(vardecls);
    }
    // insert new stmts into funbody
    FUNBODY_STMTS(node) = assignStmts;

    return node;
}
