char *VarTypeToString(enum Type type);
char *BinopToString(enum BinOpType type);
char *MonopToString(enum MonOpType type);
int checkExprDimension(node_st *dims);
int checkParamDimension(node_st *dims);
node_st *exprsToExprsNode(node_st *dims);
node_st *idsToExprsNode(node_st *dims);