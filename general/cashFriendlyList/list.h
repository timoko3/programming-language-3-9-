#ifndef LIST_H
#define LIST_H

#include "protection_list.h"
#include "general_list.h"

// #define DEBUG

extern const int SEARCH_NOT_FOUND_VALUE;

const size_t listValueMaxLen = 64;

listStatus listCtor(list_t* list, size_t capacity, listCmpFunc_t cmpFunc, listCopyFunc_t copyFunc);
listStatus listDtor(list_t* list, listFreeDataFunc_t freeDataFunc = NULL);

listStatus listInsertAfter(list_t* list, int index, listVal_t insValue);
listStatus listInsertBefore(list_t* list, int insIndex, listVal_t insValue);
listStatus lisInsertToHead(list_t* list, listVal_t insValue);
listStatus listInsertToTail(list_t* list, listVal_t insValue);
listStatus listDelete(list_t* list, int index);
listStatus listFind(list_t* list, listVal_t findValue, int* findIndex);

listStatus listOptimize(list_t* list);
listStatus listLinearize(list_t* list);
listStatus listFreeUnusedMem(list_t* list);

listStatus listCopy(list_t* dest, list_t* src);

#endif /* LIST_H */