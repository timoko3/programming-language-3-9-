#ifndef CMD_MATH_H
#define CMD_MATH_H

typedef double cmdParam_t;

bool addMath(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
bool subMath(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
bool mulMath(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
bool divMath(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
bool sqrtMath(cmdParam_t param,  cmdParam_t* result);
bool absMath(cmdParam_t param,  cmdParam_t* result);

bool ltMath(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
bool leMath(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
bool gtMath(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
bool geMath(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
bool eqMath(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
bool neMath(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);

bool areEqualDouble(cmdParam_t a, cmdParam_t b);

#endif /* CMD_MATH_H */