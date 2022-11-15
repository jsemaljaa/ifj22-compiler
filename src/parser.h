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
#include "parser.h"
#include "str.h"
#include "symstack.h"
#include "symtable.h"
#include "scanner.h"

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

static const char *internals[] = {
    "reads",
    "readi",
    "readf",
    "write",
    "floatval",
    "intval",
    "strval",
    "strlen",
    "substring",
    "ord",
    "chr"
};

typedef struct {
    htable globalSymt;
    htable localSymt;
} parser_data_t;

int parse(parser_data_t* pData);

// 1. <program> -> <prolog> <list_of_statements>
static bool program(parser_data_t* pData);

//2. <prolog> -> <?php declare(strict_types=1);
static bool prolog(parser_data_t* pData);

//3. <list_of_statements> -> EOF
//4. <list_of_statements> -> <statement> <list_of_statements> 
static bool list_of_statements(parser_data_t* pData);

/*
5. <statement> -> <function_definition>
6. <statement> -> <variable_definition>
7. <statement> -> <function_call>
8. <statement> -> if ( <expression> ) { <list_of_statements> } else { <list_of_statements> }
9. <statement> -> while ( <expression> ) { <list_of_statements> }
10. <statement> -> return <return_expressions>;
*/
static bool statement(parser_data_t* pData);

/*
11. <return_expressions> -> <expression>
12. <return_expressions> -> <variable>
*/
static bool return_expressions(parser_data_t* pData);

// 13. <function_definition> -> function ID( <list_of_parameters> ) : <list_of_datatypes_ret> { <list_of_statements> }
static bool function_definition(parser_data_t* pData);

//14. <variable_definition> -> <variable> = <var_def_expr>
static bool variable_definition(parser_data_t* pData);

/*
15. <var_def_expr> -> <function_call>
16. <var_def_expr> -> <expression>;
*/
static bool var_def_expr(parser_data_t* pData);

//17. <function_call> -> ID( <list_of_call_parameters> );
static bool function_call(parser_data_t* pData);

/*
18. <list_of_call_parameters> -> ε
19. <list_of_call_parameters> -> <call_parameter> <list_of_call_parameters_n>
*/
static bool list_of_call_parameters(parser_data_t* pData);

/*
20. <call_parameter> -> <variable>
21. <call_parameter> -> "string" // ?????
*/
static bool call_parameter(parser_data_t* pData);

/*
22. <list_of_call_parameters_n> -> , <call_parameter> <list_of_call_parameters_n>
23. <list_of_call_parameters_n> -> ε 
*/
static bool list_of_call_parameters_n(parser_data_t* pData);

/*
24. <list_of_parameters> -> ε
25. <list_of_parameters> -> <parameter> <list_of_parameters_n>
*/
static bool list_of_parameters(parser_data_t* pData);

//26. <parameter> -> <list_of_datatypes> <variable>
static bool parameter(parser_data_t* pData);

/*
27. <list_of_datatypes> -> int
28. <list_of_datatypes> -> bool
29. <list_of_datatypes> -> float
30. <list_of_datatypes> -> string
31. <list_of_datatypes> -> nil
*/
static bool list_of_datatypes(parser_data_t* pData);

//32. <variable> -> $ID
static bool variable(parser_data_t* pData);

/*
33. <list_of_parameters_n> -> ε
34. <list_of_parameters_n> -> , <parameter> <list_of_parameters_n>
*/
static bool list_of_parameters_n(parser_data_t* pData);

/*
35. <list_of_datatypes_ret> -> void
36. <list_of_datatypes_ret> -> <list_of_datatypes>
*/
static bool list_of_datatypes_ret(parser_data_t* pData);


#endif //PARSER_H


