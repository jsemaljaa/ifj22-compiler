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

// flag for prolog
int prolog_flag = 1;

int get_next_token(token_t *token)
{
    // initialization of string
    string_t string, *str = &string;
    if (str_init(str) == 1)
        return exit_free(INTERNAL_ERROR, str);

    token->attribute.string = dynStr;
    int state = STATE_START;
    char c;

    while (1)
    {
        // read next character
        c = (char)getc(stdin);

        // check if prolog is correct
        if (prolog_flag == 1)
        {
            if (c != '<')
            {
                return exit_free(LEXICAL_ERROR, str);
            }
            else
            {
                prolog_flag = 0;
            }
        }

        switch (state)
        {

        case STATE_START:

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
                // add first number to string
                str_add_char(str, c);
                state = STATE_NUMBER;
            }

            else if (c == '$')
            {
                str_add_char(str, c);
                state = STATE_DOLLAR;
            }

            else if (isalpha(c) || c == '_')
            {
                // add first character to string
                str_add_char(str, c);
                state = STATE_IDENTIFIER_OR_KEYWORD;
            }

            else
            {
                return exit_free(LEXICAL_ERROR, str);
            }

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
                // if there is php after <?
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
            if (c == '=')     // >=
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
            else if (c == 's' || c == 'f' || c == 'i')
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
            {
                return exit_free(LEXICAL_ERROR, str);
            }
            break;

        case STATE_DIV: // /
            if (c == '/')
            {
                state = STATE_COMMENTARY; // //
            }
            else if (c == '*')
            {
                state = STATE_BLOCK_COMMENTARY; // /*
            }
            else if (c == EOF)
            {
                return exit_free(LEXICAL_ERROR, str);
            }
            else
            {
                token->type = TOKEN_DIV;
                ungetc(c, stdin);
                return exit_free(NO_ERRORS, str);
            }
            break;

        case STATE_COMMENTARY: // commentaries

            // skip all characters until new line or EOF
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
            {
                state = STATE_BLOCK_COMMENTARY_LEAVE; // /**
            }
            else if (c == EOF)
            {
                return exit_free(LEXICAL_ERROR, str);
            }
            break;

        case STATE_BLOCK_COMMENTARY_LEAVE: // /**
            if (c == '/')
            {
                state = STATE_START; // /**/
            }
            else if (c == '*')
            {
                state = STATE_BLOCK_COMMENTARY_LEAVE; // /***
            }
            else if (c == EOF)
            {
                return exit_free(LEXICAL_ERROR, str);
            }
            else
            {
                state = STATE_BLOCK_COMMENTARY;
            }
            break;

        case STATE_DOLLAR:
            // $ and numbers can't be at the beginning of identifier
            if (isalpha(c) || c == '_')
            {
                str_add_char(str, c);
                state = STATE_IDENTIFIER_OR_KEYWORD;
            }
            else
            {
                return exit_free(LEXICAL_ERROR, str);
            }
            break;

        case STATE_IDENTIFIER_OR_KEYWORD:
            // identifier or keyword
            if (isalnum(c) || c == '_')
            {
                // add character to string
                str_add_char(str, c);
            }
            else
            {

                ungetc(c, stdin);
                // check if it is keyword
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
                    // check if it is float or float_n
                    if (str->str[0] == '?')
                    {
                        token->attribute.keyword = K_FLOAT_N;
                    }
                    else
                    {
                        token->attribute.keyword = K_FLOAT;
                    }
                    token->type = TOKEN_KEY_W;
                }

                else if ((str_cmp_const_str(str, "string") == 0) || (str_cmp_const_str(str, "?string") == 0))
                {
                    if (str->str[0] == '?')
                    {
                        token->attribute.keyword = K_STRING_N;
                    }
                    else
                    {
                        token->attribute.keyword = K_STRING;
                    }
                    token->type = TOKEN_KEY_W;
                }

                else if ((str_cmp_const_str(str, "int") == 0) || (str_cmp_const_str(str, "?int") == 0))
                {
                    if (str->str[0] == '?')
                    {
                        token->attribute.keyword = K_INT_N;
                    }
                    else
                    {
                        token->attribute.keyword = K_INT;
                    }
                    token->type = TOKEN_KEY_W;
                }

                else
                {
                    // it is identifier
                    token->type = TOKEN_ID;
                    str_copy_string(token->attribute.string, str);
                }

                return exit_free(NO_ERRORS, str);
            }
            break;

        case STATE_NUMBER:
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
                // convert string to integer

                token->type = TOKEN_TYPE_INT;
                token->attribute.integer = atoi(str->str);

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
            {
                return exit_free(LEXICAL_ERROR, str);
            }
            break;

        case STATE_NUMBER_DOUBLE:
            if (isdigit(c))
            {
                str_add_char(str, c);
            }
            else if (c == 'e' || c == 'E')
            {
                state = STATE_NUMBER_EXPONENT;
                str_add_char(str, c);
            }
            else
            {
                ungetc(c, stdin);
                // convert string to double
                token->attribute.decimal = atof(str->str);
                token->type = TOKEN_TYPE_FLOAT;
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
            {
                return exit_free(LEXICAL_ERROR, str);
            }
            break;

        case STATE_NUMBER_EXPONENT_SIGN:
            if (isdigit(c))
            {
                state = STATE_NUMBER_EXPONENT_FINAL;
                str_add_char(str, c);
            }
            else
            {
                return exit_free(LEXICAL_ERROR, str);
            }
            break;

        case STATE_NUMBER_EXPONENT_FINAL:
            if (isdigit(c))
            {
                str_add_char(str, c);
            }
            else
            {
                ungetc(c, stdin);
                // convert string to double
                token->attribute.decimal = atof(str->str);
                token->type = TOKEN_TYPE_FLOAT;
                return exit_free(NO_ERRORS, str);
            }
            break;

        case STATE_STRING_START:
            if (c == '"')
            {
                token->type = TOKEN_TYPE_STRING;
                str_copy_string(token->attribute.string, str);
                return exit_free(NO_ERRORS, str);
            }
            else if (c == '\\')
            {
                state = STATE_STRING_SEQUENCE;
            }

            else if (c < 32 || c == EOF || c == '$')
            {
                return exit_free(LEXICAL_ERROR, str);
            }
            else
            {
                str_add_char(str, c);
            }
            break;

        case STATE_STRING_SEQUENCE:
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
                state = STATE_STRING_SEQUENCE_X;
            }
            else if (c == '0')
            {
                state = STATE_STRING_SEQUENCE_ZERO;
            }
            else if (c == '1' || c == '2' || c == '3')
            {
                state = STATE_STRING_SEQUENCE_ONE_THREE;
            }
            else
            {
                return exit_free(LEXICAL_ERROR, str);
            }
            break;

        case STATE_STRING_SEQUENCE_X:
            if (c == '0')
            {
                state = STATE_STRING_SEQUENCE_X_ZERO;
            }
            else if ((isdigit(c) && c != '0') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
            {
                state = STATE_STRING_SEQUENCE_X_NOT_ZERO;
            }
            else
            {
                return exit_free(LEXICAL_ERROR, str);
            }
            break;

        case STATE_STRING_SEQUENCE_X_ZERO:
            if ((isdigit(c) && c != '0') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
            {
                state = STATE_STRING_START;
            }
            else
            {
                return exit_free(LEXICAL_ERROR, str);
            }
            break;

        case STATE_STRING_SEQUENCE_X_NOT_ZERO:
            if (isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
            {
                state = STATE_STRING_START;
            }
            else
            {
                return exit_free(LEXICAL_ERROR, str);
            }
            break;

        case STATE_STRING_SEQUENCE_ZERO:
            if (c == '0')
            {
                state = STATE_STRING_SEQUENCE_ZERO_ZERO;
            }
            else if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7')
            {
                state = STATE_STRING_SEQUENCE_REST;
            }
            else
            {
                return exit_free(LEXICAL_ERROR, str);
            }
            break;

        case STATE_STRING_SEQUENCE_ZERO_ZERO:
            if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7')
            {
                state = STATE_STRING_START;
            }
            else
            {
                return exit_free(LEXICAL_ERROR, str);
            }
            break;

        case STATE_STRING_SEQUENCE_REST:
            if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7')
            {
                state = STATE_STRING_START;
            }
            else
            {
                return exit_free(LEXICAL_ERROR, str);
            }
            break;

        case STATE_STRING_SEQUENCE_ONE_THREE:
            if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7')
            {
                state = STATE_STRING_SEQUENCE_REST;
            }
            else
            {
                return exit_free(LEXICAL_ERROR, str);
            }
            break;
        }
    }
}