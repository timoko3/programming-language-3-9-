#include "general_list.h"

#include <assert.h>

const void* LIST_POISON = ((void*)0xDEADBEEF);

listVal_t* data(list_t* list, int index){
    assert(list);

    return &list->elem[index].data;
}

int* next(list_t* list, int index){
    assert(list);
    
    return &list->elem[index].next;
}

int* prev(list_t* list, int index){
    assert(list);
    
    return &list->elem[index].prev;
}

int* head(list_t* list){
    assert(list);

    return &list->elem->next;
}

int* tail(list_t* list){
    assert(list);

    return &list->elem->prev;
}

int* freeInd(list_t* list){
    assert(list);

    return &list->freeCellInd;
}