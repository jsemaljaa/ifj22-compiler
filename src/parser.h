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

#define GET_TOKEN()                 \
    ret = get_next_token(&token);   \

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

typedef struct {
    htable globalSymt;
    htable localSymt;
} parser_data_t;

int parse();

static int symt_add_internal_functions();

// 1. <program> -> <prolog> <list_of_statements>
static int program();

//2. <prolog> -> <?php declare(strict_types=1);
static int prolog();

//3. <list_of_statements> -> EOF
//4. <list_of_statements> -> <statement> <list_of_statements> 
static int list_of_statements();

/*
5. <statement> -> <function_definition>
6. <statement> -> <variable_definition>
7. <statement> -> <function_call>
8. <statement> -> if ( <expression> ) { <list_of_statements> } else { <list_of_statements> }
9. <statement> -> while ( <expression> ) { <list_of_statements> }
10. <statement> -> return <return_expressions>;
*/
static int statement();

/*
11. <return_expressions> -> <expression>
12. <return_expressions> -> <variable>
*/
static int return_expressions();

// 13. <function_definition> -> function ID( <list_of_parameters> ) : <list_of_datatypes_ret> { <list_of_statements> }
static int function_definition();

//14. <variable_definition> -> <variable> = <var_def_expr>
static int variable_definition();

/*
15. <var_def_expr> -> <function_call>
16. <var_def_expr> -> <expression>;
*/
static int var_def_expr();

//17. <function_call> -> ID( <list_of_call_parameters> );
static int function_call();

/*
18. <list_of_call_parameters> -> ε
19. <list_of_call_parameters> -> <call_parameter> <list_of_call_parameters_n>
*/
static int list_of_call_parameters(ht_item_t* item);

/*
20. <call_parameter> -> <variable>
21. <call_parameter> -> "string" // ?????
*/
static int call_parameter(ht_item_t* item, string_t params);

/*
22. <list_of_call_parameters_n> -> , <call_parameter> <list_of_call_parameters_n>
23. <list_of_call_parameters_n> -> ε 
*/
static int list_of_call_parameters_n(ht_item_t* item, string_t params);

/*
24. <list_of_parameters> -> ε
25. <list_of_parameters> -> <parameter> <list_of_parameters_n>
*/
static int list_of_parameters(ht_item_t* item);

//26. <parameter> -> <list_of_datatypes> <variable>
static int parameter(ht_item_t* item);

/*
27. <list_of_datatypes> -> int
28. <list_of_datatypes> -> bool
29. <list_of_datatypes> -> float
30. <list_of_datatypes> -> string
31. <list_of_datatypes> -> nil
*/
static int list_of_datatypes(ht_item_t* item);

//32. <variable> -> $ID
static int variable();

/*
33. <list_of_parameters_n> -> ε
34. <list_of_parameters_n> -> , <parameter> <list_of_parameters_n>
*/
static int list_of_parameters_n();

/*
35. <list_of_datatypes_ret> -> void
36. <list_of_datatypes_ret> -> <list_of_datatypes>
*/
static int list_of_datatypes_ret();

static int inside_if();

static int inside_while();

static int check_id_for_keyword(string_t *word);

static string_t collecting_an_expression();


#endif //PARSER_H


