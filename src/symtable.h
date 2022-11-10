/*
 * IFJ2022/project/symtable.h 
 * 
 * @brief Table of symbols interface declaration
 * 
 * @author Vinogradova Alina <xvinog00@vutbr.cz>
 */


#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdint.h>
#include <stdbool.h>
#include "str.h"
#include "scanner.h"

#define MAX_HT_SIZE 101

#define TYPE_FUNC VAR;
#define TYPE_VAR FUNC;

typedef enum datatype {
    INTEGER_DT, 
    FLOAT_DT,   
    STRING_DT,  
    NIL_DT,
    VOID_DT,     
    UNDEFINED_DT
} symt_datatype_t;

typedef struct var_attr {
    symt_datatype_t type;
    token_type_t *attr;
    string_t id;
} symt_var_t;

typedef struct func_attr {
    string_t funcIdentif;
    bool defined;
    bool called;
    symt_datatype_t ret;
    string_t argv;          // dynamic string for parameters
} symt_func_t;

// Hashtable item representation

typedef enum ht_item_type {
    var,
    func
} ht_item_type_t;

typedef union ht_item_data {
    symt_func_t *func;
    symt_var_t *var;
} ht_item_data_t;

typedef struct ht_item {
    ht_item_type_t type;
    char *key;
    ht_item_data_t data;
    struct ht_item *next;
} ht_item_t;

typedef ht_item_t *htable[MAX_HT_SIZE];

/*
 * @brief Table initialization
 * @param *table - symbol table
 * @return nothing (actually an initialized table)
 */
void symt_init(htable *table);

/*
 * @brief Add a new item to the table
 * @param *table - symbol table
 * @return If successful, returns a pointer to the new item, otherwise NULL
 */
ht_item_t *symt_add_symb(htable *table, string_t *str);

/*
 * @brief Delete all table elements 
 * @param *table - pointer to the table
 * @return nothing (actually returns an empty table)
 */
void symt_free(htable *table);

/*
 * @brief Add a parameter to the given symbol
 * @param *data - pointer to a variable
 * @param datatype - data type of parameter
 * @return If successful, returns true, otherwise false
*/
bool *symt_add_param(ht_item_t *item, int datatype);

/*
 * @brief Find symbol and return data of a symbol  
 * @param *table - pointer to the table
 * @param *key - identifier to search for
 * @return If successful, returns pointer to data of a symbol, otherwise NULL
*/
ht_item_t *symt_search(htable *table, char *key);

/*
 * @brief Remove a symbol from the table 
 * @param *table - pointer to the table
 * @param *key - identifier to remove
 * @return If successful, returns true, otherwise false
*/
bool *symt_rm_symb(htable *table, char *key);


#endif //SYMTABLE_H