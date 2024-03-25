#include "ccngen/ast.h"
#include "ccngen/enum.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "palm/dbug.h"

bool getMonOpResultType(enum MonOpType op, enum Type operandType);
enum Type getBinOpResultType(enum BinOpType op, enum Type leftType, enum Type rightType);
enum Type getType(node_st *node);
char *getName(node_st *node);
