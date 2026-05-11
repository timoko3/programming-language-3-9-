#include "list.h"
#include "general_list.h"
#include "general/file.h"
#include "general/poison.h"
#include "general/debug.h"

#include <malloc.h>
#include <string.h>
#include <assert.h>

extern "C" {
    int optimizedStrcmp(char* str1, char* str2);
}

#define verify(list) if(verifyList(list, __FUNCTION__, __FILE__, __LINE__) != PROCESS_OK_LIST) return list->status.type

const int SEARCH_NOT_FOUND_VALUE = -1;

static listStatus listInit(list_t* list, size_t startIndex = 1);
static listStatus reallocateList(list_t* list); 
static void placeNodeRight(list_t* list, int logicalInd, int physicalInd);

listStatus listCtor(list_t* list, size_t capacity, listCmpFunc_t cmpFunc, listCopyFunc_t copyFunc){
    assert(list);
    assert(cmpFunc);
    assert(copyFunc);
    
    list->capacity = capacity;
    assert(list->capacity > 2);

    list->size     = 0;
    *freeInd(list) = 1;

    list->elem = (listElem_t*) calloc(list->capacity, sizeof(listElem_t));
    assert(list->elem);

    list->cmpFunc  = cmpFunc; 
    list->copyFunc = copyFunc;

    listInit(list, 0);
    
    list->status.type = PROCESS_OK_LIST;
    return PROCESS_OK_LIST;
}

listStatus listDtor(list_t* list, listFreeDataFunc_t freeDataFunc){
    assert(list);

    if(freeDataFunc){
        for(size_t i = 0; i < list->capacity; i++){
            if(list->elem[i].data != LIST_POISON) freeDataFunc(list->elem[i].data);
        }        
    }


    poisonMemory(list->elem, sizeof(listElem_t) * list->capacity);
    free(list->elem);
    list->elem = NULL;

    poisonMemory(&list->size,        sizeof(list->size));
    poisonMemory(&list->capacity,    sizeof(list->capacity));
    poisonMemory(&*freeInd(list),    sizeof(*freeInd(list)));
    poisonMemory(&list->status,      sizeof(list->status));

    return PROCESS_OK_LIST;
}

listStatus listInsertAfter(list_t* list, int insIndex, listVal_t insValue){
    assert(list);

    #ifdef DEBUG
    verify(list);
    log(list, "before %s %d", "insertAfter", insIndex);
    #endif /* DEBUG */

    if((list->capacity - list->size) <= 2){
        reallocateList(list);
    }

    *data(list, *freeInd(list)) = list->copyFunc(*data(list, *freeInd(list)), insValue);

    int insertedCellPhysInd = *freeInd(list);
    *freeInd(list) = *next(list, *freeInd(list));

    *next(list, insertedCellPhysInd) = *next(list, insIndex);
    *prev(list, insertedCellPhysInd) = insIndex;

    *prev(list, *next(list, insIndex)) = insertedCellPhysInd;
    *next(list, insIndex) = insertedCellPhysInd;

    *next(list, *tail(list)) = 0;
    *prev(list, *freeInd(list)) = *tail(list);

    (list->size)++;

    #ifdef DEBUG
    verify(list);
    log(list, "after %s %d", "insertAfter", insIndex);
    #endif /* DEBUG */

    return PROCESS_OK_LIST;
}

listStatus listInsertBefore(list_t* list, int insIndex, listVal_t insValue){
    assert(list);

    insIndex = *prev(list, insIndex);
    listInsertAfter(list, insIndex, insValue);

    return PROCESS_OK_LIST;
}

listStatus listInsertToTail(list_t* list, listVal_t insValue){
    assert(list);
    
    listInsertBefore(list, 0, insValue);

    return PROCESS_OK_LIST;
}

listStatus lisInsertToHead(list_t* list, listVal_t insValue){
    assert(list);
    
    listInsertAfter(list, 0, insValue);

    return PROCESS_OK_LIST;
}

listStatus listDelete(list_t* list, int deleteIndex){
    assert(list);

    #ifdef DEBUG
    verify(list);
    log(list, "before %s %d", "delete", deleteIndex);
    #endif /* DEBUG */ 

    *next(list, *prev(list, deleteIndex)) = *next(list, deleteIndex);
    *prev(list, *next(list, deleteIndex)) = *prev(list, deleteIndex);

    *data(list, deleteIndex) = (void*) LIST_POISON;
    *next(list, deleteIndex) = *freeInd(list);
    *prev(list, deleteIndex) = *tail(list);

    *freeInd(list) = deleteIndex;
    *next(list, *tail(list)) = *freeInd(list);

    *next(list, *tail(list)) = 0;

    (list->size)--;

    #ifdef DEBUG
    verify(list);
    log(list, "after %s %d", "delete", deleteIndex);
    #endif /* DEBUG */

    return PROCESS_OK_LIST;
}

listStatus listFind(list_t* list, listVal_t findValue, int* findIndex){
    assert(list);
    assert(findIndex);

    *findIndex = SEARCH_NOT_FOUND_VALUE;

    int curElem = list->elem->next;
    LPRINTF("size = %llu\n", list->size);
    
    for(size_t i = 0; i < list->size; i++){
        if((list->elem[curElem].data != LIST_POISON) && !list->cmpFunc(findValue, list->elem[curElem].data)){
            *findIndex = curElem;
            break; 
        }
        
        curElem = list->elem[curElem].next;
    }

    return PROCESS_OK_LIST;
}

static listStatus listInit(list_t* list, size_t startIndex){
    assert(list);

    static size_t initCount = 0;

    for(int allocInd = startIndex; allocInd < list->capacity; allocInd++){
        // *data(list, allocInd) = (char*) calloc(listValueMaxLen, sizeof(char));
        // assert(*data(list, allocInd));
        
        // LPRINTF("memAllocated = %llu\n", malloc_usable_size(*data(list, allocInd)));
    }

    for(size_t fillInd = startIndex; fillInd < list->capacity; fillInd++){
        LPRINTF("fillInd = %d\n", (int) fillInd);

        *data(list, (int) fillInd) = (void*) LIST_POISON;
        *next(list, (int) fillInd) = (int) fillInd + 1;
        *prev(list, (int) fillInd) = (int) fillInd - 1;
    }

    if(startIndex == 0){
        *data(list, 0) = (void*) LIST_POISON;
        *head(list) = 0;
        *tail(list) = 0;
    }

    initCount++;

    return PROCESS_OK_LIST;
}

listStatus listLinearize(list_t* list){
    assert(list);

    #ifdef DEBUG
    verify(list);
    log(list, "before %s", "linearization");
    #endif /* DEBUG */

    int logicalInd = *head(list);
    for(int physicalInd = *head(list); (*data(list, physicalInd) != LIST_POISON); physicalInd = *next(list, physicalInd)){
        $
        if(logicalInd != physicalInd){
            $
            // if(*data(list, logicalInd) == LIST_POISON){
                placeNodeRight(list, logicalInd, physicalInd);
                physicalInd = logicalInd;
                
                
            // }
        }
        logicalInd++;
    }

    #ifdef DEBUG
    // verify(list);
    log(list, "after %s", "linearization");
    #endif /* DEBUG */ 

    return PROCESS_OK_LIST;
}

listStatus listFreeUnusedMem(list_t* list){
    assert(list);

    #ifdef DEBUG
    verify(list);
    log(list, "before %s", "linearization");
    #endif /* DEBUG */

    int curCellInd = *head(list);
    for(; (*next(list, curCellInd) != 0); curCellInd = *next(list, curCellInd)){
        $
        continue;

    }
    LPRINTF("listCurCellInd = %d, capacity = %lu\n", curCellInd, list->capacity);

    list->capacity = (size_t) curCellInd + 1 ;  

    LPRINTF("listCurCellInd = %d, capacity = %lu\n", curCellInd, list->capacity);
    listElem_t* temp = (listElem_t*) realloc(list->elem, list->capacity * sizeof(listElem_t));
    assert(temp);

    list->elem = temp;

    #ifdef DEBUG
    verify(list);
    log(list, "after %s", "linearization");
    #endif /* DEBUG */ 

    return PROCESS_OK_LIST;
}

listStatus listOptimize(list_t* list){
    assert(list);

    listLinearize(list);
    listFreeUnusedMem(list);

    return PROCESS_OK_LIST;
}

static void placeNodeRight(list_t* list, 
        int logicalInd, 
        int physicalInd){
    assert(list);
            
    if(*data(list, logicalInd) == LIST_POISON) *freeInd(list) = physicalInd;

    listElem_t temp = list->elem[physicalInd];
$
    list->elem[physicalInd] = list->elem[logicalInd];
$
    list->elem[logicalInd] = temp;
    
    *next(list, *prev(list, logicalInd)) = logicalInd;
    *prev(list, *next(list, logicalInd)) = logicalInd;

}

static listStatus reallocateList(list_t* list){
    assert(list);

    static size_t reallocationCount = 0;

    #ifdef DEBUG
    verify(list);
    log(list, "before %s %d", "reallocation", (listVal_t) reallocationCount);
    #endif /* DEBUG */

    LPRINTF("difference: %lu\n", list->capacity - list->size);

    size_t initStartIndex = list->capacity;

    list->capacity = list->capacity * 2;
    listElem_t* temp = (listElem_t*) realloc(list->elem, list->capacity * sizeof(listElem_t));
    assert(temp);

    list->elem = temp;
    
    listInit(list, initStartIndex);
    
    reallocationCount++;

    #ifdef DEBUG
    verify(list);
    log(list, "after %s %d", "reallocation", (listVal_t) reallocationCount);
    #endif /* DEBUG */

    return PROCESS_OK_LIST;
}

listStatus listCopy(list_t* dest, list_t* src){
    assert(dest);
    assert(src);

    #ifdef DEBUG
    verify(dest);
    verify(src);
    log(dest, "before %s", "copy");
    #endif /* DEBUG */

    int curElem = src->elem->next;

    for(size_t i = 0; i < src->size; i++){
        listInsertToTail(dest, src->elem[curElem].data);

        curElem = src->elem[curElem].next;
    }

    #ifdef DEBUG
    verify(dest);
    log(dest, "after %s", "copy");
    #endif /* DEBUG */

    return PROCESS_OK_LIST;
}