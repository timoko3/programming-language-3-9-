#include "backEnd.h"

#include <unistd.h>
#include <assert.h>
#include <stdio.h>

#include <string.h>

const size_t MAX_FILE_NAME_SIZE = 64;

void cmdFlagsHandle(int argc, char* argv[], char* sourceFileName, char* destFileName, backend_t* backendType);

int main(int argc, char* argv[]){
    char sourceFileName       [MAX_FILE_NAME_SIZE] = "syntaxTree.txt";
    char destBackendFileName  [MAX_FILE_NAME_SIZE] = "circleTest.s";
    backend_t backendType = NASMBE;

    cmdFlagsHandle(argc, argv, sourceFileName, destBackendFileName, &backendType);

    compilerBackend(sourceFileName, destBackendFileName, backendType);
}

void cmdFlagsHandle(int argc, char* argv[], char* sourceFileName, char* destFileName, backend_t* backendType){
    assert(argv);
    assert(sourceFileName);
    assert(destFileName);
    assert(backendType);

    int opt;
    while((opt = getopt(argc, argv, "i:o:nsx")) != -1){
        switch(opt){
        case 'i':
            strncpy(sourceFileName, optarg, MAX_FILE_NAME_SIZE);
            break;
        
        case 'o':
            strncpy(destFileName,   optarg, MAX_FILE_NAME_SIZE);
            break;
        case 'n':
            *backendType = NASMBE;
            break;
        case 's':
            *backendType = SPUBE;
            break;
        case 'x':
            *backendType = X86BE;
            break;
        default:
            printf("-%c - флаг недопустим\n", opt);
            break;
        }
    }
}