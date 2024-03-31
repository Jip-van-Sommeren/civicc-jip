/**
 * @file forlooptowhileloop.c
 * @brief For loop to While loop generation.
 *
 * This file contains the initital steps for the conversion of for loop to
 * while loop, it sets the initializers for the loop iterators.
 *
 * @author Jip van Sommeren
 * @date 31-03-24
 */

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
#include "forlooptowhileloophelper.h"

node_st *FTWfunbody(node_st *node)
{
    node_st *stmts = FUNBODY_STMTS(node); // Assuming this gets the starting point for checking
    node_st *prev = NULL;
    while (stmts != NULL)
    {

        processForLoop(&FUNBODY_STMTS(node), &FUNBODY_VARDECLS(node), prev, STMTS_STMT(stmts));
        prev = stmts;
        stmts = STMTS_NEXT(stmts); // Move to the next statement in the function body
    }
    return node;
}
