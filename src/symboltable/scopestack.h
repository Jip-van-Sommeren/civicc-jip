typedef struct Scope
{
    int level;
    // Other fields as necessary
} Scope;

typedef struct ScopeStack
{
    Scope *scopes;
    int top;
    int capacity;
} ScopeStack;