#include "palm/hash_table.h"
#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "ccngen/enum.h"
#include "ccngen/trav_data.h"
#include "stdio.h"
#include "string.h"
#include "symboltable.h"
#include "palm/dbug.h"

/**
 * @fn SAinit
 */
void SAinit()
{
    return;
}

/**
 * @fn SAinit
 */
void SAfini()
{
    return;
}

node_st *SAbool(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *SAfloat(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *SAnum(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *SAvar(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *SAmonop(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *SAbinop(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *SAcast(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *SAfuncall(node_st *node)
{

    TRAVchildren(node);
    return node;
}
node_st *SAarrexpr(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *SAexprs(node_st *node)
{
    TRAVchildren(node);
    return node;
}