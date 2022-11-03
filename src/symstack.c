/*
 * IFJ2022/project/symstack.h 
 * 
 * @brief Stack of symtables implementation
 * 
 * @author Vinogradova Alina <xvinog00@vutbr.cz>
 */


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "symtable.h"
#include "error.h"
#include "symstack.h"

int err;

bool add_new_symtable(arr_symtables_t *symtables){
    const int size = symtables->size;

    htable *new = realloc(symtables->table, (size + 1) * sizeof(htable));
    if(new == NULL){
        // TODO: process alloc errors
        return false;
    }

    symtables->table = new;
    symt_init(&symtables->table[size]);
    ++(symtables->size);

    return true;
}

var_t *find_in_symtables(arr_symtables_t *symtables, char *key){
    var_t *item = NULL;
    int pos = symtables->size - 1; 
    while(pos >= 0){
        item = symt_search(&symtables->table[pos], key);
        if(item != NULL) return item;
        --pos;
    }
}

void delete_last_symtable(arr_symtables_t *symtables){
    int pos = symtables->size - 1;
    symt_free(&symtables->table[pos]);
    --pos;
}

void free_arr_symtables(arr_symtables_t *symtables){
    const int size = symtables->size;
    int i = 0;

    while(i < size){
        delete_last_symtable(symtables);
        ++i;
    }

    free(symtables->table);
}