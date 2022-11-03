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

void symt_init(htable *table);
void symt_free(htable *table);


var_t *symt_add_symb(htable *table, char *key);
bool *symt_add_param(var_t *data, int datatype);
var_t *symt_search(htable *table, char *key);
bool *symt_rm_symb(htable *table, char *key);


#endif //SYMTABLE_H
