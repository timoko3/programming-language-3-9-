#include "mathHandlers.h"

#define DEBUG

#include "debug.h"

#include <math.h>
#include <assert.h>
#include <stdio.h>

double addH(double* params){
    assert(params);

    return params[0] + params[1];
}

double subH(double* params){
    assert(params);

    return params[0] - params[1];
}

double mulH(double* params){
    assert(params);

    return params[0] * params[1];
} 

double divH(double* params){
    assert(params);

    if((params[1] - 0) < 0.0001){
        printf("Деление на 0 невозможно!\n");
    }

    return params[0] / params[1];
} 

bool isEqualDouble(double num1, double num2){
    if(fabs(num2 - num1) < 0.00001){
        // LPRINTF("%lf и %lf равны");
        return true;
    }

    return false; 
}