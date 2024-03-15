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
#include "palm/ctinfo.h"
#include "symboltable.h"

static bool first_vardecls = false;
static bool first_fundefs = false;

/**
 * @fn reportDoubleDeclarationError
 */
void reportDoubleDeclarationError(char *name, struct SymbolInfo *info)
{
    if (info != NULL)
    {
        CTI(CTI_ERROR, true, "Double declaration of '%s' declared at line %d", name, info->declaredAtLine);
    }
    else
    {
        CTI(CTI_ERROR, true, "Double declaration of '%s'", name);
    }
}

/**
 * @fn checkDecl
 */
bool checkDecl(struct data_st *data, char *name)
{
    Scope *currentScope = &(data->scopeStack->scopes[data->scopeStack->top]);
    htable_st *currentSymbolTable = currentScope->symbolTable;
    SymbolInfo *info = HTlookup(currentSymbolTable, name);
    if (info != NULL)
    {
        reportDoubleDeclarationError(name, info);
        CTIabortOnError();
        return true;
    }
    return false;
}

/**
 * @fn insertSymbol
 */
void insertSymbol(struct data_st *data, char *name, enum Type type, int declaredAtLine, int isFunction)
{
    if (!data || data->scopeStack->top < 0)
    {
        fprintf(stderr, "No current scope available for symbol insertion.\n");
        return;
    }
    Scope *currentScope = &(data->scopeStack->scopes[data->scopeStack->top]);
    htable_st *currentSymbolTable = currentScope->symbolTable;

    SymbolInfo *info = (SymbolInfo *)malloc(sizeof(SymbolInfo));
    if (!info)
    {
        fprintf(stderr, "Memory allocation failed for SymbolInfo.\n");
        return;
    }

    info->name = strdup(name);
    info->type = type;
    info->declaredAtLine = declaredAtLine;
    info->isFunction = isFunction;
    info->scopeLevel = currentScope->level;

    if (!HTinsert(currentSymbolTable, info->name, info))
    {
        fprintf(stderr, "Failed to insert symbol '%s' into the symbol table.\n", name);
    }
}

void ST_pushScopeLevel(struct data_st *data)
{
    if (data->scopeStack->top + 1 >= data->scopeStack->capacity)
    {
        // Reallocation logic for scopeStack
        int newCapacity = data->scopeStack->capacity * 2;
        data->scopeStack->scopes = realloc(data->scopeStack->scopes, sizeof(Scope) * newCapacity);
        data->scopeStack->capacity = newCapacity;
    }
    data->scopeStack->top++;
    data->scopeStack->scopes[data->scopeStack->top].level = data->scopeStack->top;
    data->scopeStack->scopes[data->scopeStack->top].symbolTable = HTnew_String(128); // Initialize a new symbol table for the new scope
}

/**
 * @fn STinit
 */
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

    // Allocate memory for the scopeStack object itself before initializing its members
    data->scopeStack = malloc(sizeof(ScopeStack));
    if (!data->scopeStack)
    {
        fprintf(stderr, "Memory allocation failed for scopeStack in STinit.\n");
        return; // Exit if allocation failed
    }
    data->scopeStack->scopes = malloc(sizeof(Scope) * initialCapacity); // Allocate for Scope structures
    if (!data->scopeStack->scopes)
    {
        fprintf(stderr, "Memory allocation failed for scope structures in STinit.\n");
        free(data->scopeStack); // Cleanup previously allocated memory
        return;
    }
    data->scopeStack->capacity = initialCapacity;
    data->scopeStack->top = -1; // Indicates that the stack is empty

    // Automatically create the global scope with its own symbol table
    ST_pushScopeLevel(data);
}

node_st *ST_popScopeLevel(struct data_st *data)
{

    htable_iter_st *iter = HTiterate(data->scopeStack->scopes[data->scopeStack->top].symbolTable);
    node_st *symboltable = NULL;
    while (iter != NULL)
    {
        SymbolInfo *info = HTiterValue(iter);
        node_st *newentry = ASTsymbolentry(strdup(info->name), info->type, info->declaredAtLine, info->scopeLevel, info->isFunction);
        printf("%d\n", info->type);
        symboltable = ASTsymboltable(newentry, symboltable);
        free(info->name);
        free(info);
        iter = HTiterateNext(iter);
    }
    HTdelete(data->scopeStack->scopes[data->scopeStack->top].symbolTable); // Cleanup the symbol table of the current scope
    data->scopeStack->top--;

    return symboltable;
}
/**
 * @fn STvardecls
 */
node_st *STprogram(node_st *node)
{
    struct data_st *data = DATA_ST_GET();
    TRAVchildren(node);
    node_st *symboltable = ST_popScopeLevel(data);

    PROGRAM_SYMBOLTABLE(node) = symboltable;

    return node;
}

/**
 * @fn ST_currentScopeLevel
 */
int ST_currentScopeLevel(struct data_st *data)
{
    if (data->scopeStack->top > -1)
    {
        return data->scopeStack->scopes[data->scopeStack->top].level;
    }
    return -1;
}

/**
 * @fn STfini
 */
void STfini()
{
    struct data_st *data = DATA_ST_GET();
    while (data->scopeStack->top > -1)
    {

        ST_popScopeLevel(data);
    }
    if (data->scopeStack != NULL)
    {
        // No need to iterate through scopes for symbol table cleanup if done elsewhere
        free(data->scopeStack->scopes); // Free the scopes array
        free(data->scopeStack);         // Free the scope stack itself
        data->scopeStack = NULL;        // Safeguard
    }
    else
    {
        fprintf(stderr, "Scope stack was already uninitialized or never initialized.\n");
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
    // char *typestr = VarTypeToString(type);
    int declaredAtLine = NODE_BLINE(node);
    int isFunction = 0; // Assuming this should be 0 for variables

    // Check if the variable is already declared
    if (!checkDecl(data, identifier))
    {
        // Only insert the symbol if it was not already declared
        insertSymbol(data, identifier, type, declaredAtLine, isFunction);
    }
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

/**
 * @fn STfundef
 */
node_st *STfundef(node_st *node)
{
    struct data_st *data = DATA_ST_GET();

    // Retrieve the current scope level from the data structure

    char *identifier = FUNDEF_NAME(node); // Extract function name
    enum Type type = FUNDEF_TYPE(node);
    // char *typestr = VarTypeToString(type); // Extract function return type
    int declaredAtLine = NODE_BLINE(node);
    int isFunction = 1;

    if (!checkDecl(data, identifier))
    {
        // Only insert the symbol if it was not already declared
        insertSymbol(data, identifier, type, declaredAtLine, isFunction);
    }
    // Insert the function symbol into the symbol table with the current scope level
    // Increment the scope level for the function body
    ST_pushScopeLevel(data);

    TRAVchildren(node);

    // Once done processing the function body, decrement the scope level to exit the function's scope
    node_st *symboltable = ST_popScopeLevel(data);

    FUNDEF_SYMBOLTABLE(node) = symboltable;
    return node;
}

/**
 * @fn STglobdecl
 */
node_st *STglobdecl(node_st *node)
{
    struct data_st *data = DATA_ST_GET();

    // Extract the variable's name, type, and declaration line number
    char *identifier = GLOBDECL_NAME(node);
    enum Type type = GLOBDECL_TYPE(node);
    // char *typestr = VarTypeToString(type);
    int declaredAtLine = NODE_BLINE(node);
    int isFunction = 1;

    if (!checkDecl(data, identifier))
    {
        // Only insert the symbol if it was not already declared
        insertSymbol(data, identifier, type, declaredAtLine, isFunction);
    }

    return node;
}

/**
 * @fn STglobdef
 */
node_st *STglobdef(node_st *node)
{
    struct data_st *data = DATA_ST_GET();

    // Extract the variable's name, type, and declaration line number
    char *identifier = GLOBDEF_NAME(node);
    enum Type type = GLOBDEF_TYPE(node);
    // char *typestr = VarTypeToString(type);
    int declaredAtLine = NODE_BLINE(node);
    int isFunction = 0;

    if (!checkDecl(data, identifier))
    {
        // Only insert the symbol if it was not already declared
        insertSymbol(data, identifier, type, declaredAtLine, isFunction);
    }

    return node;
}