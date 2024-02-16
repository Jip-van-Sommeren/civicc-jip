#include "palm/hash_table.h"
#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "ccngen/enum.h"
#include "ccngen/trav_data.h"

void ICinit(struct data_ic *data)
{
    data->identifierCounts = HTnew_String(128);
}
void ICfini(struct data_ic *data)
{
    htable_iter_st *iter = HTiterate(data->identifierCounts);
    while (iter != NULL)
    {
        char *identifier = HTiterKey(iter);
        int *count = HTiterValue(iter);
        printf("%s: %d\n", identifier, *count);
        iter = HTiterateNext(iter);
    }
    HTdelete(data->identifierCounts); // Free the hash table and its contents
}

node_st *ICvar(node_st *node)
{
    struct data_ic *data = DATA_IC_GET();
    char *identifier = VAR_NAME(node);
    int *count = HTlookup(data->identifierCounts, identifier);

    if (count == NULL)
    {
        count = malloc(sizeof(int));
        *count = 1;
        HTinsert(data->identifierCounts, identifier, count);
    }
    else
    {
        (*count)++;
    }

    return node;
}

node_st *ICvarlet(node_st *node)
{
    struct data_ic *data = DATA_IC_GET();
    char *identifier = VARLET_NAME(node);
    int *count = HTlookup(data->identifierCounts, identifier);

    if (count == NULL)
    {
        count = malloc(sizeof(int));
        *count = 1;
        HTinsert(data->identifierCounts, strdup(identifier), count);
    }
    else
    {
        (*count)++;
    }

    return node;
}