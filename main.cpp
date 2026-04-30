// #include "frontEnd.h"

// #include <unistd.h>
// #include <assert.h>
// #include <stdio.h>

// #include <string.h>

// const size_t MAX_FILE_NAME_SIZE = 64;

// void cmdFlagsHandle(int argc, char* argv[], char* sourceFileName, char* destFileName, bool* debugSymbols);

// int main(int argc, char* argv[]){
//     char sourceFileName       [MAX_FILE_NAME_SIZE] = "examples/factorial.tale";
//     char destFrontendFileName [MAX_FILE_NAME_SIZE] = "syntaxTree.txt";
//     bool debugSymbols                              = false;

//     cmdFlagsHandle(argc, argv, sourceFileName, destFrontendFileName, &debugSymbols);

//     compilerFrontend(sourceFileName, destFrontendFileName, debugSymbols);
// }

// void cmdFlagsHandle(int argc, char* argv[], char* sourceFileName, char* destFileName, bool* debugSymbols){
//     assert(argv);
//     assert(sourceFileName);
//     assert(destFileName);

//     int opt;
//     while((opt = getopt(argc, argv, "i:o:g")) != -1){
//         switch(opt){
//         case 'i':
//             strncpy(sourceFileName, optarg, MAX_FILE_NAME_SIZE);
//             break;
        
//         case 'o':
//             strncpy(destFileName,   optarg, MAX_FILE_NAME_SIZE);
//             break;
//         case 'g':
//             *debugSymbols = true;
//             break;

//         default:
//             break;
//         }
//     }
// }