#include "scanner.h"
#include "parser.h"
#include <stdio.h>
#include "error.h"
#include "str.h"


htable globalSymTab;
htable localSymTab;

// TODO str_print() + fix get_next_token assignment to token
int main(int argc, char **argv)
{
    symt_init(&globalSymTab);
    symt_init(&localSymTab);

    token_t token;
    string_t string, *str = &string;
    str_init(&string);
    set_dynamic_string(&string);
    while (token.type != TOKEN_END_OF_FILE)
    {
        if(get_next_token(&token) == 1){
            exit_error(LEXICAL_ERROR);
        }
        printf("Token: %s \n", list[token.type]);
        
        switch (token.type)
        {
        case TOKEN_TYPE_INT:
            printf("Value: %d \n", token.attribute.integer);
            break;
        
        case TOKEN_TYPE_FLOAT:
            printf("Value: %f \n", token.attribute.decimal);
            break;

        case TOKEN_KEY_W:
            printf("Value: %d \n", token.attribute.keyword);
            break; 
        
        case TOKEN_ID:
            symt_add_symb(&globalSymTab, token.attribute.string);
            ht_item_t *item = symt_search(&globalSymTab, token.attribute.string->str);
            printf("\tItem in Htable is %s\n", item->key);
            break;
        case TOKEN_TYPE_STRING:
            printf("Value: %s \n", token.attribute.string->str);
            break;
        
        default:
            break;
        }
    }

    fprintf(stderr, "\nNo error in scanner\n");
    return NO_ERRORS;
}