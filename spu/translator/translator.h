#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "general/file.h"
#include "general/strFunc.h"
#include "cmd.h"

#define DEBUG_TRANSLATOR 0

const int    ASSEMBLE_FAILURE       = -1;

const size_t COMMAND_NAME_MAX_SIZE  = 50;
const size_t REGISTER_NAME_MAX_SIZE = 5;
const size_t LABEL_NAME_MAX_SIZE    = 20;

const size_t N_LABELS     = 20;
const int    LABEL_POISON = 46878756;

typedef int value;
struct label_t{
    const char* name;
    unsigned long hash;
    value addr;
};

struct state_t{
    param_t par;
    size_t  StringInd;
    size_t  commandInd;
    char*   cmdName;
    char*   labelName;
    size_t  labelInd;
};

struct translator_t{ 
    command_t* cmds; 
    strings_t  input_buffer; 
    buffer_t   opcode;
    label_t*   labels;
    state_t    curState;
};

bool translatorCtor(translator_t* translator);
bool loadTextCommands(translator_t* translator, strings_t textCommands);
bool assemble(translator_t* translator);
bool translatorDtor(translator_t* translator);

#if DEBUG_TRANSLATOR
void printByteCodeBuffer(int* buffer, size_t curByteBufferSize);
#endif /* DEBUG */

#endif /* TRANSLATOR_H */
