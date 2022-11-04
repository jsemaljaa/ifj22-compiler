#include "scanner.h"
#include "parser.h"
#include <stdio.h>
#include "error.h"
#include "str.h"

//TODO str_print() + fix get_next_token assignment to token
int main(int argc, char **argv){
    token_t *token ;

    while(token->type != TOKEN_END_OF_FILE){
        token = get_next_token(token);
    }
    fprintf(stderr, "\nNo error in scanner\n");
    return 0;
}