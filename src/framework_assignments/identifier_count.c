#include "palm/hash_table.h"
#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "ccngen/enum.h"
#include "ccngen/trav_data.h"
#include "stdio.h"
#include "string.h"

void ICinit()
{
    struct data_ic *data = DATA_IC_GET();
    data->symbolTable = HTnew_String(128);
}
void ICfini()
{
    struct data_ic *data = DATA_IC_GET();
    if (data->symbolTable == NULL)
    {
        fprintf(stderr, "Symbol table is uninitialized.\n");
        return;
    }
    htable_iter_st *iter = HTiterate(data->symbolTable);
    while (iter != NULL)
    {
        int *count = HTiterValue(iter);
        free(count);
        iter = HTiterateNext(iter);
    }
    HTdelete(data->symbolTable); // Cleanup the hash table
}

node_st *ICglobdef(node_st *node)
{

    struct data_ic *data = DATA_IC_GET();
    if (data->symbolTable == NULL)
    {
        fprintf(stderr, "heretest2\n");
    }
    char *identifier = GLOBDEF_NAME(node);
    printf("%s \n", identifier);
    int *count = HTlookup(data->symbolTable, identifier);

    if (count == NULL)
    {
        count = malloc(sizeof(int));
        *count = 1;
        printf("%d \n", *count);
        if (!HTinsert(data->symbolTable, identifier, count))
        {
            fprintf(stderr, "Here2");
        }
    }
    else
    {
        (*count)++;
    }

    return node;
}
