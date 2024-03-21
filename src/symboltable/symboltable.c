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
#include "vartypetostring.h"

#define TABLE_SIZE 128
#define STACK_SIZE 3

/**
 * @fn reportDoubleDeclarationError
 */
void reportDoubleDeclarationError(char *name, node_st *entry)
{

    CTI(CTI_ERROR, true, "Double declaration of '%s' declared at line %d", name, SYMBOLENTRY_DECLAREDATLINE(entry));
}

void undeclaredVar(char *name, int declaredAtline)
{

    CTI(CTI_ERROR, true, "No declaration of '%s' used in line %d found", name, declaredAtline);
}

/**
 * @fn checkDecl
 */
bool checkDecl(struct data_st *data, char *name)
{
    Scope *currentScope = &(data->scopeStack->scopes[data->scopeStack->top]);
    htable_st *currentSymbolTable = currentScope->symbolTable;
    node_st *entry = HTlookup(currentSymbolTable, name);
    if (entry != NULL)
    {
        reportDoubleDeclarationError(name, entry);
        return true;
    }
    return false;
}

/**
 * @fn insertSymbol
 */
void insertSymbol(struct data_st *data, char *name, enum Type type, int declaredAtLine, int nodetype, node_st *dims, node_st *params)
{
    if (!data || data->scopeStack->top < 0)
    {
        fprintf(stderr, "No current scope available for symbol insertion.\n");
        return;
    }
    Scope *currentScope = &(data->scopeStack->scopes[data->scopeStack->top]);
    htable_st *currentSymbolTable = currentScope->symbolTable;
    node_st *newentry = ASTsymbolentry(dims, params, strdup(name), type, declaredAtLine, currentScope->level, nodetype);

    if (!HTinsert(currentSymbolTable, name, newentry))
    {
        fprintf(stderr, "Failed to insert symbol '%s' into the symbol table.\n", name);
    }
}

node_st *findLink(struct data_st *data, char *name)
{
    for (int i = data->scopeStack->top; i > -1; --i)
    {
        node_st *entry = HTlookup(data->scopeStack->scopes[i].symbolTable, name);
        if (entry != NULL)
        {
            return entry;
        }
    }
    return NULL;
}

enum Type findReturnType(struct data_st *data)
{
    return data->scopeStack->scopes[data->scopeStack->top].type;
}

/**
 * @fn ST_pushScopeLevel
 */
void ST_pushScopeLevel(struct data_st *data, enum Type type)
{
    if (data->scopeStack->top + 1 >= data->scopeStack->capacity)
    {
        // Reallocation logic for scopeStack
        int newCapacity = data->scopeStack->capacity + 1;
        data->scopeStack->scopes = realloc(data->scopeStack->scopes, sizeof(Scope) * newCapacity);
        data->scopeStack->capacity = newCapacity;
    }
    data->scopeStack->top++;
    data->scopeStack->scopes[data->scopeStack->top].level = data->scopeStack->top;
    data->scopeStack->scopes[data->scopeStack->top].type = type;
    data->scopeStack->scopes[data->scopeStack->top].symbolTable = HTnew_String(TABLE_SIZE); // Initialize a new symbol table for the new scope
}

/**
 * @fn ST_popScopeLevel
 */
node_st *ST_popScopeLevel(struct data_st *data)
{
    htable_iter_st *iter = HTiterate(data->scopeStack->scopes[data->scopeStack->top].symbolTable);
    node_st *symboltable = NULL;
    node_st *symboltableTail = NULL;
    while (iter != NULL)
    {
        node_st *entry = HTiterValue(iter); // Extract st entries

        if (symboltable == NULL)
        {
            // This is the first assignment statement.
            symboltable = ASTsymboltable(entry, NULL);
            symboltableTail = symboltable; // The tail is the same as head now.
        }
        else
        {
            // Append to the tail and update the tail pointer.
            node_st *newSymboltable = ASTsymboltable(entry, NULL);
            SYMBOLTABLE_NEXT(symboltableTail) = newSymboltable; // Append to the end.
            symboltableTail = newSymboltable;                   // Update the tail.
        }
        iter = HTiterateNext(iter);
    }
    HTdelete(data->scopeStack->scopes[data->scopeStack->top].symbolTable); // Cleanup the symbol table of the current scope
    data->scopeStack->top--;

    return symboltable;
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
 * @fn STinit
 */
void STinit()
{
    struct data_st *data = DATA_ST_GET();
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
    data->scopeStack->scopes = malloc(sizeof(Scope) * STACK_SIZE); // Allocate for Scope structures
    if (!data->scopeStack->scopes)
    {
        fprintf(stderr, "Memory allocation failed for scope structures in STinit.\n");
        free(data->scopeStack); // Cleanup previously allocated memory
        return;
    }
    data->scopeStack->capacity = STACK_SIZE;
    data->scopeStack->top = -1; // Indicates that the stack is empty
    data->unresolvedFuncall = HTnew_String(TABLE_SIZE);

    // Automatically create the global scope with its own symbol table
    ST_pushScopeLevel(data, 0);
}

/**
 * @fn STfini
 */
void STfini()
{
    struct data_st *data = DATA_ST_GET();
    // This is just to make sure mem gets freed stack should be empty always at this point.
    while (data->scopeStack->top > -1)
    {
        ST_popScopeLevel(data);
    }
    if (data->scopeStack != NULL)
    {
        // No need to iterate through scopes for symbol table cleanup
        free(data->scopeStack->scopes); // Free the scopes array
        free(data->scopeStack);         // Free the scope stack itself
        data->scopeStack = NULL;        // Safeguard
    }
    else
    {
        fprintf(stderr, "Scope stack was already uninitialized or never initialized.\n");
    }
    if (HTelementCount(data->unresolvedFuncall) > 0)
    {
        htable_iter_st *iter = HTiterate(data->unresolvedFuncall);
        while (iter != NULL)
        {
            node_st *entry = HTiterValue(iter); // Extract st entries
            CTI(CTI_ERROR, true, "%s was never declared", SYMBOLENTRY_NAME(entry));
            iter = HTiterateNext(iter);
        }
    }
    HTdelete(data->unresolvedFuncall);
    CTIabortOnError();
}

/**
 * @fn STprogram
 */
node_st *STprogram(node_st *node)
{
    struct data_st *data = DATA_ST_GET();
    TRAVchildren(node);
    node_st *symboltable = ST_popScopeLevel(data);

    PROGRAM_SYMBOLTABLE(node) = symboltable;

    return node;
}

node_st *STvardecl(node_st *node)
{

    struct data_st *data = DATA_ST_GET();

    // Extract the variable's name, type, and declaration line number
    char *identifier = VARDECL_NAME(node);
    enum Type type = VARDECL_TYPE(node);
    int declaredAtLine = NODE_BLINE(node);
    int nodetype = NODE_TYPE(node);

    node_st *dims = NULL;
    if (VARDECL_DIMS(node) != NULL)
    {
        // we need to make a new node because it symbol entry child
        dims = VARDECL_DIMS(node);
    }

    // Check if the variable is already declared
    if (!checkDecl(data, identifier))
    {
        // Only insert the symbol if it was not already declared
        insertSymbol(data, identifier, type, declaredAtLine, nodetype, dims, NULL);
    }
    TRAVchildren(node);
    return node;
}

void resolveFuncall(struct data_st *data, node_st *entry, enum Type type, char *identifier, int declaredAtLine, node_st *params)
{
    SYMBOLENTRY_TYPE(entry) = type;
    SYMBOLENTRY_DECLAREDATLINE(entry) = declaredAtLine;
    Scope *currentScope = &(data->scopeStack->scopes[data->scopeStack->top]);
    SYMBOLENTRY_SCOPELEVEL(entry) = currentScope->level;
    SYMBOLENTRY_PARAMS(entry) = params;
    HTinsert(data->scopeStack->scopes[data->scopeStack->top].symbolTable, identifier, entry);
    HTremove(data->unresolvedFuncall, identifier);
    return;
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
    int declaredAtLine = NODE_BLINE(node);
    int nodetype = NODE_TYPE(node);
    node_st *params = FUNDEF_PARAMS(node);
    node_st *unresolvedEntry = HTlookup(data->unresolvedFuncall, identifier);
    if (unresolvedEntry != NULL)
    {
        resolveFuncall(data, unresolvedEntry, type, identifier, declaredAtLine, params);
    }
    else if (!checkDecl(data, identifier))
    {
        // Only insert the symbol if it was not already declared
        insertSymbol(data, identifier, type, declaredAtLine, nodetype, NULL, params);
    }
    // sert the function symbol into the symbol table with the current scope level
    //  Increment the scope level for the function body
    ST_pushScopeLevel(data, type);

    // TRAVparams(node);
    // TRAVdo(FUNDEF_BODY(node));
    TRAVchildren(node);

    // Once done processing the function body, decrement the scope level to exit the function's scope
    node_st *symboltable = ST_popScopeLevel(data);
    // Assign symboltable to fundef node
    FUNDEF_SYMBOLTABLE(node) = symboltable;
    return node;
}

node_st *STparam(node_st *node)
{
    struct data_st *data = DATA_ST_GET();

    // Extract the variable's name, type, and declaration line number
    char *identifier = PARAM_NAME(node);
    enum Type type = PARAM_TYPE(node);
    // char *typestr = VarTypeToString(type);
    int declaredAtLine = NODE_BLINE(node);
    int nodetype = NODE_TYPE(node);

    node_st *dims = NULL;

    // Only insert the symbol if it was not already declared
    insertSymbol(data, identifier, type, declaredAtLine, nodetype, dims, NULL);
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
    int nodetype = NODE_TYPE(node);

    node_st *dims = NULL;

    if (!checkDecl(data, identifier))
    {
        // Only insert the symbol if it was not already declared
        insertSymbol(data, identifier, type, declaredAtLine, nodetype, dims, GLOBDECL_PARAMS(node));
    }
    TRAVchildren(node);
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
    int nodetype = NODE_TYPE(node);
    node_st *dims = NULL;
    if (GLOBDEF_DIMS(node) != NULL)
    {
        // we need to make a new node because it symbol entry child
        dims = GLOBDEF_DIMS(node);
    }

    if (!checkDecl(data, identifier))
    {
        // Only insert the symbol if it was not already declared
        insertSymbol(data, identifier, type, declaredAtLine, nodetype, dims, NULL);
    }
    TRAVchildren(node);
    return node;
}

/**
 * @fn STglobdef
 */
node_st *STvar(node_st *node)
{
    struct data_st *data = DATA_ST_GET();
    char *name = VAR_NAME(node);
    node_st *entry = findLink(data, name);
    if (entry != NULL)
    {
        VAR_SYMBOLENTRY(node) = entry;
        VAR_TYPE(node) = SYMBOLENTRY_TYPE(entry);
    }
    else
    {
        undeclaredVar(name, NODE_BLINE(node));
    }
    TRAVchildren(node);
    return node;
}

/**
 * @fn STglobdef
 */
node_st *STvarlet(node_st *node)
{
    struct data_st *data = DATA_ST_GET();
    char *name = VARLET_NAME(node);
    node_st *entry = findLink(data, name);
    if (entry != NULL)
    {
        VARLET_SYMBOLENTRY(node) = entry;
        VARLET_TYPE(node) = SYMBOLENTRY_TYPE(entry);
    }
    else
    {
        undeclaredVar(name, NODE_BLINE(node));
    }
    TRAVchildren(node);
    return node;
}

/**
 * @fn STglobdef
 */
node_st *STfuncall(node_st *node)
{
    struct data_st *data = DATA_ST_GET();
    char *name = FUNCALL_NAME(node);
    node_st *entry = findLink(data, name);
    if (entry != NULL)
    {
        FUNCALL_SYMBOLENTRY(node) = entry;

        FUNCALL_TYPE(node) = SYMBOLENTRY_TYPE(entry);
    }
    else
    {
        // Make symbolentry node, set all properties to -1, will be updated when corresponding fundef is found.
        node_st *entry = ASTsymbolentry(NULL, NULL, strdup(name), -1, -1, -1, -1);
        FUNCALL_SYMBOLENTRY(node) = entry;
        HTinsert(data->unresolvedFuncall, name, entry);
    }

    TRAVchildren(node);
    return node;
}

node_st *STreturn(node_st *node)
{
    struct data_st *data = DATA_ST_GET();
    RETURN_TYPE(node) = findReturnType(data);
    TRAVchildren(node);
    return node;
}