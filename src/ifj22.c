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
    parser_data_t pData;
    symt_init(&pData.globalSymt);
    symt_init(&pData.localSymt);


    token_t token;
    string_t string, *str = &string;
    str_init(&string);
    set_dynamic_string(&string);
    while (token.type != TOKEN_END_OF_FILE)
    {
        // parse(&pData);
        get_next_token(&token);
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
            printf("Value: %s \n", token.attribute.string->str);
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