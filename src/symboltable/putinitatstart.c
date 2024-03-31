
/**
 * @file putinitatstart.c
 * @brief Put __init function at start of.
 *
 * This traversal moves the __init function to the start of the AST decls node, it was kept at the
 * end of the file for the semantic analysis and symbol table phases.
 * @author Jip van Sommeren
 * @date 31-03-24
 */

#include "palm/hash_table.h"
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
#include "sahelper.h"

node_st *PASdecls(node_st *node)
{
    moveLastToFront(&node);
    return node;
}
