#include "scanner.h"
#include "parser.h"
#include <stdio.h>
#include "error.h"
#include "str.h"

static const char *list[] = {
    "PLUS",
    "MINUS",
    "MUL",
    "DIV",
    "EQUAL",
    "NOT_EQUAL",
    "LESS",
    "GREATER",
    "LESS_EQ",
    "GREATER_EQ",
    "LEFT_PAR",
    "RIGHT_PAR",
    "SEMICOLON",
    "COLON",
    "ASSIGN",
    "COMMA",
    "CONC",
    "LEFT_BR",
    "RIGHT_BR",
    "DO_QUOTES",
    "QUEST",
    "ID",
    "KEY_W",
    "TYPE_INT",
    "TYPE_FLOAT",
    "TYPE_STRING",
    "END_OF_FILE", 
    "PROLOG"};

// TODO str_print() + fix get_next_token assignment to token
int main(int argc, char **argv)
{
    token_t token;
     string_t string, *str = &string;
     str_init(&string);
     set_dynamic_string(&string);
    while (token.type != TOKEN_END_OF_FILE)
    {
        //get_next_token(&token);
       if( get_next_token(&token) == 1){
        exit_error(LEXICAL_ERROR);
       }
        
        printf("Token: %s \n", list[token.type]);
        if (token.type == TOKEN_TYPE_INT){
            printf("Value: %d \n", token.attribute.integer);
        }
        if (token.type == TOKEN_TYPE_FLOAT){
            printf("Value: %f \n", token.attribute.decimal);
        }
        if (token.type == TOKEN_KEY_W){
            printf("Value: %d \n", token.attribute.keyword);
        }
        if(token.type == TOKEN_ID){
            printf("Value: %s \n", token.attribute.string->str);
        }
        if(token.type == TOKEN_TYPE_STRING){
            printf("Value: %s \n", token.attribute.string->str);
        }
        //if (token.type == TOKEN_END_OF_FILE)
           // break;
    }
    fprintf(stderr, "\nNo error in scanner\n");
    return 0;
}