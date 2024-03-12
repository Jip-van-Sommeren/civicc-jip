#include "palm/hash_table.h"
#include "string.h"
#include "deepcopyhashtable.h"
#include "stdio.h"

SymbolInfo *deepCopySymbolInfo(const SymbolInfo *original)
{
    SymbolInfo *copy = malloc(sizeof(SymbolInfo));
    if (!copy)
    {
        // Handle memory allocation failure
        return NULL;
    }
    copy->name = strdup(original->name);
    copy->type = strdup(original->type);
    copy->declaredAtLine = original->declaredAtLine;
    copy->isFunction = original->isFunction; // Copy if you have this field
    copy->scopeLevel = original->scopeLevel; // Copy if you have this field
    return copy;
}

// Function to deep copy the hash table
htable_st *deepCopyHashTable(htable_st *original)
{
    // Assuming HTnew_String creates a new empty hash table
    htable_st *copy = HTnew_String(128); // You need to implement or replace HTcapacity() as needed
    if (!copy)
    {
        // Handle memory allocation failure
        return NULL;
    }

    htable_iter_st *iter = HTiterate(original);
    while (iter != NULL)
    {
        SymbolInfo *originalInfo = HTiterValue(iter);
        SymbolInfo *copyInfo = deepCopySymbolInfo(originalInfo);
        if (!copyInfo)
        {
            HTdelete(copy); // Clean up the partially copied hash table
            return NULL;
        }
        HTinsert(copy, copyInfo->name, copyInfo);
        iter = HTiterateNext(iter);
    }

    return copy;
}

void freeSymbolInfo(SymbolInfo *info)
{
    if (info)
    {
        free(info->name);
        free(info->type);
        free(info);
    }
}

// Function to free the deep copied hash table
void freeDeepCopyHashTable(htable_st *table)
{
    if (table)
    {
        htable_iter_st *iter = HTiterate(table);
        while (iter != NULL)
        {
            SymbolInfo *info = HTiterValue(iter);
            freeSymbolInfo(info);
            iter = HTiterateNext(iter);
        }
        HTdelete(table);
    }
}