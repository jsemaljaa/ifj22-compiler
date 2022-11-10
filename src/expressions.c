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
	//case TOKEN_DOLLAR:
		//return DOLLAR;
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

prec_rules_t get_rule(int num_of_symbols_in_rule){
	switch (num_of_symbols_in_rule)
	{
	case 1: 
		if (op1->symbol == ID || op1->symbol == INT_TYPE || op1->symbol == FLOAT_TYPE || op1->symbol == STRING_TYPE)
			return ID_R;
		return NOT_RULE;

	case 3:
		if (op1->symbol == LEFT_BRACKET && op2->symbol == NONTERM && op3->symbol == RIGHT_BRACKET)
			return BRACKETS_R;
		if (op1->symbol == NONTERM && op3->symbol == NONTERM)
		{
			switch (op2->symbol)
			{
			case MUL:
				return MUL_R;
			case DIV:
				return DIV_R;	
			case PLUS:
				return PLUS_R;
			case MINUS:
				return MINUS_R;
			case CONC:
				return CONC_R;
			case GREATER:
				return GREATER_R;
			case GREQ:
				return GREQ_R;
			case LESS:
				return LESS_R;
			case LEEQ:
				return LEEQ_R;
			case TYPE_EQ:
				return TYPE_EQ_R;
			case NTYPE_EQ:
				return NTYPE_EQ_R;
			case EQ_R:
				return EQ_R;   
			}
		}
		return NOT_RULE;
	}
	return NOT_RULE;
}




