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
#include "scaner.h"

// promenna pro ulozeni vstupniho souboru
FILE *source;

void setSourceFile(FILE *f)
{
    source = f;
}

int getNextToken(string_t *attr)
// hlavni funkce lexikalniho analyzatoru
{
    int state = 0;
    int c;
    token_t *token;
    // vymazeme obsah atributu a v pripade identifikatoru
    // budeme postupne do nej vkladat jeho nazev
    // strClear(attr);

    while (1)
    {
        // nacteni dalsiho znaku
        // TODO: connect with setSourceFile
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
                strClear(attr);
                printf("%d\n", token->type);
                return token->type;
            }

            else if (c == '-')
            {
                token->type = MINUS;
                strClear(attr);
                printf("%d\n", token->type);
                return token->type;
            }

            else if (c == '*')
            {
                token->type = MUL;
                strClear(attr);
                printf("%d\n", token->type);
                return token->type;
            }

            else if (c == '(')
            {
                token->type = LEFT_PAR;
                strClear(attr);
                printf("%d\n", token->type);
                return token->type;
            }

            else if (c == ')')
            {
                token->type = RIGHT_PAR;
                strClear(attr);
                printf("%d\n", token->type);
                return token->type;
            }

            else if (c == ':')
            {
                token->type = COLON;
                strClear(attr);
                printf("%d\n", token->type);
                return token->type;
            }

            else if (c == '{')
            {
                token->type = LEFT_BR;
                strClear(attr);
                printf("%d\n", token->type);
                return token->type;
            }

            else if (c == '}')
            {
                token->type = RIGHT_BR;
                strClear(attr);
                printf("%d\n", token->type);
                return token->type;
            }

            else if (c == ';')
            {
                token->type = SEMICOLON;
                strClear(attr);
                printf("%d\n", token->type);
                return token->type;
            }

            else if (c == EOF)
            {
                token->type = END_OF_FILE;
                strClear(attr);
                printf("%d\n", token->type);
                return token->type;
            }

            else if (c == '.')
            {
                token->type = POINT;
                strClear(attr);
                printf("%d\n", token->type);
                return token->type;
            }

            else if (c == ',')
            {
                token->type = COMMA;
                strClear(attr);
                printf("%d\n", token->type);
                return token->type;
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
                strAddChar(attr, c);
                state = STATE_NUMBER;
                printf("%s\n", token->attribute.string);
            }

            else if (isalpha(c) || c == '_' || c == '$')
            {
                strAddChar(attr, c);
                state = STATE_IDENTIFIER_OR_KEYWORD;
                printf("%s\n", token->attribute.string);
            }

            else
                return LEX_ERROR;

            break;

        case STATE_LESS_THAN: // <

            if (c == '=') // <=
            {
                token->type = LESS_EQ;
                printf("%d\n", token->type);
            }
            else
            {
                ungetc(c, source);
                token->type = LESS;
                printf("%d\n", token->type);
            }
            strClear(attr);
            printf("%s\n", token->attribute.string);
            return token->type;

            break;

        case STATE_MORE_THAN: // >
            // komentar
            if (c == '=') // >=
            {
                token->type = GREATER_EQ;
            }

            else
            {
                ungetc(c, source);
                token->type = GREATER;
            }
            strClear(attr);
            return token->type;

            break;

        case STATE_EQUAL_START: // =
            if (c == '=')
                state = STATE_EQUAL; // ==
            else
            {
                ungetc(c, source);
                token->type = ASSIGN;
                strClear(attr);
                return token->type;
            }
            break;

        case STATE_NOT_EQUAL_START: // !
            if (c == '=')
                state = STATE_NOT_EQUAL; // !=
            else
                return LEX_ERROR;
            break;

        case STATE_EQUAL: // ==
            if (c == '=')
                return EQUAL; // ===
            else
                return LEX_ERROR;
            break;

        case STATE_NOT_EQUAL: // !=
            if (c == '=')
                return NOT_EQUAL; // !==
            else
                return LEX_ERROR;
            break;

        case STATE_DIV: // /
            // komentar
            if (c == '/')
                state = STATE_COMMENTARY; // //

            else if (c == '*')
                state = STATE_BLOCK_COMMENTARY; // /*

            else if (c == EOF)
                return LEX_ERROR;

            ungetc(c, source);
            return DIV;
            break;

        case STATE_COMMENTARY: // //
            if (c == '\n' || c == EOF)
            {
                state = STATE_START;
                ungetc(c, source);
            }
            break;

        case STATE_BLOCK_COMMENTARY: // /*
            if (c == '*')
                state = STATE_BLOCK_COMMENTARY_LEAVE; // /**
            else if (c == EOF)
                return LEX_ERROR;
            break;

        case STATE_BLOCK_COMMENTARY_LEAVE: // /**
            if (c == '/')
                state = STATE_START; // /**/
            else if (c == EOF)
                return LEX_ERROR;
            else
                state = STATE_BLOCK_COMMENTARY;
            break;

        case STATE_IDENTIFIER_OR_KEYWORD:
            // identifikator nebo klicove slovo
            if (isalpha(c) || c == '_')
                // identifikator pokracuje
                strAddChar(attr, c);
            else
            // konec identifikatoru
            {
                ungetc(c, source); // POZOR! Je potreba vratit posledni nacteny znak

                // kontrola, zda se nejedna o klicove slovo
                if (strCmpConstStr(attr, "else") == 0)
                {
                    token->attribute.keyword = ELSE;
                    token->type = KEY_W;
                }

                else if (strCmpConstStr(attr, "float") == 0)
                {
                    token->attribute.keyword = FLOAT;
                    token->type = KEY_W;
                }

                else if (strCmpConstStr(attr, "function") == 0)
                {
                    token->attribute.keyword = FUNCTION;
                    token->type = KEY_W;
                }

                else if (strCmpConstStr(attr, "if") == 0)
                {
                    token->attribute.keyword = IF;
                    token->type = KEY_W;
                }

                else if (strCmpConstStr(attr, "int") == 0)
                {
                    token->attribute.keyword = INT;
                    token->type = KEY_W;
                }

                else if (strCmpConstStr(attr, "null") == 0)
                {
                    token->attribute.keyword = K_NULL;
                    token->type = KEY_W;
                }

                else if (strCmpConstStr(attr, "return") == 0)
                {

                    token->attribute.keyword = RETURN;
                    token->type = KEY_W;
                }

                else if (strCmpConstStr(attr, "string") == 0)
                {
                    token->attribute.keyword = STRING;
                    token->type = KEY_W;
                }
                else if (strCmpConstStr(attr, "void") == 0)
                {
                    token->attribute.keyword = VOID;
                    token->type = KEY_W;
                }
                else if (strCmpConstStr(attr, "while") == 0)
                {
                    token->attribute.keyword = WHILE;
                    token->type = KEY_W;
                }

                // jednalo se skutecne o identifikator
                token->type = ID;
            }
            break;

        case STATE_NUMBER:
            if (c == 'e' || c == 'E')
            {
                state = STATE_NUMBER_EXPONENT;
                strAddChar(attr, c);
            }
            // TODO начинается функция с else if, хотя еще не было просто if. Либо ты потеряла где-то первый if, либо просто заменить первый else if на if, ошибка (expected a statement)
            else if (isdigit(c))
                strAddChar(attr, c);
            else if (c == '.')
            {
                state = STATE_NUMBER_POINT;
                strAddChar(attr, c);
            }
            else
            {
                ungetc(c, source);
                return TYPE_INT;
            }
            break;
        // TODO switch закончился на 343 строке, нового нет, выдает ошибку (a case label may only be used within a switch)
        case STATE_NUMBER_POINT:
            if (isdigit(c))
            {
                state = STATE_NUMBER_DOUBLE;
                strAddChar(attr, c);
            }
            else
                return LEX_ERROR;
            break;

        case STATE_NUMBER_DOUBLE:
            if (isdigit(c))
                strAddChar(attr, c);
            else if (c == 'e' || c == 'E')
            {
                state = STATE_NUMBER_EXPONENT;
                strAddChar(attr, c);
            }
            else
            {
                ungetc(c, source);
                return TYPE_FLOAT;
            }
            break;

        case STATE_NUMBER_EXPONENT:
            if (c == '+' || c == '-')
            {
                state = STATE_NUMBER_EXPONENT_SIGN;
                strAddChar(attr, c);
            }
            else if (isdigit(c))
            {
                state = STATE_NUMBER_EXPONENT_FINAL;
                strAddChar(attr, c);
            }
            else
                return LEX_ERROR;
            break;

        case STATE_NUMBER_EXPONENT_SIGN:
            if (isdigit(c))
            {
                state = STATE_NUMBER_EXPONENT_FINAL;
                strAddChar(attr, c);
            }
            else
                return LEX_ERROR;
            break;

        case STATE_NUMBER_EXPONENT_FINAL:
            if (isdigit(c))
                strAddChar(attr, c);
            else
            {
                ungetc(c, source);
                return TYPE_FLOAT;
            }
            break;

        case STATE_STRING_START:
            if (c == '"')
                return TYPE_STRING;
            else if (c == '\\')
                state = STATE_STRING_ESCAPE;
            else if (c < 32)
                return LEX_ERROR;
            else if (c == EOF)
                return LEX_ERROR;
            else
                strAddChar(attr, c);
            break;

        case STATE_STRING_ESCAPE:
            if (c == 'n')
            {
                strAddChar(attr, '\n');
                state = STATE_STRING_START;
            }
            else if (c == 't')
            {
                strAddChar(attr, '\t');
                state = STATE_STRING_START;
            }
            else if (c == '"')
            {
                strAddChar(attr, c);
                state = STATE_STRING_START;
            }
            else if (c == '\\')
            {
                strAddChar(attr, c);
                state = STATE_STRING_START;
            }
            else if (c == '$')
            {
                strAddChar(attr, c);
                state = STATE_STRING_START;
            }
            else if (c == 'x')
            {
                state = STATE_STRING_ESCAPE_X;
                strAddChar(attr, c);
            }
            else if (c == '0')
            {
                strAddChar(attr, c);
                state = STATE_STRING_ESCAPE_ZERO;
            }
            else if (c == '1' || c == '2' || c == '3')
            {
                strAddChar(attr, c);
                state = STATE_STRING_ESCAPE_ONE_THREE;
            }
            else
                return LEX_ERROR;
            break;

        case STATE_STRING_ESCAPE_X:
            if (c == '0')
            {
                strAddChar(attr, c);
                state = STATE_STRING_ESCAPE_X_ZERO;
            }
            else if ((isdigit(c) && c != '0') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
            {
                strAddChar(attr, c);
                state = STATE_STRING_ESCAPE_X_NOT_ZERO;
            }
            else
                return LEX_ERROR;
            break;

        case STATE_STRING_ESCAPE_X_ZERO:
            if ((isdigit(c) && c != '0') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
            {
                strAddChar(attr, c);
                state = STATE_STRING_START;
            }
            else
                return LEX_ERROR;
            break;

        case STATE_STRING_ESCAPE_X_NOT_ZERO:
            if (isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
            {
                strAddChar(attr, c);
                state = STATE_STRING_START;
            }
            else
                return LEX_ERROR;
            break;

        case STATE_STRING_ESCAPE_ZERO:
            if (c == '0')
            {
                strAddChar(attr, c);
                state = STATE_STRING_ESCAPE_ZERO_ZERO;
            }
            else if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7')
            {
                strAddChar(attr, c);
                state = STATE_STRING_ESCAPE_REST;
            }
            else
                return LEX_ERROR;
            break;

        case STATE_STRING_ESCAPE_ZERO_ZERO:
            if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7')
            {
                strAddChar(attr, c);
                state = STATE_STRING_START;
            }
            else
                return LEX_ERROR;
            break;

        case STATE_STRING_ESCAPE_REST:
            if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7')
            {
                strAddChar(attr, c);
                state = STATE_STRING_START;
            }
            else
                return LEX_ERROR;
            break;

        case STATE_STRING_ESCAPE_ONE_THREE:
            if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7')
            {
                strAddChar(attr, c);
                state = STATE_STRING_ESCAPE_REST;
            }
            else
                return LEX_ERROR;
            break;
        }
    }
}