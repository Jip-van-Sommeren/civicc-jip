/**
 * @file
 *
 * Traversal: StrengthReduction
 * UID      : SR
 *
 *
 */

#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"

/**
 * @fn SRbinop
 */
node_st *SRbinop(node_st *node)
{
    TRAVchildren(node);
    return node;
}

