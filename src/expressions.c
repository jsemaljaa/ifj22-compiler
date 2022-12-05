#include "expressions.h"
#include "symstack.h"
#include "parser.h"

htable globalS;
htable localS;
token_t token;

int code;

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

prec_symbs_t get_symbol(token_t token)
{
	switch (token.type) {
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
		default:
			return ERROR;
		}
}

/**
 * Function converts token type to data type.
 *
 */
prec_datatypes_t get_data_type(token_t token /*, from parser*/)
{
	ht_item_t* symbol;
	switch (token.type)
	{
		case TOKEN_ID:
			symbol = symt_search(&globalS, token.attribute.string->str);
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

void shift_operation(){
	/*
	1. check if we have a NONTERMINAL on the top of the stack
	   1.1 if yes then add a STOP sign before it
	2. now we can add a symbol to stack
	*/
	// check if type of top_stack is NONTERM to correct pushing STOP
	// if(head == NONTERM)
	// {
	// 	prec_stack_pop();
	// 	prec_stack_push(stop_symbol);
	// 	prec_stack_push();//adding this non terminal again
	// 	prec_stack_push();//adding an operator
	// }
	// else
	// {
	// 	prec_stack_push(stop_symbol);
	// 	prec_stack_push();//adding non terminal
	// }

}

void reduce_operation(){
	/*

	 reduce the operands and symbols until the stop sign according to rule(switch case)
	*/
}

bool compatibility_test(){

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
/*
prec_index_t get_prec_table_index (prec_symbs_t symbol){
    switch (symbol) {
        case ID:
		   return ID;
		case LEFT_BRACKET:
			return LB_INDEX;
		case RIGHT_BRACKET:
			return RB_INDEX;
		case MUL:
			return MUL_INDEX;
		case DIV:
			return DIV_INDEX;
		case PLUS:
			return PLUS_INDEX;
		case MINUS:
			return MINUS_INDEX;
		case CONC:
			return CONC_INDEX;
		case GREATER:
			return GREATER_INDEX;
		case GREQ:
			return GREQ_INDEX;
		case LESS:
			return LESS_INDEX;
		case LEEQ:
			return LEEQ_INDEX;
		case TYPE_EQ:
			return TYPE_EQ_INDEX;
		case NTYPE_EQ:
	 		return NTYPE_EQ_INDEX;
		default:
			return DOLLAR_INDEX;
    }
}
*/

int parse_expression(){

	int number_of_symbols = 0;


	//here we already have an expression from parser looking like: a + b*10/(b+c)
 	prec_stack_t* prec_stack = prec_stack_init();
 	prec_stack_push(&prec_stack, "$", UNDEFINED_DT); // REDO STACK PUSH, push a dollar sign
 	prec_stack_push(&prec_stack); // push the first symbol
	prec_stack_item_t* current;
	prec_stack_item_t* next;
   // ID is always going to the stack with the STOP sign before
 	while (token.type != TOKEN_LEFT_BR || token.type != TOKEN_SEMICOLON)
 	{
		GET_TOKEN();
 		current = symbol_stack_top(&prec_stack); // TODO this func in symstack
		next->symb = get_symbol(token);
		next->datatype = get_data_type(token);
		next->next = NULL;
		
		// if(get_symb(token) == ERROR) return SYNTAX_ERROR;

		if((prec_table[current][next]) == '<')
		{
			shift_operation(); // with putting of the STOP symbol to it(it is not a dollar sign)
            // small function, we can write it up here
		}
		else if((prec_table[current][next]) == '>')
		{
         // count the number of symbols after the stop sign
         	rule = get_rule();
			reduce_operation(); // reducing the stack according to the rule
		}
		else if((prec_table[current][next]) == 'e')
		{
			return SYNTAX_ERROR; // ????
		}
		else if((prec_table[current][next]) == '=')
		{
			equal_operation(); // the main function is to reduce everything between the () ???????
		}
		current++;
	}
		prec_stack_free(&prec_stack);


}
