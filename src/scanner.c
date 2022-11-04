/*
 * IFJ2022/project/scaner.c
 * 
 * @brief Scanner interface implementation
 * 
 * @author Taipova Evgenia <xtaipo00@vutbr.cz>
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include "str.h"
#include "scanner.h"

static string_t *attribute;

static int exit_free(int code, string_t *str){
    strFree(str);
    return code;
}


int get_next_token(token_t *token)
// hlavni funkce lexikalniho analyzatoru
{
    string_t string, *str = &string;
    if(strInit(str) == 1) exit_error(LEXICAL_ERROR);

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
                token->type = PLUS;
                  
                return exit_free(NO_ERRORS, str);
            }

            else if (c == '-')
            {
                token.type = MINUS;
                  
                 
                return token;
            }

            else if (c == '*')
            {
                token.type = MUL;
                  
                 
                return token;
            }

            else if (c == '(')
            {
                token.type = LEFT_PAR;
                  
                 
                return token;
            }

            else if (c == ')')
            {
                token.type = RIGHT_PAR;
                  
                 
                return token;
            }

            else if (c == ':')
            {
                token.type = COLON;
                  
                 
                return token;
            }

            else if (c == '{')
            {
                token.type = LEFT_BR;
                  
                 
                return token;
            }

            else if (c == '}')
            {
                token.type = RIGHT_BR;
                  
                 
                return token;
            }

            else if (c == ';')
            {
                token.type = SEMICOLON;
                  
                 
                return token;
            }

            else if (c == EOF)
            {
                token.type = END_OF_FILE;
                  
                 
                return token;
            }

            else if (c == '.')
            {
                token.type = POINT;
                  
                 
                return token;
            }

            else if (c == ',')
            {
                token.type = COMMA;
                  
                 
                return token;
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
                //printf("%s\n", token.attribute.string);
            }

            else if (isalpha(c) || c == '_' || c == '$')
            {
                  
                state = STATE_IDENTIFIER_OR_KEYWORD;
                //printf("%s\n", token.attribute.string);
            }

            else
                exit_free(LEXICAL_ERROR, str);

            break;

        case STATE_LESS_THAN: // <
            // printf("%d --- %d\n", c, state);
            // exit(0);
            if (c == '=') // <=
            {
                token.type = LESS_EQ;
                 
            }
            else
            {
                ungetc(c, stdin);
                token.type = LESS;
                 
            }
              
            //printf("%s\n", token.attribute.string);
            return token;

            break;

        case STATE_MORE_THAN: // >
            // komentar
            if (c == '=') // >=
            {
                token.type = GREATER_EQ;
            }

            else
            {
                ungetc(c, stdin);
                token.type = GREATER;
            }
              
            return token;

            break;

        case STATE_EQUAL_START: // =
            if (c == '=')
                state = STATE_EQUAL; // ==
            else
            {
                ungetc(c, stdin);
                token.type = ASSIGN;
                  
                return token;
            }
            break;

        case STATE_NOT_EQUAL_START: // !
            if (c == '=')
                state = STATE_NOT_EQUAL; // !=
            else
                exit_error(LEXICAL_ERROR);
            break;

        case STATE_EQUAL: // ==
            if (c == '='){
                token.type = EQUAL;  // ===
                return token;
            } else exit_error(LEXICAL_ERROR);
            break;

        case STATE_NOT_EQUAL: // !=
            if (c == '='){
                token.type = NOT_EQUAL;
                return token; // !==
            }
            else exit_error(LEXICAL_ERROR);
            break;

        case STATE_DIV: // /
            // komentar
            if (c == '/')
                state = STATE_COMMENTARY; // //

            else if (c == '*')
                state = STATE_BLOCK_COMMENTARY; // /*

            else if (c == EOF)
                exit_error(LEXICAL_ERROR);

            ungetc(c, stdin);

            token.type = DIV;
            // return token;
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
                exit_error(LEXICAL_ERROR);
            break;

        case STATE_BLOCK_COMMENTARY_LEAVE: // /**
            if (c == '/')
                state = STATE_START; // /**/
            else if (c == EOF)
                exit_error(LEXICAL_ERROR);
            else
                state = STATE_BLOCK_COMMENTARY;
            break;

        case STATE_IDENTIFIER_OR_KEYWORD:
            // identifikator nebo klicove slovo
            if (isalpha(c) || c == '_')
                // identifikator pokracuje
                strAddChar(token.attribute.string, c);
            else
            // konec identifikatoru
            {
                ungetc(c, stdin); // POZOR! Je potreba vratit posledni nacteny znak

                // kontrola, zda se nejedna o klicove slovo
                if (strCmpConstStr(token.attribute.string, "else") == 0)
                {
                    token.attribute.keyword = ELSE;
                    token.type = KEY_W;
                }

                else if (strCmpConstStr(token.attribute.string, "float") == 0)
                {
                    token.attribute.keyword = FLOAT;
                    token.type = KEY_W;
                }

                else if (strCmpConstStr(token.attribute.string, "function") == 0)
                {
                    token.attribute.keyword = FUNCTION;
                    token.type = KEY_W;
                }

                else if (strCmpConstStr(token.attribute.string, "if") == 0)
                {
                    token.attribute.keyword = IF;
                    token.type = KEY_W;
                }

                else if (strCmpConstStr(token.attribute.string, "int") == 0)
                {
                    token.attribute.keyword = INT;
                    token.type = KEY_W;
                }

                else if (strCmpConstStr(token.attribute.string, "null") == 0)
                {
                    token.attribute.keyword = K_NULL;
                    token.type = KEY_W;
                }

                else if (strCmpConstStr(token.attribute.string, "return") == 0)
                {

                    token.attribute.keyword = RETURN;
                    token.type = KEY_W;
                }

                else if (strCmpConstStr(token.attribute.string, "string") == 0)
                {
                    token.attribute.keyword = STRING;
                    token.type = KEY_W;
                }
                else if (strCmpConstStr(token.attribute.string, "void") == 0)
                {
                    token.attribute.keyword = VOID;
                    token.type = KEY_W;
                }
                else if (strCmpConstStr(token.attribute.string, "while") == 0)
                {
                    token.attribute.keyword = WHILE;
                    token.type = KEY_W;
                }

                // jednalo se skutecne o identifikator
                token.type = ID;
            }
            break;

        case STATE_NUMBER:
            if (c == 'e' || c == 'E')
            {
                state = STATE_NUMBER_EXPONENT;
                  
            }
            // TODO начинается функция с else if, хотя еще не было просто if. Либо ты потеряла где-то первый if, либо просто заменить первый else if на if, ошибка (expected a statement)
            else if (isdigit(c))
                strAddChar(token.attribute.string, c);
            else if (c == '.')
            {
                state = STATE_NUMBER_POINT;
                  
            }
            else
            {
                ungetc(c, stdin);
                token.type = TYPE_INT;
                return token;
            }
            break;
        // TODO switch закончился на 343 строке, нового нет, выдает ошибку (a case label may only be used within a switch)
        case STATE_NUMBER_POINT:
            if (isdigit(c))
            {
                state = STATE_NUMBER_DOUBLE;
                  
            }
            else
                exit_error(LEXICAL_ERROR);
            break;

        case STATE_NUMBER_DOUBLE:
            if (isdigit(c))
                strAddChar(token.attribute.string, c);
                  
            else if (c == 'e' || c == 'E')
            {
                state = STATE_NUMBER_EXPONENT;
                  
            }
            else
            {
                ungetc(c, stdin);
                token.type = TYPE_FLOAT;
                return token;
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
                exit_error(LEXICAL_ERROR);
            break;

        case STATE_NUMBER_EXPONENT_SIGN:
            if (isdigit(c))
            {
                state = STATE_NUMBER_EXPONENT_FINAL;
                  
            }
            else
                exit_error(LEXICAL_ERROR);
            break;

        case STATE_NUMBER_EXPONENT_FINAL:
            if (isdigit(c))
                strAddChar(token.attribute.string, c);
                  
            else
            {
                ungetc(c, stdin);
                token.type = TYPE_FLOAT;
                return token;
            }
            break;

        case STATE_STRING_START:
            if (c == '"'){
                token.type = TYPE_STRING;
                return token;
            }
            else if (c == '\\')
                state = STATE_STRING_ESCAPE;
            else if (c < 32)
                exit_error(LEXICAL_ERROR);
            else if (c == EOF)
                exit_error(LEXICAL_ERROR);
            else
                  
            break;

        case STATE_STRING_ESCAPE:
            if (c == 'n')
            {
                strAddChar(token.attribute.string, '\n');
                state = STATE_STRING_START;
            }
            else if (c == 't')
            {
                strAddChar(token.attribute.string, '\t');
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
                exit_error(LEXICAL_ERROR);
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
                exit_error(LEXICAL_ERROR);
            break;

        case STATE_STRING_ESCAPE_X_ZERO:
            if ((isdigit(c) && c != '0') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
            {
                  
                state = STATE_STRING_START;
            }
            else
                exit_error(LEXICAL_ERROR);
            break;

        case STATE_STRING_ESCAPE_X_NOT_ZERO:
            if (isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
            {
                  
                state = STATE_STRING_START;
            }
            else
                exit_error(LEXICAL_ERROR);
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
                exit_error(LEXICAL_ERROR);
            break;

        case STATE_STRING_ESCAPE_ZERO_ZERO:
            if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7')
            {
                  
                state = STATE_STRING_START;
            }
            else
                exit_error(LEXICAL_ERROR);
            break;

        case STATE_STRING_ESCAPE_REST:
            if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7')
            {
                  
                state = STATE_STRING_START;
            }
            else
                exit_error(LEXICAL_ERROR);
            break;

        case STATE_STRING_ESCAPE_ONE_THREE:
            if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7')
            {
                  
                state = STATE_STRING_ESCAPE_REST;
            }
            else
                exit_error(LEXICAL_ERROR);
            break;
        }
    }
}