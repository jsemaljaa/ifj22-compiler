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
    "PROLOG",
    "LESS_QUEST"};

// TODO str_print() + fix get_next_token assignment to token
int main(int argc, char **argv)
{
    token_t token;
    while (token.type != TOKEN_END_OF_FILE)
    {
//get_next_token(&token);
       if( get_next_token(&token) == 1){
        printf("error\n");
        return LEXICAL_ERROR;
       }
        
        printf("Token: %s \n", list[token.type]);

         //printf("Value: %s \n", token.attribute.string->str);
        //if (token.type == TOKEN_END_OF_FILE)
           // break;
    }
    fprintf(stderr, "\nNo error in scanner\n");
    return 0;
}