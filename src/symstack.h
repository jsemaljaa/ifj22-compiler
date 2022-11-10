/*
 * IFJ2022/project/symstack.h 
 * 
 * @brief Stack of symbols interface for precedence analysis declaration
 * 
 * @author Vinogradova Alina <xvinog00@vutbr.cz>
 */


#ifndef SYMSTACK_H
#define SYMSTACK_H

#include <stdint.h>
#include <stdbool.h>
#include "scanner.h"
#include "str.h"
#include "symtable.h"
#include "expressions.h"


/*
 * @brief Data structure representing a stack of symbols 
*/
typedef struct precStackItem {
    prec_symbs_t symb;
    prec_datatypes_t datatype;
    struct precStackItem *next;
} prec_stack_item_t;

typedef struct precStack {
    int maxSize;
    int actualSize;
    prec_stack_item_t *top;
} prec_stack_t;

/*
 * @brief Stack initialization 
 * @param size - size of stack to initialize 
 * @return If successful, returns a pointer to stack
*/
prec_stack_t *prec_stack_init(int size);

/*
 * @brief Deleting the stack and all the data inside 
 * @param *stack - pointer to a stack to delete
 * @return nothing 
*/
void prec_stack_free(prec_stack_t *stack);

/*
 * @brief Count the size of a stack 
 * @param *stack - pointer to a stack 
 * @return If successful, returns an integer size of a given stack
*/
int prec_stack_size(prec_stack_t *stack);

/*
 * @brief Check if stack is full 
 * @param *stack - pointer to a stack
 * @return true if stack is empty, false otherwise
*/
bool prec_stack_is_empty(prec_stack_t *stack);

/*
 * @brief Check if stack is full 
 * @param *stack - pointer to a stack
 * @return true if stack is full, false otherwise
*/
bool prec_stack_is_full(prec_stack_t *stack);

/*
 * @brief Push given symbol into the stack  
 * @param *stack - pointer to a stack
 * @param symb - symbol to be pushed
 * @param datatype - datatype of a symb
 * @return If successful, returns true, else otherwise
*/
bool prec_stack_push(prec_stack_t *stack, prec_symbs_t symb, prec_datatypes_t datatype);

/*
 * @brief Pop a symbol from the top of a stack 
 * @param *stack - pointer to a stack
 * @return If successful, return true, else otherwise
*/
bool prec_stack_pop(prec_stack_t *stack);

/*
 * @brief Find the first terminal in the stack 
 * @param *stack - pointer to a stack
 * @return If successful, returns a pointer to an item containing first terminal data
*/
prec_stack_item_t *prec_stack_first_terminal(prec_stack_t *stack);
#endif // SYMSTACK_H