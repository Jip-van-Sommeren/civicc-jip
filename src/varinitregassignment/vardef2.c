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
    // Placeholder for the first new assignment statement.
    node_st *newAssignStmts = NULL;
    // Tail of the new assignment statements list.
    node_st *newAssignStmtsTail = NULL;

    node_st *vardecls = FUNBODY_VARDECLS(node);
    node_st *existingStmts = FUNBODY_STMTS(node);

    while (vardecls != NULL)
    {
        node_st *vardecl = VARDECLS_VARDECL(vardecls);

        if (VARDECL_INIT(vardecl) != NULL)
        {
            node_st *varlet = ASTvarlet(NULL, strdup(VARDECL_NAME(vardecl)));
            VARLET_TYPE(varlet) = VARDECL_TYPE(vardecl);
            node_st *assign = ASTassign(varlet, makeExpr(VARDECL_INIT(vardecl)));
            if ((NODE_TYPE(VARDECL_INIT(vardecl)) == NT_FUNCALL && strcmp(FUNCALL_NAME(VARDECL_INIT(vardecl)), "__allocate") == 0))
            {
                if (newAssignStmts == NULL)
                {
                    newAssignStmts = ASTstmts(assign, NULL);
                    newAssignStmtsTail = newAssignStmts;
                }
                else
                {
                    // Append to the list and update the tail.
                    node_st *newStmtsNode = ASTstmts(assign, NULL);
                    STMTS_NEXT(newAssignStmtsTail) = newStmtsNode;
                    newAssignStmtsTail = newStmtsNode;
                }
            }

            else if (NODE_TYPE(VARDECL_INIT(vardecl)) == NT_ARREXPR)
            {
                if (newAssignStmts == NULL)
                {
                    newAssignStmts = ASTstmts(assign, NULL);
                    newAssignStmtsTail = newAssignStmts;
                }
                else
                {
                    // Append to the list and update the tail.
                    node_st *newStmtsNode = ASTstmts(assign, NULL);
                    STMTS_NEXT(newAssignStmtsTail) = newStmtsNode;
                    newAssignStmtsTail = newStmtsNode;
                }
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
                // // make exprs node for pseudo __allocate funargs

                // make arrays to iterate in reverse order and build the nested for loops
                int dimsCount = checkExprDimension(VARDECL_DIMS(vardecl));
                node_st *dimsExpr[dimsCount];
                char *names[dimsCount];
                int dimsExprIndex = 0;
                while (exprs != NULL)
                {
                    // logic to add loop vardecls
                    int i = 0;
                    // get dimscount for variable name logic, since they have been left free in prev traversal

                    sscanf(VAR_NAME(EXPRS_EXPR(exprs)), "tmp_%d", &i);
                    if (i != 0)
                    {

                        sprintf(str, "tmp_%d", (i + 1 + dimsCount));
                        printf("%s\n", str);
                    }
                    else
                    {
                        sprintf(str, "tmp_%s", VAR_NAME(EXPRS_EXPR(exprs)));
                    }
                    // hardcode type, loop index has to be int
                    // make vardecls for loop iteration and insert them
                    node_st *newVarDecl = ASTvardecl(NULL, NULL, strdup(str), CT_int);
                    node_st *newVardeclsNode = ASTvardecls(newVarDecl, NULL);
                    // make vars for varlet indeces and recursively insert
                    node_st *var = ASTvar(NULL, strdup(str));

                    appendExprAndUpdateTail(&fun_args, &fun_argsTail, makeExpr(EXPRS_EXPR(exprs)));
                    appendExprAndUpdateTail(&dims, &dimsTail, var);
                    // super ugly solution but works for now,
                    // #todo maybe fix?
                    dimsExpr[dimsExprIndex] = makeExpr(EXPRS_EXPR(exprs));
                    names[dimsExprIndex] = strdup(str);
                    dimsExprIndex++;

                    if (VARDECLS_NEXT(vardecls) != NULL)
                    {
                        VARDECLS_NEXT(newVardeclsNode) = VARDECLS_NEXT(vardecls);
                        VARDECLS_NEXT(vardecls) = newVardeclsNode;
                    }
                    else
                    {
                        VARDECLS_NEXT(vardecls) = newVardeclsNode;
                    }
                    vardecls = VARDECLS_NEXT(vardecls);

                    exprs = EXPRS_NEXT(exprs);
                }
                node_st *innerMostStmt = ASTstmts(assign, NULL); // The innermost statement, initially just the assignment
                node_st *forLoop = NULL;
                for (int i = dimsCount; i > 0; i--)
                {
                    // Assuming you have a way to access the expression for the ith dimension
                    // For illustration, let's say you have an array or list `dimExprs` of expressions representing the stop condition for each dimension
                    node_st *expr = dimsExpr[i - 1]; // Adjust indexing as needed
                    char *str = names[i - 1];
                    if (i == dimsCount)
                    {
                        // For the innermost loop, include the initial assignment statement
                        forLoop = ASTfor(ASTnum(0), expr, NULL, innerMostStmt, str, false);
                    }
                    else
                    {
                        // For outer loops, nest the current `forLoop` as the block of the next loop
                        forLoop = ASTfor(ASTnum(0), expr, NULL, ASTstmts(forLoop, NULL), str, false);
                    }
                }

                VARLET_INDICES(varlet) = dims;

                node_st *funcall = ASTfuncall(fun_args, strdup("__allocate"));
                FUNCALL_TYPE(funcall) = CT_void;
                // // make exprStmt to insert into the statements
                node_st *exprStmt = ASTassign(ASTvarlet(NULL, strdup(VARDECL_NAME(vardecl))), funcall);

                // If there are no new statements yet, this becomes the first one.
                if (newAssignStmts == NULL)
                {
                    // This is the first pair of statements.
                    newAssignStmts = ASTstmts(exprStmt, ASTstmts(forLoop, NULL)); // exprStmt followed by assign
                    newAssignStmtsTail = STMTS_NEXT(newAssignStmts);              // The tail is the assign node
                }
                else
                {
                    // There are existing statements. Append both exprStmt and assign to the list.
                    node_st *newAssignNode = ASTstmts(forLoop, NULL);             // New assign node
                    node_st *newExprStmtNode = ASTstmts(exprStmt, newAssignNode); // New exprStmt node, pointing to the assign node
                    STMTS_NEXT(newAssignStmtsTail) = newExprStmtNode;             // Append newExprStmtNode to the end
                    newAssignStmtsTail = newAssignNode;                           // Update the tail to the new assign node
                }
            }
            else
            {
                if (newAssignStmts == NULL)
                {
                    newAssignStmts = ASTstmts(assign, NULL);
                    newAssignStmtsTail = newAssignStmts;
                }
                else
                {
                    // Append to the list and update the tail.
                    node_st *newStmtsNode = ASTstmts(assign, NULL);
                    STMTS_NEXT(newAssignStmtsTail) = newStmtsNode;
                    newAssignStmtsTail = newStmtsNode;
                }
            }

            // Reset the initialization expression of the vardecl.
            freeInitExpr(VARDECL_INIT(vardecl));
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
