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
 * @link http://www.cse.yorku.ca/~oz/hash.html
 * @cite djb2 algorithm
 */
static unsigned int hash_func(const char *key){
    unsigned int hash = 5381, i = 0;
    unsigned int length = strlen(key);
    for(i = 0; i < length; ++i){
        hash = ((hash << 5) + hash) + key[i];
    }

    return hash;
}

void symt_init(htable *table){
    for(int i = 0; i < MAX_HT_SIZE; ++i){
        (*table)[i] = NULL;
    }
}


ht_item_t *symt_add_symb(htable *table, string_t *key){
    if(table == NULL) {
        exit_error(INTERNAL_ERROR);
    }
    
    unsigned int pos = hash_func(key->str);
    ht_item_t *item = symt_search(table, key->str);
    
    
    if(item != NULL && item->type == func){
        exit_error(SEM_DEF_FUNC_ERROR);
    } else {
        
        ht_item_t *new = malloc(sizeof(ht_item_t));
        if(new == NULL) exit_error(ALLOCATION_ERROR);
        
        new->key = malloc((key->length + 1) * sizeof(char));
        if(new->key == NULL){
            free(new);
            exit_error(ALLOCATION_ERROR);
        }

        strcpy(new->key, key->str);
        new->next = (*table)[pos % MAX_HT_SIZE];
        (*table)[pos % MAX_HT_SIZE] = new;

        return new;
    }
}

void symt_free(htable *table){
    if(table == NULL) 
        // TODO: return error from void ??
        return;

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

bool *symt_add_param(ht_item_t *item, int datatype){
    if(item == NULL) 
        return (bool *)false;

    else if(item->type = func){
        switch (datatype){
            case INTEGER_DT:
                if(str_add_char(&item->data.func->argv, 'i') == 1)
                return (bool *)false;
            break;
    
            case FLOAT_DT:
                if(str_add_char(&item->data.func->argv, 'f') == 1)
                return (bool *)false;
            break;
    
            case STRING_DT:
                if(str_add_char(&item->data.func->argv, 's') == 1)
                return (bool *)false;
            break;

            default:
            break;
    }
        return (bool *)true;
    }
    return (bool *)false;
}

ht_item_t *symt_search(htable *table, char *key){
    if(table == NULL || key == NULL){
        exit_error(INTERNAL_ERROR);
        // return NULL;
    }

    unsigned int pos = hash_func(key);
    ht_item_t *item = (*table)[pos % MAX_HT_SIZE];
    
    while(item != NULL){
        if(strcmp(item->key, key) == 0) return item;
    
        item = item->next;
    }
    // TODO: if no item was found 
    return NULL;
}

// bool *symt_rm_symb(htable *table, char *key){
//     if(table == NULL || key == NULL){
//         // TODO: errors processing
//         return (bool *)false;
//     }

//     unsigned int pos = hash_func(key);

//     ht_item_t *last = NULL, *current = (*table)[pos];

//     while(current != NULL){
//         if(!strcmp(key, current->key)){
//             if(last == NULL) (*table)[pos] = (*table)[pos]->next;
//             else last->next = current->next;

//             free(current->key);

//             if(current->type == var){
//                 if(current->data.var->id.str != NULL)
//                 str_free(current->data.var->id.str);
//                 free(current->data);
//             }

//             free(current);
//             return (bool *)true;
//         }

//         last = current;
//         current = current->next;
//     }

//     return (bool *)false;
// }