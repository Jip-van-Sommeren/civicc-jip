#include <stdio.h>
#include "ccngen/ast.h"
#include "string.h"
#include "palm/memory.h"
#include "ccngen/enum.h"
#include "ccn/ccn.h"

node_st *makeVarNode(node_st *expr, char *name)
{
    node_st *entry;
    switch (NODE_TYPE(expr))
    {
    case NT_NUM:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = CT_int;
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_num);
        MEMfree(expr);
        break;
    case NT_FLOAT:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = CT_float;
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_float);
        MEMfree(expr);
        break;
    case NT_BOOL:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = CT_bool;
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_bool);
        MEMfree(expr);
        break;
    case NT_FUNCALL:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = FUNCALL_TYPE(expr);
        VAR_SYMBOLENTRY(entry) = FUNCALL_SYMBOLENTRY(expr);
        MEMfree(expr->data.N_funcall->name);
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_funcall);
        MEMfree(expr);
        break;
    case NT_VAR:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = VAR_TYPE(expr);
        VAR_SYMBOLENTRY(entry) = VAR_SYMBOLENTRY(expr);
        MEMfree(expr->data.N_var->name);
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_var);
        MEMfree(expr);
        break;
    case NT_VARLET:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = VARLET_TYPE(expr);
        VAR_SYMBOLENTRY(entry) = VARLET_SYMBOLENTRY(expr);
        MEMfree(expr->data.N_varlet->name);
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_varlet);
        MEMfree(expr);
        break;
    case NT_BINOP:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = BINOP_TYPE(expr);
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_binop);
        MEMfree(expr);
        break;
    case NT_MONOP:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = MONOP_TYPE(expr);
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_monop);
        MEMfree(expr);
        break;
    case NT_CAST:
        entry = ASTvar(NULL, strdup(name));
        VAR_TYPE(entry) = CAST_TYPE(expr);
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_cast);
        MEMfree(expr);
        break;
    case NT_ARREXPR:
        entry = ASTvar(NULL, strdup(name));
        MEMfree(NODE_FILENAME(expr));
        MEMfree(expr->data.N_arrexpr);
        MEMfree(expr);
        break;
    default:
        break;
    }
    return entry;
}

node_st *exprsToExprs(node_st *dims)
{
    node_st *exprsHead = NULL;     // Head of the new exprs list
    node_st *lastExprsNode = NULL; // To keep track of the last node in the new list

    while (dims != NULL)
    {
        // Assuming dims is a list of Expr nodes

        TRAVdo(EXPRS_EXPR(dims));
        node_st *currentExpr = CCNcopy(EXPRS_EXPR(dims)); // Extract the current expression from dims

        // Create a new Exprs node with the current expression
        node_st *newExprsNode = ASTexprs(currentExpr, NULL);

        if (exprsHead == NULL)
        {
            // If this is the first node, set it as the head of the list
            exprsHead = newExprsNode;
        }
        else
        {
            // Otherwise, link it to the last node in the list
            EXPRS_NEXT(lastExprsNode) = newExprsNode;
        }

        // Update the last node to the current one
        lastExprsNode = newExprsNode;

        // Move to the next set of dimensions
        dims = EXPRS_NEXT(dims);
    }

    return exprsHead;
}

node_st *idsToExprs(node_st *dims)
{
    node_st *exprsHead = NULL;     // Head of the new exprs list
    node_st *lastExprsNode = NULL; // To keep track of the last node in the new list

    while (dims != NULL)
    {
        // Assuming dims is a list of Expr nodes
        node_st *currentExpr = ASTvar(NULL, strdup(IDS_NAME(dims)));
        VAR_TYPE(currentExpr) = CT_int;

        // Create a new Exprs node with the current expression
        node_st *newExprsNode = ASTexprs(currentExpr, NULL);

        if (exprsHead == NULL)
        {
            // If this is the first node, set it as the head of the list
            exprsHead = newExprsNode;
        }
        else
        {
            // Otherwise, link it to the last node in the list
            EXPRS_NEXT(lastExprsNode) = newExprsNode;
        }

        // Update the last node to the current one
        lastExprsNode = newExprsNode;

        // Move to the next set of dimensions
        dims = IDS_NEXT(dims);
    }

    return exprsHead;
}

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

void appendAssignStmt(node_st **assignStmts, node_st **assignStmtsTail, node_st *assign)
{
    if (*assignStmts == NULL)
    {
        // This is the first assignment statement.
        *assignStmts = ASTstmts(assign, NULL);
        *assignStmtsTail = *assignStmts; // The tail is the same as head now.
    }
    else
    {
        // Append to the list and update the tail.
        node_st *newStmtsNode = ASTstmts(assign, NULL);
        STMTS_NEXT(*assignStmtsTail) = newStmtsNode; // Append to the end.
        *assignStmtsTail = newStmtsNode;             // Update the tail.
    }
}

void appendSymbolTableAndUpdateTail(node_st **symbolTable, node_st **symbolTableTail, node_st *newEntry)
{
    // Check if the list is empty
    if (*symbolTable == NULL)
    {
        // This is the first expression in the list.
        *symbolTable = ASTsymboltable(newEntry, NULL);
        *symbolTableTail = *symbolTable; // The tail is the first node itself.
    }
    else
    {
        // There are already expressions in the list. Append the new expression.
        node_st *newSymbolTableNode = ASTsymboltable(newEntry, NULL);
        EXPRS_NEXT(*symbolTableTail) = newSymbolTableNode; // Append new expression to the end.
        *symbolTableTail = newSymbolTableNode;             // Update the tail to the new node.
    }
}

node_st *createNestedForLoops(node_st **dimsExpr, char **names, int dimsCount, node_st *assignStmt)
{
    node_st *innerMostStmt = ASTstmts(assignStmt, NULL); // The innermost statement, initially just the assignment
    node_st *forLoop = NULL;
    for (int i = dimsCount; i > 0; i--)
    {
        // Assuming you have a way to access the expression for the ith dimension
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
    return forLoop; // Returns the outermost loop
}

void appendAssignAndForLoopStmts(node_st **stmtsHead, node_st **stmtsTail, node_st *assignMem, node_st *forLoop)
{
    if (*stmtsHead == NULL)
    {
        // This is the first pair of statements.
        *stmtsHead = ASTstmts(assignMem, ASTstmts(forLoop, NULL)); // exprStmt followed by forLoop
        *stmtsTail = STMTS_NEXT(*stmtsHead);                       // The tail is the forLoop node
    }
    else
    {
        // There are existing statements. Append both exprStmt and forLoop to the list.
        node_st *newForLoopNode = ASTstmts(forLoop, NULL);              // New forLoop node
        node_st *newExprStmtNode = ASTstmts(assignMem, newForLoopNode); // New exprStmt node, pointing to the forLoop node
        STMTS_NEXT(*stmtsTail) = newExprStmtNode;                       // Append newExprStmtNode to the end
        *stmtsTail = newForLoopNode;                                    // Update the tail to the new forLoop node
    }
}

void insertVarDeclAfterCurrent(node_st *vardecls, node_st *newVardeclsNode)
{
    if (VARDECLS_NEXT(vardecls) != NULL)
    {
        // There is a next vardecl, insert newVardeclsNode in between
        VARDECLS_NEXT(newVardeclsNode) = VARDECLS_NEXT(vardecls);
    }
    // Whether or not there's a next vardecl, link newVardeclsNode right after current vardecls
    VARDECLS_NEXT(vardecls) = newVardeclsNode;
}

void insertVarDecl(node_st **head, node_st *newVardeclsNode, node_st *prevVardeclsNode)
{
    if (prevVardeclsNode == NULL)
    {
        // Inserting at the start of the list
        VARDECLS_NEXT(newVardeclsNode) = *head;
        *head = newVardeclsNode;
    }
    else
    {
        // Inserting in the middle or at the end
        VARDECLS_NEXT(newVardeclsNode) = VARDECLS_NEXT(prevVardeclsNode);
        VARDECLS_NEXT(prevVardeclsNode) = newVardeclsNode;
    }
}

int insertVarDeclAtEndAndReturnCount(node_st **head, node_st *newVardeclsNode)
{
    int count = 0;
    if (*head == NULL)
    {
        // The list is empty, so newVardeclsNode becomes the head
        *head = newVardeclsNode;
    }
    else
    {
        // Traverse the list to find the last node
        node_st *current = *head;
        while (VARDECLS_NEXT(current) != NULL)
        {
            count++;
            current = VARDECLS_NEXT(current);
        }
        // Insert newVardeclsNode at the end of the list
        VARDECLS_NEXT(current) = newVardeclsNode;
    }
    return count;
}

void insertVarDeclAtEnd(node_st **head, node_st *newVardeclsNode)
{
    if (*head == NULL)
    {
        // The list is empty, so newVardeclsNode becomes the head
        *head = newVardeclsNode;
    }
    else
    {
        // Traverse the list to find the last node
        node_st *current = *head;
        while (VARDECLS_NEXT(current) != NULL)
        {
            current = VARDECLS_NEXT(current);
        }
        // Insert newVardeclsNode at the end of the list
        VARDECLS_NEXT(current) = newVardeclsNode;
    }
}

void insertStmts(node_st **head, node_st *newStmtsNode, node_st *prevStmtsNode)
{
    if (prevStmtsNode == NULL)
    {
        // Inserting at the start of the list
        STMTS_NEXT(newStmtsNode) = *head;
        *head = newStmtsNode;
    }
    else
    {
        // Inserting in the middle or at the end
        STMTS_NEXT(newStmtsNode) = STMTS_NEXT(prevStmtsNode);
        STMTS_NEXT(prevStmtsNode) = newStmtsNode;
    }
}
