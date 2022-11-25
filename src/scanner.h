/*
 * IFJ2022/project/scanner.h
 *
 * @brief Scanner interface declaration
 *
 * @author Taipova Evgenia <xtaipo00@vutbr.cz>
 */

// hlavicka pro lexikalni analyzator
#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "str.h"
#include "error.h"

typedef enum
{
    // klicova slova
    K_ELSE,
    K_FLOAT,
    K_FUNCTION,
    K_IF,
    K_INT,
    K_NULL,
    K_RETURN,
    K_STRING,
    K_VOID,
    K_WHILE,
    K_PHP,
    K_INT_N,
    K_FLOAT_N,
    K_STRING_N
} keyword_t;
// operatory

typedef enum
{
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_EQUAL,
    TOKEN_NOT_EQUAL,
    TOKEN_LESS,
    TOKEN_GREATER,
    TOKEN_LESS_EQ,
    TOKEN_GREATER_EQ,
    TOKEN_LEFT_PAR,
    TOKEN_RIGHT_PAR,
    TOKEN_SEMICOLON,
    TOKEN_COLON,
    TOKEN_ASSIGN,
    TOKEN_COMMA,
    TOKEN_CONC,
    TOKEN_LEFT_BR,
    TOKEN_RIGHT_BR,
    TOKEN_ID,
    TOKEN_KEY_W,
    TOKEN_TYPE_INT,
    TOKEN_TYPE_FLOAT,
    TOKEN_TYPE_STRING,
    TOKEN_END_OF_FILE,
    TOKEN_PROLOG
} token_type_t;

typedef union
{
    int integer;       /// Integer value.
    double decimal;    /// Decimal value.
    string_t *string;  /// String or identifier value.
    keyword_t keyword; /// Keyword, one of the KEYWORD_... constant

} token_attribute_t;

typedef struct
{
    token_type_t type;
    token_attribute_t attribute;
} token_t;

// Scanner states.
#define STATE_START 200
#define STATE_BACKSLASH 202
#define STATE_COMMENTARY 201
#define STATE_BLOCK_COMMENTARY 203
#define STATE_BLOCK_COMMENTARY_LEAVE 204
#define STATE_IDENTIFIER_OR_KEYWORD 205

#define STATE_EQUAL_START 206
#define STATE_EQUAL 207
#define STATE_NOT_EQUAL 208
#define STATE_NOT_EQUAL_START 209
#define STATE_DIV 210
#define STATE_NUMBER 211
#define STATE_NUMBER_POINT 212
#define STATE_NUMBER_DOUBLE 213
#define STATE_NUMBER_EXPONENT 214
#define STATE_NUMBER_EXPONENT_SIGN 215
#define STATE_NUMBER_EXPONENT_FINAL 216

#define STATE_STRING_START 217
#define STATE_STRING 218
#define STATE_STRING_ESCAPE 219
#define STATE_STRING_ESCAPE_X 220
#define STATE_STRING_ESCAPE_X_ZERO 221
#define STATE_STRING_ESCAPE_X_NOT_ZERO 222
#define STATE_STRING_ESCAPE_ZERO 223
#define STATE_STRING_ESCAPE_ZERO_ZERO 224
#define STATE_STRING_ESCAPE_ONE_THREE 225
#define STATE_STRING_ESCAPE_ONE_THREE_REST 226
#define STATE_STRING_ESCAPE_REST 227
#define STATE_LESS_THAN 228
#define STATE_MORE_THAN 229
#define STATE_QUEST 230
#define STATE_PROLOG_START 231
#define STATE_EOF 232
// hlavicka funkce simulujici lexikalni analyzator
int get_next_token(token_t *token);
void dyn_string_set(string_t *string);

#endif // SCANNER_H