#include "ccngen/ast.h"
node_st *makeExpr(node_st *expr);
node_st *makeVarNode(node_st *expr, char *name);
void freeInitExpr(node_st *expr);
