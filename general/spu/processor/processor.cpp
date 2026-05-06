#include "processor.h"
#include "../cmd.h"
#include "../general/poison.h"
#include "../general/file.h" 

#include <math.h>

const double STACK_FRAME_BASE = 25;
const size_t STACK_POINTER_REG_IND = 8;

#define $ fprintf(stderr, "MEOW in %s:%d\n", __FILE__, __LINE__);

static void simplePrintStack(stack* stk);
static void printByteCode(int* byteCode, size_t byteCodeSize, size_t pc);
static void printRegs(regParam_t* regs);
static void printRam(ram_t* ram);

processorStatus processorCtor(processor* spu, ram_t* ram){
    assert(spu);
    assert(ram);
    
    stackCtor(&spu->stk, 10);
    stackCtor(&spu->funcRetAddr, 10);
    spu->pc = 0;
    spu->isWork = true;
    spu->regs = (regParam_t*) calloc(N_REGISTERS, sizeof(regParam_t));
    assert(spu->regs);

    spu->regs[0] = STACK_FRAME_BASE;
    spu->regs[1] = STACK_FRAME_BASE;

    spu->RAM  = ram;

    return SPU_PROCESS_OK;
}

processorStatus processorDtor(processor* spu){
    assert(spu);

    stackDtor(&spu->stk);
    stackDtor(&spu->funcRetAddr);

    poisonMemory(spu->regs, sizeof(spu->regs));
    free(spu->regs);
    
    poisonMemory(&spu->pc, sizeof(spu->pc));

    spu->isWork = false;

    return SPU_PROCESS_OK;
}

bool loadOpcode(processor* spu, buffer_t opcode){
    assert(spu);

    spu->opcode = opcode;

    return true;
}

bool runProcessor(processor* spu){
    assert(spu);

    bool result = false;
    while(spu->pc < (spu->opcode.size)){
        if(executeCommand(spu) == false) return false;
    }

    return result;
}

bool executeCommand(processor* spu){
    assert(spu);
    
    processorDump(spu);
    
    if(spu->isWork == false){
        return false;
    }
    
    for(size_t curCommandInd = 0; curCommandInd <  sizeof(commands) / sizeof(command_t); curCommandInd++){
        if(commands[curCommandInd].code == spu->opcode.ptr[spu->pc]){
            command_t curCmd = commands[curCommandInd];
            assert(curCmd.handler);
            size_t old_ps = spu->pc;
            ///
            curCmd.handler(spu);
            // return_value == if command don't change
            if (spu->pc == old_ps) {
                (spu->pc)++;
                spu->pc += (size_t) curCmd.nArgs;
            }
            
            break;
        } 
    } 
    
    return true;
}

bool spuPush(processor* spu, stackData_t data){
    assert(spu);

    stackPush(&(spu->stk), data); 

    return true;
}

bool spuPop(processor* spu, stackData_t* data){
    assert(spu);

    if(stackPop(&(spu->stk), data) == EMPTY_STACK){
        return false;
    }

    return true;
}

bool spuGet(processor* spu, size_t elemIndex, stackData_t* data){
    assert(spu);

    if(stackGet(&(spu->funcRetAddr), elemIndex, data) == EMPTY_STACK){
        return false;
    }

    return true;
}

bool spuSet(processor* spu, size_t elemIndex, stackData_t data){
    assert(spu);

    if(elemIndex >= (size_t) spu->regs[STACK_POINTER_REG_IND]){
        (spu->regs[STACK_POINTER_REG_IND])++;
    }

    if(stackSet(&(spu->funcRetAddr), elemIndex, data) == EMPTY_STACK){
        return false;
    }
    
    return true;
}

bool spuPushRt(processor* spu, stackData_t data){
    assert(spu);

    stackPush(&(spu->funcRetAddr), data); 

    (spu->regs[STACK_POINTER_REG_IND])++;

    return true;
}

bool spuPopRt(processor* spu, stackData_t* data){
    assert(spu);

    if(stackPop(&(spu->funcRetAddr), data) == EMPTY_STACK){
        return false;
    }

    (spu->regs[STACK_POINTER_REG_IND])--;

    return true;
}


bool spuJump(processor* spu, cmdParam_t pos){
    assert(spu);

    spu->pc = (size_t) pos; 

    return true;
}

bool spuGetArg(processor* spu, stackData_t* index){ 
    assert(spu);

    *index = (double) spu->opcode.ptr[spu->pc + 1];

    return true;
}  

bool spuPushReg(processor* spu, int* regNumber){
    assert(spu);
    assert(regNumber);

    regParam_t curRegValue = spu->regs[*regNumber];
    stackPush(&spu->stk, curRegValue);

    return true;
}

bool spuPopReg(processor* spu, int* regNumber){
    assert(spu);
    assert(regNumber);

    stackPop(&spu->stk, &spu->regs[*regNumber]);

    return true;
}

bool spuPushRegRt(processor* spu, int* regNumber){
    assert(spu);
    assert(regNumber);

    regParam_t curRegValue = spu->regs[*regNumber];
    stackPush(&spu->funcRetAddr, curRegValue);

    (spu->regs[STACK_POINTER_REG_IND])++;

    return true;
}

bool spuPopRegRt(processor* spu, int* regNumber){
    assert(spu);
    assert(regNumber);

    stackPop(&spu->funcRetAddr, &spu->regs[*regNumber]);

    (spu->regs[STACK_POINTER_REG_IND])--;

    return true;
}

bool spuPushM(processor* spu, int* memCellNum){
    assert(spu);
    assert(memCellNum);

    stackData_t data = 0;
    data = spu->RAM[*memCellNum];

    stackPush(&spu->stk, data);

    return true;
}

bool spuPopM(processor* spu, int* memCellNum){
    assert(spu);
    assert(memCellNum);

    stackData_t data = 0;
    stackPop(&spu->stk, &data);

    spu->RAM[*memCellNum] = data;

    return true;
}

bool spuHlt(processor* spu){
    assert(spu);

    spu->isWork = false;

    return true;
}

bool spuCall(processor* spu){
    assert(spu);

    stackPush(&(spu->funcRetAddr), (cmdParam_t) spu->pc + 2);
    
    cmdParam_t pos = 0;
    spuGetArg(spu, &pos);
    
    spuJump(spu, pos);
    
    (spu->regs[STACK_POINTER_REG_IND])++;
    return true;
}

bool spuRet(processor* spu){
    assert(spu);

    regParam_t retAddr = 0;
    stackPop(&(spu->funcRetAddr), &retAddr);
    spu->pc = (size_t) retAddr;

    (spu->regs[STACK_POINTER_REG_IND])--;
    return true;
}

void processorDump(processor* spu){
    assert(spu);

    printf("\nSPU dump:");

    printf("\n\tpc:     ");
    printf(SET_STYLE_BOLD_FONT_PURPLE "%lu" RESET, spu->pc);

    printf("\n\tstack: ");
    simplePrintStack(&spu->stk);
    printf("\n");

    printf("\n\tretAddrStack: ");
    simplePrintStack(&spu->funcRetAddr);
    printf("\n");

    printf("\tCode:");
    printByteCode(spu->opcode.ptr, spu->opcode.size, spu->pc);
    printf("\n");

    printf("\tRegs:");
    printRegs(spu->regs);
    printf("\n");

    printf("\tRAM:");
    printRam(spu->RAM);

    getchar();
    printf("\n\n");
}

static void simplePrintStack(stack* stk){
    assert(stk);
    
    for(size_t curStackElem = 0; curStackElem < stk->capacity; curStackElem++){
        if((curStackElem % 4) == 0) printf("\n\t\t");

        if(fabs(stk->data[curStackElem] - POISON_NUMBER) < 1e-9){
            printf(SET_STYLE_ITALICS_FONT_TURQUOISE "POISON " RESET);
        }
        else {
            printf("%lg ", stk->data[curStackElem]);
        }
    }
}

static void printByteCode(int* byteCode, size_t byteCodeSize, size_t pc){
    assert(byteCode);

    printf("\n\t\t " SET_STYLE_BOLD_FONT_RED "%08x %08x %08x %08x" RESET, (unsigned int) 0,
                                                                          (unsigned int) 1,
                                                                          (unsigned int) 2,
                                                                          (unsigned int) 3);
    for(size_t curByte = 0; curByte < byteCodeSize; curByte++){
        if((curByte % 1) == 0) printf(" ");
        if(((curByte % 4) == 0)) printf("\n\t" SET_STYLE_BOLD_FONT_RED "%08x " RESET, (unsigned int) curByte);
        if((curByte == pc)){
            printf(SET_STYLE_BOLD_FONT_PURPLE "%08x" RESET, (unsigned int) *(byteCode + curByte));
        }
        else{
            printf(SET_STYLE_BOLD_FONT_YELLOW "%08x" RESET, (unsigned int) *(byteCode + curByte));
        }
    }
}

static void printRegs(regParam_t* regs){
    assert(regs);
    
    printf("\n\t\t");
    printf("AX  BX  CX  DX  EX  FX  GX  HX  IX  JX\n");
    printf("\n\t\t");
    for(size_t curReg = 0; curReg < N_REGISTERS; curReg++){
        printf("%-3lg ", regs[curReg]);
    }
}

static void printRam(ram_t* ram){
    assert(ram);

    for(size_t curMemCell = 0; curMemCell < RAM_CAPACITY; curMemCell++){
        if(((curMemCell % 32) == 0)) printf("\n\t\t");
        printf(SET_STYLE_ITALICS_FONT_GREEN "%lg " RESET, ram[curMemCell]);
    }
}