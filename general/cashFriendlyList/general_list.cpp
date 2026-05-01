#include "general_list.h"

#include <assert.h>

void* LIST_POISON = NULL;

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