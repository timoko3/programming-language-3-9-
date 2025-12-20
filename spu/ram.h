#ifndef RAM_H
#define RAM_H

#include <stddef.h>

typedef double ram_t;

const size_t RAM_CAPACITY = 1000;

ram_t* ramCtor();
bool ramDtor(ram_t* ram);

#endif /* RAM_H */