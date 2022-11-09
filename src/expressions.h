#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#define PREC_TABLE_SIZE 15

/**
 * Rules for analysis.
 */
typedef enum {
	NOT_RULE,		// rule doesn't exist
	ID_R,           // E -> id
	BRACKETS_R, 	// E -> (E)
	MUL_R,		    // E -> E * E
	DIV_R, 		    // E -> E / E
	PLUS_R, 		// E -> E + E
	MINUS_R, 	    // E -> E - E
	CONC_R, 	    // E -> E . E
	LESS_R, 		// E -> E < E
	LEEQ_R,			// E -> E <= E
	GREATER_R, 		// E -> E > E
	GREQ_R, 	    // E -> E => E
	TYPE_EQ_R,     	// E -> E === E
	NTYPE_EQ_R,   	// E -> E !== E
	EQ_R            // E -> E = E
} prec_rules_t;


/*
 * Symbols for precendece analysis.
 */
typedef enum {
	ID,	            // id
	LEFT_BRACKET,	// (
	RIGHT_BRACKET,	// )
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
} prec_symbs_t;

typedef enum {
	INT_TYPE,
    FLOAT_TYPE,
    STRING_TYPE,
	UNDEFINED_TYPE
} prec_datatypes_t;



/**
 * Function converts token type to data type.
 */
// data_type_ table get_data_type(Token* token, PData* data)






#endif // EXPRESSIONS_H