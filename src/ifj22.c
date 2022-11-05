#include "scanner.h"
#include "parser.h"
#include <stdio.h>
#include "error.h"
#include "str.h"

//TODO str_print() + fix get_next_token assignment to token
int main(){
    token_t token;
    str_init(&token.attribute.string);
    
    int ret;
    do{
        ret = get_next_token(&token);
        if(ret == LEXICAL_ERROR) exit_error(LEXICAL_ERROR);
    } while(token.type != TOKEN_END_OF_FILE);
    fprintf(stderr, "\nNo error in scanner\n");
    
    str_clear(&token.attribute.string);
    return 0;
}