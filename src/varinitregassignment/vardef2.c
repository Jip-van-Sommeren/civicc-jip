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
#include "palm/ctinfo.h"
#include "vartypetostring.h"
#include "vardefhelper.h"

void VDRATinit()
{
    return;
}

void VDRATfini()
{
    return;
}

node_st *VDRATprogram(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *VDRATfunbody(node_st *node)
{
    TRAVchildren(node);
    return node;
}
