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

node_st *MASassign(node_st *node)
{
    node_st *AssemblylistTail = NULL;
    generateAssemblyForStmt(&ASSIGN_ASSEMBLY(node), &AssemblylistTail, node);

    return node;
}
node_st *MASexprstmt(node_st *node)
{
    node_st *AssemblylistTail = NULL;
    generateAssemblyForStmt(&EXPRSTMT_ASSEMBLY(node), &AssemblylistTail, node);
    return node;
}
node_st *MASifelse(node_st *node)
{
    node_st *AssemblylistTail = NULL;
    generateAssemblyForStmt(&IFELSE_ASSEMBLY(node), &AssemblylistTail, node);
    return node;
}
node_st *MASwhile(node_st *node)
{
    node_st *AssemblylistTail = NULL;
    generateAssemblyForStmt(&WHILE_ASSEMBLY(node), &AssemblylistTail, node);

    return node;
}
node_st *MASdowhile(node_st *node)
{
    node_st *AssemblylistTail = NULL;
    generateAssemblyForStmt(&DOWHILE_ASSEMBLY(node), &AssemblylistTail, node);
    return node;
}

node_st *MASfor(node_st *node)
{
    // i put in here for semantic but there should be no for nodes in the ast at this stage.
    node_st *AssemblylistTail = NULL;
    generateAssemblyForStmt(&FOR_ASSEMBLY(node), &AssemblylistTail, node);
    return node;
}
node_st *MASreturn(node_st *node)
{
    node_st *AssemblylistTail = NULL;
    generateAssemblyForStmt(&RETURN_ASSEMBLY(node), &AssemblylistTail, node);
    return node;
}
