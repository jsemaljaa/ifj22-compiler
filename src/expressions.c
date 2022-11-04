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

static symbol_prec_table get_symbol(token_type_t token)
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
	case TOKEN_DOLLAR:
		return DOLLAR;
	}
}




