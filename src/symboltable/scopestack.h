typedef struct Scope
{
    int level;
    enum Type type;
    htable_st *symbolTable;
    // Other fields as necessary
} Scope;

typedef struct ScopeStack
{
    Scope *scopes;
    int top;
    int capacity;
} ScopeStack;
