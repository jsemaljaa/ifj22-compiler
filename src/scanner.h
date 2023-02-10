/*
 * IFJ2022/project/scanner.h
 *
 * @brief Scanner interface declaration
 *
 * @author Taipova Evgenia <xtaipo00@vutbr.cz>
 */

#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "str.h"
#include "error.h"

//structure of keywords
typedef enum
{
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

//structure of token types
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
    TOKEN_DO_QUOTES,
    TOKEN_QUEST,
    TOKEN_ID,
    TOKEN_KEY_W,
    TOKEN_TYPE_INT,
    TOKEN_TYPE_FLOAT,
    TOKEN_TYPE_STRING,
    TOKEN_END_OF_FILE,
    TOKEN_PROLOG
} token_type_t;

//structure of token attributes
typedef union
{
    int integer;       
    double decimal;    
    string_t *string;  
    keyword_t keyword; 
} token_attribute_t;

//structure of token
typedef struct
{
    token_type_t type;
    token_attribute_t attribute;
} token_t;

//scanner finite states
#define STATE_START 200 
#define STATE_BACKSLASH 202 
#define STATE_IDENTIFIER_OR_KEYWORD 205
#define STATE_EQUAL_START 206
#define STATE_DIV 210
#define STATE_NUMBER 211
#define STATE_NUMBER_DOUBLE 213
#define STATE_NUMBER_EXPONENT_FINAL 216
#define STATE_STRING 218
#define STATE_LESS_THAN 228
#define STATE_MORE_THAN 229
#define STATE_QUEST 230

//scanner non-finate states 
#define STATE_COMMENTARY 201 
#define STATE_BLOCK_COMMENTARY 203
#define STATE_BLOCK_COMMENTARY_LEAVE 204
#define STATE_EQUAL 207
#define STATE_NOT_EQUAL 208
#define STATE_NOT_EQUAL_START 209
#define STATE_NUMBER_POINT 212
#define STATE_NUMBER_EXPONENT 214
#define STATE_NUMBER_EXPONENT_SIGN 215
#define STATE_STRING_START 217
#define STATE_STRING_SEQUENCE 219
#define STATE_STRING_SEQUENCE_X 220
#define STATE_STRING_SEQUENCE_X_ZERO 221
#define STATE_STRING_SEQUENCE_X_NOT_ZERO 222
#define STATE_STRING_SEQUENCE_ZERO 223
#define STATE_STRING_SEQUENCE_ZERO_ZERO 224
#define STATE_STRING_SEQUENCE_ONE_THREE 225
#define STATE_STRING_SEQUENCE_REST 227
#define STATE_PROLOG_START 231
#define STATE_EOF 232
#define STATE_DOLLAR 233
/*
 * @brief Main function of the scanner
 * @param *token - pointer to the token structure
 * @return 0 - success, 1 - error
 */
int get_next_token(token_t *token);
/*
 * @brief Function for setting the dynamic string
 * @param *string - pointer to the string structure
 * @return void
 */
void set_dynamic_string(string_t *string);
/*
 * @brief Function for freeing the dynamic string
 * @param code - error code
 * @param *string - pointer to the string structure
 * @return code
 */
static int exit_free(int code, string_t *string);

#endif // SCANNER_H