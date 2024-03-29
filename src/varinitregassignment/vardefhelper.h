#include "ccngen/ast.h"

node_st *makeVarNode(node_st *expr, char *name);
node_st *exprsToExprs(node_st *dims);
node_st *idsToExprs(node_st *dims);
void appendExprAndUpdateTail(node_st **exprList, node_st **exprListTail, node_st *newExpr);
void appendSymbolTableAndUpdateTail(node_st **symbolTable, node_st **symbolTableTail, node_st *newEntry);
void appendAssignStmt(node_st **assignStmts, node_st **assignStmtsTail, node_st *assign);
node_st *createNestedForLoops(node_st **dimsExpr, char **names, int dimsCount, node_st *assignStmt);
void appendAssignAndForLoopStmts(node_st **stmtsHead, node_st **stmtsTail, node_st *assignMem, node_st *forLoop);
void insertVarDeclAfterCurrent(node_st *vardecls, node_st *newVardeclsNode);
void insertVarDecl(node_st **head, node_st *newVardeclsNode, node_st *prevVardeclsNode);
void insertVarDeclAtEnd(node_st **head, node_st *newVardeclsNode);
int insertVarDeclAtEndAndReturnCount(node_st **head, node_st *newVardeclsNode);
void insertStmts(node_st **head, node_st *newStmtsNode, node_st *prevStmtsNode);