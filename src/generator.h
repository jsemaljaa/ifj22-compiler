/*
 * IFJ2022/project/generator.c
 * 
 * @brief Target language generation
 * 
 * @author Ekaterina Krupenko <xkrupe00@vutbr.cz>
 */

#ifndef GENERATOR_H
#define GENERATOR_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"


void generator_statement_jumpifeq(char *func_name, int func_tree, int func_num);

void generator_statement_jump(char *func_name, int func_tree, int func_num);

/*
 * Input-output instructions
 */

void generator_read(char *dst, char *var, char *var_type);

void generator_write(char *dst, char *symb);

/*
 * Frames, calling functions
 */

void generator_move(char *dst, char *src, char *var, char *symb);

void generator_create_frame();

void generator_push_frame();

void generator_pop_frame();

void generator_def_var(char *dst, char *var);

void generator_call(char *label_name);

void generator_return();

/*
 * Stack function
 */

void generator_push_s(char *symb_type, char *symb);

void generator_pops_s(char *dst, char *var);

void generator_clear_s();

/*
 * Arithmetic, relational Boolean and conversion instructions
 */

void generator_add(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2);

void generator_sub(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2);

void generator_mul(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2);

void generator_div(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2);

// void generator_idiv();

// void generator_aritmetic_s();

void generator_LT(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2);

void generator_GT(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2);

void generator_EQ(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2);

// void generator_boolean_s();

void generator_and(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_or(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_not(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

// void generator_boolean_ss();

void generator_int_2_float(char *src, char *var, char *dst, char *symb);

void generator_float_2_int(char *src, char *var, char *dst, char *symb);

void generator_int_2_char(char *src, char *var, char *dst, char *symb);

void generator_stri_2_inr(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

// void generator_konverze_s();

/*
 * String op
 */

void generator_concat(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_getchar(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_setchar(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

/*
 * Type op
 */

void generator_type(char *src, char *var, char *dst, char *symb);

/*
 * Controlling program flow op
 */

void generator_label(char *label_name);

void generator_jump(char *label);

void generator_jump_if_eq(char *label, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_jump_if_neq(char *label, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_jump_if_eq_s();

void generator_jump_if_neq_s();

void generator_exit(char *symb);


/*
 * Debug instructions
 */

void generator_break();

// void generator_dprint(char *symb);

/*
 * Internal functions 
 */

void generator_header();

// void generator_start_main();

// void generator_end_main();

void generator_start_func(char *func_name);

void generator_end_func();

void generator_param(char *dst, char *var, char *type_symb, char *symb);

void generator_start_if(char *func_name, int func_tree, int func_num);

void generator_end_else_if(char *func_name, int func_tree, int func_num);

void generator_while_start(char *func_name, int func_tree, int func_num);

void generator_while_end(char *func_name, int func_tree, int func_num);


void generator_readi();

void generator_readf();

void generator_reads();

void generator_function_write();

void generator_strlen(char *dst, char *symb);

void generator_substr();

void generator_ord();

void generator_chr();

void generator_internal_func(char *func_name, char *type_symb, char *symb);


#endif //GENERATOR_H