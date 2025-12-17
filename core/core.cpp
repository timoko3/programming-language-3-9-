#include "core.h"
#include "DSL.h"

#include "../general/strFunc.h"

#include <assert.h>



ASTnodeData_t ASTdata[]{
    {"собрать воедино",            ADD,             "+" },
    {"убавить",                    SUB,             "-" },
    {"преумножить",                MUL,             "*" },
    {"повелеваю поделить",         DIVIDE,          "/" },

    {"с силушкой богатырской",     ASSIGN,          "=" },
    {"так и было",                 END_STATEMENT,   ";" },
    {"и я там был мед пиво "
        "пил по усам текло, " 
        "а в рот не попало",       END_PROGRAM,     ";" },
    {"Покуда",                     WHILE,          "while"},
    {"ежели случилось, что",       IF,             "if"   },
    {",",                          COMMA,          "," },
    {"двое из ларца "
     "одинаковых с лица с",        EQUAL,          "=="},
    {"ни чета",                    NOT_EQUAL,      "!="},         // не одного поля ягода
    {"и в подметки " 
     "не годится",                 LT,             "<"  },
    {"по плечу",                   LE,             "<=" },
    {"не по плечу",                GT,             ">"  },
    {"больше али равна",           GE,             ">=" },
    {"воротить",                   RETURN,         "ret"},
    {"жар-птица выжгла "
     "на земле русской",           OUT,            "out"},
    {"пойди туда — не знаю куда, "
     "принеси то — не знаю что",   IN,             "in" },
    {"вот и сказочке конец",       HLT,            "hlt"},
    {"царь-батюшка",               MAIN,           "main"},
    {"Жили-были",                  START,           "no"},
    {"Добрый молодец",             INIT_VARIABLE,   "no"},
    {"сила",                       CALL_VARIABLE,   "no"},
    {"В некотором царстве, " 
     "в некотором " 
     "государстве",                INIT_FUNC,      "no"},
    {"А коли не "
     "случилось, то",              ELSE,           "no"},
    {"А ежели случилось, что",     ELSE_IF,        "no"},
    {"(",                          BRACKL,         "no"},
    {")",                          BRACKR,         "no"},
    {"Скоро сказка "
     "сказывается, да не "
     "скоро дело делается",        END_BLOCK,      "no" },
    {"Сивка-бурка вещая каурка "
     "стань передо мной "
      "как лист перед травой",     CALL_FUNC,      "no" },
    {"побороть корень зла",        SQRT,           "sqrt"}
};

const size_t AST_DATA_COUNT = sizeof(ASTdata) / sizeof(ASTdata[0]);

ASTnodeData_t* findAstData(char* referenceStr){
    assert(referenceStr);

    for(size_t curASTnode = 0; curASTnode < AST_DATA_COUNT; curASTnode++){
        if(isEqualStrings(ASTdata[curASTnode].writeFile, referenceStr)){
            return &ASTdata[curASTnode];
        }   
    }

    return NULL;
}