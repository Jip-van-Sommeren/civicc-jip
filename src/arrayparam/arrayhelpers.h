#include "ccngen/ast.h"
#define MAX_DIMENSIONS 5

typedef struct LoopInfo
{
    int depth;
    node_st *dims[MAX_DIMENSIONS];
} LoopInfo;

node_st *generateFlattenedIndexExpr(node_st **dims, node_st **indices, int numDims);
node_st **extractIndicesFromVarlet(node_st *varlet, int numDims);
node_st **extractIndicesFromVar(node_st *var, int numDims);
void transformAssignForFlattenedArray(node_st *assignStmt, node_st **dims, int numDimsVarlet, int numDimsVar);
node_st *returnStmtsBlock(node_st *node);
node_st *extractLoopStopExpr(node_st *node);
node_st *extractLoopStartExpr(node_st *node);
void processStatementNode(node_st *node, LoopInfo *loopInfo);
void collectValues(node_st *expr, node_st **valuesTail, node_st **flattenedValues);
