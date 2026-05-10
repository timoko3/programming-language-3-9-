#ifndef ELF_GENERATE_H
#define ELF_GENERATE_H

#include "general/binaryGenerator/binGen.h"

#include <stddef.h>

struct codeGenContext;

struct elfGenerationContext_t{
    size_t elfHeaderSize;
    size_t programHeaderSize;
    size_t codeSize;
    size_t sectionTableSize;
    size_t tableNamesTs;
    binBuffer_t* codeBuffer;
};

void genPrologueX86Elf(codeGenContext* context);

#endif /* ELF_GENERATE_H */