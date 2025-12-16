#ifndef LABELS_H
#define LABELS_H

#include <stddef.h>

struct label_t{
    char* name;
    int pc;
};

struct labelsTable_t{
    label_t* data;
    size_t   size;
    size_t   capacity;
};

labelsTable_t* labelsTableCtor(labelsTable_t* labelsTable);

#endif /* LABELS_H */