#include "ram.h"
#include "processor/processor.h"
#include "translator/translator.h"
#include "general/spu/generalSpu/file.h"
#include "general/poison.h"
#include "general/spu/generalSpu/stack/stack.h"


const char* const FLAG_INPUT_FILE  = "-i";
const char* const FLAG_OUTPUT_FILE = "-o";

const char* textCommandsFileName   = "program.spu"; 

int main(int argc, char* argv[]){
    const char* filename = textCommandsFileName;
    if(argc == 2){
        filename = argv[1];
    }

    data buf = {};
    if((parseStringsFile(&buf, filename)) == EXIT_FAILURE) return false;

    translator_t translator;
    translatorCtor(&translator);

    loadTextCommands(&translator, {buf.strings, buf.nStrings});

    assemble(&translator);

    buffer_t* opcode = &translator.opcode;

    translatorDtor(&translator);
    free(buf.buffer); 
    free(buf.strings); 

    ram_t* ram = ramCtor();
    processor spu1; 
    processorCtor(&spu1, ram);

    loadOpcode(&spu1, *opcode);

    runProcessor(&spu1);
    
    // processorDump(&spu1);

    ramDtor(ram);
    processorDtor(&spu1);

    poisonMemory(spu1.opcode.ptr, spu1.opcode.size);
    free(spu1.opcode.ptr);
    spu1.opcode.ptr = NULL;
    poisonMemory(&spu1.opcode.size, sizeof(spu1.opcode.size));
}