#include "ccngen/ast.h"

void appendAssemblyListAndUpdateTail(node_st **assemblyList, node_st **assemblyListTail, node_st *assemblyEntry);
void appendConstantTable(node_st **constantTable, node_st **constantTableTail, node_st *constantTableEntry);
node_st *returnTypeAssembly(node_st *expr);
node_st *returnVarletAssembly(node_st *expr);
char *getBinOpOpAssembly(enum BinOpType op, enum Type type);
char *getMonOpOpAssembly(enum MonOpType op, enum Type type);
void generateAssemblyForExpr(node_st **head, node_st **tail, node_st *expr);
void generateAssemblyForStmt(node_st **head, node_st **tail, node_st *stmt);
void generateAssemblyForFundef(node_st **head, node_st **tail, node_st *node);
void insertAssembly(node_st **head, node_st *exprAssembly, node_st *varletAsm);
node_st *returnExprAssembly(node_st *expr);
void setToOutputFile(node_st *node);
