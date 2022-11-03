/*
 * IFJ2022/project/symtable.c
 * 
 * @brief Table of symbols implementation
 * 
 * @author Vinogradova Alina <xvinog00@vutbr.cz>
 */


#include <stdlib.h>
#include <string.h>
#include "symtable.h"
#include "error.h"


/*
 * @brief Bitwise hash algorithm (JS Hash)
 * @return hash of the string
 * @link https://www.programmingalgorithms.com/algorithm/js-hash/c/
 */
static unsigned int hash_func(char *key){
    unsigned int hash = 0, i = 0;
    unsigned int length = strlen(key);
    for(i = 0; i < length; ++i) {
        hash ^= (hash << 5) + (*key) + (hash >> 2);
    }

    return hash;
}

void symt_init(htable *table){
    for(int i = 0; i < MAX_HT_SIZE; ++i){
        (*table)[i] = NULL;
    }
}


var_t *symt_add_symb(htable *table, char *key){
    if(table == NULL || key == NULL) {
        exit_error(INTERNAL_ERROR);
        // return NULL;
    }

    unsigned int pos = hash_func(key);
    ht_item_t *last = NULL, *current = (*table)[pos];


    while(current != NULL){
        if(!strcmp(key, current->key)) return NULL;
        last = current;
        current = current->next;
    }

    ht_item_t *new = (ht_item_t*)malloc(sizeof(ht_item_t));
    if(new == NULL){
        exit_error(ALLOCATION_ERROR);
        //return NULL;
    }

    new->key = (char *)malloc((strlen(key) + 1) * sizeof(char));
    if(new->key == NULL){
        free(new);
        exit_error(ALLOCATION_ERROR);
        // return NULL;
    }

    new->value.parameters = (string_t *)malloc(sizeof(string_t));
    if(new->value.parameters == NULL){
        free(new->key);
        free(new);
        exit_error(ALLOCATION_ERROR);
        // return NULL;
    }

    if(strInit(new->value.parameters) == 1){
        free(new->key);
        free(new->value.parameters);
        free(new);

        exit_error(INTERNAL_ERROR);
        // return NULL;
    }

    strcpy(new->key, key);
        
    new->value.id = new->key;
    new->value.type = UNDEFINED_DT;
    new->value.defined = false;
    new->next = NULL;

    if(last == NULL) (*table)[pos] = new;
    else last->next = new;

    return &new->value;
}

void symt_free(htable *table){
    if(table == NULL) 
        return INTERNAL_ERROR;

    ht_item_t *current, *next;

    for(int i = 0; i < MAX_HT_SIZE; ++i){
        if((*table)[i] != NULL){
            next = (*table)[i];

            while(next->next != NULL){
                current = next;
                next = next->next;
                free(current);
            }

            (*table)[i] = NULL;
        }
    }
}

bool *symt_add_param(var_t *data, int datatype){
    if(data == NULL) 
        return (bool *)false;

    switch (datatype)
    {
    case INTEGER_DT:
        if(strAddChar(data->parameters, 'i') == 1)
            return (bool *)false;
        break;
    
    case FLOAT_DT:
        if(strAddChar(data->parameters, 'f') == 1)
            return (bool *)false;
        break;
    
    case STRING_DT:
        if(strAddChar(data->parameters, 's') == 1)
            return (bool *)false;
        break;

    default:
        break;
    }

    return (bool *)true;

}

var_t *symt_search(htable *table, char *key){
    if(table == NULL || key == NULL){
        exit_error(INTERNAL_ERROR);
        // return NULL;
    }

    unsigned int pos = hash_func(key);

    ht_item_t *current = (*table)[pos];

    while(current != NULL){
        if(!strcmp(key, current->key)) return &current->value;
        
        current = current->next;
    }

    // TODO: if no item was found 
    return NULL;
}

bool *symt_rm_symb(htable *table, char *key){
    if(table == NULL || key == NULL){
        // TODO: errors processing
        return (bool *)false;
    }

    unsigned int pos = hash_func(key);

    ht_item_t *last = NULL, *current = (*table)[pos];

    while(current != NULL){
        if(!strcmp(key, current->key)){
            if(last == NULL) (*table)[pos] = (*table)[pos]->next;
            else last->next = current->next;

            free(current->key);

            if(current->value.parameters != NULL){
                strFree(current->value.parameters);
                free(current->value.parameters);
            }

            free(current);
            return (bool *)true;
        }

        last = current;
        current = current->next;
    }

    return (bool *)false;
}