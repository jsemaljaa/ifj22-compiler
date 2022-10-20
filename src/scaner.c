/*
 * Lexikalni analyzator
 * Autor: Evgenia Taipova
 */


#include <stdio.h>
#include <ctype.h>
#include "str.h"
#include "scaner.h"

// promenna pro ulozeni vstupniho souboru
FILE *source;

void setSourceFile(FILE *f)
{
    source = f;
}

int getNextToken(string *attr)
// hlavni funkce lexikalniho analyzatoru
{
    int state = 0;
    int c;
    // vymazeme obsah atributu a v pripade identifikatoru
    // budeme postupne do nej vkladat jeho nazev
    strClear(attr);

    while (1)
    {
        // nacteni dalsiho znaku
        c = getc(source);

        switch (state)
        {
        case STATE_START:
            // zakladni stav automatu
            if (isspace(c))
                state = STATE_START;

            else if (c == '+')
                return PLUS;

            else if (c == '-')
                return MINUS;

            else if (c == '*')
                return MUL;

            else if (c == '-')
                return MINUS;

            else if (c == '(')
                return LEFT_PAR;

            else if (c == ')')
                return RIGHT_PAR;

            else if (c == ':')
                return COLON;

            else if (c == '{')
                return LEFT_BR;

            else if (c == '}')
                return RIGHT_BR;

            else if (c == ';')
                return SEMICOLON;

            else if (c == EOF)
                return END_OF_FILE;

            else if (c == '\n')
                return END_OF_LINE;

            else if (c == '.')
                return POINT;

            else if (c == ',')
                return COMMA;

            else if (c == '<')
                state = STATE_LESS_THAN;

            else if (c == '>')
                state = STATE_MORE_THAN;

            else if (c == '=')
                state = STATE_EQUAL_START;

            else if (c == '!')
                state = STATE_NOT_EQUAL_START;

            else if (c == '"')
                state = STATE_STRING_START;

            else if (c == '/')
                state = STATE_DIV;

            else if (isdigit(c))
            {
                strAddChar(attr, c);
                state = STATE_NUMBER;
            }

            else if (isalpha(c) || c == '_')
            {
                strAddChar(attr, c);
                state = STATE_IDENTIFIER_OR_KEYWORD;
            }

            else
                return LEX_ERROR;

            break;

        case STATE_LESS_THAN: // <

            if (c == '=')
                return LESS_EQ; // <=

            else if (c == EOF)
                return LEX_ERROR;

            ungetc(c, source);
            return LESS;

            break;

        case STATE_MORE_THAN: // >
            // komentar
            if (c == '=')
                return GREATER_EQ; // >=

            else if (c == EOF)
                return LEX_ERROR;

            ungetc(c, source);
            return GREATER;
            break;

        case STATE_EQUAL_START: // =
            if (c == '=')
                state = STATE_EQUAL; // ==
            else
            {
                if (c == EOF)
                    return LEX_ERROR;
                ungetc(c, source);
                return ASSIGN;
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
                    return ELSE;

                else if (strCmpConstStr(attr, "float") == 0)
                    return FLOAT;

                else if (strCmpConstStr(attr, "function") == 0)
                    return FUNCTION;

                else if (strCmpConstStr(attr, "if") == 0)
                    return IF;

                else if (strCmpConstStr(attr, "int") == 0)
                    return INT;

                else if (strCmpConstStr(attr, "null") == 0)
                   return KEYWORD_NULL;

                else if (strCmpConstStr(attr, "return") == 0)
                    return RETURN;

                else if (strCmpConstStr(attr, "string") == 0)
                    return STRING;

                else if (strCmpConstStr(attr, "void") == 0)
                    return VOID;

                else if (strCmpConstStr(attr, "while") == 0)
                    return WHILE;

                // jednalo se skutecne o identifikator
                return ID;
            }
            break;

        case STATE_NUMBER:
            if (c == 'e' || c == 'E')
            {
                state = STATE_NUMBER_EXPONENT;
                strAddChar(attr, c);
            }
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
