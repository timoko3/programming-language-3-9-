#ifndef BIN_GEN_H
#define BIN_GEN_H

#include <stdio.h>
#include <stdint.h>

#define BIN_BUFFER_SIZE(buffer)     buffer->size
#define BIN_BUFFER_CAPACITY(buffer) buffer->capacity
#define BIN_BUFFER_DATA(buffer)     buffer->data

struct binBuffer_t{
    uint8_t* data;
    size_t size;
    size_t capacity;
};

binBuffer_t* binBufferCtor(size_t startCapacity);
void binBufferDtor(binBuffer_t* buffer);

void writeU8Le(FILE* stream, uint8_t x);
void writeU32Le(FILE* stream, uint32_t x);

void writeU8Buf(binBuffer_t* buffer, uint8_t x);
void binBufferRealloc(binBuffer_t* buffer);

#endif /* BIN_GEN_H */