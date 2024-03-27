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

#define TABLESIZE 128

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
    data->constTable = HTnew_Int(TABLESIZE);
    data->index = 0;
    return;
}

void CONSTTfini()
{
    struct data_constt *data = DATA_CONSTT_GET();
    htable_iter_st *iter = HTiterate(data->constTable);
    while (iter != NULL)
    {
        int *key = HTiterKey(iter);
        free(key);

        iter = HTiterateNext(iter);
    }

    HTdelete(data->constTable);
    return;
}

node_st *makeConstantTable(struct data_constt *data)
{
    htable_iter_st *iter = HTiterate(data->constTable);
    node_st *constantTable = NULL;
    node_st *constantTableTail = NULL;

    while (iter != NULL)
    {

        node_st *entry = HTiterValue(iter);

        if (constantTable == NULL)
        {
            // This is the first assignment statement.
            constantTable = ASTconstanttable(entry, NULL);
            constantTableTail = constantTable; // The tail is the same as head now.
        }
        else
        {
            // Append to the tail and update the tail pointer.
            node_st *newSymboltable = ASTconstanttable(entry, NULL);
            CONSTANTTABLE_NEXT(constantTableTail) = newSymboltable; // Append to the end.
            constantTableTail = newSymboltable;                     // Update the tail.
        }
        iter = HTiterateNext(iter);
    }
    return constantTable;
}

node_st *CONSTTprogram(node_st *node)
{
    struct data_constt *data = DATA_CONSTT_GET();
    TRAVchildren(node);
    node_st *constantTable = makeConstantTable(data);
    PROGRAM_CONSTANTTABLE(node) = constantTable;
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
    *key = data->index;
    // Use the allocated key for lookup
    node_st *entry = HTlookup(data->constTable, key);

    if (val != 0 && val != 1 && val != -1 && entry == NULL)
    {
        // Insert using the allocated key. Assuming `node` is correctly typed to be stored as a value.
        node_st *constantentry = ASTconstantentry(node, data->index);
        HTinsert(data->constTable, key, constantentry);
        data->index++;
        NUM_LINK(node) = constantentry;
    }
    else
    {
        // If the key already exists or val is one of the excluded values, free the allocated key since it won't be used
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
    *key = data->index;
    // Use the allocated key for lookup
    node_st *entry = HTlookup(data->constTable, key);
    if (val != 0.0f && val != 1.0f && entry == NULL)
    {
        // Insert using the allocated key. Assuming `node` is correctly typed to be stored as a value.
        node_st *constantentry = ASTconstantentry(node, data->index);
        HTinsert(data->constTable, key, constantentry);
        data->index++;
        FLOAT_LINK(node) = constantentry;
    }
    else
    {
        // If the key already exists or val is 0.0 or 1.0, free the allocated key since it won't be used
        free(key);
    }

    TRAVchildren(node);
    return node;
}