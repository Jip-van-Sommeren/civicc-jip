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
#include "vartypetostring.h"
#include "palm/ctinfo.h"
#include "varinitregassignment/vardefhelper.h"
#include "arrayparam/arrayhelpers.h"
#include "global/globals.h"
#include "makeassemblyhelper.h"

node_st *MAFcast(node_st *node)
{
    node_st *AssemblylistTail = NULL;
    generateAssemblyForExpr(&CAST_ASSEMBLY(node), &AssemblylistTail, node);
    return node;
}
node_st *MAFfuncall(node_st *node)
{
    node_st *AssemblylistTail = NULL;
    generateAssemblyForExpr(&FUNCALL_ASSEMBLY(node), &AssemblylistTail, node);

    return node;
}
node_st *MAFvar(node_st *node)
{
    node_st *AssemblylistTail = NULL;
    generateAssemblyForExpr(&VAR_ASSEMBLY(node), &AssemblylistTail, node);
    return node;
}
node_st *MAFarrexpr(node_st *node)
{
    return node;
}
node_st *MAFtern(node_st *node)
{
    node_st *AssemblylistTail = NULL;
    generateAssemblyForExpr(&TERN_ASSEMBLY(node), &AssemblylistTail, node);
    return node;
}

node_st *MAFnum(node_st *node)
{
    node_st *AssemblylistTail = NULL;
    generateAssemblyForExpr(&NUM_ASSEMBLY(node), &AssemblylistTail, node);
    return node;
}
node_st *MAFfloat(node_st *node)
{
    node_st *AssemblylistTail = NULL;
    generateAssemblyForExpr(&FLOAT_ASSEMBLY(node), &AssemblylistTail, node);
    return node;
}

node_st *MAFbool(node_st *node)
{
    node_st *AssemblylistTail = NULL;
    generateAssemblyForExpr(&BOOL_ASSEMBLY(node), &AssemblylistTail, node);
    return node;
}
node_st *MAFbinop(node_st *node)
{
    TRAVchildren(node);
    node_st *AssemblylistTail = NULL;
    generateAssemblyForExpr(&BINOP_ASSEMBLY(node), &AssemblylistTail, node);

    return node;
}
node_st *MAFmonop(node_st *node)
{
    TRAVchildren(node);
    node_st *AssemblylistTail = NULL;
    generateAssemblyForExpr(&MONOP_ASSEMBLY(node), &AssemblylistTail, node);

    return node;
}