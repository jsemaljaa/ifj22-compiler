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

struct count{
    int ifCount;
    int whileCount;
    int varCount;
    int loopCount;
    int maxArgs;        
    char *varsId[MAX_HT_SIZE];
};

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

void generator_add(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_sub(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_mul(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_div(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_idiv(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_add_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_sub_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_mul_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_div_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_idiv_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_LT(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_GT(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_EQ(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_LT_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_GT_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

void generator_EQ_s(char *src, char *var, char *dst1, char *symb1, char *dst2, char *symb2);

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

void generator_dprint(char *symb);

/*
 * Internal functions
 */

void generator_header();

void generator_get_arg(char *id,  ht_item_t *arg);

char *generator_get_type(symt_datatype_t type);

bool generator_check_var(ht_item_t *var);

void generator_get_new_var(ht_item_t *var);

void generator_start_func(ht_item_t *func);

void generator_end_func(ht_item_t *func);

void call_func(ht_item_t *func);

char *generator_str_convert(char *str);

ht_item_t *generator_default_val(token_t token);

void generator_start_if();

void generator_end_if();

void generator_start_while(int max_while);

void generator_loop_condition();

// void generator_stop_loop_while();

void generator_end_while();

void generator_readi();

void generator_readf();

void generator_reads();

void generator_strlen(char *str);

void generator_substr();

void generator_ord();

void generator_chr();

void generator_internal_func(char *func_name);

void generator_operation(token_type_t operation, ht_item_t *var, ht_item_t *symb1, ht_item_t *symb2);


#endif // GENERATOR_H