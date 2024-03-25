#include "ccngen/ast.h"
node_st *makeExpr(node_st *expr);
node_st *makeVarNode(node_st *expr, char *name);
node_st *makeVarNodeNoFree(node_st *expr, char *name);
void freeInitExpr(node_st *expr);
node_st *exprsToExprs(node_st *dims);
node_st *idsToExprs(node_st *dims);
void appendExprAndUpdateTail(node_st **exprList, node_st **exprListTail, node_st *newExpr);