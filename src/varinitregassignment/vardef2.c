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

node_st *VDRATprogram(node_st *node)
{
    TRAVchildren(node);
    return node;
}

// node_st *VDRATfunbody(node_st *node)
// {

//     node_st *assignStmts = FUNBODY_STMTS(node); // Works if FUNBODY_STMS(node) is NULL or already has stmts in it.
//     printf("here\n");
//     if (assignStmts == NULL)
//     {
//         printf("here\n");
//     }
//     node_st *assignStmtsTail = NULL;

//     node_st *vardecls = FUNBODY_VARDECLS(node);
//     node_st *prevVardeclsNode = NULL;

//     while (vardecls != NULL)
//     {
//         node_st *vardecl = VARDECLS_VARDECL(vardecls);

//         node_st *varlet = ASTvarlet(NULL, strdup(VARDECL_NAME(vardecl)));
//         node_st *assign = ASTassign(varlet, makeExpr(VARDECL_INIT(vardecl)));

//         if (VARDECL_DIMS(vardecl) != NULL)
//         {
//             char str[20];
//             node_st *exprs = VARDECL_DIMS(vardecl);

//             // set forloop and dims as zero, will be made in the loop
//             // node_st *forloop = NULL;
//             // node_st *dims = NULL;
//             // // make exprs node for pseudo __allocate funargs
//             // node_st *fun_args = NULL;
//             while (exprs != NULL)
//             {
//                 // logic to add loop vardecls
//                 int i;
//                 // get dimscount for variable name logic, since they have been left free in prev traversal
//                 int dimsCount = checkExprDimension(VARDECL_DIMS(vardecl));

//                 sscanf(VAR_NAME(EXPRS_EXPR(exprs)), "tmp_%d", &i);

//                 sprintf(str, "tmp_%d", (i + 1 + dimsCount));
//                 // hardcode type, loop index has to be int
//                 // make vardecls for loop iteration and insert them
//                 node_st *newVarDecl = ASTvardecl(NULL, NULL, strdup(str), CT_int);
//                 node_st *newVardeclsNode = ASTvardecls(newVarDecl, NULL);
//                 // make vars for varlet indeces and recursively insert
//                 // node_st *var = ASTvar(NULL, strdup(str));
//                 // dims = ASTexprs(var, dims);

//                 // fun_args = ASTexprs(makeExpr(EXPRS_EXPR(exprs)), fun_args);
//                 // if (forloop == NULL)
//                 // {
//                 //     forloop = ASTfor(ASTnum(0), makeExpr(EXPRS_EXPR(exprs)), NULL, ASTstmts(assign, NULL), strdup(str));
//                 // }
//                 // else
//                 // {
//                 //     forloop = ASTfor(ASTnum(0), makeExpr(EXPRS_EXPR(exprs)), NULL, ASTstmts(forloop, NULL), strdup(str));
//                 // }
//                 // If this is the first new VarDecls node, set it as the start of the new list
//                 if (prevVardeclsNode == NULL)
//                 {
//                     // This means we're inserting right at the start
//                     VARDECLS_NEXT(newVardeclsNode) = vardecls;
//                     FUNBODY_VARDECLS(node) = newVardeclsNode;
//                 }
//                 else
//                 {
//                     // Inserting in the middle or at the end
//                     VARDECLS_NEXT(newVardeclsNode) = VARDECLS_NEXT(prevVardeclsNode);
//                     VARDECLS_NEXT(prevVardeclsNode) = newVardeclsNode;
//                 }

//                 prevVardeclsNode = newVardeclsNode;

//                 exprs = EXPRS_NEXT(exprs);
//             }
//             // set varlet indeces;
//             // VARLET_INDICES(varlet) = dims;
//             // node_st *funcall = ASTfuncall(fun_args, strdup("__allocate"));
//             // FUNCALL_TYPE(funcall) = CT_void;
//             // // make exprStmt to insert into the statements
//             // node_st *exprStmt = ASTexprstmt(funcall);

//             if (assignStmts == NULL)
//             {
//                 printf("here\n");
//                 // This is the first assignment statement.
//                 assignStmts = ASTstmts(assign, NULL);
//                 assignStmtsTail = assignStmts; // The tail is the same as head now.
//             }
//             else
//             {
//                 // Append to the tail and update the tail pointer.
//                 node_st *newStmtsNode = ASTstmts(assign, NULL);
//                 STMTS_NEXT(assignStmtsTail) = newStmtsNode; // Append to the end.
//                 assignStmtsTail = newStmtsNode;             // Update the tail.
//             }
//             // if (assignStmts == NULL)
//             // {
//             //     // This is the first assignment statement.
//             //     assignStmts = ASTstmts(exprStmt, NULL);
//             //     assignStmtsTail = assignStmts; // The tail is the same as head now.
//             // }
//             // else
//             // {
//             //     // Append to the tail and update the tail pointer.
//             //     node_st *newStmtsNode = ASTstmts(exprStmt, NULL);
//             //     STMTS_NEXT(assignStmtsTail) = newStmtsNode; // Append to the end.
//             //     assignStmtsTail = newStmtsNode;             // Update the tail.
//             // }

//             // assignStmts = ASTstmts(exprStmt, assignStmts);
//             // assignStmts = ASTstmts(forloop, assignStmts)
//         }
//         else
//         {
//             //
//             if (assignStmts == NULL)
//             {
//                 // This is the first assignment statement.
//                 assignStmts = ASTstmts(assign, NULL);
//                 assignStmtsTail = assignStmts; // The tail is the same as head now.
//             }
//             else
//             {
//                 // Append to the tail and update the tail pointer.
//                 node_st *newStmtsNode = ASTstmts(assign, NULL);
//                 STMTS_NEXT(assignStmtsTail) = newStmtsNode; // Append to the end.
//                 assignStmtsTail = newStmtsNode;             // Update the tail.
//             }
//             // assignStmts = ASTstmts(assign, assignStmts);
//             // free the vardecl init after the assign and set to zero
//             prevVardeclsNode = vardecls;
//         }
//         // free the vardecl init after the assign and set to zero
//         freeInitExpr(VARDECL_INIT(VARDECLS_VARDECL(vardecls)));
//         VARDECL_INIT(VARDECLS_VARDECL(vardecls)) = NULL;

//         // lastDecls = vardecls;
//         vardecls = VARDECLS_NEXT(vardecls);
//     }
//     // insert new stmts into funbody
//     FUNBODY_STMTS(node) = assignStmts;

//     return node;
// }

void appendExprAndUpdateTail(node_st **exprList, node_st **exprListTail, node_st *newExpr)
{
    // Check if the list is empty
    if (*exprList == NULL)
    {
        // This is the first expression in the list.
        *exprList = ASTexprs(newExpr, NULL);
        *exprListTail = *exprList; // The tail is the first node itself.
    }
    else
    {
        // There are already expressions in the list. Append the new expression.
        node_st *newExprNode = ASTexprs(newExpr, NULL);
        EXPRS_NEXT(*exprListTail) = newExprNode; // Append new expression to the end.
        *exprListTail = newExprNode;             // Update the tail to the new node.
    }
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
        node_st *varlet = ASTvarlet(NULL, strdup(VARDECL_NAME(vardecl)));
        node_st *assign = ASTassign(varlet, makeExpr(VARDECL_INIT(vardecl)));

        if (VARDECL_DIMS(vardecl) != NULL)
        {
            char str[20];
            node_st *exprs = VARDECL_DIMS(vardecl);
            // set forloop and dims as zero, will be made in the loop
            node_st *forloop = NULL;
            node_st *fun_args = NULL;
            node_st *fun_argsTail = NULL;
            node_st *dims = NULL;
            node_st *dimsTail = NULL;
            // // make exprs node for pseudo __allocate funargs
            while (exprs != NULL)
            {
                // logic to add loop vardecls
                int i;
                // get dimscount for variable name logic, since they have been left free in prev traversal
                int dimsCount = checkExprDimension(VARDECL_DIMS(vardecl));

                sscanf(VAR_NAME(EXPRS_EXPR(exprs)), "tmp_%d", &i);

                sprintf(str, "tmp_%d", (i + 1 + dimsCount));

                printf("%s\n", str);
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
                if (dimsCount == 1)
                {
                    forloop = ASTfor(ASTnum(0), makeExpr(EXPRS_EXPR(exprs)), NULL, ASTstmts(assign, NULL), strdup(str));
                }
                else if (dimsCount == 2 && forloop == NULL)
                {
                    forloop = ASTfor(ASTnum(0), makeExpr(EXPRS_EXPR(exprs)), NULL, NULL, strdup(str));
                }
                else if (dimsCount == 2 && forloop != NULL)
                {
                    FOR_BLOCK(forloop) = ASTstmts(ASTfor(ASTnum(0), makeExpr(EXPRS_EXPR(exprs)), NULL, ASTstmts(assign, NULL), strdup(str)), NULL);
                }
                else
                {
                    fprintf(stderr, "how did you get here? Should be impossible\n");
                }

                // Inserting in the middle or at the end
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
            VARLET_INDICES(varlet) = dims;

            node_st *funcall = ASTfuncall(fun_args, strdup("__allocate"));
            FUNCALL_TYPE(funcall) = CT_void;
            // // make exprStmt to insert into the statements
            node_st *exprStmt = ASTexprstmt(funcall);

            // If there are no new statements yet, this becomes the first one.
            if (newAssignStmts == NULL)
            {
                // This is the first pair of statements.
                newAssignStmts = ASTstmts(exprStmt, ASTstmts(forloop, NULL)); // exprStmt followed by assign
                newAssignStmtsTail = STMTS_NEXT(newAssignStmts);              // The tail is the assign node
            }
            else
            {
                // There are existing statements. Append both exprStmt and assign to the list.
                node_st *newAssignNode = ASTstmts(forloop, NULL);             // New assign node
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
