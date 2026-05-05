#include "ram.h"
#include "general/poison.h"

#include <stdlib.h>
#include <assert.h>

ram_t* ramCtor(){

    ram_t* ram = (ram_t*) calloc(RAM_CAPACITY, sizeof(ram_t));

    return ram;
}

bool ramDtor(ram_t* ram){
    assert(ram);

    poisonMemory(ram, sizeof(ram));
    free(ram);

    return true;
}