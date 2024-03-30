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
#include "varinitregassignment/vardefhelper.h"

#define TABLE_SIZE 128
#define STACK_SIZE 4

static bool inForLoop = false;
static int forCounter = 0;
static int vardeclIndex = 0;
static int fundefIndex = 0;
static int globdeclIndexFun = 0;
static int globdeclIndexVar = 0;
static int globdefIndex = 0;
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
void insertSymbol(struct data_st *data, char *name, enum Type type, int declaredAtLine, int nodetype, node_st *dims, int dimsCount, node_st *params, int index, bool externB, bool global)
{
    if (!data || data->scopeStack->top < 0)
    {
        fprintf(stderr, "No current scope available for symbol insertion.\n");
        return;
    }
    Scope *currentScope = &(data->scopeStack->scopes[data->scopeStack->top]);
    htable_st *currentSymbolTable = currentScope->symbolTable;

    node_st *newentry = ASTsymbolentry(dims, strdup(name), type, declaredAtLine, currentScope->level, nodetype, dimsCount, strdup(currentScope->name));

    if (params != NULL)
    {
        SYMBOLENTRY_PARAMS(newentry) = params;
    }

    SYMBOLENTRY_INDEX(newentry) = index;
    SYMBOLENTRY_EXTERNB(newentry) = externB;
    SYMBOLENTRY_GLOBAL(newentry) = global;

    if (!HTinsert(currentSymbolTable, name, newentry))
    {
        fprintf(stderr, "Failed to insert symbol '%s' into the symbol table.\n", name);
        return;
    }
    return;
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

node_st *findLink1D(struct data_st *data, char *name)
{

    node_st *entry = HTlookup(data->scopeStack->scopes[data->scopeStack->top].symbolTable, name);
    if (entry != NULL)
    {
        return entry;
    }

    return NULL;
}

node_st *findGlobLink(struct data_st *data, char *name)
{

    for (int i = data->scopeStack->top - 1; i > -1; --i)
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
void ST_pushScopeLevel(struct data_st *data, enum Type type, char *name)
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
    data->scopeStack->scopes[data->scopeStack->top].name = name;
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
    HTdelete(data->scopeStack->scopes[data->scopeStack->top].symbolTable);
    free(data->scopeStack->scopes[data->scopeStack->top].name);
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
    ST_pushScopeLevel(data, 0, strdup(""));
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
        dims = exprsToExprs(VARDECL_DIMS(node));
    }
    int dimsCount = checkExprDimension(VARDECL_DIMS(node));

    // Check if the variable is already declared
    if (!checkDecl(data, identifier))
    {
        // Only insert the symbol if it was not already declared
        insertSymbol(data, identifier, type, declaredAtLine, nodetype, dims, dimsCount, NULL, vardeclIndex, false, false);
        vardeclIndex++;
    }
    TRAVchildren(node);
    return node;
}

void resolveFuncall(struct data_st *data, node_st *entry, enum Type type, char *identifier, int declaredAtLine, node_st *params, int index)
{
    SYMBOLENTRY_TYPE(entry) = type;
    SYMBOLENTRY_DECLAREDATLINE(entry) = declaredAtLine;
    Scope *currentScope = &(data->scopeStack->scopes[data->scopeStack->top]);
    SYMBOLENTRY_SCOPELEVEL(entry) = currentScope->level;
    SYMBOLENTRY_SCOPENAME(entry) = strdup(identifier);
    SYMBOLENTRY_PARAMS(entry) = params;
    SYMBOLENTRY_INDEX(entry) = index;
    HTinsert(data->scopeStack->scopes[data->scopeStack->top].symbolTable, identifier, entry);
    HTremove(data->unresolvedFuncall, identifier);
    return;
}

void resolveGlobDef(struct data_st *data, node_st *unresolvedEntry, enum Type type, char *identifier, int declaredAtLine, int index, node_st *dims, int dimsCount, bool glob)
{
    SYMBOLENTRY_TYPE(unresolvedEntry) = type;
    SYMBOLENTRY_DECLAREDATLINE(unresolvedEntry) = declaredAtLine;
    Scope *currentScope = &(data->scopeStack->scopes[data->scopeStack->top]);
    SYMBOLENTRY_SCOPELEVEL(unresolvedEntry) = currentScope->level;
    SYMBOLENTRY_SCOPENAME(unresolvedEntry) = strdup(identifier);
    SYMBOLENTRY_INDEX(unresolvedEntry) = index;
    SYMBOLENTRY_DIMS(unresolvedEntry) = dims;
    SYMBOLENTRY_DIMSCOUNT(unresolvedEntry) = dimsCount;
    SYMBOLENTRY_GLOBAL(unresolvedEntry) = glob;
    HTinsert(data->scopeStack->scopes[data->scopeStack->top].symbolTable, identifier, unresolvedEntry);
    HTremove(data->unresolvedFuncall, identifier);
}

char *makeScopeName(struct data_st *data, char *name)
{
    char *str = malloc(256 * sizeof(char));
    if (!str)
        return NULL; // Allocation check

    // Initialize the string to be empty
    str[0] = '\0';

    for (int i = 0; i < data->scopeStack->top + 1; ++i)
    {
        Scope *currentScope = &(data->scopeStack->scopes[i]);
        // Append the current scope name with an underscore as needed
        // Check if it's not the first scope name being appended to avoid leading underscores
        if (data->scopeStack->top != 0 && i != 0)
        {
            strcat(str, "_");
            strcat(str, currentScope->name);
        }
    }
    strcat(str, "_");
    strcat(str, name);

    return str;
}
/**
 * @fn STfundef
 */
node_st *STfundef(node_st *node)
{
    struct data_st *data = DATA_ST_GET();
    forCounter = 1;
    vardeclIndex = 0;

    // Retrieve the current scope level from the data structure

    char *identifier = FUNDEF_NAME(node); // Extract function name
    enum Type type = FUNDEF_TYPE(node);
    int declaredAtLine = NODE_BLINE(node);
    int nodetype = NODE_TYPE(node);
    node_st *params = FUNDEF_PARAMS(node);
    node_st *unresolvedEntry = HTlookup(data->unresolvedFuncall, identifier);

    int index = -1;
    if (!FUNDEF_EXPORT(node))
    {

        index = fundefIndex;
        fundefIndex++;
    }
    FUNDEF_INDEX(node) = index;
    FUNDEF_SCOPENAME(node) = makeScopeName(data, identifier);
    Scope *currentScope = &(data->scopeStack->scopes[data->scopeStack->top]);
    bool global = false;
    if (currentScope->level == 0)
    {
        global = true;
    }
    printf("scopename: %s\n", FUNDEF_SCOPENAME(node));
    if (unresolvedEntry != NULL)
    {

        resolveFuncall(data, unresolvedEntry, type, identifier, declaredAtLine, params, index);
    }
    else if (!checkDecl(data, identifier))
    {
        // Only insert the symbol if it was not already declared
        insertSymbol(data, identifier, type, declaredAtLine, nodetype, NULL, 0, params, index, false, global);
    }
    // sert the function symbol into the symbol table with the current scope level
    //  Increment the scope level for the function body
    ST_pushScopeLevel(data, type, strdup(identifier));

    TRAVopt(FUNDEF_PARAMS(node));
    TRAVopt(FUNDEF_BODY(node));

    // Once done processing the function body, decrement the scope level to exit the function's scope
    node_st *symboltable = ST_popScopeLevel(data);
    // Assign symboltable to fundef node
    FUNDEF_SYMBOLTABLE(node) = symboltable;
    return node;
}

node_st *STfor(node_st *node)
{
    struct data_st *data = DATA_ST_GET();

    // Retrieve the current scope level from the data structure
    char *identifier = FOR_VAR(node); // Extract function name
    bool newVar = FOR_NEWVAR(node);
    enum Type type = CT_int;
    int declaredAtLine = NODE_BLINE(node);
    int nodetype = NT_VAR;
    // check if initializer
    char str[10];

    sprintf(str, "for_%s_%d", identifier, forCounter);
    node_st *entry = findLink(data, str);
    if (entry != NULL)
    {
        forCounter++;
        sprintf(str, "for_%s_%d", identifier, forCounter);
    }

    if (newVar)
    {
        insertSymbol(data, str, type, declaredAtLine, nodetype, NULL, 0, NULL, vardeclIndex, false, false);
        vardeclIndex++;
        MEMfree(FOR_VAR(node));
        FOR_VAR(node) = strdup(str);
    }
    else
    {
        node_st *entry = findLink(data, identifier);
        FOR_VARINDEX(node) = SYMBOLENTRY_INDEX(entry);
    }

    // set new name as var name

    // sert the function symbol into the symbol table with the current scope level
    //  Increment the scope level for the function body
    inForLoop = true;
    TRAVchildren(node);
    inForLoop = false;
    // Once done processing the function body, decrement the scope level to exit the function's scope
    // Assign symboltable to fundef node
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
    if (PARAM_DIMS(node) != NULL)
    {
        dims = idsToExprs(PARAM_DIMS(node));
    }
    node_st *paramDims = PARAM_DIMS(node);
    while (paramDims != NULL)
    {
        char *id = IDS_NAME(paramDims);
        insertSymbol(data, id, CT_int, declaredAtLine, NT_VAR, NULL, 0, NULL, vardeclIndex, false, false);
        vardeclIndex++;
        paramDims = IDS_NEXT(paramDims);
    }

    int dimsCount = checkParamDimension(PARAM_DIMS(node));
    // in case of global scope mostly in case of globdecl param insertion not needed otherwise we might get collission
    // since params of fundefs in global scope should be at scope level 1 this should be fine
    if (data->scopeStack->top > 0)
    {
        insertSymbol(data, identifier, type, declaredAtLine, nodetype, dims, dimsCount, NULL, vardeclIndex, false, false);
        vardeclIndex++;
    }

    // Only insert the symbol if it was not already declared
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
    int dimsCount = checkParamDimension(GLOBDECL_DIMS(node));
    node_st *dims = NULL;

    int globdeclIndex = 0;
    if (GLOBDECL_ISVAR(node))
    {
        globdeclIndex = globdeclIndexVar;
        globdeclIndexVar++;
    }
    else
    {
        globdeclIndex = globdeclIndexFun;
        globdeclIndexFun++;
    }
    if (GLOBDECL_DIMS(node) != NULL)
    {
        dims = idsToExprs(GLOBDECL_DIMS(node));
    }

    if (!checkDecl(data, identifier))
    {
        // Only insert the symbol if it was not already declared

        insertSymbol(data, identifier, type, declaredAtLine, nodetype, dims, dimsCount, GLOBDECL_PARAMS(node), globdeclIndex, true, true);
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
    node_st *unresolvedEntry = HTlookup(data->unresolvedFuncall, identifier);
    GLOBDEF_INDEX(node) = globdefIndex;

    if (GLOBDEF_DIMS(node) != NULL)
    {
        // we need to make a new node because it symbol entry child
        dims = exprsToExprs(GLOBDEF_DIMS(node));
    }
    int dimsCount = checkExprDimension(GLOBDEF_DIMS(node));

    printf(" globdef index %d\n", globdefIndex);
    if (unresolvedEntry != NULL)
    {

        resolveGlobDef(data, unresolvedEntry, type, identifier, declaredAtLine, globdefIndex, dims, dimsCount, true);
    }
    else if (!checkDecl(data, identifier))
    {
        // Only insert the symbol if it was not already declared
        insertSymbol(data, identifier, type, declaredAtLine, nodetype, dims, dimsCount, NULL, globdefIndex, false, true);
    }

    globdefIndex++;
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
    printf("vardecl index %d, %s var name line number %d, declared at line", vardeclIndex, VAR_NAME(node), NODE_BLINE(node));
    node_st *entry = findLink(data, name);
    if (entry == NULL && inForLoop)
    {
        char str2[10];
        sprintf(str2, "for_%s_%d", name, forCounter);
        entry = findLink(data, str2);
        if (entry != NULL)
        {
            VAR_SYMBOLENTRY(node) = entry;
            VAR_TYPE(node) = SYMBOLENTRY_TYPE(entry);
            MEMfree(VAR_NAME(node));
            VAR_NAME(node) = strdup(str2);
            VAR_INDEX(node) = SYMBOLENTRY_INDEX(entry);
        }
    }
    else if (entry != NULL)
    {
        entry = findLink(data, name);
        if ((int)NODE_BLINE(node) <= SYMBOLENTRY_DECLAREDATLINE(entry))
        {
            entry = findGlobLink(data, name);
            if (!entry)
            {
                undeclaredVar(name, NODE_BLINE(node));
            }
        }
        VAR_SYMBOLENTRY(node) = entry;
        printf("%d\n", SYMBOLENTRY_DECLAREDATLINE(entry));
        VAR_TYPE(node) = SYMBOLENTRY_TYPE(entry);
        VAR_INDEX(node) = SYMBOLENTRY_INDEX(entry);
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
        VARLET_INDEX(node) = SYMBOLENTRY_INDEX(entry);
    }
    else
    {
        if (HTlookup(data->unresolvedFuncall, name) == NULL)
        {
            node_st *entry = ASTsymbolentry(NULL, strdup(name), -1, -1, -1, -1, -1, NULL);
            VARLET_SYMBOLENTRY(node) = entry;
            HTinsert(data->unresolvedFuncall, name, entry);
        }
        else
        {
            VARLET_SYMBOLENTRY(node) = HTlookup(data->unresolvedFuncall, name);
        }
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
    if (strcmp(name, "__allocate") == 0)
    {
        return node;
    }
    node_st *entry = findLink(data, name);
    if (entry != NULL)
    {

        FUNCALL_SYMBOLENTRY(node) = entry;

        FUNCALL_TYPE(node) = SYMBOLENTRY_TYPE(entry);
        FUNCALL_INDEX(node) = SYMBOLENTRY_INDEX(entry);
        FUNCALL_SCOPE(node) = ST_currentScopeLevel(data);
    }
    // Check if funcall is not already in unresolved calls, if not add to unresolved calls
    // else link the symbolentry node.
    else
    {
        if (HTlookup(data->unresolvedFuncall, name) == NULL)
        {
            node_st *entry = ASTsymbolentry(NULL, strdup(name), -1, -1, -1, -1, -1, NULL);
            FUNCALL_SYMBOLENTRY(node) = entry;
            FUNCALL_SCOPE(node) = ST_currentScopeLevel(data);
            HTinsert(data->unresolvedFuncall, name, entry);
        }
        else
        {
            FUNCALL_SYMBOLENTRY(node) = HTlookup(data->unresolvedFuncall, name);
            FUNCALL_SCOPE(node) = ST_currentScopeLevel(data);
        }
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