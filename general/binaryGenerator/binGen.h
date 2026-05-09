#ifndef BIN_GEN_H
#define BIN_GEN_H

#include <stdio.h>
#include <stdint.h>

struct binBuffer_t{
    uint8_t* data;
    size_t size;
    size_t capacity;
};

void writeU8Le(FILE* stream, uint8_t x);
void writeU32Le(FILE* stream, uint32_t x);

#endif /* BIN_GEN_H */