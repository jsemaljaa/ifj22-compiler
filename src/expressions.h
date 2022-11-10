#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#define PREC_TABLE_SIZE 15

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
	LESS_R, 		// 8. E -> E < E
	LEEQ_R,			// 9. E -> E <= E
	GREATER_R, 		// 10. E -> E > E
	GREQ_R, 	    // 11. E -> E => E
	TYPE_EQ_R,     	// 12. E -> E === E
	NTYPE_EQ_R,   	// 13. E -> E !== E
	EQ_R            // 14. E -> E = E
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
	STOP,		    // @
	NONTERM
} prec_symbs_t;

typedef enum {
	INT_TYPE,
    FLOAT_TYPE,
    STRING_TYPE,
	UNDEFINED_TYPE
} prec_datatypes_t;

typedef enum {
	I_plus_minus,	// 0 +-
	I_mul_div,		// 1 */
	I_rel_op,		// 2 RO
	I_comp_op,		// 3 CO
	I_open_par,		// 4 (
	I_close_par,	// 5 )
	I_data,			// 6 i
	I_dollar		// 7 $
} PT_idx; //precedence table index



/**
 * Function converts token type to data type.
 */
// data_type_ table get_data_type(Token* token, PData* data)






#endif // EXPRESSIONS_H