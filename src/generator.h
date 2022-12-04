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
#include "symtable.h"


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

void generator_idiv(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2);

void generator_add_s(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2);

void generator_sub_s(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2);

void generator_mul_s(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2);

void generator_div_s(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2);

void generator_idiv_s(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2);

void generator_LT(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2);

void generator_GT(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2);

void generator_EQ(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2);

void generator_LT_s(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2);

void generator_GT_s(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2);

void generator_EQ_s(char *src, char *var, char *dst1, char *symb1, char *dst2 , char *symb2);

void generator_and(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_or(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_not(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_and_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_or_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_not_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_int_2_float(char *src, char *var, char *dst, char *symb);

void generator_float_2_int(char *src, char *var, char *dst, char *symb);

void generator_int_2_char(char *src, char *var, char *dst, char *symb);

void generator_stri_2_int(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_int_2_float_S(char *src, char *var, char *dst, char *symb);

void generator_float_2_int_s(char *src, char *var, char *dst, char *symb);

void generator_int_2_char_s(char *src, char *var, char *dst, char *symb);

void generator_stri_2_int_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

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
 * Controlling program flow
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

void generator_dprint(char * symb);

/*
 * Internal functions 
 */

void generator_header();

char *generator_get_type(keyword_t type);

float generator_get_float(keyword_t type, int i, float f, char *s);

int generator_get_int(keyword_t type, int i, float f, char *s);

void generator_get_new_var(symt_var_t var);

void generator_start_func(char *func_name);

void generator_end_func(char * func_name);

void call_func(char *dst, char *name_func);

char *generator_str_convert(char *str);

void generator_start_if(int if_max_index);

void generator_end_if();

void generator_start_while(int max_while_count);

void generator_start_loop_while();

void generator_loop_condition();

void generator_stop_loop_while();

void generator_end_while();

void generator_readi();

void generator_readf();

void generator_reads();

void generator_strlen(char *str);

void generator_substr(symt_var_t s, symt_var_t i, symt_var_t j);

void generator_ord(symt_var_t c);

void generator_chr(symt_var_t i);

void generator_internal_func(char *func_name, symt_var_t var, symt_var_t symb1, symt_var_t symb2);

void generator_operation(token_type_t operation_type, symt_var_t var, symt_var_t symb1, symt_var_t symb2);

#endif //GENERATOR_H