/*
 * IFJ2022/project/generator.c
 *
 * @brief Target language generation
 *
 * @author Ekaterina Krupenko <xkrupe00@vutbr.cz>
 */

#include "generator.h"

//в последствии заменить на индекс как часть структуры симтейбла
int if_count;
int func_count;
int while_count;
int var_count;
int param_count;
int loop_count;

char *vars_id[MAX_HT_SIZE];
int id_num;

/*
 * Input-output instructions
 */

void generator_read(char *dst, char *var, char *var_type)
{
    printf("READ %s@%s int\n", dst, var);
}

void generator_write(char *dst, char *symb)
{
    printf("WRITE %s@%s\n", dst, symb);
}

/*
 * Frames, calling functions
 */

void generator_move(char *dst, char *src, char *var, char *symb)
{
    printf("MOVE %s@%s %s@%s\n", dst, var, src, symb);
}

void generator_create_frame()
{
    printf("CREATEFRAME\n");
}

void generator_push_frame()
{
    printf("PUSHFRAME\n");
}

void generator_pop_frame()
{
    printf("POPFRAME\n");
}

void generator_def_var(char *dst, char *var)
{
    printf("DEFVAR %s@%s\n", dst, var);
}

void generator_call(char *label_name)
{
    printf("CALL $%s\n", label_name);
}

void generator_return()
{
    printf("RETURN\n");
}

/*
 * Stack function
 */

void generator_push_s(char *symb_type, char *symb)
{
    printf("PUSHS %s@%s\n", symb_type, symb);
}

void generator_pops_s(char *dst, char *var)
{
    printf("POPS %s@%s\n", dst, var);
}

void generator_clear_s()
{
    printf("CLEARS\n");
}

/*
 * Arithmetic, relational Boolean and conversion instructions
 */

void generator_add(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("ADD %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_sub(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("SUB %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_mul(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("MUL %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_div(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("DIV %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_idiv(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("IDIV %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_add_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("ADDS %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_sub_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("SUBS %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_mul_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("MULS %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_div_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("DIVS %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_idiv_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("IDIVS %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_LT(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("LT %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_GT(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("GT %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_EQ(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("EQ %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_LT_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("LTS %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_GT_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("GTS %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_EQ_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("EQS %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_and(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("AND %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_or(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("OR %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_not(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("NOT %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_and_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("ANDS %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_or_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("ORS %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_not_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("NOTS %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_int_2_float(char *src, char *var, char *dst, char *symb)
{
    printf("INT2FLOAT %s@%s %s@%s\n", src, var, dst, symb);
}

void generator_float_2_int(char *src, char *var, char *dst, char *symb)
{
    printf("FLOAT2INT %s@%s %s@%s\n", src, var, dst, symb);
}

void generator_int_2_char(char *src, char *var, char *dst, char *symb)
{
    printf("INT2CHAR %s@%s %s@%s\n", src, var, dst, symb);
}

void generator_stri_2_int(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("STRI2INT %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_int_2_float_S(char *src, char *var, char *dst, char *symb)
{
    printf("INT2FLOAT %s@%s %s@%s\n", src, var, dst, symb);
}

void generator_float_2_int_s(char *src, char *var, char *dst, char *symb)
{
    printf("FLOAT2INT %s@%s %s@%s\n", src, var, dst, symb);
}

void generator_int_2_char_s(char *src, char *var, char *dst, char *symb)
{
    printf("INT2CHAR %s@%s %s@%s\n", src, var, dst, symb);
}

void generator_stri_2_int_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("STRI2INT %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

/*
 * String op
 */

void generator_concat(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("CONCAT %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_getchar(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("GETCHAR %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_setchar(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("SETCHAR %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

/*
 * Type op
 */

void generator_type(char *src, char *var, char *dst, char *symb)
{
    printf("TYPE %s@%s %s@%s\n", src, var, dst, symb);
}

/*
 * Controlling program flow
 */

void generator_label(char *label_name)
{
    printf("LABEL %s\n", label_name);
}

void generator_jump(char *label)
{
    printf("JUMP %s\n", label);
}

void generator_jump_if_eq(char *label, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("LUMPIFEQ %s %s@%s %s@%s\n", label, dst1, symb1, dst2, symb2);
}

void generator_jump_if_neq(char *label, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("LUMPIFNEQ %s %s@%s %s@%s\n", label, dst1, symb1, dst2, symb2);
}

void generator_jump_if_eq_s()
{
    printf("JUMPIFEQS\n");
}

void generator_jump_if_neq_s()
{
    printf("JUMPIFEQS\n");
}

void generator_exit(char *symb)
{
    printf("EXIT int@%s\n", symb);
}

/*
 * Debug instructions
 */

void generator_break()
{
    printf("BREAK\n");
}

void generator_dprint(char *symb)
{
    printf("DPRINT %s\n", symb);
}

/*
 * Internal functions
 */

void generator_header()
{
    printf(".IFJcode21\n");

    if_count = 0;
    func_count = 0;
    while_count = 0;
    var_count = 0;
    param_count = 0;
    loop_count = 0;
    id_num = 0;
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
        if(vars_id[j] == NULL)
            continue;
        else{
        if(!strcmp(var->key, vars_id[j]))
            return false;
        }
    }

    vars_id[id_num] = (char *)malloc((strlen(var->key)) * sizeof(char));

    if(!vars_id[id_num])
        return NULL;

    for(int i = 0; i < strlen(var->key); i++)
        vars_id[id_num][i] = var->key[i];

    return true;
}

void generator_get_new_var(ht_item_t *var)
{
    if (!strcmp("int", generator_get_type(var->data.var->type)))
    {
        printf("DEFVAR TF@$%s\n", var->key); // var.index
        printf("MOVE TF@$%s int@%d\n", var->key, var->data.var->attr->integer);
    }
    else if (!strcmp("float", generator_get_type(var->data.var->type)))
    {
        printf("DEFVAR TF@$%s\n", var->key); // var.index
        printf("MOVE TF@$%s float@%a\n", var->key, var->data.var->attr->decimal);
    }
    else if (!strcmp("string", generator_get_type(var->data.var->type)))
    {
        printf("DEFVAR TF@$%s\n", var->key); // var.index
        printf("MOVE TF@$%s string@%s\n", var->key, generator_str_convert(var->data.var->attr->string->str));
    }
}

void generator_start_func(ht_item_t *func)
{
    func_count++;
    printf("LABEL !%s\n", func->key); // funk index
    generator_push_frame();
}

void generator_end_func(ht_item_t *func)
{
    // TODO продумать индекс закрытия функции
    printf("LABEL !end_%s\n", func->key); // end_funk index = current func index

    generator_pop_frame(); // здесь перестают существовать наши пременные все каунтеры = 0
    generator_return();
}

void call_func(ht_item_t *func)
{
    // generator_push_frame();
    printf("CALL !%s\n", func->key); // func.index
    // generator_pop_frame();
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
        case '!':
            strcat(new_str, "\\033");
            break;
        case '"':
            strcat(new_str, "\\034");
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

    sprintf(buf, "%d", var_count);

    var_count++;

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
    if_count++;
    // printf("LABEL !%s\n", if_func->key);
    printf("LABEL !if_func%d\n", if_count);
    generator_push_frame();
}

void generator_end_if()
{
    // printf("LABEL !end_%s\n", end_if_func->key);
    printf("LABEL !end_if_func%d\n", if_count);
    generator_pop_frame();
    generator_return();
}

void generator_start_while(int max_while)
{
    printf("LABEL !while_func%d\n", while_count);
    generator_push_frame();

    printf("DEFVAR TF@$loop%d\n", while_count);         // while count
    printf("MOVE TF@$loop%d bool@true\n", while_count); // while count

    printf("LABEL !loop_while%d\n", loop_count);
}

void generator_loop_condition()
{
    printf("MOVE TF@loop%d bool@false\n", while_count);
}

// void generator_stop_loop_while()
// {
//     printf("JUMPIFEQ !loop_while%d TF@loop%d bool@false\n", loop_count, while_count);
// }

void generator_end_while()
{
    printf("JUMPIFEQ !loop_while%d TF@loop%d bool@false\n", loop_count, while_count);
    // TODO продумать индекс закрытия while
    printf("LABEL !end_while_func%d\n", while_count);
    generator_pop_frame();
    generator_return();
}

void generator_readi()
{
    printf("!readi\n");
    generator_push_frame();

    printf("DEFVAR LF@$readi\n");
    printf("READ LF@$readi\n");

    generator_pop_frame();
    generator_return();
}

void generator_readf()
{
    printf("!readf\n");
    generator_push_frame();

    printf("DEFVAR LF@$readf\n");
    printf("READ LF@$readf\n");

    generator_pop_frame();
    generator_return();
}

void generator_reads()                                                                                   
{
    printf("!reads\n");
    generator_push_frame();

    printf("DEFVAR LF@$reads\n");
    printf("READ LF@$reads\n");

    generator_pop_frame();
    generator_return();
}

void generator_strlen(char *str)
{
    generator_def_var("LF", "$len");
    printf("MOVE LF@$str string@\n");
    printf("STRLEN LF@$len LF@%s\n", str);
}

//не понимаю надо ли конвертировать стринг
void generator_substr(ht_item_t *s, ht_item_t *i, ht_item_t *j) //, ht_item_t *i, ht_item_t *j
{
    printf("!substr\n");
    generator_push_frame();

    generator_def_var("LF", "$result");
    generator_def_var("LF", "$str_param");
    generator_def_var("LF", "$str_param1");
    generator_def_var("LF", "$str_param2");
    generator_def_var("LF", "$str");
    generator_def_var("LF", "$char");

    generator_move("LF", "$result", "string", NULL);
    printf("MOVE LF@$str string@%s\n", generator_str_convert(s->data.var->attr->string->str));
    printf("MOVE LF@$str_param1 int@%d\n", i->data.var->attr->integer);
    printf("MOVE LF@$str_param2 int@%d\n", j->data.var->attr->integer);


    generator_jump_if_eq("error_func", "LF", "$str", "nil", "nil");
    generator_jump_if_eq("error_func", "LF", "$str_param1", "nil", "nil");
    generator_jump_if_eq("error_func", "LF", "$str_param2", "nil", "nil");

    generator_LT("LF", "$str_param", "LF", "$str_param1", "int", "1");
    generator_jump_if_eq("!end_substr", "LF", "$str_param", "bool", "true");
    generator_LT("LF", "$str_param", "LF", "$str_param2", "int", "1");
    generator_jump_if_eq("!end_substr", "LF", "$str_param", "bool", "true");
    generator_strlen("$str");
    generator_GT("LF", "$str_param", "LF", "$str_param1", "LF", "$len");
    generator_jump_if_eq("!end_substr", "LF", "$str_param", "bool", "true");
    generator_GT("LF", "$str_param", "LF", "$str_param2", "LF", "$len");
    generator_jump_if_eq("!end_substr", "LF", "$str_param", "bool", "true");
    generator_LT("LF", "$str_param", "LF", "$str_param2", "LF", "$str_param1");
    generator_jump_if_eq("!end_substr", "LF", "$str_param", "bool", "true");

    generator_sub("LF", "$str_param1", "LF", "$str_param1", "int", "1");
    generator_label("!loop_substr");
    generator_getchar("LF", "$char", "LF", "$str", "LF", "$str_param1");
    generator_concat("LF", "$result", "LF", "$result", "LF", "$char");
    generator_add("LF", "$str_param1", "LF", "$str_param1", "int", "1");
    generator_LT("LF", "$str_param", "LF", "$str_param1", "LF", "$str_param2");
    generator_jump_if_eq("!loop_substr", "LF", "$str_param", "bool", "true");

    printf("LABEL !end_substr\n");
    generator_pop_frame();
    generator_return();
}

void generator_ord(ht_item_t *c)
{
    printf("!ord\n");
    generator_push_frame();

    generator_def_var("LF", "$result");
    generator_def_var("LF", "$param");
    generator_def_var("LF", "$str");

    generator_move("LF", "$result", "nil", "nil");
    generator_move("LF", "$str", "string", generator_str_convert(c->data.var->attr->string->str));

    generator_jump_if_eq("error", "LF", "$str", "nil", "nil");

    generator_strlen(generator_str_convert(c->data.var->attr->string->str));
    generator_LT("LF", "$param", "LF", "$len", "int", "1");
    generator_jump_if_eq("!end_ord", "LF", "$param", "bool", "true");
    generator_stri_2_int("LF", "$result", "LF", "$str", "int", "1");

    printf("LABEL !end_ord\n");
    generator_pop_frame();
    generator_return();
}

void generator_chr(ht_item_t *i)
{
    printf("!chr\n");
    generator_push_frame();

    generator_def_var("LF", "$result");
    generator_def_var("LF", "$param");
    generator_def_var("LF", "$params");
    printf("MOVE LF@$param int&%d\n", i->data.var->attr->integer);

    generator_move("LF", "$result", "nil", "nil");

    generator_jump_if_eq("error", "LF", "$param", "nil", "nil");

    generator_LT("LF", "$params", "LF", "$param", "int", "0");
    generator_jump_if_eq("end_chr", "LF", "$params", "bool", "true");
    generator_GT("LF", "$params", "LF", "$param", "int", "255");
    generator_jump_if_eq("!end_chr", "LF", "$params", "bool", "true");
    generator_int_2_char("LF", "$result", "LF", "$param");

    printf("LABEL !end_chr\n");
    generator_pop_frame();
    generator_return();
}

void generator_internal_func(char *func_name, ht_item_t *var, ht_item_t *symb1, ht_item_t *symb2) // , ht_item_t *symb1, ht_item_t *symb2)
{
    char *func[] = {"reads", "readi", "readf", "write", "strlen", "substring", "ord", "chr"};

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
                // if(var.index == 0) then...
                if (!strcmp(generator_get_type(var->data.var->type), "int"))
                    printf("WRITE int@%d\n", var->data.var->attr->integer);
                else if (!strcmp(generator_get_type(var->data.var->type), "float"))
                    printf("WRITE float@%a\n", var->data.var->attr->decimal);
                else if (!strcmp(generator_get_type(var->data.var->type), "string"))
                    printf("WRITE string@%s\n", generator_str_convert(var->data.var->attr->string->str));
                // else
                break;
            case 4:
                // if(var.index != 0) then...
                generator_strlen(var->data.var->attr->string->str);
                break;
            case 5:
                generator_substr(var, symb1, symb2);
                break;
            case 6:
                generator_ord(var);
                break;
            case 7:
                generator_chr(var);
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
        printf("ADDS TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key); // var.index, synb1.index, symb2.index
        break;
    case TOKEN_MINUS:
        printf("SUBS TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key); // var.index, synb1.index, symb2.index
        break;
    case TOKEN_MUL:
        printf("MULS TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key); // var.index, synb1.index, symb2.index
        break;
    case TOKEN_DIV:
        if (!strcmp("float", generator_get_type(symb1->data.var->type)) && !strcmp("float", generator_get_type(symb2->data.var->type)))
            printf("DIVS TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key); // var.index, symb1.index, symb2.index

        else if (!strcmp("int", generator_get_type(symb1->data.var->type)) && !strcmp("float", generator_get_type(var->next->next->data.var->type)))
        {
            printf("INT2FLOAT TF@$var%d int@%d\n", var_count, symb1->data.var->attr->integer);
            printf("DIVS TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key); // var.index, symb1.index, symb2.index
        }
        else if (!strcmp("float", generator_get_type(symb1->data.var->type)) && !strcmp("int", generator_get_type(var->next->next->data.var->type)))
        {
            printf("INT2FLOAT TF@$var%d int@%d\n", var_count, symb2->data.var->attr->integer);
            printf("DIVS TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key); // var.index, symb1.index, symb2.index
        }
        else if (!strcmp("int", generator_get_type(symb1->data.var->type)) && !strcmp("int", generator_get_type(var->next->next->data.var->type)))
            printf("IDIVS TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key); // var.index, symb1.index, symb2.index
        break;
    case TOKEN_EQUAL:
        printf("EGS TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key); // var.index, symb1.index, symb2.index
        break;
    case TOKEN_NOT_EQUAL:
        printf("EQS TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key);  // var.index, symb1.index, symb2.index
        printf("NOTS TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key); // var.index, symb1.index, symb2.index
        break;
    case TOKEN_LESS:
        printf("LTS TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key); // var.index, symb1.index, symb2.index
        break;
    case TOKEN_GREATER:
        printf("GTS TF@$%s TF@$%s TF@$%s\n", var->key, symb1->key, symb2->key); // var.index, symb1.index, symb2.index
        break;
    case TOKEN_LESS_EQ:
        param_count++;
        printf("DEFVAR TF@$param%d\n", param_count); // param_count - 1
        param_count++;
        printf("DEFVAR TF@$param%d\n", param_count);                                                // param_count
        printf("LTS TF@$param%d TF@$%s TF@$%s\n", param_count - 1, symb1->key, symb2->key);     // param_count - 1, symb1.index, symb2.index
        printf("EQS TF@$param%d TF@$%s TF@$%s\n", param_count, symb1->key, symb2->key);         // param_count, symb1.index, symb2.index
        printf("ORS TF@$%s TF@$$param%d TF@$$param%d\n", var->key, param_count - 1, param_count); // var.index, param_count - 1, param_count
        break;
    case TOKEN_GREATER_EQ:
        param_count++;
        printf("DEFVAR TF@$param%d\n", param_count); // param_count - 1
        param_count++;
        printf("DEFVAR TF@$param%d\n", param_count);                                                // param_count
        printf("GTS TF@$param%d TF@$%s TF@$%s\n", param_count - 1, symb1->key, symb2->key);     // param_count - 1, symb1.index, symb2.index
        printf("EQS TF@$param%d TF@$%s TF@$%s\n", param_count, symb1->key, symb2->key);         // param_count, symb1.index, symb2.index
        printf("ORS TF@$%s TF@$param%d TF@$param%d\n", var->key, param_count - 1, param_count); // var.index, param_count - 1, param_count
        break;
    }
}