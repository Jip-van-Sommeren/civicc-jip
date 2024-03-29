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

node_st *MATcast(node_st *node)
{
    TRAVchildren(node);
    CCNfree(CAST_ASSEMBLY(node));
    return node;
}
node_st *MATfuncall(node_st *node)
{
    TRAVchildren(node);
    CCNfree(FUNCALL_ASSEMBLY(node));
    return node;
}
node_st *MATvar(node_st *node)
{
    TRAVchildren(node);
    CCNfree(VAR_ASSEMBLY(node));
    return node;
}
node_st *MATarrexpr(node_st *node)
{
    return node;
}
node_st *MATtern(node_st *node)
{
    TRAVchildren(node);
    CCNfree(TERN_ASSEMBLY(node));
    return node;
}

node_st *MATnum(node_st *node)
{
    TRAVchildren(node);
    CCNfree(NUM_ASSEMBLY(node));
    return node;
}
node_st *MATfloat(node_st *node)
{
    TRAVchildren(node);
    CCNfree(FLOAT_ASSEMBLY(node));
    return node;
}

node_st *MATbool(node_st *node)
{
    TRAVchildren(node);
    CCNfree(BOOL_ASSEMBLY(node));
    return node;
}
node_st *MATbinop(node_st *node)
{
    TRAVchildren(node);
    CCNfree(BINOP_ASSEMBLY(node));

    return node;
}
node_st *MATmonop(node_st *node)
{
    TRAVchildren(node);
    CCNfree(MONOP_ASSEMBLY(node));
    return node;
}

node_st *MATassign(node_st *node)
{
    TRAVchildren(node);
    CCNfree(ASSIGN_ASSEMBLY(node));

    return node;
}
node_st *MATexprstmt(node_st *node)
{
    TRAVchildren(node);
    CCNfree(EXPRSTMT_ASSEMBLY(node));
    return node;
}
node_st *MATifelse(node_st *node)
{
    TRAVchildren(node);
    CCNfree(IFELSE_ASSEMBLY(node));
    return node;
}
node_st *MATwhile(node_st *node)
{
    TRAVchildren(node);
    CCNfree(WHILE_ASSEMBLY(node));
    return node;
}
node_st *MATdowhile(node_st *node)
{
    TRAVchildren(node);
    CCNfree(DOWHILE_ASSEMBLY(node));
    return node;
}

node_st *MATfor(node_st *node)
{
    TRAVchildren(node);
    CCNfree(FOR_ASSEMBLY(node));
    return node;
}
node_st *MATreturn(node_st *node)
{
    TRAVchildren(node);
    CCNfree(RETURN_ASSEMBLY(node));
    return node;
}
