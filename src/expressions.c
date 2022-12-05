/*
 * IFJ2022/project/expressions.c 
 * 
 * @brief Precedence analysis implementation
 * 
 * @author Vinogradova Alina <xvinog00@vutbr.cz>
 * @author Mazurava Maryia <xmazur08@vutbr.cz>
 */


#include "expressions.h"
#include "symstack.h"
#include "parser.h"

prec_stack_t stack;
token_t token;

int code;

char prec_table[PREC_TABLE_SIZE][PREC_TABLE_SIZE] = {
        //id   (    )    *    /    +    -    .    >   >=    <   <=   ===  !==   $    i    f    s
        {'e', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', 'e', 'e', 'e'}, // id

        {'<', '<', '=', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', 'e', '<', '<', '<'}, // (

        {'<', 'e', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', 'e', 'e', 'e'}, // )

        {'<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '<', 'e'}, // *

        {'<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '<', 'e'}, // /

        {'<', '<', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '<', 'e'}, // +

        {'<', '<', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '<', 'e'}, // -

        {'<', '<', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', 'e', 'e', '<'}, // .

        {'<', '<', '>', '<', '<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', 'e'}, // >

        {'<', '<', '>', '<', '<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', 'e'}, // >=

        {'<', '<', '>', '<', '<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', 'e'}, // <

        {'<', '<', '>', '<', '<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', 'e'}, // <=

        {'<', '<', '>', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '>', '<', '<', 'e'}, // ===

        {'<', '<', '>', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '>', '<', '<', 'e'}, // !==

        {'<', '<', 'e', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', 'e', '<', '<', '<'}, // $

		{'<', '<', '>', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '>', 'e', 'e', 'e'}, // i

        {'<', '<', '>', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '>', 'e', 'e', 'e'}, // f

        {'<', '<', 'e', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', 'e', 'e', 'e', 'e'}, // s


};

prec_symbs_t get_symbol(token_t token)
{
	switch (token.type) {
		case TOKEN_ID:
			return ID;
		case TOKEN_LEFT_PAR:
			return LEFT_PAR;
		case TOKEN_RIGHT_PAR:
			return RIGHT_PAR;
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
		case TOKEN_EQUAL:
			return TYPE_EQ;
		case TOKEN_NOT_EQUAL:
			return NTYPE_EQ;
		default:
			printf_token_debug(token);
			return ERROR;
		}
}

/**
 * Function converts token type to data type.
 *
 */
prec_datatypes_t get_data_type(token_t token, htable *table)
{
	ht_item_t* symbol;
	switch (token.type)
	{
		case TOKEN_ID:
			symbol = symt_search(table, token.attribute.string->str);
			if (symbol == NULL)
				return UNDEFINED_TYPE;
			switch (symbol->type)
			{
			case INTEGER_DT:
				return INT_TYPE;
			case FLOAT_DT:
				return FLOAT_TYPE;
			case STRING_DT:
				return STRING_TYPE;
			default:
				return UNDEFINED_TYPE;
			}
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
		if (first->symb == LEFT_PAR && second->symb == NONTERM && third->symb == RIGHT_PAR)
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


int parse_expression(htable *table){
	// token here is a start of the expression
	
 	prec_stack_init(&stack);
 	
	prec_stack_push(&stack, STOP, UNDEFINED_DT); // push a dollar sign

	prec_symbs_t tokenSymb = get_symbol(token);
	prec_datatypes_t tokenDatatype = get_data_type(token, table);

	prec_stack_push(&stack, tokenSymb, tokenDatatype); // push the first symbol


	prec_stack_item_t *headStack;

	bool exit = false;

    // ID is always going to the stack with the STOP sign before
 	while (1)
 	{	
 		headStack = prec_stack_head(&stack);
		
		GET_TOKEN();
		if(token.type == TOKEN_SEMICOLON || token.type == TOKEN_LEFT_BR) return NO_ERRORS;

		tokenSymb = get_symbol(token);
		tokenDatatype = get_data_type(token, table);
		prec_stack_push(&stack, tokenSymb, tokenDatatype);
		// if(get_symb(token) == ERROR) return SYNTAX_ERROR;

		printf("input: %d \tstack: %d\t", tokenSymb, headStack->symb);


		if((prec_table[tokenSymb][headStack->symb]) == '<')
		{
			printf("found symbol <\n");
			exit = true;
			// shift_operation(); // with putting of the STOP symbol to it(it is not a dollar sign)
            // small function, we can write it up here
		}
		else if((prec_table[tokenSymb][headStack->symb]) == '>')
		{
			printf("found symbol >\n");
			exit = true;
         // count the number of symbols after the stop sign
         	//rule = get_rule();
			//reduce_operation(); // reducing the stack according to the rule
		}
		else if((prec_table[tokenSymb][headStack->symb]) == 'e')
		{
			printf("found symbol e\n");
			exit = true;

			// return SYNTAX_ERROR; // ????
		}
		else if((prec_table[tokenSymb][headStack->symb]) == '=')
		{
			printf("found symbol =\n");
			exit = true;
			// equal_operation(); // the main function is to reduce everything between the () ???????
		} else {
			printf("\nerror\n");
			printf("input: %d \tstack: %d\t", tokenSymb, headStack->symb);
			prec_stack_free(&stack);
			return SYNTAX_ERROR;
		}
	
	}


	prec_stack_free(&stack);
}
