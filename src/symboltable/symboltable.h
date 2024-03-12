
typedef struct SymbolInfo
{
    char *name;         // Identifier's name
    char *type;         // Identifier's type, for simplicity
    int declaredAtLine; // Line number of declaration for simplicity
    int scopeLevel;     // Scope level of the identifier (0 for global)
    int isFunction;
} SymbolInfo;
