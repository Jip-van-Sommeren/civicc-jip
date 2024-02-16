/**
 * @file
 *
 * Traversal: ArithmeticCount
 * UID      : AC
 *
 *
 */

#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"

void ACinit() { return; }
void ACfini() { return; }

/**
 * @fn ACbinop
 */
node_st *ACbinop(node_st *node)
{
    TRAVchildren(node);
    return node;
}

/**
 * @fn ACprogram
 */
node_st *ACprogram(node_st *node)
{
    TRAVchildren(node);
    return node;
}

