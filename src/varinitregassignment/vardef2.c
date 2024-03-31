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

void VDRATinit()
{
    return;
}

void VDRATfini()
{
    return;
}

node_st *VDRATfunbody(node_st *node)
{
    TRAVchildren(node);
    // Placeholder for the first new assignment statement.
    node_st *newAssignStmts = NULL;
    node_st *newAssignStmtsTail = NULL;

    node_st *vardecls = FUNBODY_VARDECLS(node);
    node_st *existingStmts = FUNBODY_STMTS(node);
    // added global counter to not rely on sscanf, and possibly introduce errors that way
    // after testing proofed quite reliable
    int j = 0;
    while (vardecls != NULL)
    {
        node_st *vardecl = VARDECLS_VARDECL(vardecls);

        if (VARDECL_INIT(vardecl) != NULL)
        {
            node_st *varlet = ASTvarlet(NULL, strdup(VARDECL_NAME(vardecl)));
            VARLET_TYPE(varlet) = VARDECL_TYPE(vardecl);
            node_st *assign = ASTassign(varlet, CCNcopy(VARDECL_INIT(vardecl)));
            if ((NODE_TYPE(VARDECL_INIT(vardecl)) == NT_FUNCALL && strcmp(FUNCALL_NAME(VARDECL_INIT(vardecl)), "__allocate") == 0))
            {
                appendAssignStmt(&newAssignStmts, &newAssignStmtsTail, assign);
            }

            else if (NODE_TYPE(VARDECL_INIT(vardecl)) == NT_ARREXPR)
            {
                appendAssignStmt(&newAssignStmts, &newAssignStmtsTail, assign);
            }

            else if (VARDECL_DIMS(vardecl) != NULL)
            {
                char str[20];
                node_st *exprs = VARDECL_DIMS(vardecl);
                // set forloop and dims as zero, will be made in the loop
                node_st *fun_args = NULL;
                node_st *fun_argsTail = NULL;
                node_st *dims = NULL;
                node_st *dimsTail = NULL;
                // make arrays to iterate in reverse order and build the nested for loops
                int dimsCount = checkExprDimension(VARDECL_DIMS(vardecl));
                j += dimsCount + 1;
                node_st *dimsExpr[dimsCount];
                char *names[dimsCount];
                int dimsExprIndex = 0;
                while (exprs != NULL)
                {
                    j++;
                    sprintf(str, "tmp_%d", j);
                    // make vardecls for loop iteration and insert them
                    node_st *newVarDecl = ASTvardecl(NULL, NULL, strdup(str), CT_int);
                    node_st *newVardeclsNode = ASTvardecls(newVarDecl, NULL);
                    // make vars for varlet indeces and recursively insert
                    node_st *var = ASTvar(NULL, strdup(str));

                    appendExprAndUpdateTail(&fun_args, &fun_argsTail, CCNcopy(EXPRS_EXPR(exprs)));
                    appendExprAndUpdateTail(&dims, &dimsTail, var);

                    dimsExpr[dimsExprIndex] = CCNcopy(EXPRS_EXPR(exprs));
                    names[dimsExprIndex] = strdup(str);
                    dimsExprIndex++;

                    insertVarDeclAfterCurrent(vardecls, newVardeclsNode);
                    vardecls = VARDECLS_NEXT(vardecls);

                    exprs = EXPRS_NEXT(exprs);
                }

                node_st *forLoop = createNestedForLoops(dimsExpr, names, dimsCount, assign);
                VARLET_INDICES(varlet) = dims;

                node_st *funcall = ASTfuncall(fun_args, strdup("__allocate"));
                FUNCALL_TYPE(funcall) = CT_void;
                // // make exprStmt to insert into the statements
                node_st *assignMem = ASTassign(ASTvarlet(NULL, strdup(VARDECL_NAME(vardecl))), funcall);

                // If there are no new statements yet, this becomes the first one.
                appendAssignAndForLoopStmts(&newAssignStmts, &newAssignStmtsTail, assignMem, forLoop);
            }
            else
            {
                appendAssignStmt(&newAssignStmts, &newAssignStmtsTail, assign);
            }

            // Reset the initialization expression of the vardecl.
            CCNfree(VARDECL_INIT(vardecl));
            VARDECL_INIT(vardecl) = NULL;
        }
        vardecls = VARDECLS_NEXT(vardecls);
    }

    // After processing all vardecls and creating new assignment statements,
    // append the existing statements to the tail of the new statements list.
    if (newAssignStmtsTail != NULL)
    {
        STMTS_NEXT(newAssignStmtsTail) = existingStmts;
    }
    else
    {
        // If no new statements were created, just keep the existing ones.
        newAssignStmts = existingStmts;
    }

    // Update the function body with the new list of statements.
    FUNBODY_STMTS(node) = newAssignStmts;

    return node;
}
