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


// Structure with counters and an array of variables
struct count
{
    int ifCount;
    int elseCount;
    int whileCount;
    int varCount;
    int argCount;

    char *varsId[MAX_HT_SIZE];
};

/*
 * @brief Print a header, init counters
 * @return nothing
 */
void generator_header();

/*
 * @brief Get the type of term as a string
 * @param type - term type as a enum structure
 * @return string ( type of term )
 */
char *generator_get_type(symt_datatype_t type);

/*
 * @brief Check if a variable isn't defined
 * @param *var - term for check
 * @return true ( if a variable is not defined yet )
 * @return false ( if a variable is defined )
 */
bool generator_check_var(ht_item_t *var);

/*

 * @brief Define a new variable
 * @param *var - term for a new variable
 * @return nothing
 */
void generator_get_new_var(ht_item_t *var);

/*
 * @brief Define arguments
 * @param *id - id ( name ) of function
 * @param *arg - term of argument
 * @return NULL ( if malloc failed )
 */

bool generator_get_arg(char *id, token_t arg, symt_datatype_t idDataType);

/*

 * @brief Start a function
 * @param *func - term of function
 * @return nothing
 */
void generator_start_func(ht_item_t *func);

/*
 * @brief End a function
 * @param *func - term of function
 * @return nothing
 */
void generator_end_func(ht_item_t *func);

/*
 * @brief Call a funcion
 * @param *func - term of function
 * @return nothing
 */
void generator_call_func(ht_item_t *func);

/*
 * @brief Convert a string
 * @param *str - string for convert
 * @return converted string
 */
char *generator_str_convert(char *str);

/*
 * @brief Start  a if functuon
 * @return nothing
 */
void generator_start_if();

/*
 * @brief End a if function
 * @return nothing
 */
void generator_end_if();

/*
 * @brief Start  a if functuon
 * @return nothing
 */
void generator_start_else();

/*
 * @brief End a if function
 * @return nothing
 */
void generator_end_else();

/*
 * @brief Start a while function
 * @return nothing
 */
void generator_start_while(); 

/*
 * @brief End a while function
 * @return nothing
 */
void generator_end_while();

/*
 * @brief Read int type variable from the input
 * @return int 
 */
void generator_readi();

/*
 * @brief Read float type variable from the input
 * @return float
 */
void generator_readf();

/*
 * @brief Read string type variable from the input
 * @return string
 */
void generator_reads();

/*
 * @brief Table initialization
 * @param *table - symbol table
 * @return nothing (actually an initialized table)
 */
void generator_strlen(char *str);

/*
 * @brief Table initialization
 * @param *table - symbol table
 * @return nothing (actually an initialized table)
 */
void generator_substr();

/*
 * @brief Table initialization
 * @param *table - symbol table
 * @return nothing (actually an initialized table)
 */
void generator_ord();

/*
 * @brief Table initialization
 * @param *table - symbol table
 * @return nothing (actually an initialized table)
 */
void generator_chr();

/*
 * @brief Print code of one of built-in functions
 * @param *func_name - name of built-in function
 * @return nothing
 */
void generator_internal_func(char *func_name);

#endif // GENERATOR_H