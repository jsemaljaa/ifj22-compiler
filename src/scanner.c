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

string_t *dynStr;

static int exit_free(int code, string_t *str)
{
    str_free(str);
    return code;
}

void set_dynamic_string(string_t *string)
{
    dynStr = string;
}

int get_next_token(token_t *token)
// hlavni funkce lexikalniho analyzatoru
{ //инициализация динамических строк
    string_t string, *str = &string;
    if (str_init(str) == 1)
        return exit_free(LEXICAL_ERROR, str);

    token->attribute.string = dynStr;
    int state = STATE_START;
    char c;
    // vymazeme obsah atributu a v pripade identifikatoru
    // budeme postupne do nej vkladat jeho nazev

    while (1)
    {
        // nacteni dalsiho znaku
        // TODO: connect with setstdinFile
        c = (char)getc(stdin);

        // printf("c = %c\n", c);
        switch (state)
        {
        case STATE_START:
            // zakladni stav automatu
            if (isspace(c))
                state = STATE_START;

            else if (c == '+')
            {
                token->type = TOKEN_PLUS;
                // printf("------------------: %d\n", token->type);
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
            {
                state = STATE_QUEST;
            }

            else if (c == '"')
                state = STATE_STRING_START;

            else if (c == '/')
                state = STATE_DIV;

            else if (isdigit(c))
            {
                // printf("DIGIT\n" );
                if (str_add_char(str, c))
                    return exit_free(LEXICAL_ERROR, str);
                state = STATE_NUMBER;
                // printf("%s\n", token->attribute.string);
            }

            else if (isalpha(c) || c == '_' || c == '$')
            {
                str_add_char(str, c);
                state = STATE_IDENTIFIER_OR_KEYWORD;
                // printf("%s\n", token->attribute.string->str);
            }

            else
                return exit_free(LEXICAL_ERROR, str);

            break;

        case STATE_LESS_THAN: // <
            if (c == '=')     // <=
            {
                token->type = TOKEN_LESS_EQ;
            }
            else if (c == '?')
            {
                state = STATE_PROLOG_START;
            }
            else
            {
                ungetc(c, stdin);
                token->type = TOKEN_LESS;
                return exit_free(NO_ERRORS, str);
            }

            break;

        case STATE_PROLOG_START: // <?
            if (isalpha(c))
            {
                str_add_char(str, c); 
            }
            else
            {
                ungetc(c, stdin);
                if (str_cmp_const_str(str, "php") == 0)
                {
                    token->attribute.keyword = K_PHP;
                    token->type = TOKEN_PROLOG;
                }
                else
                {
                    return exit_free(LEXICAL_ERROR, str);
                }
                return exit_free(NO_ERRORS, str);
            }

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

        case STATE_QUEST: // ?
            if (c == '>')
            {
                state = STATE_EOF;
            }
            else if ( c == 's' || c == 'f' || c == 'i')
            {
                str_add_char(str, c);
                state = STATE_IDENTIFIER_OR_KEYWORD;
            }
            else
            {
                return exit_free(LEXICAL_ERROR, str);
            }
            break;

        case STATE_EOF:
            if (c == EOF)
            {
                ungetc(c, stdin);
                token->type = TOKEN_END_OF_FILE;
                return exit_free(NO_ERRORS, str);
            }
            else
            {
                return exit_free(LEXICAL_ERROR, str);
            }
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
            if (c == '=')
            {
                token->type = TOKEN_EQUAL; // ===
                return exit_free(NO_ERRORS, str);
            }
            else
            {
                return exit_free(LEXICAL_ERROR, str);
            }
            break;

        case STATE_NOT_EQUAL: // !=
            if (c == '=')
            {
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
            else
            {
                token->type = TOKEN_DIV;
                ungetc(c, stdin);
                return exit_free(NO_ERRORS, str);
            }
            break;

        case STATE_COMMENTARY: // //

            while (1)
            {
                if (c == '\n' || c == EOF)
                {
                    ungetc(c, stdin);
                    break;
                }
                else if (c != '\n')
                {
                    c = (char)getc(stdin);
                }
                else
                    break;
            }

            state = STATE_START;

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
            if (isalnum(c) || c == '_')
            {
                // identifikator pokracuje
                str_add_char(str, c);
            }
            else
            // konec identifikatoru
            {
                ungetc(c, stdin); // POZOR! Je potreba vratit posledni nacteny znak
                // kontrola, zda se nejedna o klicove slovo
                if (str_cmp_const_str(str, "else") == 0)
                {
                    token->attribute.keyword = K_ELSE;
                    token->type = TOKEN_KEY_W;
                }

                else if (str_cmp_const_str(str, "function") == 0)
                {
                    token->attribute.keyword = K_FUNCTION;
                    token->type = TOKEN_KEY_W;
                }

                else if (str_cmp_const_str(str, "if") == 0)
                {
                    token->attribute.keyword = K_IF;
                    token->type = TOKEN_KEY_W;
                }

                else if (str_cmp_const_str(str, "null") == 0)
                {
                    token->attribute.keyword = K_NULL;
                    token->type = TOKEN_KEY_W;
                }

                else if (str_cmp_const_str(str, "return") == 0)
                {

                    token->attribute.keyword = K_RETURN;
                    token->type = TOKEN_KEY_W;
                }

                else if (str_cmp_const_str(str, "void") == 0)
                {
                    token->attribute.keyword = K_VOID;
                    token->type = TOKEN_KEY_W;
                }

                else if (str_cmp_const_str(str, "while") == 0)
                {
                    token->attribute.keyword = K_WHILE;
                    token->type = TOKEN_KEY_W;
                }

                else if ((str_cmp_const_str(str, "float") == 0) || (str_cmp_const_str(str, "?float") == 0))
                {
                    if(str->str[0] == '?'){
                        token->attribute.keyword = K_FLOAT_N;
                    } else token->attribute.keyword = K_FLOAT;
                    
                    token->type = TOKEN_KEY_W;
                }

                else if ((str_cmp_const_str(str, "string") == 0) || (str_cmp_const_str(str, "?string") == 0))
                {
                    if(str->str[0] == '?'){
                        token->attribute.keyword = K_STRING_N;
                    } else token->attribute.keyword = K_STRING;
                    
                    token->type = TOKEN_KEY_W;
                }

                else if ((str_cmp_const_str(str, "int") == 0) || (str_cmp_const_str(str, "?int") == 0))
                {
                    if(str->str[0] == '?'){
                        token->attribute.keyword = K_INT_N;
                    } else token->attribute.keyword = K_INT;
                    
                    token->type = TOKEN_KEY_W;
                }

                // jednalo se skutecne o identifikator
                else
                {
                    token->type = TOKEN_ID;
                    str_copy_string(token->attribute.string, str);
                }

                return exit_free(NO_ERRORS, str);
            }
            break;

        case STATE_NUMBER:
            // printf("DIGIT num\n" );
            if (c == 'e' || c == 'E')
            {
                state = STATE_NUMBER_EXPONENT;
                str_add_char(str, c);
            }
            else if (isdigit(c))
            {
                str_add_char(str, c);
            }
            else if (c == '.')
            {
                state = STATE_NUMBER_POINT;
                str_add_char(str, c);
            }
            else
            {
                ungetc(c, stdin);
                char *endptr;
                int val = (int)strtol(str->str, &endptr, 10);

                if (*endptr)
                {
                    return exit_free(LEXICAL_ERROR, str);
                }

                token->attribute.integer = val;
                token->type = TOKEN_TYPE_INT;
                // printf("%d\n", token->attribute.integer);
                return exit_free(NO_ERRORS, str);
            }
            break;

        case STATE_NUMBER_POINT:
            if (isdigit(c))
            {
                state = STATE_NUMBER_DOUBLE;
                str_add_char(str, c);
            }
            else
                return exit_free(LEXICAL_ERROR, str);
            break;

        case STATE_NUMBER_DOUBLE:
            if (isdigit(c))
                str_add_char(str, c);
            else if (c == 'e' || c == 'E')
            {
                // printf("e\n");
                state = STATE_NUMBER_EXPONENT;
                str_add_char(str, c);
            }
            else
            {
                ungetc(c, stdin);
                char *endptr;

                double val = strtod(str->str, &endptr);
                if (*endptr)
                {
                    return exit_free(LEXICAL_ERROR, str);
                }

                token->attribute.decimal = val;
                token->type = TOKEN_TYPE_FLOAT;
                // printf("%f\n", token->attribute.decimal);
                return exit_free(NO_ERRORS, str);
            }
            break;

        case STATE_NUMBER_EXPONENT:
            if (c == '+' || c == '-')
            {
                state = STATE_NUMBER_EXPONENT_SIGN;
                str_add_char(str, c);
            }
            else if (isdigit(c))
            {
                state = STATE_NUMBER_EXPONENT_FINAL;
                str_add_char(str, c);
            }
            else
                return exit_free(LEXICAL_ERROR, str);
            break;

        case STATE_NUMBER_EXPONENT_SIGN:
            if (isdigit(c))
            {
                state = STATE_NUMBER_EXPONENT_FINAL;
                str_add_char(str, c);
            }
            else
                return exit_free(LEXICAL_ERROR, str);
            break;

        case STATE_NUMBER_EXPONENT_FINAL:
            if (isdigit(c))
                str_add_char(str, c);
            else
            {
                ungetc(c, stdin);
                char *endptr;

                double val = strtod(str->str, &endptr);
                if (*endptr)
                {
                    return exit_free(LEXICAL_ERROR, str);
                }

                token->attribute.decimal = val;
                token->type = TOKEN_TYPE_FLOAT;
                // printf("%f\n", token->attribute.decimal);
                return exit_free(NO_ERRORS, str);
            }
            break;

        case STATE_STRING_START:
            // printf("c = %c\n",c);
            if (c == '"')
            {
                token->type = TOKEN_TYPE_STRING;
                  str_copy_string(token->attribute.string, str);
                return exit_free(NO_ERRORS, str);
            }
            else if (c == '\\')
            {
                state = STATE_STRING_ESCAPE;
            }

            else if (c < 32)
                return exit_free(LEXICAL_ERROR, str);
            else if (c == EOF)
                return exit_free(LEXICAL_ERROR, str);
            else
                str_add_char(str, c);
            break;

        case STATE_STRING_ESCAPE:
            if (c == 'n')
            {
                str_add_char(str, '\n');
                state = STATE_STRING_START;
            }
            else if (c == 't')
            {
                str_add_char(str, '\t');
                state = STATE_STRING_START;
            }
            else if (c == '"')
            {
                str_add_char(str, '"');
                state = STATE_STRING_START;
            }
            else if (c == '\\')
            {
                str_add_char(str, '\\');
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