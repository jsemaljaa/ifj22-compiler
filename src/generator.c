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

    tmp.ifCount = 0;
    tmp.whileCount = 0;
    tmp.varCount = 0;
    tmp.loopCount = 0;
    tmp.maxArgs = -1;
}

bool generator_get_arg(char *id,  ht_item_t *arg)
{
    tmp.maxArgs++;


    if(generator_check_var)
        generator_get_new_var(arg);
    else{
        tmp.varCount++;

        tmp.varsId[tmp.varCount] = (char *)malloc((strlen(arg->key)) * sizeof(char));

        if(!tmp.varsId[tmp.varCount])
            return NULL;

        for(int i = 0; i < strlen(arg->key); i++)
            tmp.varsId[tmp.varCount][i] = arg->key[i];
    }


    if(!strcmp(id, "write"))
        printf("WRITE TF@%s\n", tmp.varsId[tmp.varCount]);


    else if(!strcmp(id, "strlen"))
        generator_strlen(tmp.varsId[tmp.varCount]);

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
        // case K_BOOL_N:
        //     return "bool";
        //     break;
    case NIL_DT:
        return "nil";
    default:
        return "";
        break;
    }
}

bool generator_check_var(ht_item_t *var)
{
    for(int j = 0; j < 101; j++)
    {
        if(tmp.varsId[j] == NULL)
            continue;
        else{
        if(!strcmp(var->key, tmp.varsId[j]))
            return false;
        }
    }

    tmp.varsId[tmp.varCount] = (char *)malloc((strlen(var->key)) * sizeof(char));

    if(!tmp.varsId[tmp.varCount])
        return NULL;

    for(int i = 0; i < strlen(var->key); i++)
        tmp.varsId[tmp.varCount][i] = var->key[i];
    
    tmp.varCount++;

    return true;
}

void generator_get_new_var(ht_item_t *var)
{
    if (!strcmp("int", generator_get_type(var->data.var->type)))
    {
        printf("DEFVAR TF@$%s\n", var->key);
        printf("MOVE TF@$%s int@%d\n", var->key, var->data.var->attr->integer);
    }
    else if (!strcmp("float", generator_get_type(var->data.var->type)))
    {
        printf("DEFVAR TF@$%s\n", var->key);
        printf("MOVE TF@$%s float@%a\n", var->key, var->data.var->attr->decimal);
    }
    else if (!strcmp("string", generator_get_type(var->data.var->type)))
    {
        printf("DEFVAR TF@$%s\n", var->key);
        printf("MOVE TF@$%s string@%s\n", var->key, generator_str_convert(var->data.var->attr->string->str));
    }
}

void generator_start_func(ht_item_t *func)
{
    printf("LABEL !%s\n", func->key);
    //generator_push_frame();
    printf("PUSHFRAME\n");
}

void generator_end_func(ht_item_t *func)
{
    tmp.maxArgs = -1;
    printf("LABEL !end_%s\n", func->key);

    //generator_pop_frame();
    //generator_return();
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void generator_call_func(ht_item_t *func)
{
    printf("CALL !%s\n", func->key);
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

    if(!result_str)
        return NULL;

    for (int j = 0; j < strlen(new_str); j++)
        result_str[j] = new_str[j];

    return result_str;
}

ht_item_t *generator_default_val(token_t token)
{
    ht_item_t *item;
    char buf[MAX_HT_SIZE];

    tmp.varCount++;

    sprintf(buf, "%d", tmp.varCount);

    switch(token.type)
    {
        case TOKEN_TYPE_INT:
            strcat(buf, "int");
            item->data.var->type = INTEGER_DT;
            item->data.var->attr->integer = token.attribute.integer;
            break;
        case TOKEN_TYPE_FLOAT:
            strcat(buf, "float");
            item->data.var->type = FLOAT_DT;
            item->data.var->attr->decimal= token.attribute.decimal;
            break;
        case TOKEN_TYPE_STRING:
            strcat(buf, "string");
            item->data.var->type = STRING_DT;
            item->data.var->attr->string->str = token.attribute.string->str;
            break;
        default:

            break;
    }

    item->key = (char *)malloc((strlen(buf)) * sizeof(char));

    if(!item->key)
        return NULL;

    for(int i = 0; i < strlen(buf); i++)
        item->key[i] = buf[i];

    return item;   
}

void generator_start_if()
{
    tmp.ifCount++;

    printf("LABEL !if_func%d\n", tmp.ifCount);
    //generator_push_frame();
    printf("PUSHFRAME\n");
}

void generator_end_if()
{
    // printf("LABEL !end_%s\n", end_if_func->key);
    printf("LABEL !end_if_func%d\n", tmp.ifCount);
    //generator_pop_frame();
    //generator_return();
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void generator_start_while(int max_while)
{
    printf("LABEL !while_func%d\n", tmp.whileCount);
    //generator_push_frame();
    printf("PUSHFRAME\n");

    printf("DEFVAR TF@$loop%d\n", tmp.whileCount);
    printf("MOVE TF@$loop%d bool@true\n", tmp.whileCount);

    printf("LABEL !loop_while%d\n", tmp.loopCount);
}

void generator_loop_condition()
{
    printf("MOVE TF@loop%d bool@false\n", tmp.whileCount);
}

// void generator_stop_loop_while()
// {
//     printf("JUMPIFEQ !loop_while%d TF@loop%d bool@false\n", loop_count, while_count);
// }

void generator_end_while()
{
    printf("JUMPIFEQ !loop_while%d TF@loop%d bool@false\n", tmp.loopCount, tmp.whileCount);
    printf("LABEL !end_while_func%d\n", tmp.whileCount);
    //generator_pop_frame();
    //generator_return();
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void generator_readi()
{
    // printf("!readi\n");
    // generator_push_frame();

    printf("DEFVAR LF@$readi\n");
    printf("READ LF@$readi\n");

    // generator_pop_frame();
    // generator_return();
}

void generator_readf()
{
    // printf("!readf\n");
    // generator_push_frame();

    printf("DEFVAR LF@$readf\n");
    printf("READ LF@$readf\n");

    // generator_pop_frame();
    // generator_return();
}

void generator_reads()                                                                                   
{
    // printf("!reads\n");
    // generator_push_frame();

    printf("DEFVAR LF@$reads\n");
    printf("READ LF@$reads\n");

    // generator_pop_frame();
    // generator_return();
}

void generator_strlen(char *str)
{
    printf("DEFVAR LF@$len\n");
    printf("MOVE LF@$len string@\n");
    printf("STRLEN LF@$len TF@%s\n", str);
}

void generator_substr()
{
    // printf("!substr\n");
    // generator_push_frame();
    printf("DEFVAR LF@$result\n");
    printf("DEFVAR LF@$str_param\n");
    printf("DEFVAR LF@$str_param1\n");
    printf("DEFVAR LF@$str_param2\n");
    printf("DEFVAR LF@$str\n");
    printf("DEFVAR LF@$char\n");

    printf("MOVE LF@$result string@\n");
    printf("MOVE LF@$str TF&%s\n", tmp.varsId[tmp.varCount - 2]);
    printf("MOVE LF@$str_param1 TF&%s\n", tmp.varsId[tmp.varCount - 1]);
    printf("MOVE LF@$str_param2 TF&%s\n", tmp.varsId[tmp.varCount]);

    // printf("MOVE LF@$str string@%s\n", generator_str_convert(s->data.var->attr->string->str));
    // printf("MOVE LF@$str_param1 int@%d\n", i->data.var->attr->integer);
    // printf("MOVE LF@$str_param2 int@%d\n", j->data.var->attr->integer);

    printf("JUMPIFEQ !end_substr LF@$str nil@nil\n");
    printf("JUMPIFEQ !end_substr LF@$str_param1 nil@nil\n");
    printf("JUMPIFEQ !end_substr LF@$str_param2 nil@nil\n");

    printf("LT LF@$str_param LF@$str_param1 int@1\n");
    printf("JUMPIFEQ !end_substr LF@$str_param bool@true\n");
    printf("LT LF@$str_param LF@$str_param2 int@1\n");
    printf("JUMPIFEQ !end_substr LF@$str_param bool@true\n");
    generator_strlen("$str");
    printf("GT LF@$str_param LF@str_param1 LF@$len\n");
    printf("JUMPIFEQ !end_substr LF@$str_param bool@true\n");
    printf("GT LF@$str_param LF@str_param2 LF@$len\n");
    printf("JUMPIFEQ !end_substr LF@$str_param bool@true\n");
    printf("LT LF@$str_param LF@$str_param2 LF@str_param1\n");
    printf("JUMPIFEQ !end_substr LF@$str_param bool@true\n");
    printf("SUB LF@$str_param1 LF@str_param1 int@1\n");
    
    printf("LABEL !loop_substr\n");
    printf("GETCHAR LF@$char LF@$str LF@$str_param1\n");
    printf("CONCAT LF@$result LF@$result LF@$char\n");
    printf("ADD LF@$str_param1 LF@$str_param1 int@1\n");
    printf("LT LF@$str_param LF@$str_param1 LF@$str_param2\n");
    printf("JUMPIFEQ !loop_substr LF@$str_param bool@true\n");

    // printf("LABEL !end_substr\n");
    // generator_pop_frame();
    // generator_return();
}

void generator_ord()
{
    // printf("!ord\n");
    // generator_push_frame();

    printf("DEFVAR LF@$result\n");
    printf("DEFVAR LF@$param\n");
    printf("DEFVAR LF@$str\n");

    printf("MOVE LF@$str TF&%s\n", tmp.varsId[tmp.varCount]);

    printf("JUMPIFEQ !end_ord LF@$str nil@nil\n");

    generator_strlen("$str");

    printf("LT LF@$param LF@$len int@1\n");
    printf("JUMPIFEQ !end_ord LF@$param bool@true\n");
    printf("STRI2INT LF@$result LF@$str int@1\n");

    // printf("LABEL !end_ord\n");
    // generator_pop_frame();
    // generator_return();
}

void generator_chr()
{
    // printf("!chr\n");
    // generator_push_frame();

    printf("DEFVAR LF@$result\n");
    printf("DEFVAR LF@$param\n");
    printf("DEFVAR LF@$params\n");

    printf("MOVE LF@$param TF&%s\n", tmp.varsId[tmp.varCount]);

    printf("JUMPIFEQ !end_chr LF@$param nil@nil\n");

    printf("LT LF@$params LF@$param int@0\n");
    printf("JUMPIFEQ !end_chr LF@$params bool@true\n");
    printf("GT LF@$params LF@$param int@255\n");
    printf("JUMPIFEQ !end_chr LF@$params bool@true\n");
    printf("INT2CHAR LF@$result LF@$param\n");

    // printf("LABEL !end_chr\n");
    // generator_pop_frame();
    // generator_return();
}

void generator_internal_func(char *func_name)
{
    char *func[] = {"reads", "readi", "readf", "substring", "ord", "chr"};

    for (int i = 0; i <= 7; i++)
    {
        if (!strcmp(func[i], func_name))
        {
            switch (i)
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
    if(generator_check_var(var))
        generator_get_new_var(var);

    else if(generator_check_var(symb1))
        generator_get_new_var(symb1);

    else if(generator_check_var(symb1))
        generator_get_new_var(symb1);
    

    switch (operation)
    {
    case TOKEN_PLUS:
        printf("ADD TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key);
        break;
    case TOKEN_MINUS:
        printf("SUB TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key);
        break;
    case TOKEN_MUL:
        printf("MUL TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key);
        break;
    case TOKEN_DIV:
        if (!strcmp("float", generator_get_type(symb1->data.var->type)) && !strcmp("float", generator_get_type(symb2->data.var->type)))
            printf("DIV TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key); // var.index, symb1.index, symb2.index

        // else if (!strcmp("int", generator_get_type(symb1->data.var->type)) && !strcmp("float", generator_get_type(var->next->next->data.var->type)))
        // {
        //     printf("INT2FLOAT TF@$var%d int@%d\n", var_count, symb1->data.var->attr->integer);
        //     printf("DIV TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key); // var.index, symb1.index, symb2.index
        // }
        // else if (!strcmp("float", generator_get_type(symb1->data.var->type)) && !strcmp("int", generator_get_type(var->next->next->data.var->type)))
        // {
        //     printf("INT2FLOAT TF@$var%d int@%d\n", var_count, symb2->data.var->attr->integer);
        //     printf("DIV TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key); // var.index, symb1.index, symb2.index
        // }
        else if (!strcmp("int", generator_get_type(symb1->data.var->type)) && !strcmp("int", generator_get_type(var->next->next->data.var->type)))
            printf("IDIV TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key);
        break;
    case TOKEN_EQUAL:
        printf("EQ TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key);
        break;
    case TOKEN_NOT_EQUAL:
        printf("EQ TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key);
        printf("NOT TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key);
        break;
    case TOKEN_LESS:
        printf("LT TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key);
        break;
    case TOKEN_GREATER:
        printf("GT TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key);
        break;
    case TOKEN_LESS_EQ:
        printf("DEFVAR TF@$param1\n");
        printf("DEFVAR TF@$param2\n");
        printf("LT TF@$param1 TF@$%s TF@$%s\n", symb1->key, symb2->key);
        printf("EQ TF@$param2 TF@$%s TF@$%s\n", symb1->key, symb2->key);
        printf("OR TF@$%s TF@$$param1 TF@$$param2\n", var->key);
        break;
    case TOKEN_GREATER_EQ:
        printf("DEFVAR TF@$param1\n");
        printf("DEFVAR TF@$param2\n");
        printf("GT TF@$param1 TF@$%s TF@$%s\n", symb1->key, symb2->key);
        printf("EQ TF@$param2 TF@$%s TF@$%s\n", symb1->key, symb2->key);
        printf("OR TF@$%s TF@$param1 TF@$param2\n", var->key);
        break;
    }
}