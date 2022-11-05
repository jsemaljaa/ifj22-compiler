/*
 * IFJ2022/project/scaner.c
 * 
 * @brief Scanner interface implementation
 * 
 * @author Taipova Evgenia <xtaipo00@vutbr.cz>
 */


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "str.h"
#include "scanner.h"

// static string_t *attribute;

static int exit_free(int code, string_t *attr)
{
    str_free(attr);
    return code;
}

static int get_identifier_or_keyword(string_t *str, token_t *token){
    // kontrola, zda se nejedna o klicove slovo
    if (str_cmp_const_str(str, CONST_ELSE) == 0){
        token->attribute.keyword = K_ELSE;
        token->type = TOKEN_KEY_W;
    } else if (str_cmp_const_str(str, CONST_FLOAT) == 0){
        token->attribute.keyword = K_FLOAT;
        token->type = TOKEN_KEY_W;
    } else if (str_cmp_const_str(str, CONST_FUNCTION) == 0){
        token->attribute.keyword = K_FUNCTION;
        token->type = TOKEN_KEY_W;
    } else if (str_cmp_const_str(str, CONST_IF) == 0){
        token->attribute.keyword = K_IF;
        token->type = TOKEN_KEY_W;
    } else if (str_cmp_const_str(str, CONST_INT) == 0){
        token->attribute.keyword = K_INT;
        token->type = TOKEN_KEY_W;
    } else if (str_cmp_const_str(str, CONST_NULL) == 0){
        token->attribute.keyword = K_NULL;
        token->type = TOKEN_KEY_W;
    } else if (str_cmp_const_str(str, CONST_RETURN) == 0){
        token->attribute.keyword = K_RETURN;
        token->type = TOKEN_KEY_W;
    } else if (str_cmp_const_str(str, CONST_STRING) == 0){
        token->attribute.keyword = K_STRING;
        token->type = TOKEN_KEY_W;
    } else if (str_cmp_const_str(str, CONST_VOID) == 0){
        token->attribute.keyword = K_VOID;
        token->type = TOKEN_KEY_W;
    } else if (str_cmp_const_str(str, CONST_WHILE) == 0){
        token->attribute.keyword = K_WHILE;
        token->type = TOKEN_KEY_W;
    } else // jednalo se skutecne o identifikator
        token->type = TOKEN_ID;

    if(token->type == TOKEN_KEY_W) return exit_free(NO_ERRORS, str);

    if(str_copy_string(&token->attribute.string, str) == 1) return exit_free(INTERNAL_ERROR, str);

    return exit_free(NO_ERRORS, str);
}

int get_next_token(token_t *token)
// hlavni funkce lexikalniho analyzatoru
{
    // str_print(&token->attribute.string);
    token->attribute.keyword = K_EMPTY;
    token->type = TOKEN_EMPTY;
    
    // String attribute inicialization
    string_t ptr, *attr = &ptr;
    if(str_init(attr) == 1)  return exit_free(LEXICAL_ERROR, attr);

    // token.attribute.string = attribute;
    int state = STATE_START;
    
    int c;

    // vymazeme obsah atributu a v pripade identifikatoru
    // budeme postupne do nej vkladat jeho nazev
    //   

    while (1)
    {
        //printf("char: %c\nstring pointer is %p\n", c, token->attribute.string.attr);
        printf("-- BEGIN DEBUG: TOKEN INFO\n");
        printf("   token type: %d\n", token->type);
        printf("   token keyw: %d\n", token->attribute.keyword);
        // str_print(&token->attribute.string);
        printf("-- END DEBUG: TOKEN INFO\n");
        printf("state: %d || symbol: %c\n", state, c);
        // nacteni dalsiho znaku
        c = getc(stdin);
        switch (state)
        {
        case STATE_START:
            // zakladni stav automatu
            if (isspace(c))
                state = STATE_START;

            else if (c == '+')
            {
                token->type = TOKEN_PLUS;
                return exit_free(NO_ERRORS, attr);
            }

            else if (c == '-')
            {
                token->type = TOKEN_MINUS;
                return exit_free(NO_ERRORS, attr);
            }

            else if (c == '*')
            {
                token->type = TOKEN_MUL;
                return exit_free(NO_ERRORS, attr);
            }

            else if (c == '(')
            {
                token->type = TOKEN_LEFT_PAR;
                return exit_free(NO_ERRORS, attr);
            }

            else if (c == ')')
            {
                token->type = TOKEN_RIGHT_PAR;
                return exit_free(NO_ERRORS, attr);
            }

            else if (c == ':')
            {
                token->type = TOKEN_COLON;
                return exit_free(NO_ERRORS, attr);
            }

            else if (c == '{')
            {
                token->type = TOKEN_LEFT_BR;
                return exit_free(NO_ERRORS, attr);
            }

            else if (c == '}')
            {
                token->type = TOKEN_RIGHT_BR;
                return exit_free(NO_ERRORS, attr);
            }

            else if (c == ';')
            {
                token->type = TOKEN_SEMICOLON;
                return exit_free(NO_ERRORS, attr);
            }

            else if (c == EOF)
            {
                token->type = TOKEN_END_OF_FILE;
                return exit_free(NO_ERRORS, attr);
            }

            else if (c == '.')
            {
                token->type = TOKEN_CONC;
                return exit_free(NO_ERRORS, attr);
            }

            else if (c == ',')
            {
                token->type = TOKEN_COMMA;
                return exit_free(NO_ERRORS, attr);
            }

            else if (c == '<')
                state = STATE_LESS_THAN;

            else if (c == '>')
                state = STATE_MORE_THAN;

            else if (c == '=')
                state = STATE_EQUAL_START;

            else if (c == '!')
                state = STATE_NOT_EQUAL_START;

            else if (c == '?')
                state = STATE_QUEST;

            else if (c == '"')
                state = STATE_STRING_START;

            else if (c == '/')
                state = STATE_DIV;

            else if (isdigit(c))
                state = STATE_NUMBER;

            else if (isalpha(c) || c == '_' || c == '$'){  
                if(str_add_char(attr, (char)c) == 1) return exit_free(INTERNAL_ERROR, attr);
                state = STATE_IDENTIFIER_OR_KEYWORD;
            }
            else
                return exit_free(LEXICAL_ERROR, attr);

            break;

        case STATE_LESS_THAN: // <

            if (c == '=') // <=
            {
                token->type = TOKEN_LESS_EQ;
            }
            else
            {
                ungetc(c, stdin);
                token->type = TOKEN_LESS;
                 
            }
            
            return exit_free(NO_ERRORS, attr);

            break;

        case STATE_MORE_THAN: // >
            // komentar
            if (c == '=') // >=
            {
                token->type = TOKEN_GREATER_EQ;
            }
            else
            {
                ungetc(c, stdin);
                token->type = TOKEN_GREATER;
            }
            return exit_free(NO_ERRORS, attr);

            break;

        case STATE_EQUAL_START: // =
            if (c == '=')
                state = STATE_EQUAL; // ==
            else
            {
                ungetc(c, stdin);
                token->type = TOKEN_ASSIGN;
                  
                return exit_free(NO_ERRORS, attr);
            }
            break;

        case STATE_NOT_EQUAL_START: // !
            if (c == '=')
                state = STATE_NOT_EQUAL; // !=
            else
                return exit_free(LEXICAL_ERROR, attr);
            break;

        case STATE_EQUAL: // ==
            if (c == '='){
                token->type = TOKEN_EQUAL;  // ===
                return exit_free(NO_ERRORS, attr);
            } else 
                return exit_free(LEXICAL_ERROR, attr);
            break;

        case STATE_NOT_EQUAL: // !=
            if (c == '='){
                token->type = TOKEN_NOT_EQUAL;
                return exit_free(NO_ERRORS, attr); // !==
            }
            else 
                return exit_free(LEXICAL_ERROR, attr);
            break;

        case STATE_DIV: // /
            // komentar
            if (c == '/')
                state = STATE_COMMENTARY; // //

            else if (c == '*')
                state = STATE_BLOCK_COMMENTARY; // /*

            else if (c == EOF)
                
                return exit_free(LEXICAL_ERROR, attr);

            ungetc(c, stdin);

            token->type = TOKEN_DIV;
            
                //return exit_free(NO_ERRORS, attr);
            break;

        case STATE_COMMENTARY: // //
            if (c == 10 || c == EOF)
            {
                state = STATE_START;
                ungetc(c, stdin);
            }
            break;

        case STATE_BLOCK_COMMENTARY: // /*
            if (c == '*')
                state = STATE_BLOCK_COMMENTARY_LEAVE; // /**
            else if (c == EOF)
                return exit_free(LEXICAL_ERROR, attr);
            break;

        case STATE_BLOCK_COMMENTARY_LEAVE: // /**
            if (c == '/')
                state = STATE_START; // /**/
            else if (c == EOF)
                return exit_free(LEXICAL_ERROR, attr);
            else
                state = STATE_BLOCK_COMMENTARY;
            break;

        case STATE_IDENTIFIER_OR_KEYWORD:
            // identifikator nebo klicove slovo
            // str_print(&token->attribute.string);
            // printf("current char is %c\n", c);
            if (isalnum(c) || c == '_'){
                // identifikator pokracuje
                if(str_add_char(attr, (char)c) == 1) exit_error(INTERNAL_ERROR);
            } else
            // konec identifikatoru
            {
                ungetc(c, stdin); // POZOR! Je potreba vratit posledni nacteny znak
                get_identifier_or_keyword(attr, token); 
                state = STATE_START;
            }
            break;

        case STATE_NUMBER:
            if (c == 'e' || c == 'E')
            {
                state = STATE_NUMBER_EXPONENT;
                  
            }
            else if (isdigit(c))
                str_add_char(&token->attribute.string, c);
            else if (c == '.')
            {
                state = STATE_NUMBER_POINT;
                  
            }
            else
            {
                ungetc(c, stdin);
                token->type = TOKEN_TYPE_INT;
                return exit_free(NO_ERRORS, attr);
            }
            break;
        case STATE_NUMBER_POINT:
            if (isdigit(c))
            {
                state = STATE_NUMBER_DOUBLE;
            }
            else
                return exit_free(LEXICAL_ERROR, attr);
            break;

        case STATE_NUMBER_DOUBLE:
            if (isdigit(c))
                str_add_char(&token->attribute.string, c);
            else if (c == 'e' || c == 'E')
            {
                state = STATE_NUMBER_EXPONENT;
            }
            else
            {
                ungetc(c, stdin);
                token->type = TOKEN_TYPE_FLOAT;
                return exit_free(NO_ERRORS, attr);
            }
            break;

        case STATE_NUMBER_EXPONENT:
            if (c == '+' || c == '-')
            {
                state = STATE_NUMBER_EXPONENT_SIGN;
            }
            else if (isdigit(c))
            {
                state = STATE_NUMBER_EXPONENT_FINAL;
            }
            else
                return exit_free(LEXICAL_ERROR, attr);
            break;

        case STATE_NUMBER_EXPONENT_SIGN:
            if (isdigit(c))
            {
                state = STATE_NUMBER_EXPONENT_FINAL;
            }
            else
                return exit_free(LEXICAL_ERROR, attr);
            break;

        case STATE_NUMBER_EXPONENT_FINAL:
            if (isdigit(c))
                str_add_char(&token->attribute.string, c);
            else
            {
                ungetc(c, stdin);
                token->type = TOKEN_TYPE_FLOAT;
                return exit_free(NO_ERRORS, attr);
            }
            break;

        case STATE_STRING_START:
            if (c == '"'){
                token->type = TOKEN_TYPE_STRING;
                return exit_free(NO_ERRORS, attr);
            }
            else if (c == '\\')
                state = STATE_STRING_ESCAPE;
            else if (c < 32)
                return exit_free(LEXICAL_ERROR, attr);
            else if (c == EOF)
                return exit_free(LEXICAL_ERROR, attr);
            else
                  
            break;

        case STATE_STRING_ESCAPE:
            if (c == 'n')
            {
                str_add_char(&token->attribute.string, '\n');
                state = STATE_STRING_START;
            }
            else if (c == 't')
            {
                str_add_char(&token->attribute.string, '\t');
                state = STATE_STRING_START;
            }
            else if (c == '"')
            {
                  
                state = STATE_STRING_START;
            }
            else if (c == '\\')
            {
                  
                state = STATE_STRING_START;
            }
            else if (c == '$')
            {
                  
                state = STATE_STRING_START;
            }
            else if (c == 'x')
            {
                state = STATE_STRING_ESCAPE_X;
                  
            }
            else if (c == '0')
            {
                  
                state = STATE_STRING_ESCAPE_ZERO;
            }
            else if (c == '1' || c == '2' || c == '3')
            {
                  
                state = STATE_STRING_ESCAPE_ONE_THREE;
            }
            else
                return exit_free(LEXICAL_ERROR, attr);
            break;

        case STATE_STRING_ESCAPE_X:
            if (c == '0')
            {
                  
                state = STATE_STRING_ESCAPE_X_ZERO;
            }
            else if ((isdigit(c) && c != '0') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
            {
                  
                state = STATE_STRING_ESCAPE_X_NOT_ZERO;
            }
            else
                 return exit_free(LEXICAL_ERROR, attr);
            break;

        case STATE_STRING_ESCAPE_X_ZERO:
            if ((isdigit(c) && c != '0') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
            {
                state = STATE_STRING_START;
            }
            else
                 return exit_free(LEXICAL_ERROR, attr);
            break;

        case STATE_STRING_ESCAPE_X_NOT_ZERO:
            if (isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
            {
                  
                state = STATE_STRING_START;
            }
            else
                 return exit_free(LEXICAL_ERROR, attr);
            break;

        case STATE_STRING_ESCAPE_ZERO:
            if (c == '0')
            {
                  
                state = STATE_STRING_ESCAPE_ZERO_ZERO;
            }
            else if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7')
            {
                  
                state = STATE_STRING_ESCAPE_REST;
            }
            else
                 return exit_free(LEXICAL_ERROR, attr);
            break;

        case STATE_STRING_ESCAPE_ZERO_ZERO:
            if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7')
            {
                  
                state = STATE_STRING_START;
            }
            else
                 return exit_free(LEXICAL_ERROR, attr);
            break;

        case STATE_STRING_ESCAPE_REST:
            if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7')
            {
                  
                state = STATE_STRING_START;
            }
            else
                 return exit_free(LEXICAL_ERROR, attr);
            break;

        case STATE_STRING_ESCAPE_ONE_THREE:
            if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7')
            {
                  
                state = STATE_STRING_ESCAPE_REST;
            }
            else
                 return exit_free(LEXICAL_ERROR, attr);
            break;
        }
    }
}