/*
 * IFJ2022/project/scaner.h 
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

typedef enum keyword
{
    // klicova slova
    ELSE,
    FLOAT,
    FUNCTION,
    IF,
    INT,
    K_NULL,
    RETURN,
    STRING,
    VOID,
    WHILE
} keyword_t;
// operatory
typedef enum token
{
    PLUS,
    MINUS,
    MUL,
    DIV,
    EQUAL,
    NOT_EQUAL,
    LESS,
    GREATER,
    LESS_EQ,
    GREATER_EQ,
    LEFT_PAR,
    RIGHT_PAR,
    SEMICOLON,
    COLON,
    ASSIGN,
    COMMA,
    POINT,
    LEFT_BR,
    RIGHT_BR,
    DO_QUOTES,
    QUEST,
    ID,
    KEY_W,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    END_OF_FILE
} token_type_t;

typedef union attribute
{
    int integer;       /// Integer value.
    double decimal;    /// Decimal value.
    char *string;      /// String or identifier value.
    keyword_t keyword; /// Keyword, one of the KEYWORD_... constant

} token_attribute_t;

typedef struct struct_token
{
    token_type_t type;
    token_attribute_t attribute;
} token_t;

// chybove hlasky
#define LEX_ERROR 1

// Scanner states.
#define STATE_START 200                  /// Starting state every new token processing starts here and initializes other states
#define STATE_BACKSLASH 202              /// Operator / OR start of (block) comment - next symbol must be (*) /
#define STATE_COMMENTARY 201             /// Starts with //, ignores every symbol, ends with EOL or EOF
#define STATE_BLOCK_COMMENTARY 203       /// Starts with /* and ignores every symbol except * - this might be the end of block comment
#define STATE_BLOCK_COMMENTARY_LEAVE 204 /// Ends with */ the * is read, if the next symbol is /, leave, else if *, stay, else go back to state before
#define STATE_IDENTIFIER_OR_KEYWORD 205  /// Starts with letter or _, if next symbols are alphanumeric or _, add them to string, which is later compared with reserved words | Returns either keyword or string as ID

#define STATE_EQUAL_START 206           /// Starts with =, if next symbol is =, return EQUAL, else return ASSIGN
#define STATE_EQUAL 207                 /// Starts with =, if next symbol is =, return EQUAL, else return ASSIGN
#define STATE_NOT_EQUAL 208             /// Starts with !, if next symbol is =, return NOT_EQUAL, else return LEX_ERROR
#define STATE_NOT_EQUAL_START 209       /// Starts with !, if next symbol is =, return NOT_EQUAL, else return LEX_ERROR
#define STATE_DIV 210                   /// Starts with /, if next symbol is /, return DIV, else return LEX_ERROR
#define STATE_NUMBER 211                /// Start of number processing, accepts numbers, e/E and . | Can return integer number
#define STATE_NUMBER_POINT 212          /// If symbol was ., the number has type double
#define STATE_NUMBER_DOUBLE 213         /// The last symbol was number | Can return double number
#define STATE_NUMBER_EXPONENT 214       /// The last symbol was e or E, the number has type double, continues with optional symbols +/- or number
#define STATE_NUMBER_EXPONENT_SIGN 215  /// Optional symbol was read, continue with numbers only
#define STATE_NUMBER_EXPONENT_FINAL 216 /// Returns double number with exponent

#define STATE_STRING_START 217  /// String starts with !" else returns error
#define STATE_STRING 218        /// Sequence !" was read, ends with ", if ASCII value is lower than 32, returns error, these symbols can be written using escape sequence | Returns string
#define STATE_STRING_ESCAPE 219 /// If symbol \ was loaded, can replace char with escape sequence symbols
#define STATE_STRING_ESCAPE_X 220
#define STATE_STRING_ESCAPE_X_ZERO 221
#define STATE_STRING_ESCAPE_X_NOT_ZERO 222
#define STATE_STRING_ESCAPE_ZERO 223      /// 0 loaded, accepts only digits
#define STATE_STRING_ESCAPE_ZERO_ZERO 224 /// 00 loaded, accepts only digits
#define STATE_STRING_ESCAPE_ONE_THREE 225 /// first 2 then 5 was loaded, accepts only 0 to 5 digits, returns symbol with ASCII value
#define STATE_STRING_ESCAPE_ONE_THREE_REST 226
#define STATE_STRING_ESCAPE_REST 227 /// the rest cases, when the third number can be only digit, from 001 to 249
#define STATE_LESS_THAN 228          /// Starts with < | Returns <= or <
#define STATE_MORE_THAN 229          /// Starts with > | Returns > or >=
#define STATE_QUEST 230
// hlavicka funkce simulujici lexikalni analyzator
void setSourceFile(FILE *f);
int getNextToken(string_t *attr);

#endif // SCANNER_H