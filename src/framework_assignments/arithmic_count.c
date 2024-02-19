#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "ccngen/enum.h"
#include "ccngen/trav_data.h"

node_st *ACprogram(node_st *node);

void ACinit() { return; }

void ACfini(node_st *root)
{
    ACprogram(root);
}

node_st *ACbinop(node_st *node)
{
    struct data_ac *data = DATA_AC_GET();

    switch (BINOP_OP(node))
    {
    case BO_add:
        data->plusCount++;
        break;
    case BO_sub:
        data->minusCount++;
        break;
    case BO_mul:
        data->mulCount++;
        break;
    case BO_div:
        data->divCount++;
        break;
    case BO_mod:
        data->modCount++;
        break;
    default:
        // Handle other cases or do nothing
        break;
    }

    TRAVchildren(node);
    return node;
}

node_st *ACprogram(node_st *node)
{
    struct data_ac *data = DATA_AC_GET();

    // Transfer the counts from the traversal data to the Program node's attributes
    PROGRAM_PLUSCOUNT(node) = data->plusCount;
    PROGRAM_MINUSCOUNT(node) = data->minusCount;
    PROGRAM_MULCOUNT(node) = data->mulCount;
    PROGRAM_DIVCOUNT(node) = data->divCount;
    PROGRAM_MODCOUNT(node) = data->modCount;

    // Continue traversal for any children of the Program node, if necessary
    TRAVchildren(node);

    return node;
}