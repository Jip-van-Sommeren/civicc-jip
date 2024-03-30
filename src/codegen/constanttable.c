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
#include "makeassemblyhelper.h"

#define TABLESIZE 128
// use global consttable to make,
// use ht for cheking if the node already exists
node_st *constTab = NULL;
node_st *tail = NULL;
static size_t floatHash(void *val)
{
    float *f = (float *)val;
    return (size_t)(*f);
}

static bool floatEqual(void *ptr1, void *ptr2)
{
    float *f1 = (float *)ptr1;
    float *f2 = (float *)ptr2;

    return *f1 == *f2;
}

void CONSTTinit()
{
    struct data_constt *data = DATA_CONSTT_GET();
    data->intTable = HTnew_Int(TABLESIZE);
    data->floatTable = HTnew(TABLESIZE, floatHash, floatEqual);
    data->index = 0;
    return;
}

void CONSTTfini()
{
    struct data_constt *data = DATA_CONSTT_GET();
    htable_iter_st *iterInt = HTiterate(data->intTable);
    htable_iter_st *iterFloat = HTiterate(data->floatTable);
    while (iterInt != NULL)
    {
        int *key = HTiterKey(iterInt);
        free(key);

        iterInt = HTiterateNext(iterInt);
    }
    while (iterFloat != NULL)
    {
        int *key = HTiterKey(iterFloat);
        free(key);

        iterFloat = HTiterateNext(iterFloat);
    }
    HTdelete(data->floatTable);
    HTdelete(data->intTable);
    return;
}

// void makeConstantTable(node_st **head, node_st **tail, struct data_constt *data)
// {
//     htable_iter_st *iterInt = HTiterate(data->intTable);
//     htable_iter_st *iterFloat = HTiterate(data->floatTable);

//     while (iterInt != NULL)
//     {

//         node_st *entry = HTiterValue(iterInt);

//         appendConstantTable(head, tail, entry);
//         iterInt = HTiterateNext(iterInt);
//     }
//     while (iterFloat != NULL)
//     {

//         node_st *entry = HTiterValue(iterFloat);
//         appendConstantTable(head, tail, entry);
//         iterFloat = HTiterateNext(iterFloat);
//     }

//     return;
// }

node_st *CONSTTprogram(node_st *node)
{
    TRAVchildren(node);
    PROGRAM_CONSTANTTABLE(node) = constTab;
    return node;
}

node_st *CONSTTnum(node_st *node)
{
    struct data_constt *data = DATA_CONSTT_GET();
    int val = NUM_VAL(node);

    // Allocate space for the int key and store the value in it
    int *key = malloc(sizeof(int));
    if (!key)
    {
        // Handle memory allocation failure
        fprintf(stderr, "Memory allocation failed for hashtable key\n");
        return node;
    }
    // printf("key %d\n", data->index);
    *key = val;
    // Use the allocated key for lookup
    node_st *entry = HTlookup(data->intTable, key);

    if (val != 0 && val != 1 && val != -1 && entry == NULL)
    {
        // Insert using the allocated key. Assuming `node` is correctly typed to be stored as a value.
        printf("%d %d\n", NUM_VAL(node), data->index);
        node_st *constantentry = ASTconstantentry(node, data->index);
        appendConstantTable(&constTab, &tail, constantentry);
        HTinsert(data->intTable, key, constantentry);
        data->index++;
        NUM_LINK(node) = constantentry;
    }
    else
    {
        // If the key already exists or val is one of the excluded values, free the allocated key since it won't be used
        NUM_LINK(node) = entry;
        free(key);
    }
    TRAVchildren(node);
    return node;
}
node_st *CONSTTfloat(node_st *node)
{
    struct data_constt *data = DATA_CONSTT_GET();
    float val = FLOAT_VAL(node);
    // Allocate space for the float key and store the value in it
    int *key = malloc(sizeof(int));
    if (!key)
    {
        // Handle memory allocation failure
        fprintf(stderr, "Memory allocation failed for hashtable key\n");
        return node;
    }
    *key = val;
    // Use the allocated key for lookup
    node_st *entry = HTlookup(data->floatTable, key);
    if (val != 0.0f && val != 1.0f && entry == NULL)
    {
        // Insert using the allocated key. Assuming `node` is correctly typed to be stored as a value.
        node_st *constantentry = ASTconstantentry(node, data->index);
        HTinsert(data->floatTable, key, constantentry);
        appendConstantTable(&constTab, &tail, constantentry);
        data->index++;
        FLOAT_LINK(node) = constantentry;
    }
    else
    {
        // If the key already exists or val is 0.0 or 1.0, free the allocated key since it won't be used
        FLOAT_LINK(node) = entry;
        free(key);
    }

    TRAVchildren(node);
    return node;
}