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

node_st *MAprogram(node_st *node)
{

    TRAVchildren(node);
    return node;
}

// node_st *findConstant(node_st *constantTable, node_st *expr, enum Type type)
// {
//     while (constantTable != NULL)
//     {
//         node_st *entry = CONSTANTTABLE_ENTRY(constantTable);
//         if (NODE_TYPE(expr) == type && NODE_TYPE(entry) == NT_NUM)
//         {
//         }
//         if (NODE_TYPE(expr) == NT_FLOAT && NODE_TYPE(entry) == NT_NUM)
//         {
//         }
//         constantTable = CONSTANTTABLE_NEXT(constantTable);
//     }
// }

char *returnTypeAssembly(enum Type type, int i, node_st *expr)
{
    char *str = malloc(20 * sizeof(char)); // Allocate memory dynamically
    if (!str)
        return NULL; // Check for allocation failure

    switch (type)
    {
    case CT_int:
        if (NUM_VAL(expr) != -1 && NUM_VAL(expr) != 0 && NUM_VAL(expr) != 1)
        {
            sprintf(str, "iloadc %d", CONSTANTENTRY_INDEX(NUM_LINK(expr)));
        }
        else if (NUM_VAL(expr) == -1)
        {
            sprintf(str, "iloadc_m1");
        }
        else if (NUM_VAL(expr) == 0)
        {
            sprintf(str, "iloadc_0");
        }
        else
        { // NUM_VAL(expr) == 1
            sprintf(str, "iloadc_1");
        }
        break;
    case CT_float:
        if (FLOAT_VAL(expr) != 1.0f && FLOAT_VAL(expr) != 0.0f)
        {
            sprintf(str, "floadc %d", CONSTANTENTRY_INDEX(FLOAT_LINK(expr)));
        }
        else if (FLOAT_VAL(expr) == 1.0f)
        {
            sprintf(str, "floadc_1");
        }
        else
        { // FLOAT_VAL(expr) == 0.0f
            sprintf(str, "floadc_0");
        }
        break;
    case CT_bool:
        sprintf(str, BOOL_VAL(expr) ? "bloadc_t" : "bloadc_f");
        break;
    default:
        free(str); // Free allocated memory if default case is hit
        return NULL;
    }
    return str;
}

node_st *MAfundef(node_st *node)
{
    int i = 0;
    node_st *stmts = FUNBODY_STMTS(FUNDEF_BODY(node));
    while (stmts != NULL)
    {
        node_st *stmt = STMTS_STMT(stmts);
        // if (NODE_TYPE(stmt) == NT_ASSIGN)
        // {
        //     if (NODE_TYPE(ASSIGN_EXPR(stmt)) == NT_NUM || NODE_TYPE(ASSIGN_EXPR(stmt)) == NT_FLOAT)
        //     {
        //         char *str = returnTypeAssembly(VARLET_TYPE(ASSIGN_LET(stmt)), i, ASSIGN_EXPR(stmt));
        //         printf("%s\n", str);
        //         free(str);
        //     }
        //     if (VARLET_SYMBOLENTRY(ASSIGN_LET(stmt)) != NULL)
        //     {
        //         printf("herer\n");
        //     }
        // }
        i++;
        stmts = STMTS_NEXT(stmts);
    }
    TRAVchildren(node);
    return node;
}