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

void FAinit()
{
    return;
}

void FAfini()
{

    return;
}

node_st *FAfor(node_st *node)
{
    node_st *currentForBlock = node;
    int depth = 1;
    while (currentForBlock != NULL)
    {
        bool foundNestedFor = false;
        node_st *max[3] = {NULL, NULL, NULL}; // Array to hold maxX, maxY, maxZ

        // Assuming currentForBlock is defined and valid
        if (depth >= 1 && depth <= 3)
        {
            max[depth - 1] = FOR_STOP(currentForBlock);
        }
        // Assuming that the for block can contain multiple statements,
        // and you have a way to iterate through these statements.
        node_st *stmts = FOR_BLOCK(currentForBlock); // Get the first statement in the current for block.

        while (stmts != NULL)
        {
            // (x * Y * Z) + (y * Z) + z
            // (x * Y) + y
            // Check if the statement is a for loop
            if (NODE_TYPE(STMTS_STMT(stmts)) == NT_FOR)
            {
                foundNestedFor = true;
                currentForBlock = STMTS_STMT(stmts);
                depth++; // Update currentForBlock to the nested for loop's block
                break;   // Exit the inner loop as we found a nested for loop
            }
            if (NODE_TYPE(STMTS_STMT(stmts)) == NT_ASSIGN)
            {

                node_st *varlet = ASSIGN_LET(STMTS_STMT(stmts));
                if (VARLET_INDICES(varlet) != NULL && depth > 1)
                {
                    node_st *exprs = VARLET_INDICES(varlet);
                    node_st *expr = NULL;
                    while (exprs != NULL)
                    {
                        if (expr == NULL)
                        {
                            printf("here1\n");
                            expr = ASTbinop(CCNcopy(EXPRS_EXPR(exprs)), CCNcopy(max[1]), BO_mul);
                        }
                        else
                        {
                            printf("here2\n");
                            expr = ASTbinop(expr, CCNcopy(EXPRS_EXPR(exprs)), BO_add);
                        }

                        exprs = EXPRS_NEXT(exprs);
                    }
                    CCNfree(VARLET_INDICES(varlet));

                    VARLET_INDICES(varlet) = ASTexprs(expr, NULL);
                }
                // Exit the inner loop as we found a nested for loop
            }

            // Move to the next statement in the block
            stmts = STMTS_NEXT(stmts); // Assuming STMTS_NEXT is a way to get the next statement in a block
        }

        // If no nested for loop was found in the current block, exit the loop
        if (!foundNestedFor)
        {
            break;
        }
    }
    printf("%d\n", depth);

    return node;
}