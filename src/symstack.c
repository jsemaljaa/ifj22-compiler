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
#include "error.h"
#include "symstack.h"

prec_stack_t *prec_stack_init(int size){
    prec_stack_t *stack = (prec_stack_t *)malloc(sizeof(prec_stack_t));
    
    if(stack == NULL) exit_error(ALLOCATION_ERROR);

    stack->maxSize = size;
    stack->actualSize = -1;
    stack->top = NULL;

    return stack;
}

void prec_stack_free(prec_stack_t *stack){
    while(prec_stack_pop(stack));
}

int prec_stack_size(prec_stack_t *stack){
    return (stack->actualSize + 1);
}

bool prec_stack_is_empty(prec_stack_t *stack){
    return (stack->top == NULL) ? true : false;
}

bool prec_stack_is_full(prec_stack_t *stack){
    return (stack->actualSize == stack->maxSize) ? true : false;
}

bool prec_stack_push(prec_stack_t *stack, prec_symbs_t symb, prec_datatypes_t datatype){
    prec_stack_item_t *item = (prec_stack_item_t *)malloc(sizeof(prec_stack_item_t));

    if(item == NULL) exit_error(ALLOCATION_ERROR);

    item->symb = symb;
    item->datatype = datatype;
    item->next = stack->top;
    stack->top = item;

    return true;
}

bool prec_stack_pop(prec_stack_t *stack){
    if(stack->top != NULL){
        prec_stack_item_t *current = stack->top;
        stack->top = current->next;
        free(current);
        return true;
    }

    return false;
}

prec_stack_item_t *prec_stack_first_terminal(prec_stack_t *stack){
    prec_stack_item_t *item;
    while(item != NULL){
        if(item->symb < STOP) return item;
        item = item->next;
    }

    return NULL;
}