/*
 * IFJ2022/project/scaner.c
 * 
 * @brief Scanner interface implementation
 * 
 * @author Taipova Evgenia <xtaipo00@vutbr.cz>
 */


#include <stdio.h>
//#include <windows.h>
//#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include "str.h"
#include "scanner.h"

static string_t *attribute;

static int exit_free(int code, string_t *str)
{
    str_free(str);
    return code;
}

int get_next_token(token_t *token)
// hlavni funkce lexikalniho analyzatoru
{
    string_t string, *str = &string;
    if(str_init(str) == 1)  return exit_free(LEXICAL_ERROR, str);

    token->attribute.string = attribute;
    int state = STATE_START;
    int c;
 
    // vymazeme obsah atributu a v pripade identifikatoru
    // budeme postupne do nej vkladat jeho nazev
    //   

    while (1)
    {
        // nacteni dalsiho znaku
        // TODO: connect with setstdinFile
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
                return exit_free(NO_ERRORS, str);
            }

            else if (c == '-')
            {
                token->type = TOKEN_MINUS;
                return exit_free(NO_ERRORS, str);
            }

            else if (c == '*')
            {
                token->type = TOKEN_MUL;
                return exit_free(NO_ERRORS, str);
            }

            else if (c == '(')
            {
                token->type = TOKEN_LEFT_PAR;
                return exit_free(NO_ERRORS, str);
            }

            else if (c == ')')
            {
                token->type = TOKEN_RIGHT_PAR;
                return exit_free(NO_ERRORS, str);
            }

            else if (c == ':')
            {
                token->type = TOKEN_COLON;
                return exit_free(NO_ERRORS, str);
            }

            else if (c == '{')
            {
                token->type = TOKEN_LEFT_BR;
                return exit_free(NO_ERRORS, str);
            }

            else if (c == '}')
            {
                token->type = TOKEN_RIGHT_BR;
                return exit_free(NO_ERRORS, str);
            }

            else if (c == ';')
            {
                token->type = TOKEN_SEMICOLON;
                return exit_free(NO_ERRORS, str);
            }

            else if (c == EOF)
            {
                token->type = TOKEN_END_OF_FILE;
                return exit_free(NO_ERRORS, str);
            }

            else if (c == '.')
            {
                token->type = TOKEN_CONC;
                return exit_free(NO_ERRORS, str);
            }

            else if (c == ',')
            {
                token->type = TOKEN_COMMA;
                return exit_free(NO_ERRORS, str);
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
            {
                state = STATE_NUMBER;
                //printf("%s\n", token->attribute.string);
            }

            else if (isalpha(c) || c == '_' || c == '$')
            {     
                state = STATE_IDENTIFIER_OR_KEYWORD;
                //printf("%s\n", token->attribute.string);
            }

            else
                return exit_free(LEXICAL_ERROR, str);

            break;

        case STATE_LESS_THAN: // <
            // printf("%d --- %d\n", c, state);
            // exit(0);
            if (c == '=') // <=
            {
                token->type = TOKEN_LESS_EQ;
            }
            else
            {
                ungetc(c, stdin);
                token->type = TOKEN_LESS;
                 
            }

            //printf("%s\n", token->attribute.string);
            
            return exit_free(NO_ERRORS, str);

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
            return exit_free(NO_ERRORS, str);

            break;

        case STATE_EQUAL_START: // =
            if (c == '=')
                state = STATE_EQUAL; // ==
            else
            {
                ungetc(c, stdin);
                token->type = TOKEN_ASSIGN;
                  
                return exit_free(NO_ERRORS, str);
            }
            break;

        case STATE_NOT_EQUAL_START: // !
            if (c == '=')
                state = STATE_NOT_EQUAL; // !=
            else
                return exit_free(LEXICAL_ERROR, str);
            break;

        case STATE_EQUAL: // ==
            if (c == '='){
                token->type = TOKEN_EQUAL;  // ===
                return exit_free(NO_ERRORS, str);
            } else 
                return exit_free(LEXICAL_ERROR, str);
            break;

        case STATE_NOT_EQUAL: // !=
            if (c == '='){
                token->type = TOKEN_NOT_EQUAL;
                return exit_free(NO_ERRORS, str); // !==
            }
            else 
                return exit_free(LEXICAL_ERROR, str);
            break;

        case STATE_DIV: // /
            // komentar
            if (c == '/')
                state = STATE_COMMENTARY; // //

            else if (c == '*')
                state = STATE_BLOCK_COMMENTARY; // /*

            else if (c == EOF)
                
                return exit_free(LEXICAL_ERROR, str);

            ungetc(c, stdin);

            token->type = TOKEN_DIV;
            
                //return exit_free(NO_ERRORS, str);
            break;

        case STATE_COMMENTARY: // //
            printf("symbol: %d --- state: %d\n", c, state);
            if (c == 10 || c == EOF)
            {
                state = STATE_START;
                printf("10 SYMBOL!!! --- %d\n", state);
                ungetc(c, stdin);
            } else {}
            break;

        case STATE_BLOCK_COMMENTARY: // /*
            if (c == '*')
                state = STATE_BLOCK_COMMENTARY_LEAVE; // /**
            else if (c == EOF)
                return exit_free(LEXICAL_ERROR, str);
            break;

        case STATE_BLOCK_COMMENTARY_LEAVE: // /**
            if (c == '/')
                state = STATE_START; // /**/
            else if (c == EOF)
                return exit_free(LEXICAL_ERROR, str);
            else
                state = STATE_BLOCK_COMMENTARY;
            break;

        case STATE_IDENTIFIER_OR_KEYWORD:
            // identifikator nebo klicove slovo
            if (isalpha(c) || c == '_')
                // identifikator pokracuje
                str_add_char(token->attribute.string, c);
            else
            // konec identifikatoru
            {
                ungetc(c, stdin); // POZOR! Je potreba vratit posledni nacteny znak

                // kontrola, zda se nejedna o klicove slovo
                if (str_cmp_const_str(token->attribute.string, "else") == 0)
                {
                    token->attribute.keyword = K_ELSE;
                    token->type = TOKEN_KEY_W;
                }

                else if (str_cmp_const_str(token->attribute.string, "float") == 0)
                {
                    token->attribute.keyword = K_FLOAT;
                    token->type = TOKEN_KEY_W;
                }

                else if (str_cmp_const_str(token->attribute.string, "function") == 0)
                {
                    token->attribute.keyword = K_FUNCTION;
                    token->type = TOKEN_KEY_W;
                }

                else if (str_cmp_const_str(token->attribute.string, "if") == 0)
                {
                    token->attribute.keyword = K_IF;
                    token->type = TOKEN_KEY_W;
                }

                else if (str_cmp_const_str(token->attribute.string, "int") == 0)
                {
                    token->attribute.keyword = K_INT;
                    token->type = TOKEN_KEY_W;
                }

                else if (str_cmp_const_str(token->attribute.string, "null") == 0)
                {
                    token->attribute.keyword = K_NULL;
                    token->type = TOKEN_KEY_W;
                }

                else if (str_cmp_const_str(token->attribute.string, "return") == 0)
                {

                    token->attribute.keyword = K_RETURN;
                    token->type = TOKEN_KEY_W;
                }

                else if (str_cmp_const_str(token->attribute.string, "string") == 0)
                {
                    token->attribute.keyword = K_STRING;
                    token->type = TOKEN_KEY_W;
                }
                else if (str_cmp_const_str(token->attribute.string, "void") == 0)
                {
                    token->attribute.keyword = K_VOID;
                    token->type = TOKEN_KEY_W;
                }
                else if (str_cmp_const_str(token->attribute.string, "while") == 0)
                {
                    token->attribute.keyword = K_WHILE;
                    token->type = TOKEN_KEY_W;
                }

                // jednalo se skutecne o identifikator
                token->type = TOKEN_ID;
            }
            break;

        case STATE_NUMBER:
            if (c == 'e' || c == 'E')
            {
                state = STATE_NUMBER_EXPONENT;
                  
            }
            else if (isdigit(c))
                str_add_char(token->attribute.string, c);
            else if (c == '.')
            {
                state = STATE_NUMBER_POINT;
                  
            }
            else
            {
                ungetc(c, stdin);
                token->type = TOKEN_TYPE_INT;
                return exit_free(NO_ERRORS, str);
            }
            break;
        case STATE_NUMBER_POINT:
            if (isdigit(c))
            {
                state = STATE_NUMBER_DOUBLE;
            }
            else
                return exit_free(LEXICAL_ERROR, str);
            break;

        case STATE_NUMBER_DOUBLE:
            if (isdigit(c))
                str_add_char(token->attribute.string, c);
            else if (c == 'e' || c == 'E')
            {
                state = STATE_NUMBER_EXPONENT;
            }
            else
            {
                ungetc(c, stdin);
                token->type = TOKEN_TYPE_FLOAT;
                return exit_free(NO_ERRORS, str);
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
                return exit_free(LEXICAL_ERROR, str);
            break;

        case STATE_NUMBER_EXPONENT_SIGN:
            if (isdigit(c))
            {
                state = STATE_NUMBER_EXPONENT_FINAL;
            }
            else
                return exit_free(LEXICAL_ERROR, str);
            break;

        case STATE_NUMBER_EXPONENT_FINAL:
            if (isdigit(c))
                str_add_char(token->attribute.string, c);
            else
            {
                ungetc(c, stdin);
                token->type = TOKEN_TYPE_FLOAT;
                return exit_free(NO_ERRORS, str);
            }
            break;

        case STATE_STRING_START:
            if (c == '"'){
                token->type = TOKEN_TYPE_STRING;
                return exit_free(NO_ERRORS, str);
            }
            else if (c == '\\')
                state = STATE_STRING_ESCAPE;
            else if (c < 32)
                return exit_free(LEXICAL_ERROR, str);
            else if (c == EOF)
                return exit_free(LEXICAL_ERROR, str);
            else
                  
            break;

        case STATE_STRING_ESCAPE:
            if (c == 'n')
            {
                str_add_char(token->attribute.string, '\n');
                state = STATE_STRING_START;
            }
            else if (c == 't')
            {
                str_add_char(token->attribute.string, '\t');
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
                return exit_free(LEXICAL_ERROR, str);
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
                 return exit_free(LEXICAL_ERROR, str);
            break;

        case STATE_STRING_ESCAPE_X_ZERO:
            if ((isdigit(c) && c != '0') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
            {
                state = STATE_STRING_START;
            }
            else
                 return exit_free(LEXICAL_ERROR, str);
            break;

        case STATE_STRING_ESCAPE_X_NOT_ZERO:
            if (isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
            {
                  
                state = STATE_STRING_START;
            }
            else
                 return exit_free(LEXICAL_ERROR, str);
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
                 return exit_free(LEXICAL_ERROR, str);
            break;

        case STATE_STRING_ESCAPE_ZERO_ZERO:
            if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7')
            {
                  
                state = STATE_STRING_START;
            }
            else
                 return exit_free(LEXICAL_ERROR, str);
            break;

        case STATE_STRING_ESCAPE_REST:
            if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7')
            {
                  
                state = STATE_STRING_START;
            }
            else
                 return exit_free(LEXICAL_ERROR, str);
            break;

        case STATE_STRING_ESCAPE_ONE_THREE:
            if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7')
            {
                  
                state = STATE_STRING_ESCAPE_REST;
            }
            else
                 return exit_free(LEXICAL_ERROR, str);
            break;
        }
    }
}