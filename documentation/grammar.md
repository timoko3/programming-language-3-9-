program = start_program {func } end_program

start_program = "Жили-были" ;
end_program   = "и я там был мед пиво пил по усам текло, а в рот не попало" ;

func =
    func_header
    block
    func_end ;

func_header =
    "В некотором царстве, в некотором государстве"
    NAME
    "(" [ args ] ")" ;

func_end =
    "Скоро сказка сказывается, да не скоро дело делается" ;

block =
    { statement } ;

statement =
      var_decl
    | assignment
    | if_stmt
    | while_stmt
    | return_stmt
    | call_stmt ;

var_decl =
    "Добрый молодец"
    NAME
    "с силушкой богатырской"
    expression
    "так и было" ;

assignment =
    "сила" NAME
    "с силушкой богатырской"
    expression
    "так и было" ;

call_stmt =
    "Сивка-бурка вещая каурка стань передо мной как лист перед травой"
    NAME "(" [ args ] ")"
    "так и было" ;

return_stmt =
    "воротить"
    expression
    "так и было" ;

if_stmt =
    "пойди туда — не знаю куда, принеси то — не знаю что"
    condition
    block
    if_end ;

if_end =
    "так и было" ;

while_end =
    "Скоро сказка сказывается, да не скоро дело делается" ;

while_stmt =
    "Покуда"
    "(" condition ")"
    block
    while_end
    "так и было" ;

expression = add_expr ;

add_expr =
    mul_expr { ("убавить" | "собрать воедино") mul_expr } ;

mul_expr =
    term { ("преумножить" | "поделить") term } ;

term =
      NUMBER
    | NAME
    | "(" expression ")" ;

condition =
    expression
    comparator
    expression     
    | "(" condition ")" ;

comparator =
      "по плечу"
    | "сильнее"
    | "слабее" ;

args =
    expression { "," expression } ;

NAME   = letter { letter | digit } ;
NUMBER = digit { digit } ;
