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

node_st *MACprogram(node_st *node)
{
    openGlobalOutputFile();
    TRAVchildren(node);
    return node;
}

node_st *MACfundef(node_st *node)
{

    node_st *AssemblylistTail = NULL;
    generateAssemblyForFundef(&FUNDEF_ASSEMBLYLIST(node), &AssemblylistTail, node);
    if (global.outputFile != NULL && FUNDEF_ASSEMBLYLIST(node) != NULL)
    {
        setToOutputFile(FUNDEF_ASSEMBLYLIST(node));
    }
    TRAVchildren(node);
    return node;
}