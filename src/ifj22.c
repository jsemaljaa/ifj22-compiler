#include "scanner.h"
#include "parser.h"
#include <stdio.h>
#include "error.h"
#include "str.h"

int main(int argc, char **argv){
    token_t token;

    while(token.type != END_OF_FILE){
        token = get_next_token();
    }
    fprintf(stderr, "\nNo error in scanner\n");
    return 0;
}