#include "scanner.h"
#include "parser.h"
#include <stdio.h>
#include "error.h"
#include "str.h"

int main(){

    FILE *f = stdin;
    set_source_file(f);

    token_t token;
    int ret;
    str_init(&token.attribute.string, 20);

    do {
        ret = get_next_token(&token);
        if (ret == LEXICAL_ERROR) {
            err(LEXICAL_ERROR);
            return ret;
        }
    } while (token.type != END_OF_FILE);

    str_free(&token.attribute.string);
    fprintf(stderr, "\nNo error in scanner\n");
}