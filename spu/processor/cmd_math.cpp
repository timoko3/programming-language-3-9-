#include "cmd_math.h"
#include <stdio.h>
#include <limits.h>
#include <math.h>

bool addMath(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result){
    *result = param1 + param2;
    return true;
}

bool subMath(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result){   
    *result = param1 - param2;
    return true;
}

bool mulMath(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result){  
    *result = param1 * param2;
    return true;
}

bool divMath(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result){  
    if(param2 == 0){
        printf("Деление на 0 невозможно\n");
        return false;
    }
    
    *result = param1 / param2;

    return true;
}

bool sqrtMath(cmdParam_t param, cmdParam_t* result){
    if(param < 0){
        printf("Взятие корня из числа меньше 0 невозможно\n");
        return false;
    }
    
    *result = (cmdParam_t) sqrt(param);

    return true;
}

bool absMath(cmdParam_t param, cmdParam_t* result){
    *result = (param >= 0) ? param : (-1) * param;
    return true;
}

bool ltMath(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result){
    *result = (param1 < param2);
    return true;
}

bool leMath(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result){
    *result = (param1 <= param2);
    return true;
}

bool gtMath(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result){
    *result = (param1 > param2);
    return true;
}

bool geMath(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result){
    *result = (param1 >= param2);
    return true;
}

bool eqMath(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result){
    *result = (param1 == param2);
    return true;
}

bool neMath(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result){
    *result = (param1 != param2);
    return true;
}