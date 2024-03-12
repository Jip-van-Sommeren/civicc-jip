#include "palm/hash_table.h"
#include "symboltable.h"

SymbolInfo *deepCopySymbolInfo(const SymbolInfo *original);

// Function to deep copy the hash table
htable_st *deepCopyHashTable(htable_st *original);

void freeSymbolInfo(SymbolInfo *info);

// Function to free the deep copied hash table
void freeDeepCopyHashTable(htable_st *table);
