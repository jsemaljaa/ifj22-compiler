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

#define PREC_TABLE_SIZE 18

int parse_expression(htable *table);

/**
 * Rules for analysis.
 */
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

/*
 * Symbols for precendece analysis.
 */
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
	INT_T,			// int number
	FLOAT_T,		// float number
	STRING_T,		// string
	DOLLAR,		    // $
	STOP,
	NONTERM,
	ERROR
} prec_symbs_t;

typedef enum {
	INT_TYPE,
    FLOAT_TYPE,
    STRING_TYPE,
	UNDEFINED_TYPE
} prec_datatypes_t;


#endif // EXPRESSIONS_H