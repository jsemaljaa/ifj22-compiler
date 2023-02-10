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

#define POP_TIMES(n)				\
	for(int i = 0; i < n; i++){		\
		prec_stack_pop(&stack);		\
	}								\

#define PUSH_STOP_SIGN()										\
	prec_stack_push(&stack, STOP, UNDEFINED_TYPE);		\

prec_stack_t stack;
token_t token;

int resInt;
float resFloat;
int code;
/* src:
 * 		0 from expression
 * 		1 from condition
*/
int src;


// Precedence table
char prec_table[PREC_TABLE_SIZE][PREC_TABLE_SIZE] = {
        //id   (    )    *    /    +    -    .    >   >=    <   <=   ===  !==   $
        {'e', 'e', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>'}, // id

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


void printf_stack(){
	printf("stack: ");
	prec_stack_item_t *debug = prec_stack_head(&stack);
	while(debug != NULL){
		printf("%d ", debug->symb);
		debug = debug->next;
	}
	printf("\n");
}

prec_symbs_t get_symbol(token_t token)
{
	switch (token.type) {
		case TOKEN_TYPE_INT:
		case TOKEN_TYPE_FLOAT:
		case TOKEN_TYPE_STRING:
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
			return ERROR;
		}
}

prec_datatypes_t get_data_type(token_t token, htable *table)
{
	ht_item_t* symbol = malloc(sizeof(ht_item_t));
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

prec_rules_t get_rule(int num_of_symbols_in_rule, prec_stack_item_t* first, prec_stack_item_t* second, prec_stack_item_t* third){
	switch (num_of_symbols_in_rule)
	{
	case 1: // E -> id
		if (first->symb == ID || first->datatype == INT_TYPE || first->datatype == FLOAT_TYPE || first->datatype == STRING_TYPE)
			return ID_R;
		return NOT_RULE;

	case 3: 
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
			}
		}
		return NOT_RULE;
	}
	return NOT_RULE;
}



int reduce_operation(prec_stack_item_t *headStack, prec_rules_t rule){
	prec_stack_item_t *item;
	switch (rule)
	{
	case NOT_RULE:
		return SYNTAX_ERROR;
	
	case ID_R:
		item = prec_stack_head(&stack);
		POP_TIMES(2);
		prec_stack_push(&stack, NONTERM, item->datatype);
		return NO_ERRORS;

	case MUL_R:
	case DIV_R:
	case PLUS_R:
	case MINUS_R:
		POP_TIMES(4);
		prec_stack_push(&stack, NONTERM, UNDEFINED_TYPE);
		return NO_ERRORS;
	case GREATER_R:
	case GREQ_R:
	case LESS_R:
	case LEEQ_R:
	case TYPE_EQ_R:
	case NTYPE_EQ_R: 
		POP_TIMES(4);
		prec_stack_push(&stack, NONTERM, UNDEFINED_TYPE);
		return NO_ERRORS;
	default:
		break;
	}    

	return NO_ERRORS;
}

int start_reducing(prec_stack_item_t *headStack){
	int symbolsCount = 0;
	prec_stack_item_t *tmp = headStack;
	
	// count the number of symbols after stop sign to get a correct rule
	while (tmp->symb != STOP){
		symbolsCount++;
		tmp = tmp->next;
	}   

	prec_rules_t rule = get_rule(symbolsCount, headStack, headStack->next, headStack->next->next);

	code = reduce_operation(headStack, rule);
	CHECK_ERROR(code);

	return NO_ERRORS;
}

int parse_expression(htable *table, int from){
	/*	from:
	 * 		0 - from expression
	 * 		1 - from condition 
	*/
	src = from;
	token_t first;
start_expr:
	if(token.type == TOKEN_ASSIGN) GET_TOKEN();
	if(token.type == TOKEN_ID && token.attribute.string->str[0] != '$') return -1;

	prec_stack_init(&stack);
	prec_stack_push(&stack, DOLLAR, UNDEFINED_TYPE); // push a dollar sign

	first = token;

	prec_symbs_t tokenSymb = get_symbol(token);
	prec_datatypes_t tokenDatatype = get_data_type(token, table);
	if (tokenSymb >= 3 && tokenSymb <= 13) return SEM_OTHER_ERROR; 
	if (tokenSymb == ID) PUSH_STOP_SIGN();
	prec_stack_push(&stack, tokenSymb, tokenDatatype); // push the first symbol
	
	prec_stack_item_t *headStack = prec_stack_head(&stack);

	bool endInput, exprStart; 

	// while stack doesn't look like E$ get tokens and reduce operands according to the rules
 	while (!prec_stack_is_empty(&stack))
 	{

skip_step:
		if(exprStart){
			exprStart = false;
			prec_stack_free(&stack);
			goto start_expr;	
		}

		GET_TOKEN();

		if(token.type == TOKEN_ASSIGN){
			exprStart = true;
			goto skip_step;
		} 

end_input:
		endInput = token.type == TOKEN_SEMICOLON || token.type == TOKEN_LEFT_BR;
		
		
		if(endInput){ // the end of the expression
			while(!prec_stack_is_empty(&stack)){
				headStack = prec_stack_head(&stack);    
				code = start_reducing(headStack);
				CHECK_ERROR(code);
			}				
			return NO_ERRORS;
			} else {
expr_proc:  
			tokenSymb = get_symbol(token);
			tokenDatatype = get_data_type(token, table);
			
			if(tokenSymb == ERROR) return SYNTAX_ERROR;
					
			switch (prec_table[headStack->symb][tokenSymb]){
			case '<': // shift
				headStack = prec_stack_head(&stack); 
				if (tokenSymb == ID) PUSH_STOP_SIGN();
				if (headStack->symb == NONTERM){
					POP_TIMES(1);
					PUSH_STOP_SIGN();
					prec_stack_push(&stack, NONTERM, UNDEFINED_TYPE);
				} else if (headStack->symb == LEFT_PAR && tokenSymb >= 3 && tokenSymb <= 13) return SYNTAX_ERROR; 		
				prec_stack_push(&stack, tokenSymb, tokenDatatype);
				headStack = prec_stack_first_terminal(&stack); 
				break;
			
			case '>': // reduce
				headStack = prec_stack_head(&stack);
				code = start_reducing(headStack);
				CHECK_ERROR(code);
				headStack = prec_stack_first_terminal(&stack);
				goto expr_proc;
				break;
			
			case '=': // equal
				if (tokenSymb == ID) return SEM_OTHER_ERROR;
				POP_TIMES(2);
				prec_stack_push(&stack, NONTERM, UNDEFINED_TYPE);
				headStack = prec_stack_head(&stack);
				goto skip_step;
				break;
				
			case 'e': // error
				prec_stack_free(&stack);
				return SYNTAX_ERROR;

			default:
				prec_stack_free(&stack);
				return SYNTAX_ERROR;
			}
		}
	}

	
	prec_stack_free(&stack);
}
