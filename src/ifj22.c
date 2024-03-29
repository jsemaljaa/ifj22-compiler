/*
 * IFJ2022/project/ifj22.c
 *
 * @brief Main program
 *
 * @author Vinogradova Alina <xvinog00@vutbr.cz>
 */


#include <stdio.h>
#include "scanner.h"
#include "expressions.h"
#include "parser.h"
#include "error.h"
#include "str.h"

int main(int argc, char **argv)
{
    token_t token;
    string_t string, *str = &string;
    str_init(&string);
    set_dynamic_string(&string);
    int code;

    code = parse();
    //printf("Code is %d\n", code);
    //printf("Is there an error? %s\n", code != NO_ERRORS ? "Yes" : "No");
    
    str_free(&string);
    // exit_error(code);
    return code != NO_ERRORS ? code : NO_ERRORS;

}