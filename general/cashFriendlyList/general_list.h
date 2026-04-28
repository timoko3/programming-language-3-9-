#ifndef GENERAL_LIST_H
#define GENERAL_LIST_H

#include <limits.h>
#include <stddef.h>
#include <stdarg.h>

#define log(list1, text, ...) htmlLog(list1, __FILE__, __FUNCTION__, __LINE__, text, ##__VA_ARGS__)

typedef char* listVal_t;

const listVal_t LIST_POISON = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

enum listStatus{
    PROCESS_OK_LIST,
    NULL_POINTER_LIST,
    CAPACITY_EXCEEDS_LIMIT_LIST,
    BAD_MEMORY_ALLOCATION_LIST,
    CAPACITY_IS_ZERO_LIST,
    SIZE_EXCEEDS_CAPACITY_LIST,
    NON_VALID_INDEXES_LIST,
    LIST_NOT_CONNECTED
};

struct listStatusDescription{
    listStatus  type;
    const char* text;
};

static struct listStatusDescription listStatuses[]{ 
    {PROCESS_OK_LIST,                    "Все хорошо\n"},
    {CAPACITY_EXCEEDS_LIMIT_LIST,        "Значение capacity превышает максимально возможное\n"}, 
    {NULL_POINTER_LIST,                  "Указатели не должны быть нулевыми\n"},
    {CAPACITY_IS_ZERO_LIST,              "Capacity равно 0\n"}, 
    {BAD_MEMORY_ALLOCATION_LIST,         "Некорректное выделение памяти\n"},
    {SIZE_EXCEEDS_CAPACITY_LIST,         "Размер списка превышает объем выделяемой памяти\n"},
    {NON_VALID_INDEXES_LIST,             "prev или next имеют значение не принадлежащее списку\n"},
    {LIST_NOT_CONNECTED,            "Список не соединен или соединен не верно\n"}
};

struct listElem_t{
    listVal_t data;
    int next;
    int prev;
};

struct list_t{
    listElem_t*            elem;
    int              freeCellInd;
    size_t                 size;
    size_t                 capacity;
    listStatusDescription  status;
};

listVal_t* data(list_t* list, int index);
int* next(list_t* list, int index);
int* prev(list_t* list, int index);
int* head(list_t* list);
int* tail(list_t* list);
int* freeInd(list_t* list);

#endif /* GENERAL_LIST_H */