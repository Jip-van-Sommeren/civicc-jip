#include "palm/hash_table.h"
#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "ccngen/enum.h"
#include "ccngen/trav_data.h"
#include "stdio.h"
#include "string.h"
#include "palm/dbug.h"
#include "deepcopyhashtable.h"

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
}

node_st *SAprogram(node_st *node)
{
    TRAVchildren(node);
    htable_st *deepCopy = PROGRAM_SYMBOLTABLE(node);
    htable_iter_st *iter = HTiterate(deepCopy);
    while (iter != NULL)
    {
        SymbolInfo *info = HTiterValue(iter);
        printf("t%d: %s %s scope:%d \n", info->declaredAtLine, info->name, info->type, info->scopeLevel);
        iter = HTiterateNext(iter);
    }
    freeDeepCopyHashTable(deepCopy);

    return node;
}

node_st *SAfundef(node_st *node)
{
    htable_st *deepCopy = FUNDEF_SYMBOLTABLE(node);
    htable_iter_st *iter = HTiterate(deepCopy);
    while (iter != NULL)
    {
        SymbolInfo *info = HTiterValue(iter);
        printf("f%d: %s %s scope:%d \n", info->declaredAtLine, info->name, info->type, info->scopeLevel);
        iter = HTiterateNext(iter);
    }
    freeDeepCopyHashTable(deepCopy);
    TRAVchildren(node);
    return node;
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