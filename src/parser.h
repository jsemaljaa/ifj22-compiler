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



#include <stdbool.h>
#include "scanner.h"
#include "symtable.h"


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
static int list_of_call_parameters();

/*
20. <call_parameter> -> <variable>
21. <call_parameter> -> "string" // ?????
*/
static int call_parameter();

/*
22. <list_of_call_parameters_n> -> , <call_parameter> <list_of_call_parameters_n>
23. <list_of_call_parameters_n> -> ε 
*/
static int list_of_call_parameters_n();

/*
24. <list_of_parameters> -> ε
25. <list_of_parameters> -> <parameter> <list_of_parameters_n>
*/
static int list_of_parameters();

//26. <parameter> -> <list_of_datatypes> <variable>
static int parameter();

/*
27. <list_of_datatypes> -> int
28. <list_of_datatypes> -> bool
29. <list_of_datatypes> -> float
30. <list_of_datatypes> -> string
31. <list_of_datatypes> -> nil
*/
static int list_of_datatypes();

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

int parser();


#endif //PARSER_H


