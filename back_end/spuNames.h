#ifndef SPU_NAMES_H
#define SPU_NAMES_H

#include "../general/nametable.h"

struct spuName_t{
    name_t*      description;
    int          addr;
};

struct spuNameTable_t{
    spuName_t* data;
    size_t     size;
    size_t     capacity;
};

spuNameTable_t* spuNameTableCtor(spuNameTable_t* spuNameTable);

#endif  /* SPU_NAMES_H */