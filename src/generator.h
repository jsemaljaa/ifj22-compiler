/*
 * IFJ2022/project/generator.c
 * 
 * @brief Target language generation
 * 
 * @author Ekaterina Krupenko <xkrupe00@vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

/*
 * Input-output instructions
 */

void generator_read(char *dst, char *var, char *var_type)
{
    printf("READ %s@%s int\n", dst, var);
} 

void generator_print(char *dst, char *symb)
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

// void generator_dprint(char *symb);

/*
 * Internal functions + header and main
 */

void generator_header();

void generator_main();

void generator_readi();

void generator_readf();

void generator_reads();

void generator_function_write();

void generator_strlen(char *dst, char *symb);

void generator_substr();

void generator_ord();

void generator_chr();

void generator_code(char *func_name, char *len_dst, char *len_symb);