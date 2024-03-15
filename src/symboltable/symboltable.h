#include "ccngen/enum.h"
typedef struct SymbolInfo
{
    char *name;         // Identifier's name
    enum Type type;     // Identifier's type, for simplicity
    int declaredAtLine; // Line number of declaration for simplicity
    int scopeLevel;     // Scope level of the identifier (0 for global)
    int isFunction;
} SymbolInfo;
