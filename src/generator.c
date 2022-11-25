/*
 * IFJ2022/project/generator.c
 * 
 * @brief Target language generation
 * 
 * @author Ekaterina Krupenko <xkrupe00@vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>
#include "generator.h"
#include "scanner.h"

/*
 * Internal functions + header and main
 */

void generator_header()
{
    printf(".IFJcode21\n");
    generator_def_var("GF", "$end_result$");
    generator_jump("main\n");

}

void generator_start_main() // char *func_name, char *func_type, char *dst)
{
    generator_label("main");
    generator_create_frame();
    generator_push_frame();
}

void generator_end_main()
{
    generator_pop_frame();
    generator_clear_s();
}

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

void generator_start_if(char *label, char *dst, char *symb)
{
    generator_jump_if_eq(label, "GF", "$end_result$", "bool", "false");
}

void generator_readi(char *dst)
{
    generator_label("readi");
    generator_push_frame();
    generator_def_var(dst, "var");
    generator_read(dst, "var", "int");
    generator_pop_frame();
    generator_return();
}

void generator_readf(char *dst)
{
    generator_label("readf");
    generator_push_frame();
    generator_def_var(dst, "var");
    generator_read(dst, "var", "float");
    generator_pop_frame();
    generator_return();
}

void generator_reads(char *dst)
{
    generator_label("reads");
    generator_push_frame();
    generator_def_var(dst, "var");
    generator_read(dst, "var", "string");
    generator_pop_frame();
    generator_return();
}

void generator_write(char *dst)
{
    generator_label("write");
    generator_def_var(dst, "&symbl1&");
    generator_def_var(dst, "&result&");
    generator_jump_if_neq("write_func", dst, "$symb1$", "nil", "nil");
    generator_print("string", "nil");
    generator_jump("write_end");
    generator_label("write_func");
    generator_print(dst, "$result$");
    generator_return();
    generator_label("write_end");
    generator_return();
}

//STRNUM //TODO
// void generator_float_val(token_t *token, char *dst);
// void generator_int_val(token_t *token, char *dst);
// void generator_str_val(token_t *token, char *dst);

void generator_strlen(char *dst, char *symb)
{
    generator_def_var(dst, "$len$");
    printf("STRLEN LF@$len$ %s@%s\n", dst, symb);
}

void generator_substr(char *dst)
{
    generator_label("substr\n");
    generator_push_frame();
    generator_def_var(dst, "$result$");
    generator_move(dst, "string", "$result$", "");

    generator_jump_if_eq("error", dst, "$param1$", "nil", "nil");
    generator_jump_if_eq("error", dst, "$param2$", "nil", "nil");
    generator_jump_if_eq("error", dst, "$param3$", "nil", "nil");
    generator_def_var(dst, "$flag$");
    generator_LT(dst, "$flag$", dst, "$param2$", "int", "1");
    generator_jump_if_eq("substr_end", dst, "$flag$", "bool", "true");
    generator_LT(dst, "$flag$", dst, "$param3$", "int", "1");
    generator_jump_if_eq("substr_end", dst, "$flag$", "bool", "true");
    generator_strlen(dst, "$param1$");
    generator_GT(dst, "$flag$", dst, "$param2$", dst, "var");
    generator_jump_if_eq("substr_end", dst, "$flag$", "bool", "true");
    generator_GT(dst, "$flag$", dst, "$param3$", dst, "var");
    generator_jump_if_eq("substr_end", dst, "$flag$", "bool", "true");
    generator_LT(dst, "$flag$", dst, "$param3$", dst, "$param2$");
    generator_jump_if_eq("substr_end", dst, "$flag$", "bool", "true");
    generator_sub(dst, "$param2$", dst, "$param2$", "int", "1");
    generator_def_var(dst, "tmp_char");
    generator_label("LOOP");
    generator_getchar(dst, "tmp_char", dst, "$param1$", dst, "$param2$");
    generator_concat(dst, "$result$", dst, "$result$", dst, "tmp_char");
    generator_add(dst, "$param2$", dst, "$param2$", dst, "1");
    generator_LT(dst, "$flag$", dst, "$param2$", dst, "$param3$");
    generator_jump_if_eq("LOOP", dst, "$flag$", "bool", "true");
    generator_label("substr_end");
    generator_pop_frame();
    generator_return();
}

void generator_ord(char *dst)
{
    generator_label("ord");
    generator_push_frame();
    generator_jump_if_eq("error", dst, "$param1$", "nil", "nil");
    generator_jump_if_eq("error", dst, "$param2$", "nil", "nil");
    generator_def_var(dst, "$result$");
    generator_move(dst, "$result$", "nil", "nil");
    generator_strlen(dst, "$param1$");
    generator_def_var(dst, "$flag$");
    generator_LT(dst, "$flag$", dst, "$param2$", "int", "1");
    generator_jump_if_eq("ord_end", dst, "$flag$", "bool", "true");
    generator_GT(dst, "$flag$", dst, "$param2$", dst, "var");
    generator_jump_if_eq("ord_end", dst, "$flag$", "bool", "true");
    generator_sub(dst, "$param2$", dst, "$param2$", "int", "1");
    generator_stri_2_int(dst, "$result$", dst, "$param1$", dst, "$param2$");
    generator_label("ord_end");
    generator_pop_frame();
    generator_return();
}

void generator_chr(char *dst)
{
    generator_label("chr");
    generator_push_frame();
    generator_jump_if_eq("error", dst, "$param1$", "nil", "nil");
    generator_def_var(dst, "$result$");
    generator_move(dst, "$result$", "nil", "nil");
    generator_def_var(dst, "$flag$");
    generator_LT("dst", "$flag$", dst, "$param1$", "int", "0");
    generator_jump_if_eq("chr_end", dst, "$flag$", "bool", "true");
    generator_GT(dst, "$flag$", dst, "$param1$", "int", "255");
    generator_jump_if_eq("chr_end", dst, "$flag$", "bool", "true");
    generator_int_2_char(dst, "$result$", dst, "$param1$");
    generator_label("chr_end");
    generator_pop_frame();
    generator_return();
}



// void generator_code(char *func_name, char *len_symb, char *dst)
// {
//     char *func[] = {"reads", "readi", "readf", "write", "strlen", "substring"};
//     char *type;

//     generator_header();
    
//     for(int i = 0; i <= 5; i++)
//     {

//         if(strcmp(func[i], func_name) == 0)
//         {
//             switch(i)
//             {
//                 case 0:
//                     generator_reads(dst);
//                     type = "string";
//                     break;
//                 case 1:
//                     generator_readi(dst);
//                     type = "int";
//                     break;
//                 case 2:
//                     generator_readf(dst);
//                     type = "float";
//                     break;
//                 case 3:
//                     generator_function_write(dst);
//                     type = NULL;
//                     break;
//                 case 4:
//                     generator_strlen(dst, len_symb);
//                     type = "int";
//                     break;
//                 case 5:
//                     generator_substr(dst);
//                     type = "string";
//                     break;
//             }
//         }
//     }

//     generator_main(func_name, type, dst);

// }