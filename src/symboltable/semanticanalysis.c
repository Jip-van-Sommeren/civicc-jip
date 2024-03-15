#include "palm/hash_table.h"
#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "ccngen/enum.h"
#include "ccngen/trav_data.h"
#include "stdio.h"
#include "string.h"
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
}

node_st *SAprogram(node_st *node)
{

    TRAVopt(PROGRAM_DECLS(node));
    int test = 3;
    TRAVsymbolTable(node);
    return node;
}

node_st *SAsymboltable(node_st *node)
{
    TRAVdo(SYMBOLTABLE_ENTRY(node));

    // If there is another function definition in the list, continue printing
    if (SYMBOLTABLE_NEXT(node) != NULL)
    {
        TRAVdo(SYMBOLTABLE_NEXT(node));
    }

    return node;
}

node_st *SAdecls(node_st *node)
{

    TRAVchildren(node);
    return node;
}

node_st *SAsymbolentry(node_st *node)
{
    printf("Variable Declaration TEST: %s\n", SYMBOLENTRY_NAME(node));
    return node;
}

node_st *SAfundef(node_st *node)
{

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
    if (FUNCALL_DECL(node) == NULL)
    {
        printf("hereeeee\n");
    }
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