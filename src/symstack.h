/*
 * IFJ2022/project/symstack.h 
 * 
 * @brief Stack of symtables interface declaration
 * 
 * @author Vinogradova Alina <xvinog00@vutbr.cz>
 */


#ifndef SYMSTACK_H
#define SYMSTACK_H

#include <stdint.h>
#include <stdbool.h>
#include "scaner.h"
#include "str.h"
#include "symtable.h"



typedef struct array_of_symtables{
    int size;
    htable *table;
} arr_symtables_t;

/*
 * @brief Add new symtable to array 
 * @param *symtables - pointer to the array of symtables
 */
bool add_new_symtable(arr_symtables_t *symtables);

/*
 * @brief Find an element in local tables 
 * @param *symtables - pointer to the array of symtables
 * @param *key - identifier to be found
 * @return If successful, returns a pointer to item, otherwise NULL
 */
var_t *find_in_symtables(arr_symtables_t *symtables, char *key);

/*
 * @brief Delete last symtable from array 
 * @param *symtables - pointer to the array of symtables
 */
void delete_last_symtable(arr_symtables_t *symtables);

/*
 * @brief 
 * @param *symtables - pointer to the array of symtables
 */
void free_arr_symtables(arr_symtables_t *symtables);

#endif // SYMSTACK_H