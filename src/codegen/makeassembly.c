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
#include "global/globals.h"
#include "makeassemblyhelper.h"

#define MAX_STRING_SIZE 128

node_st *returnConstAssembly(node_st *val)
{
    char *str = malloc(MAX_STRING_SIZE * sizeof(char)); // Allocate memory dynamically
    node_st *assemblyEntry = NULL;
    if (!str)
        return NULL; // Check for allocation failure

    switch (NODE_TYPE(val))
    {
    case NT_NUM:

        sprintf(str, "%d", NUM_VAL(val));
        assemblyEntry = ASTassembly(strdup(".const int "), strdup(str));
        free(str);
        break;
    case NT_FLOAT:

        sprintf(str, "%f", FLOAT_VAL(val));
        assemblyEntry = ASTassembly(strdup(".const float "), strdup(str));
        free(str);
        break;

    default:
        free(str); // Free allocated memory if default case is hit
        return NULL;
    }

    return assemblyEntry;
}

node_st *assemblyEntry(node_st *decl)
{
    node_st *assemblyEntry = NULL;
    char *str = malloc(MAX_STRING_SIZE * sizeof(char));
    switch (NODE_TYPE(decl))
    {
    case NT_GLOBDECL:
    {
        node_st *params = GLOBDECL_PARAMS(decl);

        sprintf(str, "\"%s\" %s", GLOBDECL_NAME(decl), VarTypeToString(GLOBDECL_TYPE(decl)));
        while (params != NULL)
        {
            node_st *param = PARAMS_PARAM(params);
            strcat(str, " ");
            strcat(str, VarTypeToString(PARAM_TYPE(param)));
            params = PARAMS_NEXT(params);
        }

        assemblyEntry = ASTassembly(strdup(".importfun "), strdup(str));

        free(str);
        break;
    }
    case NT_GLOBDEF:
        sprintf(str, "%s", VarTypeToString(GLOBDEF_TYPE(decl)));
        assemblyEntry = ASTassembly(strdup(".global "), strdup(str));
        free(str);
        break;
    case NT_FUNDEF:
        if (FUNDEF_EXPORT(decl))
        {
            sprintf(str, "\"%s\" %s %s", FUNDEF_NAME(decl), VarTypeToString(FUNDEF_TYPE(decl)), FUNDEF_NAME(decl));
            assemblyEntry = ASTassembly(strdup(".exportfun "), strdup(str));
        }
        free(str);
        break;

    default:
        free(str);
        break;
    }
    return assemblyEntry;
}

node_st *MAprogram(node_st *node)
{
    node_st *importsTail = NULL;
    node_st *globalsTail = NULL;
    node_st *exportTail = NULL;
    node_st *constantsTail = NULL;

    node_st *constantTable = PROGRAM_CONSTANTTABLE(node);
    while (constantTable != NULL)
    {
        node_st *entry = CONSTANTTABLE_ENTRY(constantTable);
        node_st *val = CONSTANTENTRY_VAL(entry);
        node_st *assemblyEntry = returnConstAssembly(val);
        appendAssemblyListAndUpdateTail(&PROGRAM_CONSTANT(node), &constantsTail, assemblyEntry);
        constantTable = CONSTANTTABLE_NEXT(constantTable);
    }
    node_st *decls = PROGRAM_DECLS(node);
    while (decls != NULL)
    {
        node_st *decl = DECLS_DECL(decls);
        node_st *entry = assemblyEntry(decl);
        if (NODE_TYPE(decl) == NT_GLOBDECL)
        {
            appendAssemblyListAndUpdateTail(&PROGRAM_IMPORT(node), &importsTail, entry);
        }
        else if (NODE_TYPE(decl) == NT_GLOBDEF)
        {
            appendAssemblyListAndUpdateTail(&PROGRAM_GLOBAL(node), &globalsTail, entry);
        }
        else if (NODE_TYPE(decl) == NT_FUNDEF && FUNDEF_EXPORT(decl))
        {
            appendAssemblyListAndUpdateTail(&PROGRAM_EXPORT(node), &exportTail, entry);
        }
        decls = DECLS_NEXT(decls);
    }

    if (global.outputFile != NULL && PROGRAM_CONSTANT(node) != NULL)
    {
        setToOutputFile(PROGRAM_CONSTANT(node));
    }
    if (global.outputFile != NULL && PROGRAM_EXPORT(node) != NULL)
    {
        setToOutputFile(PROGRAM_EXPORT(node));
    }
    if (global.outputFile != NULL && PROGRAM_GLOBAL(node) != NULL)
    {
        setToOutputFile(PROGRAM_GLOBAL(node));
    }
    if (global.outputFile != NULL && PROGRAM_IMPORT(node) != NULL)
    {
        setToOutputFile(PROGRAM_IMPORT(node));
    }
    closeGlobalOutputFile();

    return node;
}

// node_st *MAglobdecl(node_st *node)
// {
//     char *str = malloc(30 * sizeof(char));
//     sprintf(str, ".importfun \"%s\" %s\n", GLOBDECL_NAME(node), VarTypeToString(GLOBDECL_TYPE(node)));
//     printf("%s", str);
//     if (global.outputFile != NULL)
//     {
//         fprintf(global.outputFile, "%s", str);
//     }
//     free(str);
//     return node;
// }

// node_st *MAglobdef(node_st *node)
// {
//     char *str = malloc(30 * sizeof(char));
//     sprintf(str, ".global %s\n", VarTypeToString(GLOBDEF_TYPE(node)));
//     printf("%s", str);
//     if (global.outputFile != NULL)
//     {
//         fprintf(global.outputFile, "%s", str);
//     }
//     free(str);
//     return node;
// }
// // node_st *findConstant(node_st *constantTable, node_st *expr, enum Type type)
// // {
// //     while (constantTable != NULL)
// //     {
// //         node_st *entry = CONSTANTTABLE_ENTRY(constantTable);
// //         if (NODE_TYPE(expr) == type && NODE_TYPE(entry) == NT_NUM)
// //         {
// //         }
// //         if (NODE_TYPE(expr) == NT_FLOAT && NODE_TYPE(entry) == NT_NUM)
// //         {
// //         }
// //         constantTable = CONSTANTTABLE_NEXT(constantTable);
// //     }
// // }

// char *returnTypeAssembly(enum Type type, node_st *expr)
// {
//     char *str = malloc(20 * sizeof(char)); // Allocate memory dynamically
//     if (!str)
//         return NULL; // Check for allocation failure

//     switch (type)
//     {
//     case CT_int:
//         if (NUM_VAL(expr) != -1 && NUM_VAL(expr) != 0 && NUM_VAL(expr) != 1)
//         {
//             sprintf(str, "iloadc %d", CONSTANTENTRY_INDEX(NUM_LINK(expr)));
//         }
//         else if (NUM_VAL(expr) == -1)
//         {
//             sprintf(str, "iloadc_m1");
//         }
//         else if (NUM_VAL(expr) == 0)
//         {
//             sprintf(str, "iloadc_0");
//         }
//         else
//         { // NUM_VAL(expr) == 1
//             sprintf(str, "iloadc_1");
//         }
//         break;
//     case CT_float:
//         if (FLOAT_VAL(expr) != 1.0f && FLOAT_VAL(expr) != 0.0f)
//         {
//             sprintf(str, "floadc %d", CONSTANTENTRY_INDEX(FLOAT_LINK(expr)));
//         }
//         else if (FLOAT_VAL(expr) == 1.0f)
//         {
//             sprintf(str, "floadc_1");
//         }
//         else
//         { // FLOAT_VAL(expr) == 0.0f
//             sprintf(str, "floadc_0");
//         }
//         break;
//     case CT_bool:
//         sprintf(str, BOOL_VAL(expr) ? "bloadc_t" : "bloadc_f");
//         break;
//     default:
//         free(str); // Free allocated memory if default case is hit
//         return NULL;
//     }
//     return str;
// }

// node_st *returnConstAssembly(node_st *val)
// {
//     char *str = malloc(MAX_STRING_SIZE * sizeof(char)); // Allocate memory dynamically
//     node_st *assemblyEntry = NULL;
//     if (!str)
//         return NULL; // Check for allocation failure

//     switch (NODE_TYPE(val))
//     {
//     case NT_NUM:

//         sprintf(str, "%d", NUM_VAL(val));
//         assemblyEntry = ASTassembly(strdup(".const int "), strdup(str));
//         break;
//     case NT_FLOAT:

//         sprintf(str, "%f", FLOAT_VAL(val));
//         assemblyEntry = ASTassembly(strdup(".const float "), strdup(str));
//         break;

//     default:
//         free(str); // Free allocated memory if default case is hit
//         return NULL;
//     }

//     return assemblyEntry;
// }

// node_st *MAfundef(node_st *node)
// {
//     if (FUNDEF_EXPORT(node))
//     {
//         char *str = malloc(40 * sizeof(char));
//         sprintf(str, ".exportfun \"%s\" %s %s\n", FUNDEF_NAME(node), VarTypeToString(FUNDEF_TYPE(node)), FUNDEF_NAME(node));
//         printf("%s", str);

//         // No need to close the file here; keep it open for future writes
//         if (global.outputFile != NULL)
//         {
//             fprintf(global.outputFile, "%s", str);
//         }
//         free(str);
//     }
//     TRAVchildren(node);
//     return node;
// }

// node_st *MAconstanttable(node_st *node)
// {
//     node_st *table = node;
//     while (table != NULL)
//     {
//         node_st *entry = CONSTANTTABLE_ENTRY(table);
//         node_st *val = CONSTANTENTRY_VAL(entry);
//         char *str = returnConstAssembly(val);
//         printf("%s\n", str);
//         if (global.outputFile != NULL)
//         {
//             fprintf(global.outputFile, "%s", str);
//         }
//         free(str);
//         table = CONSTANTTABLE_NEXT(table);
//     }
//     return node;
// }