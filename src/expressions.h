/*
 * IFJ2022/project/expressions.h 
 * 
 * @brief Precedence analysis implementation declarations
 * 
 * @author Vinogradova Alina <xvinog00@vutbr.cz>
 * @author Mazurava Maryia <xmazur08@vutbr.cz>
 */


#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H


#include <stdio.h>
#include <stdlib.h>
#include "str.h"
#include "parser.h"
#include "error.h"
#include "symtable.h"

#define PREC_TABLE_SIZE 15

// Rules for analysis.
typedef enum {
	NOT_RULE,		// 0. rule doesn't exist
	ID_R,           // 1. E -> id     
	BRACKETS_R, 	// 2. E -> (E)
	MUL_R,		    // 3. E -> E * E
	DIV_R, 		    // 4. E -> E / E
	PLUS_R, 		// 5. E -> E + E
	MINUS_R, 	    // 6. E -> E - E
	CONC_R, 	    // 7. E -> E . E
	GREATER_R, 		// 8. E -> E > E
	GREQ_R, 	    // 9. E -> E >= E
	LESS_R, 		// 10. E -> E < E
	LEEQ_R,			// 11. E -> E <= E
	TYPE_EQ_R,     	// 12. E -> E === E
	NTYPE_EQ_R   	// 13. E -> E !== E
} prec_rules_t;


// Symbols for precendece analysis.
typedef enum {
	ID,	            // id
	LEFT_PAR,		// (
	RIGHT_PAR,		// )
	MUL,	        // *
	DIV,			// /
	PLUS,		    // +
	MINUS,		    // -
	CONC,		    // .
	GREATER,	    // >
	GREQ,		    // >=
	LESS,	        // <
	LEEQ,           // <=
    TYPE_EQ,        // ===
	NTYPE_EQ,	    // !==
	DOLLAR,		    // $
	STOP,
	NONTERM,
	ERROR
} prec_symbs_t;

// Datatypes
typedef enum {
	INT_TYPE,
    FLOAT_TYPE,
    STRING_TYPE,
	UNDEFINED_TYPE
} prec_datatypes_t;

// Converts datatypes of tokens to datatypes in structure prec_datatypes_t.
prec_datatypes_t get_data_type();

// Finds appropriate rule to reduce the expression.
prec_rules_t get_rule();

// Reduces operands and operators according to the rule
int reduce_operation();

// Function collecting all the necessary information to reduce the expression: number of operands and rule.
int start_reducing();

// Converts tokens to symbols of precedence table with prec_symbs_t datatype. 
prec_symbs_t get_symbol();

// The main function, where precedence rules must be found.
int parse_expression();

#endif // EXPRESSIONS_H