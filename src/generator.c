/*
 * IFJ2022/project/generator.c
 * 
 * @brief Target language generation
 * 
 * @author Ekaterina Krupenko <xkrupe00@vutbr.cz>
 */

#include "generator.h"

//в последствии заменить на индекс как часть структуры симтейбла
int index_count = 0;

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

void generator_add(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2)
{
    printf("ADD %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_sub(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2)
{
    printf("SUB %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_mul(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2)
{
    printf("MUL %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_div(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2)
{
    printf("DIV %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_idiv(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2)
{
    printf("IDIV %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_add_s(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2)
{
    printf("ADDS %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_sub_s(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2)
{
    printf("SUBS %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_mul_s(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2)
{
    printf("MULS %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_div_s(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2)
{
    printf("DIVS %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_idiv_s(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2)
{
    printf("IDIVS %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_LT(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2)
{
    printf("LT %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_GT(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2)
{
    printf("GT %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_EQ(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2)
{
    printf("EQ %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_LT_s(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2)
{
    printf("LTS %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_GT_s(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2)
{
    printf("GTS %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}

void generator_EQ_s(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2)
{
    printf("EQS %s@%s %s@%s %s@%s\n", src, var, dst1, symb1, dst2, symb2);
}
// void generator_boolean_s(); //TODO

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

void generator_dprint(char * symb)
{
    printf("DPRINT %s\n", symb);
}

/*
 * Internal functions 
 */

void generator_header()
{
    printf(".IFJcode21\n");
}

char *generator_get_type(keyword_t type)
{
    switch(type)
    {
        case K_INT_N:
            return "int";
            break;
        case K_FLOAT_N:
            return "float";
            break;
        case K_STRING_N:
            return "string";
            break;
        // case K_BOOL_N:
        //     return "bool";
        //     break;
        default:
            return "nil";
            break;
    }
}

void generator_get_new_var(symt_var_t var)
{
    if(!strcmp("int", generator_get_type(var.type)))
    {
        printf("DEFVAR TF@$var%d\n", index_count); // var.index_count
        printf("MOVE TF@$var%d int@%d\n", index_count, var.attr->integer);
    }
    else if(!strcmp("float", generator_get_type(var.type)))
    {
        printf("DEFVAR TF@$var%d\n", index_count); // var.index_count
        printf("MOVE TF@$var%d int@%a\n", index_count, var.attr->decimal);
    }
    else if(!strcmp("string", generator_get_type(var.type)))
    {
        printf("DEFVAR TF@$var%d\n", index_count); // var.index_count
        printf("MOVE TF@$var%d int@%s\n", index_count, generator_str_convert(var.attr->string->str));
    }
}

void generator_start_func(char *func_name)
{   
    printf("LABEL !func%d", index_count); // funk index_count
    generator_push_frame();
}

void generator_end_func(char * func_name)
{
    printf("LABEL !end_func%d", index_count); // end_funk index_count = current func index_count

    generator_pop_frame(); // здесь перестают существовать наши пременные все каунтеры = 0
    generator_return();
}

void call_func(char *dst, char *name_func)
{
    // generator_push_frame();
    printf("CALL !func%d", index_count); //func_index_count
    // generator_pop_frame();
}

char *generator_str_convert(char *str)
{
    char *result_str = "";
    char new_str[512] = "\0";
    char buf[2];
    int i = 0;

    for(; i < strlen(str); i++)
    {
        switch(str[i])
        {
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
            case '\n':
                strcat(new_str, "\\010");
                break;
            default:
                buf[0] = str[i];
                buf[1] = '\0';
                strcat(new_str, buf);
                break;
        }
    }

    result_str = (char*) malloc((strlen(new_str))*sizeof(char));

    if(!result_str)
        return NULL;

    for(int j = 0; j < strlen(new_str); j++)
        result_str[j] = new_str[j];

    return result_str;
}

void generator_start_if(int if_max_index_count)
{
    printf("LABEL !if_func%d\n", index_count); //if index_count
    generator_push_frame();
}

void generator_end_if()
{
    printf("LABEL !end_if_func%d\n", index_count);
    generator_pop_frame();
    generator_return();
}

void generator_start_while(int max_while_count)
{
    printf("LABEL !while_func%d\n", index_count);
    generator_push_frame();

    printf("DEFVAR TF@$loop%d\n", index_count); // while count
    printf("MOVE TF@$loop%d bool@true\n", index_count); //while count
}

void generator_start_loop_while()
{
    printf("LABEL !loop_while%d\n", index_count); //loop index = while index on start

}

void generator_loop_condition()
{
    printf("MOVE TF@loop%d bool@false", index_count);
}

void generator_stop_loop_while()
{
    printf("JUMPIFEQ !loop_while TF@loop%d bool@false", index_count);
}

void generator_end_while()
{
    printf("LABEL !end_while_func%d\n", index_count);
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

void generator_strlen(char * str)
{
    generator_def_var("LF", "$len");
    printf("STRLEN LF@$len LF@%s\n", str);
}

//не понимаю надо ли конвертировать стринг
void generator_substr(symt_var_t s, symt_var_t i, symt_var_t j)
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
    printf("MOVE LF@$str_param1 int@%d\n", i.attr->integer);
    printf("MOVE LF@$str_param2 int@%d\n", j.attr->integer);
    printf("MOVE LF@$str string@%s\n", generator_str_convert(s.attr->string->str));

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

void generator_ord(symt_var_t c)
{
    printf("!ord\n");
    generator_push_frame();

    generator_def_var("LF", "$result");
    generator_def_var("LF", "$param");
    generator_def_var("LF", "$str");

    generator_move("LF", "$result", "nil", "nil");
    generator_move("LF", "$str", "string", generator_str_convert(c.attr->string->str));


    generator_jump_if_eq("error", "LF", "$str", "nil", "nil");

    generator_strlen("$str");
    generator_LT("LF", "$param", "LF", "$len", "int", "1");
    generator_jump_if_eq("!end_ord", "LF", "$param", "bool", "true");
    generator_stri_2_int("LF", "$result", "LF", "$str", "int", "1");

    printf("LABEL !end_ord\n");
    generator_pop_frame();
    generator_return();
}

void generator_chr(symt_var_t i)
{
    printf("!chr\n");
    generator_push_frame();

    generator_def_var("LF", "$result");
    generator_def_var("LF", "$param");
    generator_def_var("LF", "$params");
    printf("MOVE LF@$param int&%d\n", i.attr->integer);

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



void generator_internal_func(char *func_name, symt_var_t var, symt_var_t symb1, symt_var_t symb2)
{
    char *func[] = {"reads", "readi", "readf", "write", "strlen", "substring", "ord", "chr"};

    for(int i = 0; i <= 7; i++)
    {

        if(!strcmp(func[i], func_name))
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
                    // if(var. index_count == 0) then...
                    if(!strcmp(generator_get_type(var.type), "int"))
                        printf("WRITE string@%d", var.attr->integer);
                    else if(!strcmp(generator_get_type(var.type), "float"))
                        printf("WRITE string@%a", var.attr->decimal);
                    else if(!strcmp(generator_get_type(var.type), "string"))
                        printf("WRITE string@%s", generator_str_convert(var.attr->string->str));
                    break;
                case 4:
                    // if(var. index_count != 0) then...
                    generator_strlen(var.attr->string->str);
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

void generator_operation(token_type_t operation_type, symt_var_t var, symt_var_t symb1, symt_var_t symb2)
{
    // if(var.index_count == 0)
    //     generator_get_new_var(var);
    // else if(symb1.index_count == 0)
    //     generator_get_new_var(symb1);
    // else if(symb1.index_count == 0)
    //     generator_get_new_var(symb1);

    switch(operation_type)
    {
        case TOKEN_PLUS:
            printf("ADDS TF@$var%d TF@$var%d TF@$var%d\n", index_count, index_count, index_count); //var.index_count, synb1.index_count, symb2.index_count
            break;
        case TOKEN_MINUS:
            printf("SUBS TF@$var%d TF@$var%d TF@$var%d\n", index_count, index_count, index_count); //var.index_count, synb1.index_count, symb2.index_count
            break;
        case TOKEN_MUL:
            printf("MULS TF@$var%d TF@$var%d TF@$var%d\n", index_count, index_count, index_count); //var.index_count, synb1.index_count, symb2.index_count
            break;
        case TOKEN_DIV:
            if(!strcmp("float", generator_get_type(symb1.type)) && !strcmp("float", generator_get_type(symb2.type)))
                printf("DIVS TF@$var%d TF@$var%d TF@$var%d\n", index_count, index_count, index_count); //var.index_count, symb1.index_count, symb2.index_count
            
            else if(!strcmp("int", generator_get_type(symb1.type)) && !strcmp("float", generator_get_type(symb2.type)))
            {
                printf("INT2FLOAT TF@$var%d int@%d\n", index_count, symb1.attr->integer);
                printf("DIVS TF@$var%d TF@$var%d TF@$var%d\n", index_count, index_count, index_count); //var.index_count, symb1.index_count, symb2.index_count
            }
            else if(!strcmp("float", generator_get_type(symb1.type)) && !strcmp("int", generator_get_type(symb2.type)))
            {
                printf("INT2FLOAT TF@$var%d int@%d\n", index_count, symb2.attr->integer);
                printf("DIVS TF@$var%d TF@$var%d TF@$var%d\n", index_count, index_count, index_count); //var.index_count, symb1.index_count, symb2.index_count
            }
            else if(!strcmp("int", generator_get_type(symb1.type)) && !strcmp("int", generator_get_type(symb2.type)))
                printf("IDIVS TF@$var%d TF@$var%d TF@$var%d\n", index_count, index_count, index_count); //var.index_count, symb1.index_count, symb2.index_count
            break;
        case TOKEN_EQUAL:
            printf("EGS TF@$var%d TF@$var%d TF@$var%d\n", index_count, index_count, index_count); //var.index_count, symb1.index_count, symb2.index_count
            break;
        case TOKEN_NOT_EQUAL:
            printf("EQS TF@$var%d TF@$var%d TF@$var%d\n", index_count, index_count, index_count); //var.index_count, symb1.index_count, symb2.index_count
            printf("NOTS TF@$var%d TF@$var%d TF@$var%d\n", index_count, index_count, index_count); //var.index_count, symb1.index_count, symb2.index_count
            break;
        case TOKEN_LESS:
            printf("LTS TF@$var%d TF@$var%d TF@$var%d\n", index_count, index_count, index_count); //var.index_count, symb1.index_count, symb2.index_count
            break;
        case TOKEN_GREATER:
            printf("GTS TF@$var%d TF@$var%d TF@$var%d\n", index_count, index_count, index_count); //var.index_count, symb1.index_count, symb2.index_count
            break;
        case TOKEN_LESS_EQ:
            printf("DEFVAR TF@$param%d\n", index_count); //index_count1
            printf("DEFVAR TF@$param%d\n", index_count); //index_count2
            printf("LTS TF@$param%d TF@$var%d TF@$var%d\n", index_count, index_count, index_count); //param.1, symb1.index_count, symb2.index_count
            printf("EQS TF@$param%d TF@$var%d TF@$var%d\n", index_count, index_count, index_count); //param.2, symb1.index_count, symb2.index_count
            printf("ORS TF@$var%d TF@$param%d TF@$param%d\n", index_count, index_count, index_count); //var.index_count, param.1, param.2
            // generator_def_var(src, "&param1&");
            // generator_def_var(src, "&param2&");
            // generator_LT(src, "&param1&", dst1, symb1, dst2, symb2);
            // generator_EQ(src, "&param2&", dst1, symb1, dst2, symb2);
            // generator_or(src, var, src, "&param1&", src, "&param2&");
            break;
        case TOKEN_GREATER_EQ:
            printf("DEFVAR TF@$param%d\n", index_count); //index_count1
            printf("DEFVAR TF@$param%d\n", index_count); //index_count2
            printf("GTS TF@$param%d TF@$var%d TF@$var%d\n", index_count, index_count, index_count); //param.1, symb1.index_count, symb2.index_count
            printf("EQS TF@$param%d TF@$var%d TF@$var%d\n", index_count, index_count, index_count); //param.2, symb1.index_count, symb2.index_count
            printf("ORS TF@$var%d TF@$param%d TF@$param%d\n", index_count, index_count, index_count); //var.index_count, param.1, param.2
            // generator_def_var(src, "&param1&");
            // generator_def_var(src, "&param2&");
            // generator_GT(src, "&param1&", dst1, symb1, dst2, symb2);
            // generator_EQ(src, "&param2&", dst1, symb1, dst2, symb2);
            // generator_or(src, var, src, "&param1&", src, "&param2&");
            break;
    }
}