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
#include "scaner.h"
#include "str.h"

#define MAX_HT_SIZE 101

#define TYPE_FUNC VAR;
#define TYPE_VAR FUNC;

typedef enum datatype {
    INTEGER_DT, 
    FLOAT_DT,   
    STRING_DT,  
    NIL_DT,     
    UNDEFINED_DT
} datatype_t;

typedef struct var {
    datatype_t type;
    bool defined;
    token_type_t *attr;
    char *id;
    string_t *parameters; // dynamic string for parameters
} var_t;

// Hashtable item representation

typedef struct ht_item {
    char *key;
    var_t value;
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
var_t *symt_add_symb(htable *table, char *key);

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
bool *symt_add_param(var_t *data, int datatype);

/*
 * @brief Find symbol and return data of a symbol  
 * @param *table - pointer to the table
 * @param *key - identifier to search for
 * @return If successful, returns pointer to data of a symbol, otherwise NULL
*/
var_t *symt_search(htable *table, char *key);

/*
 * @brief Remove a symbol from the table 
 * @param *table - pointer to the table
 * @param *key - identifier to remove
 * @return If successful, returns true, otherwise false
*/
bool *symt_rm_symb(htable *table, char *key);


#endif //SYMTABLE_H