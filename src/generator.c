/*
 * IFJ2022/project/generator.c
 * 
 * @brief Target language generation
 * 
 * @author Ekaterina Krupenko <xkrupe00@vutbr.cz>
 */

#include "generator.h"


void generator_statement_jumpifeq(char *func_name, int func_tree, int func_num)
{
    printf("JUMPIFEQ %s_%d_%d GF@$end_result$ bool@false\n", func_name, func_tree, func_num);
}

void generator_statement_jump(char *func_name, int func_tree, int func_num)
{
    printf("JUMP %s_%d_%d GF@$end_result$ bool@false\n", func_name, func_tree, func_num);
}

// void generate_statement_label(char *func_name, int func_tree, int func_num)
// {
//     printf("LABEL %s_%d_%d GF@$end_result$ bool@false\n", func_name, func_tree, func_num);
// }

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

// void generator_idiv(); //TODO

// void generator_aritmetic_s(); //TODO

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

// void generator_boolean_ss(); //TODO

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

// void generator_konverze_s(); //TODO

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
    generator_jump("BREAK\n");
}

/*
 * Internal functions 
 */

void generator_header()
{
    printf(".IFJcode21\n");
    generator_def_var("GF", "$end_result$");
    generator_jump("main\n");

}

// void generator_start_main()
// {
//     generator_label("main");
//     generator_create_frame();
//     generator_push_frame();
// }

// void generator_end_main()
// {
//     generator_pop_frame();
//     generator_clear_s();
// }

void generator_start_func(char *func_name)
{
    generator_label(func_name);
    generator_push_frame();
}

void generator_end_func()
{
    generator_label("end_func");
    generator_pop_frame();
    generator_return();
}

void generator_param(char *dst, char *var, char *type_symb, char *symb)
{
    generator_def_var(dst, var);
    generator_move(dst, var, type_symb, symb);
}

void generator_start_if(char *func_name, int func_tree, int func_num)
{
    generator_statement_jumpifeq(func_name, func_tree, func_num);
}

void generator_end_else_if(char *func_name, int func_tree, int func_num)
{
    generator_statement_jump(func_name, func_tree, func_num + 1);
}

void generator_while_start(char *func_name, int func_tree, int func_num)
{
    generator_statement_jumpifeq(func_name, func_tree, func_num);
}

void generator_while_end(char *func_name, int func_tree, int func_num)
{
    generator_statement_jump(func_name, func_tree, func_num - 1);
}

void generator_readi()
{
    generator_label("readi");
    generator_push_frame();
    generator_def_var("LF", "var");
    generator_read("LF", "var", "int");
    generator_pop_frame();
    generator_return();
}

void generator_readf()
{
    generator_label("readf");
    generator_push_frame();
    generator_def_var("LF", "var");
    generator_read("LF", "var", "float");
    generator_pop_frame();
    generator_return();
}

void generator_reads()
{
    generator_label("reads");
    generator_push_frame();
    generator_def_var("LF", "var");
    generator_read("LF", "var", "string");
    generator_pop_frame();
    generator_return();
}

//STRNUM //TODO
// void generator_float_val(token_t *token, char *dst);
// void generator_int_val(token_t *token, char *dst);
// void generator_str_val(token_t *token, char *dst);

void generator_strlen(char *dst, char *symb)
{
    generator_def_var("LF", "$len$");
    printf("STRLEN LF@$len$ %s@%s\n", dst, symb);
}

void generator_substr()
{
    generator_label("substr\n");
    generator_push_frame();
    generator_def_var("LF", "$result$");
    generator_move("LF", "string", "$result$", "");

    generator_jump_if_eq("error", "LF", "$param1$", "nil", "nil");
    generator_jump_if_eq("error", "LF", "$param2$", "nil", "nil");
    generator_jump_if_eq("error", "LF", "$param3$", "nil", "nil");
    generator_def_var("LF", "$flag$");
    generator_LT("LF", "$flag$", "LF", "$param2$", "int", "1");
    generator_jump_if_eq("substr_end", "LF", "$flag$", "bool", "true");
    generator_LT("LF", "$flag$", "LF", "$param3$", "int", "1");
    generator_jump_if_eq("substr_end", "LF", "$flag$", "bool", "true");
    generator_strlen("LF", "$param1$");
    generator_GT("LF", "$flag$", "LF", "$param2$", "LF", "var");
    generator_jump_if_eq("substr_end", "LF", "$flag$", "bool", "true");
    generator_GT("LF", "$flag$", "LF", "$param3$", "LF", "var");
    generator_jump_if_eq("substr_end", "LF", "$flag$", "bool", "true");
    generator_LT("LF", "$flag$", "LF", "$param3$", "LF", "$param2$");
    generator_jump_if_eq("substr_end", "LF", "$flag$", "bool", "true");
    generator_sub("LF", "$param2$", "LF", "$param2$", "int", "1");
    generator_def_var("LF", "tmp_char");
    generator_label("LOOP");
    generator_getchar("LF", "tmp_char", "LF", "$param1$", "LF", "$param2$");
    generator_concat("LF", "$result$", "LF", "$result$", "LF", "tmp_char");
    generator_add("LF", "$param2$", "LF", "$param2$", "LF", "1");
    generator_LT("LF", "$flag$", "LF", "$param2$", "LF", "$param3$");
    generator_jump_if_eq("LOOP", "LF", "$flag$", "bool", "true");
    generator_label("substr_end");
    generator_pop_frame();
    generator_return();
}

void generator_ord() 
{
    generator_label("ord");
    generator_push_frame();
    generator_jump_if_eq("error", "LF", "$param1$", "nil", "nil");
    generator_jump_if_eq("error", "LF", "$param2$", "nil", "nil");
    generator_def_var("LF", "$result$");
    generator_move("LF", "$result$", "nil", "nil");
    generator_strlen("LF", "$param1$");
    generator_def_var("LF", "$flag$");
    generator_LT("LF", "$flag$", "LF", "$param2$", "int", "1");
    generator_jump_if_eq("ord_end", "LF", "$flag$", "bool", "true");
    generator_GT("LF", "$flag$", "LF", "$param2$", "LF", "var");
    generator_jump_if_eq("ord_end", "LF", "$flag$", "bool", "true");
    generator_sub("LF", "$param2$", "LF", "$param2$", "int", "1");
    generator_stri_2_int("LF", "$result$", "LF", "$param1$", "LF", "$param2$");
    generator_label("ord_end");
    generator_pop_frame();
    generator_return();
}

void generator_chr()
{
    generator_label("chr");
    generator_push_frame();
    generator_jump_if_eq("error", "LF", "$param1$", "nil", "nil");
    generator_def_var("LF", "$result$");
    generator_move("LF", "$result$", "nil", "nil");
    generator_def_var("LF", "$flag$");
    generator_LT("dst", "$flag$", "LF", "$param1$", "int", "0");
    generator_jump_if_eq("chr_end", "LF", "$flag$", "bool", "true");
    generator_GT("LF", "$flag$", "LF", "$param1$", "int", "255");
    generator_jump_if_eq("chr_end", "LF", "$flag$", "bool", "true");
    generator_int_2_char("LF", "$result$", "LF", "$param1$");
    generator_label("chr_end");
    generator_pop_frame();
    generator_return();
}



void generator_internal_func(char *func_name, char *type_symb, char *symb)
{
    char *func[] = {"reads", "readi", "readf", "write", "strlen", "substring"};

    for(int i = 0; i <= 5; i++)
    {

        if(strcmp(func[i], func_name) == 0)
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
                    generator_write(type_symb, symb);
                    break;
                case 4:
                    generator_strlen("string", symb);
                    break;
                case 5:
                    generator_substr();
                    break;
            }
        }
    }

}

// void generator_operation(int op_type, char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2)
// {

//     switch(op_type)
//     {
//         case 0: //TOKEN_PLUS
//             generator_add(src, var, dst1, symb1, dst2, symb2);
//             break;
//         case 1: //TOKEN_MINUS
//             generator_sub(src, var, dst1, symb1, dst2, symb2);
//             break;
//         case 2: //TOKEN_MUL
//             generator_mul(src, var, dst1, symb1, dst2, symb2);
//             break;
//         case 3: //TOKEN_DIV
//             generator_div(src, var, dst1, symb1, dst2, symb2);
//             break;
//         case 4: //TOKEN_EQUAL
//             generator_EQ(src, var, dst1, symb1, dst2, symb2);
//             break;
//         case 5: //TOKEN_NOT_EQUAL
//             generator_EQ(src, var, dst1, symb1, dst2, symb2);
//             generator_NOT(src, var, dst1, symb1, dst2, symb2);
//             break;            
//     }
// }