// #include "translator.h"
// #include "general/file.h"

// const char* textCommandsFileName   = "quadratic.txt"; 
// const char* opcodeFileName         = "quadratic.asm";

// int main(int argc, char* argv[]){
//     const char* inputFileName  = textCommandsFileName;
//     const char* outputFileName = opcodeFileName;
//     if(argc == 2){
//         inputFileName = argv[1];
//     }

//     if(argc == 3){
//         outputFileName = argv[2];
//     }

//     data buf = {};
//     if((parseStringsFile(&buf, inputFileName)) == EXIT_FAILURE) return false;

//     translator_t translator;
//     translatorCtor(&translator);

//     loadTextCommands(&translator, {buf.strings, buf.nStrings});

//     assemble(&translator);

//     writeOpcode(&translator.opcode, outputFileName);

//     translatorDtor(&translator);
//     free(buf.buffer); 
//     free(buf.strings);
// }



