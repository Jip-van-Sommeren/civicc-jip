#include "palm/hash_table.h"
#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "ccngen/enum.h"
#include "ccngen/trav_data.h"
#include "stdio.h"
#include "string.h"
#include "symboltable.h"
#include "palm/dbug.h"

static bool first_vardecls = false;
static bool first_fundefs = false;

char *VarTypeToString(enum Type type)
{
    char *typeStr = "unknown";
    switch (type)
    {
    case CT_int:
        typeStr = "int";
        break;
    case CT_float:
        typeStr = "float";
        break;
    case CT_bool:
        typeStr = "bool";
        break;
    case CT_void:
        typeStr = "void";
        break;
    case CT_NULL:
        DBUG_ASSERT(false, "unknown type detected!");
    }
    return typeStr;
}
void insertSymbol(struct data_st *data, char *name, char *type, int declaredAtLine, int isFunction)
{
    if (!data || data->scopeStack->top < 0)
    {
        // Error handling: No current scope or data_st is NULL
        fprintf(stderr, "No current scope available for symbol insertion.\n");
        return;
    }

    Scope *currentScope = &(data->scopeStack->scopes[data->scopeStack->top]);

    SymbolInfo *info = (SymbolInfo *)malloc(sizeof(SymbolInfo));
    if (!info)
    {
        fprintf(stderr, "Memory allocation failed for SymbolInfo.\n");
        return;
    }

    info->name = strdup(name);
    info->type = strdup(type);
    info->declaredAtLine = declaredAtLine;
    info->isFunction = isFunction;
    info->scopeLevel = currentScope->level; // Utilize the current scope level from the scope stack

    HTinsert(data->symbolTable, info->name, info);
    if (!HTinsert(data->symbolTable, info->name, info))
    {
        fprintf(stderr, "Failed to insert symbol into the symbol table.\n");
        free(info->name);
        free(info->type);
        free(info);
    }
}

void ST_pushScopeLevel(struct data_st *data, Scope newScope)
{
    if (data->scopeStack->top + 1 >= data->scopeStack->capacity)
    {
        // Reallocation logic
        int newCapacity = data->scopeStack->capacity * 2;
        data->scopeStack->scopes = realloc(data->scopeStack->scopes, sizeof(Scope) * newCapacity);
        data->scopeStack->capacity = newCapacity;
    }
    data->scopeStack->top++;
    data->scopeStack->scopes[data->scopeStack->top] = newScope;
}

void STinit()
{
    struct data_st *data = DATA_ST_GET();
    int initialCapacity = 10;
    // Ensure data is not NULL
    if (!data)
    {
        fprintf(stderr, "data_st is NULL in STinit.\n");
        return;
    }

    // Initialize symbol table
    data->symbolTable = HTnew_String(128);
    if (data->symbolTable == NULL)
    {
        fprintf(stderr, "Symbol start table is uninitialized.\n");
        return;
    }

    // Allocate memory for the scopeStack object itself before initializing its members
    data->scopeStack = malloc(sizeof(ScopeStack));
    if (!data->scopeStack)
    {
        fprintf(stderr, "Memory allocation failed for scopeStack in STinit.\n");
        return;                                                         // Exit if allocation failed
    }                                                                   // Initialize symbol table with 128 buckets
    data->scopeStack->scopes = malloc(sizeof(Scope) * initialCapacity); // Allocate for Scope structures
    data->scopeStack->capacity = initialCapacity;
    data->scopeStack->top = -1; // Indicates that the stack is empty
    // Create a global scope at level 0 and push it onto the stack
    Scope globalScope = {0};
    ST_pushScopeLevel(data, globalScope);
}

void ST_popScopeLevel(struct data_st *data)
{
    if (data->scopeStack->top > -1)
    {
        data->scopeStack->top--;
    }
    // No need to free individual Scope structs since they're part of the array
}

int ST_currentScopeLevel(struct data_st *data)
{
    if (data->scopeStack->top > -1)
    {
        return data->scopeStack->scopes[data->scopeStack->top].level;
    }
    return -1;
}

void STfini()
{
    // Iterate and cleanup symbol table
    struct data_st *data = DATA_ST_GET();
    if (data->symbolTable == NULL)
    {
        fprintf(stderr, "Symbol table is uninitialized.\n");
        return;
    }
    htable_iter_st *iter = HTiterate(data->symbolTable);
    while (iter != NULL)
    {
        SymbolInfo *info = HTiterValue(iter);
        printf("Name: %s, Type: %s, Declared at line: %d, Scopelevel: %d\n", info->name, info->type, info->declaredAtLine, info->scopeLevel);

        free(info->name);
        free(info->type);
        free(info);

        iter = HTiterateNext(iter);
    }
    HTdelete(data->symbolTable); // Cleanup the hash table

    if (data->scopeStack != NULL)
    {
        // If Scope contains dynamically allocated fields, free them here
        free(data->scopeStack->scopes);
        free(data->scopeStack);
        data->scopeStack = NULL;
    }
}

/**
 * @fn STvardecls
 */
node_st *STvardecls(node_st *node)
{
    TRAVchildren(node);
    if (!first_vardecls)
    {
        first_vardecls = true;
    }
    return node;
}

node_st *STvardecl(node_st *node)
{
    struct data_st *data = DATA_ST_GET();

    // Extract the variable's name, type, and declaration line number
    char *identifier = VARDECL_NAME(node);
    enum Type type = VARDECL_TYPE(node);
    char *typestr = VarTypeToString(type);
    int declaredAtLine = NODE_BLINE(node);
    int isFunction = 1;

    insertSymbol(data, identifier, typestr, declaredAtLine, isFunction);
    return node;
}

/**
 * @fn STfundefs
 */
node_st *STfundefs(node_st *node)
{
    TRAVchildren(node);
    if (!first_fundefs)
    {
        first_fundefs = true;
    }
    return node;
}

node_st *STfundef(node_st *node)
{
    struct data_st *data = DATA_ST_GET();

    // Retrieve the current scope level from the data structure
    int currentScopeLevel = ST_currentScopeLevel(data);

    char *identifier = FUNDEF_NAME(node); // Extract function name
    enum Type type = FUNDEF_TYPE(node);
    char *typestr = VarTypeToString(type); // Extract function return type
    int declaredAtLine = NODE_BLINE(node);
    int isFunction = 0;
    // Insert the function symbol into the symbol table with the current scope level
    insertSymbol(data, identifier, typestr, declaredAtLine, isFunction);
    // Increment the scope level for the function body
    Scope newScope;
    newScope.level = currentScopeLevel + 1;
    ST_pushScopeLevel(data, newScope);

    TRAVchildren(node);

    // Once done processing the function body, decrement the scope level to exit the function's scope
    ST_popScopeLevel(data);

    return node;
}

node_st *STlocalfundef(node_st *node)
{
    struct data_st *data = DATA_ST_GET();

    // Retrieve the current scope level from the data structure
    int currentScopeLevel = ST_currentScopeLevel(data);

    char *identifier = LOCALFUNDEF_NAME(node); // Extract function name
    enum Type type = LOCALFUNDEF_TYPE(node);
    char *typestr = VarTypeToString(type); // Extract function return type
    int declaredAtLine = NODE_BLINE(node);
    int isFunction = 0;

    // Insert the function symbol into the symbol table with the current scope level
    insertSymbol(data, identifier, typestr, declaredAtLine, isFunction);
    // Increment the scope level for the function body
    Scope newScope;
    newScope.level = currentScopeLevel + 1;
    ST_pushScopeLevel(data, newScope);

    TRAVchildren(node);

    // Once done processing the function body, decrement the scope level to exit the function's scope
    ST_popScopeLevel(data);

    return node;
}

node_st *STglobdecl(node_st *node)
{
    struct data_st *data = DATA_ST_GET();

    // Extract the variable's name, type, and declaration line number
    char *identifier = GLOBDECL_NAME(node);
    enum Type type = GLOBDECL_TYPE(node);
    char *typestr = VarTypeToString(type);
    int declaredAtLine = NODE_BLINE(node);
    int isFunction = 1;

    insertSymbol(data, identifier, typestr, declaredAtLine, isFunction);

    return node;
}

node_st *STglobdef(node_st *node)
{
    struct data_st *data = DATA_ST_GET();

    // Extract the variable's name, type, and declaration line number
    char *identifier = GLOBDEF_NAME(node);
    enum Type type = GLOBDEF_TYPE(node);
    char *typestr = VarTypeToString(type);
    int declaredAtLine = NODE_BLINE(node);
    int isFunction = 1;

    insertSymbol(data, identifier, typestr, declaredAtLine, isFunction);

    return node;
}