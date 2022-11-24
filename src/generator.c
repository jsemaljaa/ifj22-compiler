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
    printf("POPFRAME");
}

void generator_def_var(char *dst, char *var)
{
    printf("DEFVAR %s@%s", dst, var);
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
    printf("AND %s@%s %s@%s %s@%s", src, var, dst1, symb1, dst2, symb2);
}

void generator_or(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("OR %s@%s %s@%s %s@%s", src, var, dst1, symb1, dst2, symb2);
}

void generator_not(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("NOT %s@%s %s@%s %s@%s", src, var, dst1, symb1, dst2, symb2);
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
 * Prace s retezci
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
 * Prace s typy
 */

void generator_type(char *src, char *var, char *dst, char *symb)
{
    printf("TYPE %s@%s %s@%s\n", src, var, dst, symb);
}

/*
 * Instrukce pro rizeni toku programu
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
    printf("LUMPIFEQ %s %s@%s %s@%s", label, dst1, symb1, dst2, symb2);
}

void generator_jump_if_neq(char *label, char *dst1, char *symb1, char *dst2, char *symb2)
{
    printf("LUMPIFNEQ %s %s@%s %s@%s", label, dst1, symb1, dst2, symb2);
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

// void generator_dprint(char *symb); //TODO

/*
 * Internal functions + header and main
 */

void generator_header()
{
    printf(".IFJcode21\n");
    generator_jump("main\n");
}


void generator_main(char *func_name, char *func_type)
{
    generator_label("main");
    generator_create_frame();
    generator_def_var("TF", "n");
    generator_read("TF", "n", func_type); // 3 variable depends on function
    generator_call(func_name); // name of function
    generator_pops_s("TF", "n");
    generator_write("TF", "n");
    generator_write("string", "\010");
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

void generator_function_write()
{
    generator_label("write");
    generator_jump_if_neq("write_func", "GF", "symb1", "nil", "nil");
    generator_write("string", "nil");
    generator_jump("write_end");
    generator_label("write_func");
    generator_write("GF", "result");
    generator_label("write_end");
    generator_return();
}

//STRNUM //TODO
// void generator_float_val(token_t *token, char *dst);
// void generator_int_val(token_t *token, char *dst);
// void generator_str_val(token_t *token, char *dst);

void generator_strlen(char *dst, char *symb)
{
    generator_def_var("LF", "var");
    printf("STRLEN LF@var %s@%s\n", dst, symb);
}

void generator_substr()
{
    generator_label("substr\n");
    generator_push_frame();
    generator_def_var("LF", "result");
    generator_move("LF", "string", "result", "");

    generator_jump_if_eq("error", "LF", "%%0", "nil", "nil");
    generator_jump_if_eq("error", "LF", "%%1", "nil", "nil");
    generator_jump_if_eq("error", "LF", "%%2", "nil", "nil");
    generator_def_var("LF", "check");
    generator_LT("LF", "check", "LF", "%%1", "int", "1");
    generator_jump_if_eq("substr_end", "LF", "check", "bool", "true");
    generator_LT("LF", "check", "LF", "%%2", "int", "1");
    generator_jump_if_eq("substr_end", "LF", "check", "bool", "true");
    generator_strlen("LF", "%%0");
    generator_GT("LF", "check", "LF", "%%1", "LF", "var");
    generator_jump_if_eq("substr_end", "LF", "check", "bool", "true");
    generator_GT("LF", "check", "LF", "%%2", "LF", "var");
    generator_jump_if_eq("substr_end", "LF", "check", "bool", "true");
    generator_LT("LF", "check", "LF", "%%2", "LF", "%%1");
    generator_jump_if_eq("substr_end", "LF", "check", "bool", "true");
    generator_sub("LF", "%%1", "LF", "%%1", "int", "1");
    generator_def_var("LF", "tmp_char");
    generator_label("LOOP");
    generator_getchar("LF", "tmp_char", "LF", "%%0", "LF", "%%1");
    generator_concat("LF", "result", "LF", "result", "LF", "tmp_char");
    generator_add("LF", "%%1", "LF", "%%1", "int", "1");
    generator_LT("LF", "check", "LF", "%%1", "LF", "%%2");
    generator_jump_if_eq("LOOP", "LF", "check", "bool", "true");
    generator_label("substr_end");
    generator_pop_frame();
    generator_return();
}

void generator_ord()
{
    generator_label("ord");
    generator_push_frame();
    generator_jump_if_eq("error", "LF", "%%0", "nil", "nil");
    generator_jump_if_eq("error", "LF", "%%1", "nil", "nil");
    generator_def_var("LF", "result");
    generator_move("LF", "result", "nil", "nil");
    generator_strlen("LF", "%%0");
    generator_def_var("LF", "check");
    generator_LT("LF", "check", "LF", "%%1", "int", "1");
    generator_jump_if_eq("ord_end", "LF", "check", "bool", "true");
    generator_GT("LF", "check", "LF", "%%1", "LF", "var");
    generator_jump_if_eq("ord_end", "LF", "check", "bool", "true");
    generator_sub("LF", "%%1", "LF", "%%1", "int", "1");
    generator_stri_2_int("LF", "result", "LF", "%%0", "LF", "%%1");
    generator_label("ord_end");
    generator_pop_frame();
    generator_return();
}

void generator_chr()
{
    generator_label("chr");
    generator_push_frame();
    generator_jump_if_eq("error", "LF", "%%0", "nil", "nil");
    generator_def_var("LF", "result");
    generator_move("LF", "result", "nil", "nil");
    generator_def_var("LF", "check");
    generator_LT("LF", "check", "LF", "%%0", "int", "0");
    generator_jump_if_eq("chr_end", "LF", "check", "bool", "true");
    generator_GT("LF", "check", "LF", "%%0", "int", "255");
    generator_jump_if_eq("chr_end", "LF", "check", "bool", "true");
    generator_int_2_char("LF", "result", "LF", "%%0");
    generator_label("chr_end");
    generator_pop_frame();
    generator_return();
}

void generator_code(char *func_name, char *len_dst, char *len_symb)
{
    char *func[] = {"reads", "readi", "readf", "write", "strlen", "substring"};
    char *type;

    generator_header();
    
    for(int i = 0; i <= 5; i++)
    {

        if(strcmp(func[i], func_name) == 0)
        {
            switch(i)
            {
                case 0:
                    generator_reads();
                    type = "string";
                    break;
                case 1:
                    generator_readi();
                    type = "int";
                    break;
                case 2:
                    generator_readf();
                    type = "float";
                    break;
                case 3:
                    generator_function_write();
                    type = NULL;
                    break;
                case 4:
                    generator_strlen(len_dst, len_symb);
                    type = "int";
                    break;
                case 5:
                    generator_substr();
                    type = "string";
                    break;
            }
        }
    }

    generator_main(func_name, type);

}