/*
 * IFJ2022/project/generator.c
 *
 * @brief Target language generation
 *
 * @author Ekaterina Krupenko <xkrupe00@vutbr.cz>
 */

#include "generator.h"

struct count tmp;

void generator_header()
{
    printf(".IFJcode22\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    tmp.ifCount = 0;
    tmp.elseCount = 0;
    tmp.whileCount = 0;
    tmp.varCount = 0;
    tmp.argCount = -1;
}

char *generator_get_type(symt_datatype_t type)
{
    switch (type)
    {
    case INTEGER_DT:
    case INTEGER_N_DT:
        return "int";
        break;
    case FLOAT_DT:
    case FLOAT_N_DT:
        return "float";
        break;
    case STRING_DT:
    case STRING_N_DT:
        return "string";
        break;
    case NIL_DT:
        return "nil";
    default:
        return "";
        break;
    }
}

bool generator_check_var(ht_item_t *var)
{
    for (int j = 0; j < 101; j++)
    {
        if (tmp.varsId[j] == NULL)
            continue;
        else
        {
            if (!strcmp(var->key, tmp.varsId[j])) // search variable among existing variables in array by name
            {
                tmp.varsId[tmp.varCount] = (char *)malloc((strlen(var->key)) * sizeof(char));

                if (!tmp.varsId[tmp.varCount])
                    return NULL;

                strcpy(tmp.varsId[tmp.varCount], var->key);

                tmp.varCount++;

                return false;
            }
        }
    }

    tmp.varsId[tmp.varCount] = (char *)malloc((strlen(var->key)) * sizeof(char)); // allock memory for new parameter in array

    if (!tmp.varsId[tmp.varCount])
        return NULL;

    for (int i = 0; i < strlen(var->key); i++)
        tmp.varsId[tmp.varCount][i] = var->key[i];

    tmp.varCount++;

    return true;
}

void generator_get_new_var(ht_item_t *var)
{
    printf("DEFVAR LF@$%s\n", var->key);

    if (!strcmp("int", generator_get_type(var->data.var->type)))                // if type of variable is int
        printf("MOVE LF@$%s int@%d\n", var->key, var->data.var->attr->integer); // generate new var with int type

    else if (!strcmp("float", generator_get_type(var->data.var->type)))
        printf("MOVE LF@$%s float@%a\n", var->key, var->data.var->attr->decimal);

    else if (!strcmp("string", generator_get_type(var->data.var->type)))
        printf("MOVE LF@$%s string@%s\n", var->key, generator_str_convert(var->data.var->attr->string->str));

    else if (!strcmp("nil", generator_get_type(var->data.var->type)))
        printf("MOVE LF@$%s nil@nil\n", var->key);
}

bool generator_get_arg(char *id, token_t arg, symt_datatype_t idDataType)
{
    tmp.argCount++;
    switch(arg.type)
    {
    case TOKEN_TYPE_INT:                                                                 // declare an argument of type int
        printf("DEFVAR LF@$arg%d\n", tmp.argCount);
        printf("MOVE LF@$arg%d int@%d\n", tmp.argCount, arg.attribute.integer);
        break;
    case TOKEN_TYPE_FLOAT:                                                               // declare an argument of type float
        printf("DEFVAR LF@$arg%d\n", tmp.argCount);                                   
        printf("MOVE LF@$arg%d float@%a\n", tmp.argCount, arg.attribute.decimal);
        break;
    case TOKEN_TYPE_STRING:                                                              // declare an argument of type string
        printf("DEFVAR LF@$arg%d\n", tmp.argCount);
        printf("MOVE LF@$arg%d string@%s\n", tmp.argCount, generator_str_convert(arg.attribute.string->str));
        break;
    case TOKEN_ID:                                                                       // declare an argument of type ID
        printf("DEFVAR LF@$arg%d\n", tmp.argCount);
        switch(idDataType)
        {
            case INTEGER_DT:
            case INTEGER_N_DT:                                                           // declare an argument of type int
                printf("MOVE LF@$arg%d int@%d\n", tmp.argCount, arg.attribute.integer);
                break;
            case FLOAT_DT:
            case FLOAT_N_DT:                                                             // declare an argument of type float
                printf("MOVE LF@$arg%d float@%a\n", tmp.argCount, arg.attribute.decimal);
                break;
            case STRING_DT:
            case STRING_N_DT:                                                            // declare an argument of type string
                printf("MOVE LF@$arg%d string@%s\n", tmp.argCount, generator_str_convert(arg.attribute.string->str));
                break;
            case UNDEFINED_DT:                                                           // declare an argument of type nil
                printf("DEFVAR LF@$arg%d\n", tmp.argCount);
                printf("MOVE LF@$arg%d nil@nil\n", tmp.argCount);
                break;
        }
        break;
        default:                                                                         // declare an argument of type nil
        printf("DEFVAR LF@$arg%d\n", tmp.argCount);
        printf("MOVE LF@$arg%d nil@nil\n", tmp.argCount);
        break;
    }

    if(!strcmp(id, "write"))
    {
        printf("WRITE LF@$arg%d\n", tmp.argCount);
    }
    else if (!strcmp(id, "strlen"))
    {
        printf("DEFVAR LF@$len%d\n", tmp.argCount);
        printf("MOVE LF@$len%d int@0\n", tmp.argCount);
        printf("STRLEN LF@$len%d LF$arg%d\n", tmp.argCount, tmp.argCount);
    }
}

void generator_start_func(ht_item_t *func)
{
    if(strcmp(func->key, "write") && strcmp(func->key, "strlen"))     // write and strlen are exceptions in the target language 
    {                                                                 // they are presented as instructions, not functions
        printf("LABEL !%s\n", func->key);
        printf("PUSHFRAME\n");
    }
}

void generator_end_func(ht_item_t *func)
{
    if(strcmp(func->key, "write") && strcmp(func->key, "strlen"))
    {
        tmp.argCount = -1;
        printf("LABEL !end_%s\n", func->key);

        printf("POPFRAME\n");
        printf("RETURN\n");

    }

}

void generator_call_func(ht_item_t *func)
{
    if(strcmp(func->key, "write") && strcmp(func->key, "strlen"))
    {
        printf("CALL !%s\n", func->key);
    }
}

char *generator_str_convert(char *str)
{
    char *result_str = "";
    char new_str[512] = "\0";
    char buf[2];
    int i = 0;

    for (; i < strlen(str); i++)
    {
        switch (str[i])
        {
        case 0:
            strcat(new_str, "\\000");
            break;
        case 1:
            strcat(new_str, "\\001");
            break;
        case 2:
            strcat(new_str, "\\002");
            break;
        case 3:
            strcat(new_str, "\\003");
            break;
        case 4:
            strcat(new_str, "\\004");
            break;
        case 5:
            strcat(new_str, "\\005");
            break;
        case 6:
            strcat(new_str, "\\006");
            break;
        case 7:
            strcat(new_str, "\\007");
            break;
        case 8:
            strcat(new_str, "\\008");
            break;
        case 9:
            strcat(new_str, "\\009");
            break;
        case 10:
            strcat(new_str, "\\010");
            break;
        case 11:
            strcat(new_str, "\\011");
            break;
        case 12:
            strcat(new_str, "\\012");
            break;
        case 13:
            strcat(new_str, "\\013");
            break;
        case 14:
            strcat(new_str, "\\014");
            break;
        case 15:
            strcat(new_str, "\\015");
            break;
        case 16:
            strcat(new_str, "\\016");
            break;
        case 17:
            strcat(new_str, "\\017");
            break;
        case 18:
            strcat(new_str, "\\018");
            break;
        case 19:
            strcat(new_str, "\\019");
            break;
        case 20:
            strcat(new_str, "\\020");
            break;
        case 21:
            strcat(new_str, "\\021");
            break;
        case 22:
            strcat(new_str, "\\022");
            break;
        case 23:
            strcat(new_str, "\\023");
            break;
        case 24:
            strcat(new_str, "\\024");
            break;
        case 25:
            strcat(new_str, "\\025");
            break;
        case 26:
            strcat(new_str, "\\026");
            break;
        case 27:
            strcat(new_str, "\\027");
            break;
        case 28:
            strcat(new_str, "\\028");
            break;
        case 29:
            strcat(new_str, "\\029");
            break;
        case 30:
            strcat(new_str, "\\030");
            break;
        case 31:
            strcat(new_str, "\\031");
            break;
        case ' ':
            strcat(new_str, "\\032");
            break;
        case '#':
            strcat(new_str, "\\035");
            break;
        case '\\':
            strcat(new_str, "\\092");
            break;
        default:
            buf[0] = str[i];
            buf[1] = '\0';
            strcat(new_str, buf);
            break;
        }
    }

    result_str = (char *)malloc((strlen(new_str)) * sizeof(char));

    if (!result_str)
        return NULL;

    strcpy(result_str, new_str);

    return result_str;
}

void generator_start_if()
{
    tmp.ifCount++;

    printf("JUMPIFEQ \n");
    printf("LABEL !if_func%d\n", tmp.ifCount);

    printf("PUSHFRAME\n");
}

void generator_end_if()
{
    printf("LABEL !end_if_func%d\n", tmp.ifCount);

    printf("POPFRAME\n");
    printf("RETURN\n");
}

void generator_start_else()
{
    tmp.elseCount++;

    printf("LABEL !else_func%d\n", tmp.elseCount);
    printf("PUSHFRAME\n");
}

void generator_end_else()
{
    printf("LABEL !end_else_func%d\n", tmp.elseCount);

    printf("POPFRAME\n");
    printf("RETURN\n");
}

void generator_start_while()
{
    tmp.whileCount++;

    printf("LABEL !while_func%d\n", tmp.whileCount);

    printf("PUSHFRAME\n");

    printf("DEFVAR LF@$loop%d\n", tmp.whileCount);
    printf("MOVE LF@$loop%d bool@true\n", tmp.whileCount);
}

void generator_end_while()
{
    printf("LABEL !end_while_func%d\n", tmp.whileCount);

    printf("POPFRAME\n");
    printf("RETURN\n");
}

void generator_readi()
{
    printf("DEFVAR LF@$readi\n");
    printf("READ LF@$readi int\n");
}

void generator_readf()
{
    printf("DEFVAR LF@$readf\n");
    printf("READ LF@$readf float\n");
}

void generator_reads()                                                                                   
{
    printf("DEFVAR LF@$reads\n");
    printf("READ LF@$reads string\n");
}

void generator_strlen(char *str)
{
    printf("DEFVAR LF@$len\n");
    printf("MOVE LF@$len string@\n");
    printf("STRLEN LF@$len LF@%s\n", str);
}

void generator_substr()
{
    printf("DEFVAR LF@$result\n");
    printf("DEFVAR LF@$str_param\n");
    printf("DEFVAR LF@$str_param1\n");
    printf("DEFVAR LF@$str_param2\n");
    printf("DEFVAR LF@$str\n");
    printf("DEFVAR LF@$char\n");

    printf("MOVE LF@$result string@\n");
    printf("MOVE LF@$str TF&arg%d\n", tmp.argCount - 2);
    printf("MOVE LF@$str_param1 LF@&arg%d\n", tmp.argCount - 1);
    printf("MOVE LF@$str_param2 LF@&arg%d\n", tmp.argCount);

    printf("JUMPIFEQ !end_substr LF@$str nil@nil\n");
    printf("JUMPIFEQ !end_substr LF@$str_param1 nil@nil\n");
    printf("JUMPIFEQ !end_substr LF@$str_param2 nil@nil\n");

    printf("LT LF@$str_param LF@$str_param1 int@1\n");
    printf("JUMPIFEQ !end_substr LF@$str_param bool@true\n");
    printf("LT LF@$str_param LF@$str_param2 int@1\n");
    printf("JUMPIFEQ !end_substr LF@$str_param bool@true\n");
    generator_strlen("$str");
    printf("GT LF@$str_param LF@$str_param1 LF@$len\n");
    printf("JUMPIFEQ !end_substr LF@$str_param bool@true\n");
    printf("GT LF@$str_param LF@$str_param2 LF@$len\n");
    printf("JUMPIFEQ !end_substr LF@$str_param bool@true\n");
    printf("LT LF@$str_param LF@$str_param2 LF@$str_param1\n");
    printf("JUMPIFEQ !end_substr LF@$str_param bool@true\n");
    printf("SUB LF@$str_param1 LF@$str_param1 int@1\n");

    printf("LABEL !loop_substr\n");
    printf("GETCHAR LF@$char LF@$str LF@$str_param1\n");
    printf("CONCAT LF@$result LF@$result LF@$char\n");
    printf("ADD LF@$str_param1 LF@$str_param1 int@1\n");
    printf("LT LF@$str_param LF@$str_param1 LF@$str_param2\n");
    printf("JUMPIFEQ !loop_substr LF@$str_param bool@true\n");
}

void generator_ord()
{
    printf("DEFVAR LF@$result\n");
    printf("DEFVAR LF@$param\n");
    printf("DEFVAR LF@$str\n");

    printf("MOVE LF@$str LF&arg%d\n", tmp.argCount);

    printf("JUMPIFEQ !end_ord LF@$str nil@nil\n");

    generator_strlen("$str");

    printf("LT LF@$param LF@$len int@1\n");
    printf("JUMPIFEQ !end_ord LF@$param bool@true\n");
    printf("STRI2INT LF@$result LF@$str int@1\n");
    printf("JUMP !end_ord\n");
    printf("LABEL !error_string_is_empty\n");
    printf("MOVE LF$result int@0\n");
}

void generator_chr()
{
    printf("DEFVAR LF@$result\n");
    printf("DEFVAR LF@$param\n");
    printf("DEFVAR LF@$params\n");

    printf("MOVE LF@$param LF&arg%d\n", tmp.argCount);

    printf("JUMPIFEQ !end_chr LF@$param nil@nil\n");

    printf("LT LF@$params LF@$param int@1\n");
    printf("JUMPIFEQ !end_chr LF@$params bool@true\n");
    printf("GT LF@$params LF@$param int@255\n");
    printf("JUMPIFEQ !end_chr LF@$params bool@true\n");
    printf("INT2CHAR LF@$result LF@$param\n");
}

void generator_internal_func(char *func_name)
{
    char *func[] = {"reads", "readi", "readf", "substring", "ord", "chr"};

    for (int i = 0; i < 6; i++)
    {
        if (!strcmp(func[i], func_name))
        {
            switch(i)
            {
            case 0:
                generator_reads();
                break;
            case 1:
                generator_readi();
                break;
            case 2:
                generator_readf();
                break;
            case 3:
                generator_substr();
                break;
            case 4:
                generator_ord();
                break;
            case 5:
                generator_chr();
                break;
            }
        }
    }
}

void generator_operation(token_type_t operation, ht_item_t *var, ht_item_t *symb1, ht_item_t *symb2)
{
    if (generator_check_var(var))   // if variable (var) is new
        generator_get_new_var(var); // get new variable

    else if (generator_check_var(symb1))
        generator_get_new_var(symb1);

    else if (generator_check_var(symb1))
        generator_get_new_var(symb1);

    switch (operation)
    {
    case TOKEN_PLUS:
        printf("ADD LF@$%s LF@$%s LF@$%s\n", var->key, symb1->key, symb2->key);
        break;
    case TOKEN_MINUS:
        printf("SUB LF@$%s LF@$%s LF@$%s\n", var->key, symb1->key, symb2->key);
        break;
    case TOKEN_MUL:
        printf("MUL LF@$%s LF@$%s LF@$%s\n", var->key, symb1->key, symb2->key);
        break;
    case TOKEN_DIV:

        if (!strcmp("float", generator_get_type(symb1->data.var->type)) && !strcmp("float", generator_get_type(symb2->data.var->type))) // if simb1 is float and symb2 is float
        {
            printf("JUMPIFEQ !division_by_zero LF@$%s float@%a\n", symb2->key, 0.0);
            printf("DIV LF@$%s LF@$%s LF@$%s\n", var->key, symb1->key, symb2->key); // do operation DIV
            printf("LABEL !division_by_zero\n");
        }
        else if (!strcmp("int", generator_get_type(symb1->data.var->type)) && !strcmp("int", generator_get_type(var->next->next->data.var->type))) // if simb1 is int and symb2 is int
        {
            printf("LABEL !division_by_zero\n");
            printf("IDIV LF@$%s LF@$%s LF@$%s\n", var->key, symb1->key, symb2->key); // do operation IDIV
            printf("LABEL !division_by_zero\n");
        }
        break;
    case TOKEN_EQUAL:
        printf("EQ LF@$%s LF@$%s LF@$%s\n", var->key, symb1->key, symb2->key);
        break;
    case TOKEN_NOT_EQUAL:
        printf("EQ LF@$%s LF@$%s LF@$%s\n", var->key, symb1->key, symb2->key); // symb1 == symb2
        printf("NOT LF@$%s LF@$%s\n", var->key, var->key);                     // NOT(var)
        break;
    case TOKEN_LESS:
        printf("LT LF@$%s LF@$%s LF@$%s\n", var->key, symb1->key, symb2->key);
        break;
    case TOKEN_GREATER:
        printf("GT LF@$%s LF@$%s LF@$%s\n", var->key, symb1->key, symb2->key);
        break;
    case TOKEN_LESS_EQ:
        printf("DEFVAR LF@$param1\n");
        printf("DEFVAR LF@$param2\n");
        printf("LT LF@$param1 LF@$%s LF@$%s\n", symb1->key, symb2->key); // symb1 < symb2
        printf("EQ LF@$param2 LF@$%s LF@$%s\n", symb1->key, symb2->key); // symb1 == symb2
        printf("OR LF@$%s LF@$$param1 LF@$$param2\n", var->key);         // true if symb1 < symb2 nebo symb1 == symb2
        break;
    case TOKEN_GREATER_EQ:
        printf("DEFVAR LF@$param1\n");
        printf("DEFVAR LF@$param2\n");
        printf("GT LF@$param1 LF@$%s LF@$%s\n", symb1->key, symb2->key);
        printf("EQ LF@$param2 LF@$%s LF@$%s\n", symb1->key, symb2->key);
        printf("OR LF@$%s LF@$param1 LF@$param2\n", var->key);
        break;
    }
}