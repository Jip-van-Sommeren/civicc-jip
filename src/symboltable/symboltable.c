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

#define TABLE_SIZE 128
#define STACK_SIZE 3

/**
 * @fn reportDoubleDeclarationError
 */
void reportDoubleDeclarationError(char *name, node_st *entry)
{
    if (entry != NULL)
    {
        CTI(CTI_ERROR, true, "Double declaration of '%s' declared at line %d", name, SYMBOLENTRY_DECLAREDATLINE(entry));
    }
    else
    {
        CTI(CTI_ERROR, true, "Double declaration of '%s'", name);
    }
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
void insertSymbol(struct data_st *data, char *name, enum Type type, int declaredAtLine, int isFunction)
{
    printf("Inserting '%s' into the symbol table.\n", name);
    if (!data || data->scopeStack->top < 0)
    {
        fprintf(stderr, "No current scope available for symbol insertion.\n");
        return;
    }
    Scope *currentScope = &(data->scopeStack->scopes[data->scopeStack->top]);
    htable_st *currentSymbolTable = currentScope->symbolTable;
    printf("Inserting '%s' into the symbol table.\n", name);
    node_st *newentry = ASTsymbolentry(strdup(name), type, declaredAtLine, currentScope->level, isFunction);

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

/**
 * @fn ST_pushScopeLevel
 */
void ST_pushScopeLevel(struct data_st *data)
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
    data->scopeStack->scopes[data->scopeStack->top].symbolTable = HTnew_String(TABLE_SIZE); // Initialize a new symbol table for the new scope
}

/**
 * @fn ST_popScopeLevel
 */
node_st *ST_popScopeLevel(struct data_st *data)
{
    htable_iter_st *iter = HTiterate(data->scopeStack->scopes[data->scopeStack->top].symbolTable);
    node_st *symboltable = NULL;
    while (iter != NULL)
    {
        node_st *entry = HTiterValue(iter);               // Extract st entries
        symboltable = ASTsymboltable(entry, symboltable); // Recursively build the symboltable node
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
    ST_pushScopeLevel(data);
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

/**
 * @fn STvardecls
 */
node_st *STvardecls(node_st *node)
{
    TRAVchildren(node);
    return node;
}

node_st *STvardecl(node_st *node)
{
    struct data_st *data = DATA_ST_GET();

    // Extract the variable's name, type, and declaration line number
    char *identifier = VARDECL_NAME(node);
    enum Type type = VARDECL_TYPE(node);
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
    return node;
}

void resolveFuncall(struct data_st *data, node_st *entry, enum Type type, char *identifier, int declaredAtLine)
{
    SYMBOLENTRY_TYPE(entry) = type;
    SYMBOLENTRY_DECLAREDATLINE(entry) = declaredAtLine;
    Scope *currentScope = &(data->scopeStack->scopes[data->scopeStack->top]);
    SYMBOLENTRY_SCOPELEVEL(entry) = currentScope->level;
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
    int isFunction = 1;
    node_st *unresolvedEntry = HTlookup(data->unresolvedFuncall, identifier);
    if (unresolvedEntry != NULL)
    {
        resolveFuncall(data, unresolvedEntry, type, identifier, declaredAtLine);
    }
    else if (!checkDecl(data, identifier))
    {
        // Only insert the symbol if it was not already declared
        insertSymbol(data, identifier, type, declaredAtLine, isFunction);
    }
    // Insert the function symbol into the symbol table with the current scope level
    // Increment the scope level for the function body
    ST_pushScopeLevel(data);

    TRAVparams(node);
    TRAVbody(node);

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
    int isFunction = 0;

    // Only insert the symbol if it was not already declared
    insertSymbol(data, identifier, type, declaredAtLine, isFunction);
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
    int isFunction = 0;

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
    printf("here  \n");
    if (entry != NULL)
    {
        FUNCALL_SYMBOLENTRY(node) = entry;
    }
    else
    {
        node_st *entry = ASTsymbolentry(strdup(name), 1, -1, -1, 1);
        FUNCALL_SYMBOLENTRY(node) = entry;
        HTinsert(data->unresolvedFuncall, name, entry);
    }

    TRAVchildren(node);
    return node;
}