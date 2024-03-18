#include "palm/hash_table.h"
#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "ccngen/enum.h"
#include "ccngen/trav_data.h"
#include "stdio.h"
#include "string.h"
#include "palm/dbug.h"

#define TABLE_SIZE 128
#define STACK_SIZE 3

void SAinit()
{
    return;
}

void SAfini()
{
    return;
}

node_st *SAprogram(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *SAfundef(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *SAbool(node_st *node)
{
    printf("val bool: %d\n", BOOL_VAL(node));
    TRAVchildren(node);
    return node;
}

node_st *SAfloat(node_st *node)
{
    printf("val float: %f\n", FLOAT_VAL(node));
    TRAVchildren(node);
    return node;
}

node_st *SAnum(node_st *node)
{
    printf("val num: %d\n", NUM_VAL(node));
    TRAVchildren(node);
    return node;
}

node_st *SAvar(node_st *node)
{
    printf("var type: %d\n", VAR_TYPE(node));
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
    printf("type: %d\n", FUNCALL_TYPE(node));
    TRAVchildren(node);
    return node;
}
node_st *SAarrexpr(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *SAreturn(node_st *node)
{
    printf("return type: %d\n", RETURN_TYPE(node));
    TRAVchildren(node);
    printf("postreturntype\n");
    return node;
}