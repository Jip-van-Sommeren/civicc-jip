/**
 * @file
 *
 * Traversal: IdentifierCount
 * UID      : IC
 *
 *
 */

#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"

void ICinit() { return; }
void ICfini() { return; }

/**
 * @fn ICvar
 */
node_st *ICvar(node_st *node)
{
    TRAVchildren(node);
    return node;
}

/**
 * @fn ICvarlet
 */
node_st *ICvarlet(node_st *node)
{
    TRAVchildren(node);
    return node;
}

