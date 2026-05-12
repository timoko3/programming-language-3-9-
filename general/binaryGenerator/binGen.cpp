#include "binGen.h"

#include <string.h>
#include <assert.h>
#include <malloc.h>

binBuffer_t* binBufferCtor(size_t startCapacity){

    binBuffer_t* buffer = (binBuffer_t*) calloc(1, sizeof(binBuffer_t));
    assert(buffer);

    BIN_BUFFER_DATA(buffer)     = (uint8_t*) calloc(startCapacity, sizeof(uint8_t));
    BIN_BUFFER_CAPACITY(buffer) = startCapacity;

    return buffer;
}

void binBufferDtor(binBuffer_t* buffer){
    free(BIN_BUFFER_DATA(buffer));
    free(buffer);
}

void writeU32LeBuf(binBuffer_t* buffer, uint32_t x){
    uint8_t byte[4];

    byte[0] = x;
    byte[1] = x >> 8;
    byte[2] = x >> 16;
    byte[3] = x >> 24;

    for(size_t i = 0; i < 4; i++){
        writeU8Buf(buffer, byte[i]);
    }
}

void writeU8Buf(binBuffer_t* buffer, uint8_t x){
    assert(buffer);

    BIN_BUFFER_DATA(buffer)[BIN_BUFFER_SIZE(buffer)] = x;
    BIN_BUFFER_SIZE(buffer)++;

    if(BIN_BUFFER_SIZE(buffer) >= BIN_BUFFER_CAPACITY(buffer)) binBufferRealloc(buffer);
}

void binBufferRealloc(binBuffer_t* buffer){
    assert(buffer);

    BIN_BUFFER_CAPACITY(buffer) *= 2;

    uint8_t* newBufferData = (uint8_t*) realloc(BIN_BUFFER_DATA(buffer), BIN_BUFFER_CAPACITY(buffer) * sizeof(uint8_t));
    assert(newBufferData);
    
    BIN_BUFFER_DATA(buffer) = newBufferData;
}

void binBufPatch(binBuffer_t* buffer, size_t position, uint32_t patchValue){
    assert(buffer);

    assert(position + sizeof(uint32_t) <= BIN_BUFFER_SIZE(buffer));

    uint8_t byte[4];

    byte[0] = patchValue;
    byte[1] = patchValue >> 8;
    byte[2] = patchValue >> 16;
    byte[3] = patchValue >> 24;

    for(size_t i = 0; i < 4; i++){
        BIN_BUFFER_DATA(buffer)[position + i] = byte[i];
    }
}