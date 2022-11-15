#include <stdio.h>
#include <stdlib.h>
#include "str.h"
#include "parser.h"
#include "error.h"
#include "expressions.h"
#include "symtable.h"
#include "symstack.h"

char prec_table[PREC_TABLE_SIZE][PREC_TABLE_SIZE] = {
        //id   (    )    *    /    +    -    .    >   >=    <   <=   ===  !==   $
        {'e', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>'}, // id

        {'<', '<', '=', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', 'e'}, // (

        {'<', 'e', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>'}, // )

        {'<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>'}, // *

        {'<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>'}, // /

        {'<', '<', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>'}, // +

        {'<', '<', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>'}, // -

        {'<', '<', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>'}, // .

        {'<', '<', '>', '<', '<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>'}, // >

        {'<', '<', '>', '<', '<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>'}, // >=

        {'<', '<', '>', '<', '<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>'}, // <

        {'<', '<', '>', '<', '<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>'}, // <=

        {'<', '<', '>', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '>'}, // ===

        {'<', '<', '>', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '>'}, // !==

        {'<', '<', 'e', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', 'e'}, // $

};

prec_symbs_t get_symbol(token_type_t token)
{
	switch (token) {
	case TOKEN_ID:
		return ID;
	case TOKEN_LEFT_BR:
		return LEFT_BRACKET;
	case TOKEN_RIGHT_BR:
		return RIGHT_BRACKET;
	case TOKEN_MUL:
		return MUL;
	case TOKEN_DIV:
		return DIV;
	case TOKEN_PLUS:
		return PLUS;
	case TOKEN_MINUS:
		return MINUS;
	case TOKEN_CONC:
		return CONC;
	case TOKEN_GREATER:
		return GREATER;
	case TOKEN_GREATER_EQ:
		return GREQ;
	case TOKEN_LESS:
		return LESS;
	case TOKEN_LESS_EQ:
		return LEEQ;
	}
}

/**
 * Function converts token type to data type.
 *
 */
prec_datatypes_t get_data_type(token_t* token /*, from parser*/)
{
	//TData* symbol; // data from parser
	switch (token->type)
	{
		case TOKEN_ID:
			symbol = symt_search(); // TODO
			if (symbol == NULL)
				return UNDEFINED_TYPE;
			return symbol->type;

		case TOKEN_TYPE_INT:
			return INT_TYPE;

		case TOKEN_TYPE_FLOAT:
			return FLOAT_TYPE;

		case TOKEN_TYPE_STRING:
			return STRING_TYPE;
		default:
			return UNDEFINED_TYPE;
		}
}

prec_rules_t get_rule(int num_of_symbols_in_rule, prec_stack_item_t* first, prec_stack_item_t* second, prec_stack_item_t* third){
	switch (num_of_symbols_in_rule)
	{
	case 1: // E -> id
		if (first->symb == ID || first->symb == INT_TYPE || first->symb == FLOAT_TYPE || first->symb == STRING_TYPE)
			return ID_R;
		return NOT_RULE;

	case 3: // E -> (E)
		if (first->symb == LEFT_BRACKET && second->symb == NONTERM && third->symb == RIGHT_BRACKET)
			return BRACKETS_R;
		if (first->symb == NONTERM && third->symb == NONTERM)
		{
			switch (second->symb)
			{
			case MUL: 			// E -> E * E
				return MUL_R;
			case DIV: 			// E -> E / E
				return DIV_R;	
			case PLUS: 			// E -> E + E
				return PLUS_R;
			case MINUS: 		// E -> E - E
				return MINUS_R;
			case CONC: 			// E -> E . E
				return CONC_R;
			case GREATER: 		// E -> E > e
				return GREATER_R;
			case GREQ: 			// E -> E >= E
				return GREQ_R;
			case LESS: 			// E -> E < E
				return LESS_R;
			case LEEQ: 			// E -> E <= E
				return LEEQ_R;
			case TYPE_EQ: 		// E -> E === E
				return TYPE_EQ_R;
			case NTYPE_EQ: 		// E -> E !== E
				return NTYPE_EQ_R;
			case EQ_R: 			// E -> E = E 
				return EQ_R;   
			}
		}
		return NOT_RULE;
	}
	return NOT_RULE;
}

prec_index_t get_prec_table_index (){
	switch ()
	{
		case :
		
			break;
	
		default:
			break;
	}

}

int parse_expression(){


}
