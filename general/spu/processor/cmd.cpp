#include "cmd_spu.h"
#include "cmd_math.h"

#include "../general/debug.h"

#define $ fprintf(stderr, "MEOW in %s:%d\n", __FILE__, __LINE__);

static bool unaryOperation(processor* spu, unaryHandler handler);
static bool binaryOperation(processor* spu, binaryHandler handler);
static bool jumpOperation(processor* spu, binaryHandler handler);

static bool get1Param(processor* spu, cmdParam_t* arg);
static bool get2Params(processor* spu, cmdParam_t* arg1, cmdParam_t* arg2);

bool add(processor* spu){
    return binaryOperation(spu, addMath);
}

bool sub(processor* spu){
    return binaryOperation(spu, subMath);
}

bool mul(processor* spu){
    return binaryOperation(spu, mulMath);
}

bool myDiv(processor* spu){
    return binaryOperation(spu, divMath);
}

bool mySqrt(processor* spu){
    return unaryOperation(spu, sqrtMath);
}

bool myAbs(processor* spu){
    return unaryOperation(spu, absMath);
}

bool lt(processor* spu){
    return binaryOperation(spu, ltMath);
}

bool le(processor* spu){
    return binaryOperation(spu, leMath);
}

bool gt(processor* spu){
    return binaryOperation(spu, gtMath);
}

bool ge(processor* spu){
    return binaryOperation(spu, geMath);
}

bool eq(processor* spu){
    return binaryOperation(spu, eqMath);
}

bool neq(processor* spu){
    return binaryOperation(spu, neMath);
}

bool jl(processor* spu){
    return jumpOperation(spu, ltMath);
}

bool jle(processor* spu){
    return jumpOperation(spu, leMath);
}

bool jg(processor* spu){
    return jumpOperation(spu, gtMath);
}

bool jge(processor* spu){
    return jumpOperation(spu, geMath);
}

bool je(processor* spu){
    return jumpOperation(spu, eqMath);
}

bool jne(processor* spu){
    return jumpOperation(spu, neMath);
}

bool draw(processor* spu){
    $
    ram_t fieldSideSize = 0;

    spuPop(spu, &fieldSideSize);
    printf("fieldSize = %lg\n", fieldSideSize);
    for(int curMemCellInd = 0; curMemCellInd < fieldSideSize * fieldSideSize; curMemCellInd++){
        spuPushM(spu, &curMemCellInd);
        ram_t curMemCellVal = 0;
        spuPop(spu, &curMemCellVal);

        if(((int) curMemCellInd % (int) fieldSideSize) == 0) printf("\n");
        
        if(!areEqualDouble(curMemCellVal, 0)){
            printf(SET_STYLE_BOLD_FONT_GREEN "%lg " RESET, curMemCellVal);
        }
        else{
            printf(SET_STYLE_BOLD_FONT_RED "%lg " RESET, curMemCellVal);
        }
    }

    return true;
}

bool jmp(processor* spu){
    stackData_t pos = 0;
    spuGetArg(spu, &pos);
    spuJump(spu, (int) pos);

    return true;
}

bool push(processor* spu){
    cmdParam_t numPush = 0;

    spuGetArg(spu, &numPush);
    spuPush(spu, (cmdParam_t) numPush);

    return true;
}

bool pushrt(processor* spu){
    cmdParam_t numPush = 0;

    spuGetArg(spu, &numPush);
    spuPushRt(spu, (cmdParam_t) numPush);

    return true;
}

bool get(processor* spu){
    cmdParam_t regNum = 0;

    spuGetArg(spu,  &regNum);

    cmdParam_t numPush = 0;
    spuGet(spu, (size_t) spu->regs[(size_t) regNum], &numPush);

    spuPush(spu, (cmdParam_t) numPush);

    return true;
}

bool set(processor* spu){
    cmdParam_t regNum = 0;
    spuGetArg(spu,  &regNum);

    cmdParam_t numPush = 0;
    spuPop(spu, &numPush);

    spuSet(spu, (size_t) spu->regs[(size_t) regNum], numPush);

    return true;
}

bool pushreg(processor* spu){
    cmdParam_t regNumDouble = 0;

    spuGetArg(spu,  &regNumDouble);

    int regNum = (int) regNumDouble;
    spuPushReg(spu, &regNum);

    return true;
}

bool popreg(processor* spu){
    cmdParam_t regNumDouble = 0;

    spuGetArg(spu,  &regNumDouble);

    int regNum = (int) regNumDouble;
    spuPopReg(spu, &regNum);

    return true;
}

bool pushregrt(processor* spu){
    cmdParam_t regNumDouble = 0;

    spuGetArg(spu,  &regNumDouble);

    int regNum = (int) regNumDouble;
    spuPushRegRt(spu, &regNum);

    return true;
}

bool popregrt(processor* spu){
    cmdParam_t regNumDouble = 0;

    spuGetArg(spu,  &regNumDouble);

    int regNum = (int) regNumDouble;
    spuPopRegRt(spu, &regNum);

    return true;
}

bool pushm(processor* spu){
    cmdParam_t regNumDouble = 0;

    spuGetArg(spu, &regNumDouble);
    
    int regNum = (int) regNumDouble;
    spuPushReg(spu, &regNum);

    cmdParam_t memCellNumDouble = 0;
    spuPop(spu, &memCellNumDouble);

    int memCellNum = (int) memCellNumDouble;
    spuPushM(spu, &memCellNum);

    return true;
}

bool popm(processor* spu){
    cmdParam_t regNumDouble = 0;

    spuGetArg(spu, &regNumDouble);

    int regNum = (int) regNumDouble;
    spuPushReg(spu, &regNum);

    cmdParam_t memCellNumDouble = 0;
    spuPop(spu, &memCellNumDouble);

    int memCellNum = (int) memCellNumDouble;
    spuPopM(spu, &memCellNum);

    return true;
}

bool pushmAddr(processor* spu){
    cmdParam_t memCellNumDouble = 0;
    spuGetArg(spu, &memCellNumDouble);

    int memCellNum = (int) memCellNumDouble;
    spuPushM(spu, &memCellNum);

    return true;
}

bool popmAddr(processor* spu){
    cmdParam_t memCellNumDouble = 0;
    spuGetArg(spu, &memCellNumDouble);

    int memCellNum = (int) memCellNumDouble;
    spuPopM(spu, &memCellNum);

    return true;
}

bool in(processor* spu){
    printf("Введите число:\n");

    cmdParam_t elem = 0;
    scanf("%lg", &elem);
    spuPush(spu, elem);

    return true;
}

bool out(processor* spu){
    printf("Верхний элемент стека:\n");

    cmdParam_t curElem = 0;
    spuPop(spu, &curElem);
    printf("%lg ", curElem);
    
    printf("\n");
    return true;
}

bool hlt(processor* spu){
    return spuHlt(spu);
}

bool callFunc(processor* spu){
    return spuCall(spu);
}

bool retFunc(processor* spu){
    return spuRet(spu);
}

static bool unaryOperation(processor* spu, unaryHandler handler){
    cmdParam_t arg = 0;
    get1Param(spu, &arg);

    cmdParam_t result = 0;
    bool check = handler(arg, &result);

    spuPush(spu, result);

    return check;
}

static bool binaryOperation(processor* spu, binaryHandler handler){
    cmdParam_t arg1 = 0, arg2 = 0;
    get2Params(spu, &arg1, &arg2);

    cmdParam_t result = 0;
    bool check = handler(arg1, arg2, &result);

    spuPush(spu, result);

    return check;
}

static bool jumpOperation(processor* spu, binaryHandler handler){
    cmdParam_t arg1 = 0, arg2 = 0;
    get2Params(spu, &arg1, &arg2);

    cmdParam_t result = 0;
    bool check = handler(arg1, arg2, &result);

    if(!areEqualDouble(result, 0)){
        jmp(spu);
    }

    return check;
}

static bool get1Param(processor* spu, cmdParam_t* arg){
    assert(spu);
    
    spuPop(spu, arg);

    return true;
}

static bool get2Params(processor* spu, cmdParam_t* arg1, cmdParam_t* arg2){
    assert(spu);
    
    spuPop(spu, arg2);
    spuPop(spu, arg1);

    return true;
}
