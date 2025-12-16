#ifndef LABELS_H
#define LABELS_H

#include <stddef.h>

struct label_t{
    char* name;
    int   number;
};

struct labelsTable_t{
    label_t* data;
    size_t   size;
    size_t   capacity;
};

labelsTable_t* labelsTableCtor(labelsTable_t* labelsTable);
int getLabelName(labelsTable_t* labelsTable, const char* name);
labelsTable_t* spuNameTableDtor(labelsTable_t* labelsTable);

#endif /* LABELS_H */