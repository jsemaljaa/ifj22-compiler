/*
 * IFJ2022/project/parser.h 
 * 
 * @brief Parser interface declaration
 * 
 * @author Vinogradova Alina <xvinog00@vutbr.cz>
 * @author Mazurava Maryia <xmazur08@vutbr.cz>
 */


#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "expressions.h"
#include "str.h"
#include "symstack.h"
#include "symtable.h"
#include "scanner.h"
#include "error.h"
#include "generator.h"

#define GET_TOKEN()                      \
    code = get_next_token(&token);        \
    if((code) != NO_ERRORS) return LEXICAL_ERROR; \

#define CHECK_KEYW(KEYW, code)                                                        \
    if(!(token.type == TOKEN_KEY_W && token.attribute.keyword == (KEYW))) {     \
        return code;                                                           \
    }

#define GET_TOKEN_CHECK_KEYW(KEYW, code)    \
    GET_TOKEN();                            \
    CHECK_KEYW(KEYW, code);                 \

#define CHECK_TOKEN(expr, code)         \
    if(!(expr)) return code;            \

#define GET_AND_CHECK_TOKEN(expr, code) \
    GET_TOKEN();                        \
    CHECK_TOKEN(expr, code);            \

#define CHECK_ERROR(code)               \
    if(code != 0) return code;          \

#define CHECK_RULE(func)                \
    code = (func);                      \
    CHECK_ERROR(code);                  \

#define CODE_GENERATE(_gen, ...)                     \
     if(!_gen(__VA_ARGS__)) return INTERNAL_ERROR;    \




#define INTERNALS_COUNT 11
#define RET_DATATYPES_COUNT 7
#define KEYW_COUNT 11


static const char *retDatatypes[RET_DATATYPES_COUNT] = {
    "I",
    "F",
    "S",
    "V",
    "?I",
    "?F",
    "?S"
};

static const char *list[] = {
    "PLUS",
    "MINUS",
    "MUL",
    "DIV",
    "EQUAL",
    "NOT_EQUAL",
    "LESS",
    "GREATER",
    "LESS_EQ",
    "GREATER_EQ",
    "LEFT_PAR",
    "RIGHT_PAR",
    "SEMICOLON",
    "COLON",
    "ASSIGN",
    "COMMA",
    "CONC",
    "LEFT_BR",
    "RIGHT_BR",
    "DO_QUOTES",
    "QUEST",
    "ID",
    "KEY_W",
    "TYPE_INT",
    "TYPE_FLOAT",
    "TYPE_STRING",
    "END_OF_FILE", 
    "PROLOG"};


/*
 * @brief Clean exit from parser
 * @param code - code to return with
 * @return nothing, ends the parser run with an error code
*/
static void clean_exit(int code);


/*
 * @brief Add internal functions to the symtable 
 * @return If successful, returns NO_ERRORS, other error code otherwise
*/
static int symt_add_internal_functions();

/*
 * @brief Main function of a parser 
 * @return Current error code if something went wrong, NO_ERRORS otherwise
*/
int parse();

/*
 * @example 1. <program> -> <prolog> <list_of_statements>
 * @brief Function to define rule 1
 * @return Current error code if something went wrong, NO_ERRORS otherwise
*/
static int program();

/*
 * @example 2. <prolog> -> <?php declare(strict_types=1); 
 * @brief Function to define rule 2 
 * @return Current error code if something went wrong, NO_ERRORS otherwise
*/
static int prolog();

/*
 * @example 3. <list_of_statements> -> EOF 
 * @example 4. <list_of_statements> -> <statement> <list_of_statements> 
 * @brief Function to define rules 3, 4
 * @return Current error code if something went wrong, NO_ERRORS otherwise
*/
static int list_of_statements();

/*
 * @example 5. <statement> -> <function_definition>
 * @example 6. <statement> -> <variable_definition>
 * @example 7. <statement> -> <function_call>
 * @example 8. <statement> -> if ( <expression> ) { <list_of_statements> } else { <list_of_statements> }
 * @example 9. <statement> -> while ( <expression> ) { <list_of_statements> }
 * @example 10. <statement> -> return <return_expressions>;
 * @brief Function to define rules 5 to 10 including
 * @return Current error code if something went wrong, NO_ERRORS otherwise
*/
static int statement();

/*
 * @example 11. <return_expressions> -> <expression>
 * @example 12. <return_expressions> -> <variable>
 * @brief Function to define rules 11, 12
 * @return Current error code if something went wrong, NO_ERRORS otherwise
*/
static int return_expressions();

/*
 * @example 13. <function_definition> -> function ID( <list_of_parameters> ) : <list_of_datatypes_ret> { <list_of_statements> }
 * @brief Function to define rule 13
 * @return Current error code if something went wrong, NO_ERRORS otherwise
*/
static int function_definition();

/*
 * @example 14. <variable_definition> -> <variable> = <var_def_expr> 
 * @brief Function to define rule 14
 * @return Current error code if something went wrong, NO_ERRORS otherwise
*/
static int variable_definition();

/*
 * @example 15. <var_def_expr> -> <function_call>
 * @example 16. <var_def_expr> -> <expression>;
 * @brief Function to define rules 15, 16
 * @return Current error code if something went wrong, NO_ERRORS otherwise 
*/
static int var_def_expr();

/*
 * @example 17. <function_call> -> ID( <list_of_call_parameters> ); 
 * @brief Function to define rule 17
 * @return Current error code if something went wrong, NO_ERRORS otherwise 
*/
static int function_call();

/*
 * @example 18. <list_of_call_parameters> -> ε 
 * @example 19. <list_of_call_parameters> -> <call_parameter> <list_of_call_parameters_n> 
 * @brief Function to define rules 18, 19
 * @return Current error code if something went wrong, NO_ERRORS otherwise  
*/
static int list_of_call_parameters(ht_item_t* item);

/*
 * @example 20. <call_parameter> -> <variable>
 * @example 21. <call_parameter> -> "string" // ?????
 * @brief Function to define rules 20, 21 
 * @return Current error code if something went wrong, NO_ERRORS otherwise   
*/
static int call_parameter(ht_item_t* item, string_t params);

/*
 * @example 22. <list_of_call_parameters_n> -> , <call_parameter> <list_of_call_parameters_n>
 * @example 23. <list_of_call_parameters_n> -> ε 
 * @brief Function to define rules 22, 23
 * @return Current error code if something went wrong, NO_ERRORS otherwise    
*/
static int list_of_call_parameters_n(ht_item_t* item, string_t params);

/*
 * @example 24. <list_of_parameters> -> ε 
 * @example 25. <list_of_parameters> -> <parameter> <list_of_parameters_n>
 * @brief Function to define rules 24, 25
 * @return Current error code if something went wrong, NO_ERRORS otherwise     
*/
static int list_of_parameters(ht_item_t* item);

/*
 * @example 26. <parameter> -> <list_of_datatypes> <variable>
 * @brief Function to define rule 26
 * @return Current error code if something went wrong, NO_ERRORS otherwise
*/
static int parameter(ht_item_t* item);

/*
 * @example 27. <list_of_datatypes> -> int
 * @example 28. <list_of_datatypes> -> bool
 * @example 29. <list_of_datatypes> -> float
 * @example 30. <list_of_datatypes> -> string
 * @example 31. <list_of_datatypes> -> nil
 * @brief Function to define rules 27 to 31 including
 * @return Current error code if something went wrong, NO_ERRORS otherwise
*/
static int list_of_datatypes(ht_item_t* item);

/*
 * @example 32. <variable> -> $ID 
 * @brief Function to define rule 32
 * @return Current error code if something went wrong, NO_ERRORS otherwise
*/
static int variable();

/*
 * @example 33. <list_of_parameters_n> -> ε
 * @example 34. <list_of_parameters_n> -> , <parameter> <list_of_parameters_n>
 * @brief Function do define rules 33, 34
 * @return Current error code if something went wrong, NO_ERRORS otherwise
*/
static int list_of_parameters_n();

/*
 * @example 35. <list_of_datatypes_ret> -> void
 * @example 36. <list_of_datatypes_ret> -> <list_of_datatypes>
 * @brief Function to define rules 35, 36
 * @return Current error code if something went wrong, NO_ERRORS otherwise
*/
static int list_of_datatypes_ret();

static int inside_if();

static int inside_while();

static int check_id_for_keyword(string_t *word);

static int collecting_an_expression(string_t *expression);


#endif //PARSER_H


