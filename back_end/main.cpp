#include "codeGeneration.h"

int main(){
    tree_t syntaxTree;
    treeCtor(&syntaxTree);

    genAsmCode(&syntaxTree);

    treeDtor(&syntaxTree);
}